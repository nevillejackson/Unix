## Virtiofs filesystem mounts ##

The easiest way to mount a host filesystem in a guest VM is to use the virt-manager graphical interface. 
Virt-manager provides two protocols for a shared filesystem, called `virtiofs` and `virt9p`.  The former is considered superior. 

The Linux `mount` statement normally mounts a device. What the virtiofs protocol provides is a means to pretend that a filesystem is a device (of type `virtiofs`), so that it can be mounted, by a normal `mount` statement.

### Preparing virt-manager to do a virtiofs mount ###

Working in the host system:

1. Open virt-manager and choose a guest system, so that it opens the QEMU/KVM window, but do not run the guest system

2. Use the `View` tab, and choose `Details`

3. Click on `Memory`  and tick the option "Enable Shared Memory".
     Click on `Apply`
     
4. Clck on `Add Hardware` at the bottom left.
  A subwindow called "Add New Virtual Hardware" should appear.
  Select `Filesystem` from the left panel
  Select Driver = virtiofs
  Under Source Path enter the directory name in the host that you wish to mount in the guest
  Under Target Path enter an arbitrary tag-name. This option is confusing... it is not a path at all , it is an arbitrary tag used by the mount statement later on.
  Click on finish
  
### Do the mount in the guest VM ###
 
 1. Run the guest VM
 
 Working in the guest system:
 
 2. Make a mount point
 
```
mkdir /mnt/mymount
```


 3. Do the mount statement , usin the mount-tag created in step 4 above     
 
 ```
 mount -t virtiofs mount-tag /mnt/mymount
 ```
 
 4. You can check it is there with `df` and examine the contents with
 
 ```
 ls /mnt/mymount
 ```
 
 5. When finished , unmount it
 
 ```
 umount /mnt/mymount
 ```
 
### Recommendation ###
Use a virtiofs mount if you want to work in the guest on a host filesystem and prefer a graphic interface. 
It is not possible to put a virtiofs mount in `/etc/fstab` in the guest system. You have to remount it every time you boot the guest system.







### Links ###
https://www.debugpoint.com/share-folder-virt-manager/


