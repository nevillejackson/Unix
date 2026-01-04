
 This is a joint post by @kovacslt and @nevj
 [quote="Laszlo Kovacs, username:kovacslt"]
 [/quote]
 and
 [quote="Neville Jackson, username:nevj"]
 [/quote]


## Using the CLI to backup or copy a Linux system ##

There are several programs commonly used to backup and restore the root filesystem of a Linux installation. A number of them ( borg, systemback, timeshift)  use rsync 'under the hood' . However it is possible to use `rsync` directly , as a CLI command. It is also possible to use `tar`. Some users prefer CLI , because it gives them full control over the backup and restore processes. 

There are other possibilities, such as `cp` and `dd`. These are not advisable for a number of reasons.  We will only investigate `tar` and `rsync` here.


### Using rsync to backup an OS ###
There are a number of precautions to consider when using `rsync` to copy Linux. 
We will look at the following
 -  choosing the most appropriate command line options
 -  using rsync with the root filesystem unmounted
 - using rsync from within the running Linux to be copied.
 - what to do  if you copy (or restore) Linux to a different partition


The general form of the `rsync` command is 
```
rsync <options> <source directory> < destination directory>

```
It is important, when copying an operating system, to choose appropriate options, so that file system features like dates, permissions, links are preserved.
I ( nevj) have always used `rsync -aAXvH ` , but on consulting the experts here

```
https://superuser.com/questions/307541/copy-entire-file-osystem-hierarchy-from-one-drive-to-another
```

it is indicated that one should also have options 'x' and 'S',  so what we should recommend is

```
rsync -avxHAXS  <source directory> <destination directory>
```

Let us have  a look at the meaning of each of these options
 - *-a* .. all files with attributes ( permissions, dates, ..). Commonly called archive mode. It operates recursively.... ie copies all subdirectories within the source. 
 - *-v* .. verbose
 - *-x* .. stay on one filesystem ( ie do not follow links to other partitions)
 - *-H* .. preserve hard links
 - *-A* .. preserve ACL's
 - *-X* .. preserve extended attributes 
 - *-S* .. copy sparse files

These options are all designed to ensure the copied OS filesystem is exactly the same as the original. 
The 'x' option will ensure that if, for example, your home directory is on a separate partition, it will not be copied if you copy '/'.
There could be occasions when one might wish to omit the 'x' option, but the others are compulsory for copying an OS.
You might encounter sparse files if your Linux uses docker containers.

Another option that may be required is `--exclude`. For example
```
rsync --exclude={/dev/*,/proc/*,/sys/*,/run/*}
``` 
will not copy the virtual filesystems dev,proc,sys, and run.  They will appear in the copy as empty directories. 
This might also be use to not copy mount points ( /mnt/* and /media/*) or to not copy the home directory (/home/*)
The curly brackets above are a `bash` shell construct. If you are not using `bash` it is necessary to list each excluded filesystem separately, ie
```
rsync --exclude-/dev/* --exclude=/proc/* --exclude=/sys/* --exclude=/run/*
```
for the above case.

Other options that may  sometimes be usefile are
 - *--delete* ..  deletes files in the destination that are not present in the source
 - *--link-dest* ..  Hey, I dont like this, it is too complicated.
 - *--filter=protect* -- protects files in the destination from being deletd even if `--delete` is used. 

We shall illustrate how apply these options with a series of examples.


 Example 1.  Rsync an unmounted root filesystem.
If you look at the filesystem of a Linux other than the one you have currently running, you will see that the filesystems /dev, /proc, /sys, and /run contain no files.  
These filesystems are populated at boot time and held in ramdisk.... they are not saved to disk at shutdown. 
Therefore, one can copy this unmounted root filesystem to another partition with the following steps
 - if the destination is a usb flash drive, format it to ext3 or ext4... do not use a fat32 formatted flash drive. 
 - if the destination is another hard disk partition it must, of course , be formatted
 - mount the source filesystem, and the destination filesystem
 - `rsync  -avxHAXS  <source directory> <destination directory>`
 - there may be cases where you would not use `-x`, for example if /boot were a separate partition.
 

Example 2. Rsync within a running Linux system 
To copy the root filesystem of the current running Linuxto a disk, you need to avoid copying /dev, /proc, /sys, and /run. So the `--exclude` option is needed. 
You should also take precautions to prevent the system writing to the root filesystem while it is being copied
 - if it is a multi-user system, go into single user mode
 - avoid doing anything while the rsync copying is active. In particular do not do an update/upgrade, suppress any cron jobs, and avoid doing anything yourself at the GUI or terminal. 
  
The destination directory needs to be formatted and mounted, If it is a USB flash fdrive, do not leave it as a FAT32 filesystem, format it to ext4.

The rsync command should be
```
rsync --exclude={/dev/*,/proc/*,/sys/*,/run/*} -avxHAXS  <source directory> <destination directory>

```
Again there may ba cases where you would not use `-x`
It may also be desirable to exclude `/mnt/*,/media/*, /tmp/*`

Exanple 3. Recovering the OS from an rsync'ed copy 

Recovery is easy.  In most cases the 'damaged' linux which one wishes to recover is not able to run, so one uses another linux to do the recovery with rsync. 
 - start some other Linux ( eg a live usb drive)
 - mount both the source directory ( ie the copy) and the destination directory ( ie the partiton to receive the recovery)
 - issue an rsync command

```
rsync  -avxHAXS --delete  <source directory> <destination directory>
```

The `--delete` option allows you to restore linux  over the top of a presumably damaged linux filesystem. If you delete the root filesystem or reformat the partition before restoring, `--delete` is not necessary. 
There is no need for the '--exclude=' options in a recovery, provided the pseudo-filesystems (proc, dev, sys, and run) are not present on the backup file..
Reboot and start the recovered Linux. 

Trying to recover to a 'live' running Linux has not been tested. The following may work. At your own risk.

```
rsync  -avxHAXS --delete --filter=protect {/dev/*,/proc/*,/sys/*,/run/*} <source directory> <destination directory>
```

Here  source directory would be the backup file, and destination directory would be `/`
The `--filter=protect` option will protect the /proc,... files in the live system from being written on or deleted. 

### Using tar to backup an OS or a filesystem ###

The utility "tar" is there exactly for backup of a filesystem. The name "tar" means "tape archive" and its original use was for copying a filesystem to magnetic tape. Today it is used mainly for arcchiving filesystems to disk.

General usage is 

```
tar [options] [file] [file]...
```

I like to use tar to backup complex directory structures with many subdirectories, possibly having symlinks in them, etc. Such a complex thing may be a WINE prefix in a users home directory, or a home directory as a whole, but also could be the complete installed OS.
The first example I show is  how I backed up a WINE prefix, so that it can be moved between computers easier.

 <sup>(Fun fact, that this WINE prefix had an activated instance of MS Office 2007, which I expected to have to reactivate on the other computer after moving there, but instead the activation survived the moving.)</sup>

```
tar -czvpf ~/winebackup.tar.gz --directory ~ .wine 
```

Let's break up this line, what it does:
-executes tar command
-passes options: `-czvpf`

The options are:

- `c` : create archive
- `z` : use gzip compression
- `v` : verbose output (useful to track what it does)
- `p` : preserve file permissions
- `f` : specify the file for archive

`~/winebackup.tar.gz` will be the archived file. As it suggests, the name will be "winebackup.tar.gz" in the home directory of the current user. 
`--directory ~`tells tar where to work. It is equivalent of changing to that dir beforehand and issueing the tar command there. Such as `cd ~ ; tar -c....`
`.wine`is the file (directory) to backup. All of its contents will be addded to the archive.

Now to "restore" the archive:

```
tar -xzvpf ~/winebackup.tar.gz  --directory ~
```

Note the options given to `tar`:
- `x` : tells its about extracting. 

All the other options mean exactly the same as above.
The filename of the archive from which the contents are extracted, has to be specified . Without specifying what to extract, tar will exctract everything. The  
 `--directory ~`   
tells here again, where to work, we may restore that dir into an other directory.

Backing up the whole OS works similarly, there are some caveats.
First, avoid  archiving the resulting archive file itself.
Say one would like to put the system archive into to root, such as `/os_backup.tar.gz`, it is best to tell tar to exclude it:

```
--exclude=/os_backup.tar.gz
```
 
Such a command line to backup the whole OS would look like:

```
tar --exclude="/mnt" --exclude="/os_backup.tar.gz" --exclude="/proc" --exclude="/sys" --exclude="/media" --exclude="/mnt" --exclude="/run", --exclude="/tmp" --exclude="/var/cache" --exclude="/var/lib/udisks2" --exclude="/var/run" --exclude="/var/tmp" -czpvf /os_backup.tar.gz / 
```


This creates a `os_backup.tar.gz` file in the root, from the contents of the `/` (root of the filesystem). It is necessary to think about what to exclude, as my example can be incomplete. For example, if a heavily used database server is running, it is wise to avoid backing it up this way, so it would be necessary to exclude /var/lib/mysql. 
I won't say this is the best practice, but I separate backing up the system itself, and backing up the data handled by the system.
So backing up a database would be a separate process.

As for restoring an OS, it's basically   process of extracting the tar archive. It will overwrite all existing files -libraries, executables, config files, etc., on the system, thus making it functional again after an unsuccessful tinkering. However won't remove any residues of the previous failed installation of something.  So unused/unneeded libraries may be still there, unless the filesystem is cleared.  Hence the rsync method is much better, for whole OS backups.


### Booting a copied Linux ###
If you move a Linux root filesystem to another disk, there are modifications required before it can be booted in that new location.

If your Linux root filesystem copy is a `tar` archive,  you obviously have to extract the filesystem from the archive and put it on a partiton, before you can consider booting. Other than that, boot considerations for a `tar` copy are the same as for an `rsync` copy.

 1. There must be a bootloader somewhere that can 'see' this Linux filesystem. In the case of an `ext` formatted USB flash drive, it is possible to add a bootloader to the flash drive . There are instructions for adding grub to a USB drive here

```
https://github.com/nevillejackson/Unix/blob/main/grub/makeusb.pdf
```

If the moved Linux filesystem is on an internal disk partition, the grub which boots your normal linux can be used. Run the following command in your normal Linux ( ie the one that controls grub)

```
update-grub
```

That should run os-prober and find the new Linux copy. If you then reboot, the grub menu should offer the new Linux copy, as well as other Linuxes you may have installed.

 2. The bootloader will not boot this new Linux, unless you modify its `/etc/fstab` file.  Use the command 

```
blkid
```
to get a list of the UUID's of all partitions on your system. You may have to install `blkid`. Then open  mount the new Linux's filesystem and cd into it

```
mount -t ext4 /dev/.... /mnt/new
cd /mnt/new
```
then edit the file  `/mnt/new/etc/fstab` in an editor , find the line that defines the root filesystem ( called /), delete its UUID, and copy/paste the new partition's UUID in its place. Do the same for /boot if it is a separate partition. The other partitions (swap and /home) should be correct.

 3. The bootloader may also have problems if the file`/boot/grub/grub.cfg` contains references to the original root filesystem. There are 3 ways to fix this
 - mount the new filesystem as above, `cd` into it, and delete the file /mnt/new/boot/grub/grub.cfg`.  It will boot without a grub.cfg
 - mount the new filesystem as above, `cd` into it, and edit the file `/mnt/new/boot/g
rub/grub.cfg`.  Change any UUID's as in fstab, and check that the line `root=...` is correct. 
 - boot to the grub menu, then choose the new Linux copy, type `e` to go into edit mode, and change the UUID and `root=..` as above. Then press F10 to boot , and when it boots run `update-grub` and it will automatically fix the file `grub.cfg` in the booted newlinux.  

After doing the modifications 1. 2. and 3. you should boot the Linux that controls grub and do `update-grub`.  Then the new Linux copy should boot from the grub menu. 

If you were then to use this 'moved'  Linux for a restore, you would have to go thru all the above steps again on the restored copy.  So, if you are wanting to use a Linux copy for a restore, it is best not to make it bootable. 


### Discussion ###
These procedures can also be used to permanently move an installed Linux to another partition. 

Using tar or rsync without compression is the fastest way to backup an entire root filesystem. If you do this regularly, and keep copying to the same partiton, rsync  will archive files incrementally, which can be really fast, but you would need to use `--delete` option to ensure files in the destination that are not in the source are deleted. 

The general principles for snapshotting an OS are
  - find a reliable procedure that suits you and stick to it
  - if copying a 'live` system , make sure it is inactive, and exclude pseudo-filesystems
  - test your restore procedure

### Acknowledgement ###

We would like to thank @easyt50 and @ihasama for prompting us to tackle this topic.


