### Filesystems in NetBSD - sharing with Linux ###

NetBSD has native support for FFS, ZFS, EXT2FS, NTFS, MSDOSFS, CD9600, and UDF filesystems.
Linux has native support for ext2, ext3, ext4, Btrfs, NTFS,  and FAT32 filesystems.
The only overlaps between NetBSD and Linux are
 - EXT2FS ( ext2)
 - MSDOSFS (FAT32)
 - NTFS ( NTFS)
Linux does have optional UFS(read only) support, and this is the same as what NetBSD calls FFS.
There is also UFS2 which is FFS2.

The critical point is that NetBSD can only mount filesystems that its kernel supports. That makes setting up a shared filesystem restrictive, and mounting of commonly used Linux filesystems, like ext4, impossible.

So how are we to use NetBSD as a work-horse in a computer which also has Linux?
Solving that is what this topic is about.

A shared filesystem is one to which both OS's can read or write individual files. It obviously has to be mountable by both NetBSD and Linux. 
The only feasable choices are EXT2FS(ext2 in Linux), MSDOSFS(fat32 in Linux), and NTFS.
I chose ext2.
Here are the steps:

 1. Make a new filesystem with `gparted` and format it to ext2.  I made a 20Gb partiton on the same disk as my NetBSD install.

 2. In Linux ( in my case MX) setup `/etc/fstab` to mount this partiton at boot.
```
# Shared ext2 partition
UUID=5584149f-d45e-4800-acc4-65995ee8c5f6 /mnt/share ext2  rw  0 2
```
with that entry added to `/etc/fstab` it mounts the 20Gb ext2 partiton at boot and it shows in my `df` as
```
$ df
Filesystem      1K-blocks      Used  Available Use% Mounted on
.....
/dev/sdb8        20112340       292   19088100   1% /mnt/share
```

 3. In NetBSD one needs to find the name of the shared partiton
```
$ dmesg
....
[     3.725782] wd2 at atabus18 drive 0
[     3.725782] wd2: <ST2000DM001-9YN164>
[     3.725782] wd2: drive supports 16-sector PIO transfers, LBA48 addressing
[     3.725782] wd2: 1863 GB, 3876021 cyl, 16 head, 63 sec, 512 bytes/sect x 3907029168 sectors (4096 bytes/physsect; first aligned sector: 8)
[     3.745782] wd2: GPT GUID: d0128e01-944a-48c0-9bce-7dfbe442e609
[     3.745782] dk10 at wd2: "LinuxRH2Tbhdd", 390189056 blocks at 1052672, type: ntfs
[     3.745782] dk11 at wd2: "NetBSD", 612485120 blocks at 391241728, type: ffs
[     3.745782] dk12 at wd2: "Linux-swap", 409597952 blocks at 1024208896, type: ext2fs
[     3.745782] dk13 at wd2: "ac7c8c29-3fee-48bc-be5c-379840db8f98", 2268422144 blocks at 1638606848, type: ext2fs
[     3.745782] dk14 at wd2: "BIOS-boot", 2014 blocks at 34, type: <unknown>
, 2268422144 blocks at 1638606848, type: ext2fs
[     3.745782] dk15 at wd2: "EFI system", 1048576 blocks at 4096, type: msdos
[     3.745782] dk16 at wd2: "NetbsdSwap", 20482048 blocks at 1003726848, type: swap
[     3.745782] dk17 at wd2: "ShareExt2", 40957952 blocks at 1433806848, type: ext2fs
[     3.745782] wd2: drive supports PIO mode 4, DMA mode 2, Ultra-DMA mode 6 (Ultra/133), WRITE DMA FUA, NCQ (32 tags)
[     3.745782] wd2(ahcisata6:0:0): using PIO mode 4, DMA mode 2, Ultra-DMA mode 6 (Ultra/133) (using DMA), NCQ (31 tags)


```
So it is "ShareExt2" , on the partiton NetBSD calls `/dev/dk17` and Linux calls `/dev/sda8`
That NAME "ShareExt2" comes from the partiton name I setup in `gparted`

Then we can modify the NetBSD `/etc/fstab` to mount it at boot
```
# NetBSD /etc/fstab
# See /usr/share/examples/fstab/ for more examples.
NAME=NetBSD             /       ffs     rw               1 1
NAME=EFI\ system /mnt/EFI/boot  msdos   rw               0 0
NAME=NetbsdSwap         none    swap    sw,dp            0 0
kernfs          /kern   kernfs  rw
ptyfs           /dev/pts        ptyfs   rw
procfs          /proc   procfs  rw
/dev/cd0a               /cdrom  cd9660  ro,noauto
tmpfs           /var/shm        tmpfs   rw,-m1777,-sram%25

NAME=ShareExt2    /mnt/share   ext2fs    rw      1 1
```
We can see that NetBSD is different to Linux, it uses partiton names rather than UUID's in its `/etc/fstab`.  
With that entry added to `etc/fstab` in NetBSD, it mounts the 20Gb EXT2FS partiton at boot and it shows in my `df` as

```
trinity: {1} df
Filesystem     512-blocks         Used        Avail %Cap Mounted on
/dev/dk11       607675824     10673184    566618856   1% /
/dev/dk15         1046488         1560      1044928   0% /mnt/EFI/boot
kernfs                  2            2            0 100% /kern
ptyfs                   2            2            0 100% /dev/pts
procfs                  8            8            0 100% /proc
tmpfs            33540464            0     33540464   0% /var/shm
/dev/dk17        40224680          584     38176200   0% /mnt/share

```
If one wanted to mount the shared filesystem manually (instead of in fstab) the command in NetBSD is
```
mount -t ext2fs /dev/dk17 /mnt/share
```

  4. With that setup I was able to share files. That is how I copied the `fstab` and `df` outputs from NetBSD to MX, where I am writing this report. 
  I had no trouble sharing text files , or images. 
The issue with sharing is, it is rather limited. I cant realistically copy my large ext4 data filesystem into the share filesystem. It is only suitable for shareing a few small files.

That is not a complete solution to using NetBSD alongside Linux, but it is a start.



