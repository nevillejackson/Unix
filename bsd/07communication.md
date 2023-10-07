## Communication between Linux host and FreeBSD guest. ##
There are two approaches to communication between host and virtual machine. One
can transfer files, or one can attempt to mount a shared directory. 

### File transfers between Linux host and FreeBSD guest. ###
We have tested all of the procedures involving use of ssp, sftp, rsync, and Thunar described in the topic 

https://itsfoss.community/t/communication-between-host-and-guest-vm-when-using-virt-manager/10841

They all work for the case of Linux host and FreeBSD guest. 

### File sharing between Linux host and FreeBSD guest ###
File sharing involves mounting a host filesystem from inside the guest. In the case where the guest is FreeBSD, one immediately encounters the problem of filesystem types being different in Linux host and FreeBSD guest. Linux is typically ext3 or ext4. FreeBSD is UFS2 (in the present case) and can be ZFS. 

#### Virtiofs mounts ####
FreeBSD does not seem to have support for `virtiofs` mount types.

#### Virt9p mounts ####
FreeBSD does not seem to support `virt9p` mount types under KVM. There is a statement in relation to FreeBSD13.0

"The bhyve(8) hypervisor
includes several changes......, VirtIO 9p filesystem sharing, ...". 

which indicates that a `virt9p` mount may be possible if one used the `bhyve` hypervisor instead of KVM. Bhyve is not available in Linux, it requires the FreeBSD kernel.

#### NFS mounts ####
NFS mounts work between a Linux host as the server and
 a FreeBSD guest as the client.

1 . In the Linux host one must setup the NFS server, as described in 
https://itsfoss.community/t/communication-between-host-and-guest-vm-when-using-virt-manager/10841

and export the required filesystem to the UID of the FreeBSD guest.

2 .  FreeBSD already has the NFS client in the installed base system, so all we need to do is enable it

```
Edit /etv/rc.conf
nfs_client_enable="YES"
```
then either reboot or start the service with

```
service nfsclient start
```

3 . Do the NFS mount , for example I have exported from the host an ext4 filesystem called `/common`, and made a mount point in the FreeBSD guest for it called 
`/mnt/host-common`, so the mount is

```
mount -t nfs -o nfsv3,nolockd trinity:/common /mnt/host-common
```

4 . Verify that the shared filesystem is mounted properly and that files can be read and written

```
#df
Filesystem       1K-blocks      Used     Avail Capacity  Mounted on
/dev/vtbd0p2      38579228   7646984  27845908    22%    /
devfs                    1         1         0   100%    /dev
procfs                   4         4         0   100%    /proc
trinity:/common 1116209476 108396248 951086292    10%    /mnt/host-common
#cd /mnt/host-common/tmp
#ls
.RData                          junk3
.Rhistory                       kdefonts
ab3220wslibas.df.2511.rda       slim.conf.devuan
junk                            slim.out
junk2
#cat junk
[*] font-alias-1.0.4_2                                          Standard aliases
 for X11 PCF fonts
....
#cat > junk4
abcd
CTRL D
#cat junk4
abcd
```

So the NFS mount works. FreeBSD can read and write to an ext4 filesystem on the host.
The mount can be put in `/etc/fstab` if required at every boot

If one requires a shared filesystem for a FreeBSD guest, the way to
do it is to use an NFS mount.  It does not matter that the Linux host filesystem will most likely be ext3 or ext4, while a FreeBSD guest will most likely use UFS or ZFS. All filesystem operations via an NFS mount  go thru the NFS server on the host, so the host system does all file operations and the daemon passes the result to the guest. Therefore one can be confident that an ext3/4 filesystem will not be corrupted if shared. 


#### Using qemu-nbd to mount a filesystems within a qcow2 file ####
NBD stands for Network Block Device, a protocol used
for accessing disks or partitions across a network.
It can be used to access the `freebsd13.1.qcow2` file as a block device, and to mount selected partitions within that device.
The steps are 

1 . Install the qemu-nbd package ( called `qemu-utils` in Debian and Ubuntu

2 . Load the NBD kernel module in the host

```
# modprobe nbd max_part=8
```

3 . Access the qqcow2 disk image, and list the partitions within it

```
# qemu-nbd -c /dev/nbd0 --read-only /var/lib/libvirt/images/freebsd13.1.qcow2
# fdisk -l /dev/nbd0
Disk /dev/nbd0: 40 GiB, 42949672960 bytes, 83886080 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: gpt
Disk identifier: 288EFBBB-4FC9-11EE-B5A9-237CFC61CBCA

Device         Start      End  Sectors  Size Type
/dev/nbd0p1       40     1063     1024  512K FreeBSD boot
/dev/nbd0p2     1064 79691815 79690752   38G FreeBSD UFS
/dev/nbd0p3 79691816 83886039  4194224    2G FreeBSD swap
# 
```
 
4 . Make sure the host can support UFS filesystems


```
# modprobe ufs
# lsmod | grep ufs
ufs                    94208  0
```

5 . Mount the required partition

```
# mount -t ufs -o ufstype=ufs2 /dev/nbd0p2 /mnt/fbsd
mount: /mnt/fbsd: WARNING: source write-protected, mounted read-only.
```

It allows read only because we specified read only in step 3.

6 . Look at the FreeBSD filesystem

```
# ls /mnt/fbsd
COPYRIGHT  boot  entropy  home	libexec  mnt  proc    root  sys  usr
bin	   dev	 etc	  lib	media	 net  rescue  sbin  tmp  var
# ls /mnt/fbsd/usr
bin   include  lib32	libexec  obj	sbin   src
home  lib      libdata	local	 ports	share  tests
# ls /mnt/fbsd/usr/local
bin  include  libdata  llvm15  openssl	share
etc  lib      libexec  man     sbin	sys
# 
# cat /mnt/fbsd/COPY*
#	@(#)COPYRIGHT	8.2 (Berkeley) 3/21/94

The compilation of software known as FreeBSD is distributed under the
following terms:

Copyright (c) 1992-2021 The FreeBSD Project.

Redistribution and use in source and binary forms, with or without
......
```

7 .  Cleanup

```
umount /mnt/fbsd
qemu-nbd --disconnect /dev/nbd0
rmmod nbd
```

**Warning: One should only do this when the guest is not running,
 particularly if the mount is in rw mode. 
Writing on a running guest filesystem can cause irreversible 
damage to the qcow2 file.**

A qemu-nbd mount is useful if you have done something 
mistaken to the guest's filesystem ( eg editing /etc/fstab)
and it will not boot. 
One can mount the qcow2 file in rw mode and repair the 
guest filesystem so it will 
boot. One needs to do this with due care, heeding the warning above.


