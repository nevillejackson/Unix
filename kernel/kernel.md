### Kernel configuration ###
I recently had the need to examine kernel configuration in several Linux distros. 
I learnt  about  how to find out what one's kernel setting are.  They are not universal, they vary between distros. One of the things distro-makers do is configure their kernel to work best with their intended application.  So all kernels are different, even within the same version number.

Sometimes one needs to examine or change the config of the kernel that comes with a distro.   
I found that to be a big learning curve. 
I would like to share the few things I have learnt here. Hopefully others may find it useful.

#### Where to find kernel config files ####
These are located in 2 places

 - in `/boot` there are files `config-$(uname-r)`. There is one file for each kernel version, which is what the `uname-r` stands for.
  Here is my MX /boot director
<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/kernel/bootdir.png?raw=true">
</p>
 You can view these files with any text editor
 
 - in `/proc/config.gz` the file for the current running kernel is stored. This needs to be `gunzip`'ed  to be viewable. It is safer to copy it to your home directory and `gunzip` it there.

Some distros have both files, some have one or the other.

####What is in the config file####
Here is a sample of part of my MX kernel config file
```

#
# Performance monitoring
#
CONFIG_PERF_EVENTS_INTEL_UNCORE=m
CONFIG_PERF_EVENTS_INTEL_RAPL=m
CONFIG_PERF_EVENTS_INTEL_CSTATE=m
CONFIG_PERF_EVENTS_AMD_POWER=m
CONFIG_PERF_EVENTS_AMD_UNCORE=y
# CONFIG_PERF_EVENTS_AMD_BRS is not set
# end of Performance monitoring

```
So what does it mean ? 
There are __2 sorts of modules__ loaded into a running kernel

 - some are statically compiled into the kernel when it is built. The lines ending in '=y' are static modules (compiled into the kernel at build time).
 
 - some are dynamically loaded when the kernel runs. The lines ending in '=m' are dynamically loaded modules (loaded either at boot time or by the user). 

A lot of modules are device drivers, but not all. Some are just settings. Some are internal pieces of software not associated with devices.  
The commented line tells us about a particular option that is not set. 

That is just a small snippet, there are many options
```
$ wc config-6.5.0-1mx-ahs-amd64
 10828  22611 265577 config-6.5.0-1mx-ahs-amd64
```
Yes, 10828 lines means 10828 options to set . That is a lot of choice. Foss'ers  ought to be excited about that. 
If each option has 2 possible 'states'  ( eg on or off) there would be 
$2^{10828}$ ... an astronomical number ... of possible different configurations. That is a lot of different kernels! That is not quite accurate, some options have more than 2 possible states, and some combinations would be inoperable.

You dont edit this config file. It is a report of what has been set. We will look at how to change things next.

#### How to change kernel config settings ####
There are 3 methods

 - compile config settings into the kernel when it is being built
 - use `grub` to add a boot parameter to the linux boot command. This can be either temporary ( by editing the grub menu) or permanent ( by making an entry in `/etc/default/grub`)
 - load or unload a loadable module . This can be temporary ( using the `modprobe` command) or permanent ( making an entry in `/etc/modules` )

Some examples:
1. To add a boot parameter to the linux command in the grub menu, hit 'e' with the chosen distro highted ....
you will drop into an editor window.... move the cursor down to the 'linux' line and add the boot parameter at the end. For example
```
linux /boot/vmlinuz-6.5.0-1mx-ahs-amd64 root=UUID=fd84d74d-325a-4e42-acbd-5ced9cfc4ebf ro intel_iommu=off quiet splash
```
There I have added `intel_iommu=off` to the linux command line.
When finished editing use F10 to boot.
This modification is temporary. It disappears next boot.
Alternatively you can edit the file `/boot/grub/grub.cfg' with any text editor. That is also temporary

2. To add a boot parameter permanently to grub.cfg file, edit the file `/etc/default/grub`. 
Add a line like the following
```
GRUB_CMDLINE_LINUX_DEFAULT="quiet splash"
GRUB_CMDLINE_LINUX="intel_iommu=off"
```
I am not sure which of those 2 lines one should use. I used the second.
Then one has to run 
```
update-grub
```
in the same distro.
Then, if grub is controlled by some other distro, one has to run
```
update-grub
```
in the controlling distro as well. 

3. To load a module in a running linux ( one would usually do this for a driver module) use
```
modprobe amd_cpu
```
to unload it use
```
modprobe -r amd_gpu
```
Of course `amd_gpu` is an example module name; substitute the module you reqiure.
Changes made with modprobe are temporary. They disappear at reboot.

4. To autoload a module at boot time edit the file  `/etc/modules`
```
# /etc/modules: kernel modules to load at boot time.
#
# This file contains the names of kernel modules that should be loaded
# at boot time, one per line. Lines beginning with "#" are ignored.

loop
```
There I have the `loop` module being loaded. 

#### Other places to look for information ####

There are a few other kernel related questions that can be answered by specific searches

**1. How to find which kernel modules are built-in  modules and which are dynamically loaded?**

We all know how to list dynamically loaded modules
```
lsmod
```
but builtin statically compiled modules need to be searched for here
```
$cd /lib/modules/$(uname -r)

$ head modules.builtin
kernel/arch/x86/events/amd/amd-uncore.ko
kernel/arch/x86/platform/intel/iosf_mbi.ko
kernel/mm/zbud.ko
kernel/fs/binfmt_script.ko
kernel/fs/exportfs/exportfs.ko
kernel/fs/nls/nls_base.ko
kernel/fs/unicode/utf8data.ko
kernel/fs/pstore/pstore.ko
kernel/security/keys/encrypted-keys/encrypted-keys.ko
kernel/crypto/crypto.ko
```
That is just a sample. In my MX distro there are many more
```
$ wc modules.builtin
 164  164 5716 modules.builtin
```
Notice the module file is a .ko extension.  It stands for kermel object file.... that is exactly what they are , object files the same as .o files. 

The actual .ko files are found here

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/kernel/drivers.png?raw=true">
</p>

You can see the bluetootj subdirectory  contains driver files.

**2. How to  find parameters of a kernel module**
For loadable kernel modules use the modinfo command
```
$ modinfo ahci
filename:       /lib/modules/6.5.0-1mx-ahs-amd64/kernel/drivers/ata/ahci.ko
version:        3.0
license:        GPL
description:    AHCI SATA low-level driver
author:         Jeff Garzik
srcversion:     9E43AEA6880180E91158B22
alias:          pci:v*d*sv*sd*bc01sc06i01*
alias:          pci:v00000014d00007A08sv*sd*bc*sc*i*
alias:          pci:v00001C44d00008000sv*sd*bc*sc*i*
.....  skipped lines
alias:          pci:v00008086d00002652sv*sd*bc*sc*i*
alias:          pci:v00008086d000006D6sv*sd*bc*sc*i*
depends:        libata,libahci
retpoline:      Y
intree:         Y
name:           ahci
vermagic:       6.5.0-1mx-ahs-amd64 SMP preempt mod_unload modversions 
parm:           marvell_enable:Marvell SATA via AHCI (1 = enabled) (int)
parm:           mobile_lpm_policy:Default LPM policy for mobile chipsets (int)
```
One can see the parameters undr 'parm:'

However that does not work for builtin modules.  You need to go to `/sys/module`  where you will see all the modules vivible to the running kernel.  Choose the relevant subdirectory 
```
$ cd /sys/module/ahci/parameters
nevj@trinity:/sys/module/ahci/parameters
$ ls
marvell_enable  mobile_lpm_policy
nevj@trinity:/sys/module/ahci/parameters
$ cat marvell_enable
0
```
If it is a dynamically loaded module , its subdirectory contains a file 'initstate', for example
```
$ cd /sys/module/amdgpu
nevj@trinity:/sys/module/amdgpu
$ ls
coresize  drivers  holders  initsize  initstate  notes  parameters  refcnt  sections  taint  uevent
nevj@trinity:/sys/module/amdgpu
$ cat initstate
live
```
So one can use that instead of `lsmod`

The choice of whether a particular module is built-in or loadable is made at kernel build time.

**3.  How to find which linux driver is used for a disk**

First find the disk type using `dmesg`
```
# dmesg | grep sdb
[    3.720868] sd 1:0:0:0: [sdb] 7814037168 512-byte logical blocks: (4.00 TB/3.64 TiB)
[    3.720885] sd 1:0:0:0: [sdb] Write Protect is off
[    3.720888] sd 1:0:0:0: [sdb] Mode Sense: 00 3a 00 00
[    3.720914] sd 1:0:0:0: [sdb] Write cache: enabled, read cache: enabled, doesn't support DPO or FUA
[    3.720969] sd 1:0:0:0: [sdb] Preferred minimum I/O size 512 bytes
[    3.722915]  sdb: sdb1 sdb2 sdb3 sdb4 sdb5 sdb6 sdb7 sdb8 sdb9 sdb10 sdb11 sdb12
[    3.724156] sd 1:0:0:0: [sdb] supports TCG Opal
[    3.724158] sd 1:0:0:0: [sdb] Attached SCSI disk
```
So it is a SCSI disk `sd 1:0:0:0`
Now look in /sys/bus/scsi
```
# cd /sys/bus/scsi
# ls
devices  drivers  drivers_autoprobe  drivers_probe  uevent
# ls drivers
sd  sr
# ls devices
0:0:0:0   24:0:0:0  host0   host13  host18  host22  host5  target0:0:0	 target24:0:0
1:0:0:0   24:0:0:1  host1   host14  host19  host23  host6  target1:0:0	 target4:0:0
10:0:0:0  24:0:0:2  host10  host15  host2   host24  host7  target10:0:0
16:0:0:0  24:0:0:3  host11  host16  host20  host3   host8  target16:0:0
23:0:0:0  4:0:0:0   host12  host17  host21  host4   host9  target23:0:0
```
That confirms it, the driver is `sd`

**4. How to find information on hotplug devices**
These are controlled by `udev` , so we can use `udevadm`  to get information
```
root@trinity:~# udevadm info /dev/sdc
P: /devices/pci0000:00/0000:00:01.0/0000:05:00.0/0000:06:03.0/0000:09:00.0/ata11/host10/target10:0:0>
M: sdc
U: block
T: disk
D: b 8:32
N: sdc
L: 0
S: disk/by-path/pci-0000:09:00.0-ata-1
S: disk/by-id/wwn-0x5001b444a555a82a
S: disk/by-path/pci-0000:09:00.0-ata-1.0
S: disk/by-id/ata-WD_Blue_SA510_2.5_1000GB_24010F474306
S: disk/by-diskseq/4
Q: 4
E: DEVPATH=/devices/pci0000:00/0000:00:01.0/0000:05:00.0/0000:06:03.0/0000:09:00.0/ata11/host10/targ>
E: DEVNAME=/dev/sdc
E: DEVTYPE=disk
E: DISKSEQ=4
E: MAJOR=8
E: MINOR=32
E: SUBSYSTEM=block
E: USEC_INITIALIZED=11918598
E: ID_ATA=1
E: ID_TYPE=disk
E: ID_BUS=ata
E: ID_MODEL=WD_Blue_SA510_2.5_1000GB
E: ID_MODEL_ENC=WD\x20Blue\x20SA510\x202.5\x201000GB\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20>
E: ID_REVISION=52048100
E: ID_SERIAL=WD_Blue_SA510_2.5_1000GB_24010F474306
E: ID_SERIAL_SHORT=24010F474306
E: ID_ATA_WRITE_CACHE=1
E: ID_ATA_WRITE_CACHE_ENABLED=1
E: ID_ATA_FEATURE_SET_PM=1
E: ID_ATA_FEATURE_SET_PM_ENABLED=1
E: ID_ATA_FEATURE_SET_SECURITY=1
E: ID_ATA_FEATURE_SET_SECURITY_ENABLED=0
E: ID_ATA_FEATURE_SET_SECURITY_ERASE_UNIT_MIN=65556
E: ID_ATA_FEATURE_SET_SMART=1
E: ID_ATA_FEATURE_SET_SMART_ENABLED=1
E: ID_ATA_FEATURE_SET_APM=1
E: ID_ATA_FEATURE_SET_APM_ENABLED=0
E: ID_ATA_DOWNLOAD_MICROCODE=1
E: ID_ATA_SATA=1
E: ID_ATA_SATA_SIGNAL_RATE_GEN2=1
E: ID_ATA_SATA_SIGNAL_RATE_GEN1=1
E: ID_ATA_ROTATION_RATE_RPM=0
E: ID_WWN=0x5001b444a555a82a
E: ID_WWN_WITH_EXTENSION=0x5001b444a555a82a
E: ID_PATH=pci-0000:09:00.0-ata-1.0
E: ID_PATH_TAG=pci-0000_09_00_0-ata-1_0
E: ID_PATH_ATA_COMPAT=pci-0000:09:00.0-ata-1
E: ID_PART_TABLE_UUID=74288115-cc5a-4f76-9cb8-0abfc720d101
E: ID_PART_TABLE_TYPE=gpt
E: DEVLINKS=/dev/disk/by-path/pci-0000:09:00.0-ata-1 /dev/disk/by-id/wwn-0x5001b444a555a82a /dev/dis>
E: TAGS=:systemd:
E: CURRENT_TAGS=:systemd:

```
My `sdc` is a hotpluggable sata disk. 
More information , including drivers, can be obtained with 
```
udevadm info -a
```
More information on `udev` is available here
https://debian-handbook.info/browse/stable/sect.hotplug.html#

**5. Consulting the source code on kernel modules**
You dont need to install the source to do this. It is available on Github. The section relevant to kernel configuration is, for example

https://github.com/torvalds/linux/blob/master/drivers/iommu/intel/Kconfig

This is actually quite an authoritative source of information.  The example is for the iommu  parameter. 
For others go back up the path to drivers and descend into what you require.

#### Conclusion
Dont be frightened of kernel configuration.  It is meant to be user-modifiable. We have come a long way since the days when one had to compile a kernel to change anything.







