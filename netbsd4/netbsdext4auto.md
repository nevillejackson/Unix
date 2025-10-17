### Automating my NetBSD ext4 mount ###
 This proved to be easier than I expected

 1. Configure the Antix VM.
I need the VM to have an NFS server installed and configured, as previously described
```
add the following to /etc/exports
/mnt/common *(rw,sync,no_subtree_check)
and do 
exportfs -a
```
I also need the VM to loop mount the physical drive passed through to qemu
```
mount -o loop /dev/vda /mnt/common
```
 I also need the VM to give eth0 a static IP. That can be done in /etc/network/interfaces
```
auto eth0
iface eth0 inet static
        address 10.0.2.15
        netmask 255.255.255.0
        gateway 10.0.2.1
```
All of the above only need to be done once

 2.  In NetBSD there is no pre-configuration. I can automate the whole process of setting up the `tap` interface and starting the Antix VM with qemu, by writing a small startup script which is executed  when NetBSD boots.
To do that I put the following entry in `/etc/rc.local`.
```
# This file is (nearly) the last thing invoked by /etc/rc during a
# normal boot, via /etc/rc.d/local.
#
# It is intended to be edited locally to add site-specific boot-time
# actions, such as starting locally installed daemons.
#
# An alternative option is to create site-specific /etc/rc.d scripts.
#

echo -n 'Starting local daemons:'

# Add your local daemons here, eg:
#
#if [ -x /path/to/daemon ]; then
#	/path/to/daemon args
#fi

if [ -x /usr/local/bin/antixvm.sh ]; then
          /usr/local/bin/antixvm.sh > /dev/null
fi


echo '.'
```
Then I put the script `antixvm.sh` in /usr/local/bin. 
Here is the script
```
#!/bin/sh

ifconfig tap0 create
ifconfig tap0 descr "AntixVM" up
ifconfig tap0 10.0.2.1 netmask 255.255.255.0

/usr/pkg/bin/qemu-system-x86_64 -cpu qemu64,-apic -m 512 -display none -boot c -hda /usr/local/qemu/antixvm.qcow2 -drive if=none,id=drive0,format=raw,file=/dev/rdk7 -device virtio-blk-pci,drive=drive0 -device e1000,netdev=net0 -netdev tap,id=net0,ifname=tap0,script=no,downscript=no &

```
Note that it sets `-display none` and uses '&' to run it as a background process.
Also I move my `antixvm.qcow2` file to `/usr/local/qemu`

3. Then reboot NetBSD and all I see is a 'Starting local daemons:'in the boot sequence. 
When I look at the interfaces
```
trinity: {4} ifconfig
...
tap0: flags=0x8843<UP,BROADCAST,RUNNING,SIMPLEX,MULTICAST> mtu 1500
	ec_capabilities=0x5<VLAN_MTU,JUMBO_MTU>
	ec_enabled=0
	description: "AntixVM"
	address: f2:0b:a4:a9:06:ff
	status: active
	inet6 fe80::f00b:a4ff:fea9:6ff%tap0/64 flags 0 scopeid 0x5
	inet 10.0.2.1/24 broadcast 10.0.2.255 flags 0
```
The tap0 interface is active and has an IP address. 
I can ping the VM,
```
trinity: {5} ping 10.0.2.15
PING 10.0.2.15 (10.0.2.15): 56 data bytes
64 bytes from 10.0.2.15: icmp_seq=0 ttl=64 time=0.397531 ms
```
which means the Antix VM is running and configured.
I can do the NFS mount
```
# Mount -t nfs 10.0.2.15:/mnt/common /mnt/common
# df
System              512-blocks         Used        Avail %Cap Mounted on
/dev/dk12                607675824     47963728    529328312   8% /
/dev/dk16                  1046488         2136      1044352   0% /mnt/EFI/boot
kernfs                           2            2            0 100% /kern
ptyfs                            2            2            0 100% /dev/pts
procfs                           8            8            0 100% /proc
tmpfs                     33540464            0     33540464   0% /var/shm
/dev/dk18                 40224680      4451608     33725176  11% /mnt/share
10.0.2.15:/mnt/common   4029495000    210612288   3614050048   5% /mnt/common
```
and there is my 'common' ext4 partiton in the last line of the `df`.

I chose not to automate the mount by putting an entry in `/etc/fstab` but that option is available

#### Concluding Remarks ####
Now that I have it automated, it is really quite efficient and goes unnoticed. It does not cause a boot delay. The memory usage is 1.41Gb with the VM running and 0.75  Gb without the VM running. So the Antix VM uses 0.7Gb. I could probably prune it to use less, but it is not worth the effort for me.

One word of WARNING. The `-drive` option in `qemu` which accesses my disk partiton does a raw physical mount of the entire disk, then picks out my nominated partition. One would not want that disk or partiton to be active when accessed as a raw device. Therefore one should not use this procedure if the desired partition is on the same disk as the NetBSD root partition, because the NetBSD root partiton will be active. Fortunately , in my case , my 'common' data partition was on a separate disk. 

It is rather clumsy , running an entire VM just to get a mount of an ext4 partition. It works because NFS has its own pseudo-filesystem and it converts everything to that before exporting, so that what NetBSD gets to work with is the NFS pseudo-filesystem, and it can cope with that. I knew , before I started this, that NFS would work between 2 separate computers with different incompatable filesystems ... I just needed to develop it for one hard install and a VM.

If anyone knows a better method, please let us know.
