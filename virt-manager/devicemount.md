## Device mounts ##
Virt-manager can make host devices (such as a hard disk partition) directly available to a guest VM. 

*A word of caution is necessary here.
 If the host and guest access the same disk partition with 
write permission, data corruption can occur.*

It is recommended that you only make available to a guest disk partitions that are not mounted by the host. 

The steps are:

### Find the physical disk partition you want to add to virt-manager ###
We need the actual id-name of the disk partition.

```
$ ls -l /dev/disk/by-id/
total 0
lrwxrwxrwx 1 root root  9 Jun 28 20:32 ata-ST2000DM001-9YN164_S2408NP5 -> ../../sda
lrwxrwxrwx 1 root root 10 Jun 28 20:32 ata-ST2000DM001-9YN164_S2408NP5-part1 -> ../../sda1
.....
lrwxrwxrwx 1 root root 10 Jun 28 20:32 ata-ST2000DM001-9YN164_S2408NP5-part3 -> ../../sda3
....

```

As an example we are going to use partition `sda3`
 which has the name `LinuxHome1` and contains
 the home directory for another Linux which is not
 the host, and is not mounted to the host. 

### Add this partition to the guest VM using virt-manager ###
On the QEMU/KVM screen select View -> Details -> Add Hardware
In the Add Hardware submenu choose Select or create custom storage. 

Then next to Manage... enter the partition name
`/dev/disk/by-id/ata-ST2000DM001-9YN164_S2408NP5-part3 -> ../../sda3`
and set Bus type to SATA.
and Finish

An entry `SATA DISK 1` should now appear in the Details menu

### Mount the added device in the guest system ###
Run the guest VM.
Identify the partition name used by the guest for our added partition

```
# blkid
/dev/vda1: LABEL="rootMX21" UUID="a9db9817-7d2f-43f2-811b-21c808e38a79" BLOCK_SIZE="4096" TYPE="ext4" PARTUUID="e1b0ab5a-01"
/dev/vda2: LABEL="swapMX" UUID="f38db7a9-6c8b-4bcc-8475-459eb4ef95f7" TYPE="swap" PARTUUID="e1b0ab5a-02"
/dev/sda: LABEL="LinuxHome1" UUID="895f2f7d-978f-499e-b958-cba663f88121" BLOCK_SIZE="4096" TYPE="ext4"

```

So in this case our "LinuxHome1" is called /dev/sda in the guest.

Make a mount point
`mkdir /mnt/partmnt`

Mount it

```
# mount /dev/sda /mnt/partmnt

# df
Filesystem       1K-blocks      Used Available Use% Mounted on
udev               4950644         0   4950644   0% /dev
tmpfs              1002028      1000   1001028   1% /run
/dev/vda1         24328544   7879712  15187676  35% /
tmpfs                 5120         8      5112   1% /run/lock
tmpfs              2266180         0   2266180   0% /dev/shm
cgroup                  12         0        12   0% /sys/fs/cgroup
tmpfs              1002024         4   1002020   1% /run/user/115
tmpfs              1002024        12   1002012   1% /run/user/1000
/dev/sda         201390520  22538472 168595836  12% /mnt/partmnt

```
It is now  mounted.
Can we read it

```

# ls /mnt/partmnt
lost+found  nevj

```

OK , there is my home directory. 
Finish up

`umount /mnt/partmnt`

### Recommendation ###
Use a device mount only if you really need to mount a whole partition or device. Otherwise filesystem mounts ( virtiofs or nfs) are safer.

## Links ##
https://ckirbach.wordpress.com/2017/07/25/how-to-add-a-physical-device-or-physical-partition-as-virtual-hard-disk-under-virt-manager/


 
