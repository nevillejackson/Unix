## Using qemu-nbd to mount a filesystems within a qcow2 file ##

Virt-manager stores images of its virtual machines at the location
`/var/lib/libvirt/images` by default.
Most users would prefer a separate partition or a directory within 
one's home directory. 
For example I have a separate partition

```
$ df
Filesystem      1K-blocks      Used Available Use% Mounted on
/dev/sdb14      329584540  26947832 285821604   9% /qemuvm
```

So I make `/var/lib/libvirt/images` a link

```
$ pwd
/var/lib/libvirt
$ ls -l
lrwxrwxrwx 1 root root       7 Jun  6 17:47 images -> /qemuvm
```

Inside `/var/lib/libvirt/images` one normally finds `.qcow2` files

```
$ ls images
MXahs.qcow2  debian.qcow2  lost+found
```

`qcow2` is the default image file format used by virt-manager.

Inside those `.qcow2` files are images of the guest VM's
filesystem. It is possible to mount those filesystems buried within the .qcow2 file
using the `qemu-nbd` command.

The `ndb` stands for Network Block Device, a protocol used
for accessing disks or partitions across a network.
The program `qemu-nbd` comes in the package `qemu-utils` in Debian and Ubuntu. 

### Using `qemu-nbd` to mount a qcow2 disk image ###
There are 4 steps
 
1 .  load the nbd kermnel module

```
# modprobe nbd max_part=8
```

2 .  mount the qcow2 disk image

```
# qemu-nbd -c /dev/nbd0 --read-only /var/lib/libvirt/images/MXahs.qcow2
```

This makes a device `/dev/nbd0` 

3 . List the partitions within the nbd device

```
 # fdisk -l /dev/nbd0
Disk /dev/nbd0: 25 GiB, 26843545600 bytes, 52428800 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0xe1b0ab5a

Device      Boot    Start      End  Sectors  Size Id Type
/dev/nbd0p1 *        2048 49793023 49790976 23.7G 83 Linux
/dev/nbd0p2      49793024 52414463  2621440  1.3G 83 Linux
```

4 . Mount the required partition, in this case /dev/nbd0p1 is
the Linux partition

```
# mount /dev/nbd0p1 /mnt/mxahs
mount: /mnt/mxahs: WARNING: source write-protected, mounted read-only.`
```

It only allows a read-only mount because we specified read-only in step 2
We can now look at the MXahs filesysten

```
# ls /mnt/mxahs
bin   dev  home  lib64	 lost+found  mnt  proc	 root  sbin  tmp  var
boot  etc  lib	 libx32  media	     opt  pulse  run   sys   usr
```

We see the MXahs root filesystem

5 . Cleanup

Unmount the filesystem

```
umount /mnt/mxahs
```

Disconnect thee nbd device

```
qemu-nbd --disconnect /dev/nbd0
```

Remove the module

```
rmmod nbd
```

### Warning ###
One should only do this when the guest is not running, particularly if the mount is in rw mode. 
Writing on a running guest filesystem can cause irreversible 
damage to the qcow2 file.

### Recommendation ###
A qemu-nbd mount is useful if you have done something 
mistaken to the guests filesystem ( eg editing /etc/fstab)
and it will not boot. 
One can mount the qcow2 file in rw mode and repair the guest filesystem so it will 
boot. One needs to do this with due care, heeding the warning above.

It may also occasionally be useful to do a qemu-nbd mount  in read only mode 
in order to transfer files to or from the host, but there are other 
ways of doing that. 







### LInks ###

https://en.wikipedia.org/wiki/Network_block_device

https://www.qemu.org/docs/master/tools/qemu-nbd.html

https://gist.github.com/shamil/62935d9b456a6f9877b5

