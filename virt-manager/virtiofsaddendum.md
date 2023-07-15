# Addendum #

## Making a virtiofs mount permanent ##
See reply No 3 in this topic for information on setting up a virtiofs mount 
in virt-manager.

Unfortunately, in reply No.3 "Virtiofs filesystem mounts" we made the statement


>  "It is not possible to put a virtiofs mount in /etc/fstab
 in the guest system. You have to remount it every time
 you boot the guest system." 

That statement is incorrect. 

To make a virtiofs mount 
occur automatically every time a guest VM is booted,
put the following entry in `/etc/fstab` in the guest system

```
mnt-tag  /mnt/mymount  virtiofs  rw,relatime  0 0
```

### Using virt9p instead of virtiofs ###
Virtiofs is only supported in systems with kernel version 5.4 or greater
and it requires `libvirt6` or greater. 
An alternative in older systems is to use `virt9p` in place of `virtiofs`.
The setup is the same, except choose `virt9p`

The mount statement in the guest for virt9p is

```
mount -t 9p mount-tag /mnt/mymount
```

and the line to put in `/etc/fstab` is

```
mount-tag /mnt/mymount 9p rw,relatime 0 0
```

### Alternatives to using /etc/fstab ###

A search for alternatives considered using `/etc/rc.local` but that is 
deprecated and not available in all distros. 
The most promising alternative seemed to be to use `cron`.

### Setting up crontab to do a mount on reboot ###
The following steps are needed in the guest system

 - setup a virtiofs mount as in reply No 3.
 - make sure the `cron` daemon is running 
 - make a script file to do the mount

```
#! /bin/bash
mount -t virtiofs mount-tag /mnt/mymount
```

call the file mountscript.sh

 - put the script file somewhere like `~/bin` 

 - make the script file executable

 - as superuser, edit the system-wide crontab file `/etc/crontab` with any text editor, and add the line

```
@reboot root /path/to/mountscript.sh
```

   -  Reboot and it should mount automatically
   
### Permission issues with virtiofs mounts ###
   The shared file on the host  that is going to be mounted by the guest 
   should ideally be owned by the same user and group as is used in the guest, and the UID and GID should be the same too.
   
   If this is not the case the same permission rules apply as would apply within a single operating system. So a different user in the guest may only be able to read files in the mounted filesystem, depending on its permissions for group and other.  
   
   
### Copy/paste issues with virt-manager ###
   If one wishes to copy/paste between a guest window and the host, it is 
   necessary to install the package `spice-vdagent` in the guest.
   After this package is installed there should be 2 daemons running in 
   the guest
   
```
   $ ps ax | grep spice
   1089 ?        Ssl    0:00 /usr/sbin/spice-vdagentd
   1488 ?        Ssl    0:00 /usr/bin/spice-vdagent
```
  If they are not both running, try a reboot. If that fails start them by hand.
  
### Links ###
  
  https://ravada.readthedocs.io/en/latest/docs/Virtiofs.html
  
  https://unix.stackexchange.com/questions/109117/virt-manager-copy-paste-functionality-to-the-vm
  
  
   
   
