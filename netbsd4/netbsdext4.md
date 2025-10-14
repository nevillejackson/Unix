
This is a 'proof of concept' investigation. I need to test if my approach will function correctly.

### Trying to access a Linux ext4 filesystem from NetBSD ###

What I would like to do is mount my 1.3Tb Linux(ext4) data partition to NetBSD. 
I can see that to get an ext4 mount one has to have a Linux environment. 
There are 2 ways to get a Linux environment inside NetBSD

 - run Linux in a VM. Give the VM hardware access , so it can mount the data partiton directly, then export the filesystem to NetBSD using NFS.

 - run a Linux environment ( minus a kernel) in a container. This is more problematic. Containers share the host's kernel.  A linux container will not work with BSD kernel. One may be able to make it function by installing Linux compatability packages in NetBSD.  These packages provide Linux system calls. That may not be enough.

Given that the 'Linux in a VM' options seems most feasable, we shall try that first.

#### Setting up a VM in NetBSD ####
There seem to be two options. 
 - NetBSD has its own native hypervisor called `nvmm` (NetBSD Virtual Machine Monitor) which is built on `qemu`. One can use `qemu` with or without nvmm. It will be slower without a hypervisor.
 - NetBSD also supports Xen ( a Type 1 hypervisor). Xen requires a special kernel.

Given that I have some knowledge of `qemu` we shall try that option first.

##### Qemu in NetBSD #####

A barebones qemu virtual machine can be started as follows:

 1. Create a file that is to be the virtual machine's virtul disk.
```
qemu-img create -f qcow2 antixvm.qcow2
```

 2. Put an OS into the VM from an .iso file
```
qemu-system-x86_64 -cpu qemu64,-apic -m 512 -nic user -display sdl,gl=on -boot d -hda antixvm.qcow2 -cdrom /home/nevj/Downloads/antiX-23.2_x64-core.iso
```
This will boot a live Antix from which one can install using the `cli-install` command.  Use the `MBR` option for grub. When the install completes, do not reboot, use `poweroff` as this will properly shutdown the VM.

This VM can now be booted from disk as follows
```
qemu-system-x86_64 -cpu qemu64,-apic -m 512 -nic user -display sdl,gl=on -boot c -hda antixvm.qcow2
```
The `-boot c` tells qemu to try to boot first from disk.
The `-cpu qemu64,-apic` tells qemu to use a particular CPU model. Apart from qemu64, kvm64 works, and the one that matches my machine (SandyBridge) works, but `-cpu host` does not work for me in NetBSD.... it says it needs KVM, which is not available. It may work with NetBSD's own hypervisor (NVMM), if I can figure out how to run NVMM.

I chose Antix core iso because it is small and CLI only. I originally tried with Alpine, but had difficulties with its default ramdisk configuration. 

The flag `-apic` means noapic. It is necessary because of a bug in the NetBSD version of qemu. I dont understand this 'fix'. NetBSD supports APIC.

With that in place we can look into making a disk or a partition available to a qemu guest. 

##### Passing a physical disk or partition to a qemu guest #####
This has to be done in the qemu command line. It can not be added dynamically to a running guest. 

I have an ext4 partiton called `common` which contains data. It is on the partiton known as /dev/dk7 to NetBSD. To pass that partition to a qemu guest I modify the qemu command as follows
```
qemu-system-x86_64 -cpu qemu64,-apic -m 512 -nic user -display sdl,gl=on -boot c -hda antixvm.qcow2 -drive if=none,id=drive0,format=raw,file=/dev/rdk7 virtio-blk-pci,drive=drive0
```
That above command has to be issued as root.
After booting as above, I get 

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd4/lsblk.png?raw=true">
</p>


so it has a disk called `/dev/vda` which is a block special device.

I can mount vda with a loop mount

```
mount -o loop /dev/vda /mnt
```
and I can go to `/mnt` and see its content
```
cd /mnt
ls
.... lots of directories
```
And I can read files.  
 I have successfully passed my /dev/dk7 partition from NetBSD host to Antix guest, and the guest is able to read its ext4 files, even though the host cant read ext4 files.

What I need to do now is export this mounted filesystem back to the host

##### Setting up NFS to export a filesystem from guest to host #####
This will require networking between host and guest. 
First of all I have to make the Antix guest an NFS server
```
apt-get update
apt-get upgrade
apt-get install nfs-kernel-server
```

Then I have to export the /mnt/common filesystem
Edit the file `/etc/exports` and add the line
```
/mnt/common *(rw,sync,no_subtree_check)
```
[ Note: I need to set the IP address to '*' in above. Using a specific IP address will not work for a VM]


Then I have to setup NFS client services in NetBSD. NetBSD does not have a package for this, you simply add the following lines to `/etc/rc.conf` to enable the required services
```
# NFS client
rpcbind=yes
nfs_client=yes
lockd=yes
statd=yes
```
The order is important.
Then either reboot or start services by hand as follows
```
# service rpcbind start
Starting rpcbind.
# service nfslocking start
Starting statd.
Starting lockd.
# 
```

Then create a mount point
```
mkdir /mnt/common
```

and do the nfs mount
```
mount -t nfs antixvm:/mnt/common /mnt/common
```
[ Note: This does not work until I fix networking. The NetBSD host does not know what 'antixvm' hostname is because antixvm does not have an IP number known to the host. ]

Given the network fix, I can now `cd /mnt/common` and can see my data 
directories and operate on files.

##### Networking a qemu guest to a NetBSD host #####
Now, fix the networking. 
For NFS to operate with the qemu guest as an NFS server, and the NetBSD host as client , I only need a very simple network link between host and guest. Therefore I will use a `tap` interface, rather than `user` or `bridge`.

 1. In the NetBSD host  create a `tap` interface
```
ifconfig tap0 create
ifconfig tap0 descr "NetBSDVM" up
ifconfig tap0 10.0.2.1 netmask 255.255.255.0
```
Then check with `ifconfig` that it has worked, the tap0 interface should exist and be up and have an IP number. 
That is like plugging another NIC card into the host and putting it on the 10.0.2.0 network.

 2. Start the Antix guest VM using the following qemu command
```
qemu-system-x86_64 -cpu qemu64,-apic -m 512 -display sdl,gl=on -boot c -hda antixvm.qcow2 -drive if=none,id=drive0,format=raw,file=/dev/rdk7 -device virtio-blk-pci,drive=drive0 -device e1000,netdev=net0 -netdev tap,id=net0,ifname=tap0,script=no,downscript=no
```
There are a lot of command line options. The critical one's for networking is -netdev and -device. That is what links the VM to the `tap0` interface in the host. It is analagous to running an ethernet cable from the virtual machine to the `tap0` port on the host. 

 3. After the VM starts, login and check its networking
```
ip a
```
Shows interface eth0 is UP , but has no IP.  We need to give it an IP
```
ip addr add 10.0.2.15/24 dev eth0
```
Now `ip a` shows eth0 is UP and has IP 10.0.2.15
But 
```
ip r
10.0.2.0/24 dev eth0 proto kernel scope link src 10.0.2.15
```
it only has the 10.0.2.0 network defined... no default route. That is sufficient for my purpose
```
ping 10.0.2.1 works from the guest
ping 10.0.2.15 works from the host
```
but I cant ping anything else, not even the hosts IP on the modem port. 

We can impove that by adding a default route
```
ip route ad default via 10.0.2.1
```
ie the default gateway is the host.
Now I can also do
```
ping 192.168.0.57 from the guest ( that is my hosts IP on the modem port)
```

I also cant ping google.com.au ... there is no dns server accessible to the guest. That does not matter.

##### Final issues #####
Given the network setup above, the guest exports my `common` directory using NFS, and the host can mount it with
```
mount -t nfs 10.0.2.15:/mnt/common /mnt/common
```
and I can do
```
cd /mnt/common
ls
```
and I see my files. 
I can read files as either root or nevj, but I can only write files as nevj, not as root. 
I assume that might be some sort of NFS security feature. 

##### Where to now  #####
I have shown that a VM NFS server  will work.  I now need to automate the steps.  
Are there any suggestions for automating this?


##### Acknowledgement #####
I am grateful for assistence from @JoelA with `qemu` and `tap` interfaces.
