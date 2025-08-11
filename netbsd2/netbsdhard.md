### Hard install of NetBSD ###
I have a multi-boot x86_64 computer with 3 disks and several Linuxes including MX, Void, and Devuan.
I am going to attempt to install NetBSD in one partition of one 2TB HDD.
I am going to do what I normally recommend , and prepare the target partition using `gparted` , before using the installer.
The disk has a GPT partition table, and contains a 1Tb data partition , called 'Common', but I was able to find a spare 300Gb partition, so I erased it and made a 9.7Gb swap space and a 292Gb space for NetBSD.

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/netbsd2/netbsdhard.png?raw=true">
</p>


I was not able to format the new partitions. `Gparted` does not do either FFS formatting, or netbsd-swap formatting. So I left the partitions unformatted.

 I cant easily  use my Linux swap partitions for BSD because Linux and BSD setup their swap partitions differently, and I would have to run `mkswap` every time I changed from BSD to Linux. So I decided to give NetBSD its own swap partition.

 I am not sure how all this is going to work, because NetBSD uses 'slices' which are about the same as Linux primary partitions, but NetBSD can also subdivide 'slices' into smaller partitions using a utility called 'disklabel'. I read that NetBSD does have support for GPT and it can boot from a GPT disk..


#### Making the install USB drive ####
NetBSD is different to Linux. You dont download the .iso file and `dd` it to the USB drive, because the .iso file is not a 'hybrib' iso.  The .iso file will work for a VM install, or on a CD/DVD, but not on a USB drive.

What you do is download another file called `NetBSD-10.1-amd64-install.img.gz`, then unzip it, and copy the unzipped file to the USB drive
```
$ gunzip NetBSD-10.1-amd64-install.img.gz
$ ls Net*
NetBSD-10.0-amd64.iso  NetBSD-10.1-amd64-install.img  NetBSD-10.1-amd64.iso
$ su
Password: 
# dd if=NetBSD-10.1-amd64-install.img of=/dev/sde conv=fsync
5120000+0 records in
5120000+0 records out
2621440000 bytes (2.6 GB, 2.4 GiB) copied, 457.558 s, 5.7 MB/s
# 

```
Now reboot from the USB drive

#### Install from USB drive ####
The USB drive boots, and after a few preliminaries like setting language and keyboard, you get to say "Install to a hard disk", and it displays the disks to choose from

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/netbsd2/nbsdinst1.png?raw=true">
</p>



My wd0 is a 1Tb hotplugged SSD, wd1 is a 4Tb HDD, and wd2 is a 2TB HDD. It does not see my 4TbSSD. I dont know why that is... it saw the other SSD on wd0?
Never mind, the disk I have prepared partitions on is wd2, so I highlight it, as shown, and continue.
Next we get to choose some partition options

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/netbsd2/nbsdinst2.png?raw=true">
</p>


I choose to use existing GPT partitons, as highlighted
Then I get a summary of the partitions it sees on wd2.

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/netbsd2/nbsdinst4.png?raw=true">
</p>


It is not perfect. It sees the 299Gb partiton I made for NetBSD as Linux-ext2.... it is not that, it is unformatted. Similarly for the 10Gb Netbsd-swap partiton. It also sees my 199Gb Linux-swap partiton as FFSv2 , and that 190Gb partiton is not MSDOS it is ext4.

What you do now is highlight the partitons you want to change , so I will highlight d: the 299Gb partiton for NetBSD.

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/netbsd2/nbsdinst5.png?raw=true">
</p>


and then you get a submenu

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/netbsd2/nbsdinst6.png?raw=true">
</p>


I have made some changes there, type: FFS,  Install:Yes, newfs: Yes, mount: Yes, mount point: /
NB. The picture shown Newfs: No ... I fixed that.

Then I say OK , and go back to do the same for the 10Gb swap partition

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/netbsd2/nbsdinst7.png?raw=true">
</p>


All I do there is set type: swap
and

Then I do the same for the EFI system partition

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/netbsd2/nbsdinstpartefi.png?raw=true">
</p>


It needs to be mounted to `/mnt/EFI/boot` (not `/boot/efi` like in most Linuxes)

Here is the final setup

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/netbsd2/nbsdinstpartsfinal.png?raw=true">
</p>


I have a 299GB FFS partiton, a 512Mb EFI partiton,  and a 10Gb swap partiton.

Now I tell it to go ahead and write NetBSD to those partions and it `fsck`'s the root partiton, and offers a choice 'Full install', 'Minimal Install' and others. I choose a 'Full Install' and it asks where to install from? I tell it a USB image. 

It goes ahead and writes all of what it calls 'sets' ( .tar.gz files ) to the root filesystem, then it asks if I want to configure?

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/netbsd2/nbsdinstconfigure.png?raw=true">
</p>


That is my configure settings. This time I remembered to tell it to setup a network, and to setup the package management software. 

It then finishes . and offers a Reboot.

 #### Booting issues after install ####
   I reboot, and it fails to boot. Time to investigate
 - boot MX and do `update-grub`.... os-prober fails to find NetBSD
 - mount the EFI partition on that 2Tb HDD. 
```
# mount /dev/sdb6 /mnt
# cd /mnt
# ls
EFI
# cd EFI
# ls
antiX  boot 
ls boot
bootia32.efi  bootx64.efi
```
 Those files `bootia32.efi`  and `bootx64.efi` are the NetBSD bootloader.
The Antix directory is there because the 2TB HDD also has Antix installed on `/dev/sda1`. It contains `grub64.efi`... the normal Linux grub file.

So, NetBSD does not write a grub file to the EFI partition on a UEFI system. It writes its own bootloader. To get it to boot with grub, one has two possibilities
 -   chainload the NetBSD bootloader. That requires making a `40_custom file in /etc/grub.d of the Linux which controls grub.
 -   tell grub directly how to boot NetBSD. That uses a command called `knetbsd` and also requires making a `40_custom` file entry.

We will try both ... add the following to `/etc/grub.d/40_custom`

```
menuentry "NetBSD-kernel"{
    insmod part_gpt
    set root=(hd4,gpt2)
    knetbsd /netbsd
}
menuentry 'NetBSD-bootloader' {
    insmod part_gpt
    set root=(hd4,gpt6)
    chainloader /EFI/boot/bootx64.efi
}
```

How did I know the disk was `hd4`? Trial and error. Grub puts disks in a different order to MX. I fear if I unplug a hotpluggable disk, it may change the order?

What we are trying to do in the second case  is to tell grub to load the NetBSD bootloader ( as if it were a program to be run) , and then allow the NetBSD bootloader to boot NetBSD. The first case is a straight grub boot. 


I make those additions to /etc/grub.d/40_custom in MX, and do `update-grub`

```
root@trinity:/etc/grub.d# update-grub
Generating grub configuration file ...
Found theme: /boot/grub/themes/mx_linux/theme.txt
Found linux image: /boot/vmlinuz-6.1.0-32-amd64
Found initrd image: /boot/initrd.img-6.1.0-32-amd64
Found linux image: /boot/vmlinuz-6.1.0-31-amd64
Found initrd image: /boot/initrd.img-6.1.0-31-amd64
Found linux image: /boot/vmlinuz-6.1.0-25-amd64
Found initrd image: /boot/initrd.img-6.1.0-25-amd64
Found Void Linux on /dev/sda5
Found Devuan GNU/Linux 5 (daedalus) on /dev/sda7
Found antiX-runit (23.1) on /dev/sdb1
Found antiX (23.1) on /dev/sdd5
Found Artix Linux (rolling) on /dev/sdd6
done
```
It says nothing
So I reboot and see if I get an entry in the grub screen.

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/netbsd2/nbsdmxgrub.png?raw=true">
</p>

Success!  
The NetBSD-bootloader option does not work.... I get the grub boot screen controlled by Antix....something wrong there, but
The NetBSD-kernel option works....I get a NetBSD login screen

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/netbsd2/nbsdlogin.png?raw=true">
</p>


#### Post-boot issues ####

So I login, and the first problem is obvious.  It starts the CTWM DE, and it is clear the screen is being driven in VGA mode .....  ie it is not using the `amdgpu` driver. 
After some investigation, I find out that the command for looking at modules in NetBSD is `modstat` ( in Linux it is `lsmod`). I try `modstat` and amdgpu is indeed not listed. 
I can load it with `modload amdgpu` , and it then shows in `modstat`, but that does not change the resolution?
Maybe I need to load it at boot time? 
To do that I edit the file `/etc/modules.conf` and add one line containing `amdgpu`
Then when I boot, it loads 'amdgpu' module but I still get VGA mode.

If the module is loaded but is not functioning, that means the firmware is missing.  To check I go to `/libdata/firmware/amdgpu`, and it contains amdgpu firmware files up to `Navi 14` but not `Navi 24`. 

I tried doing a `sysupgrade` and it did download one file called `gpufw.tar.gz`, but no extra firmware appeared. I guess my install of NetBSD-10.1 was up to date. It seems NetBSD-10 does not yet support my RX6400 Navi 24 gpu.

I am going to leave that problem.   I can not get firmware from elsewhere ( eg from a Linux distro), because foreign firmware will not work with the NetBSD kernel. I have to wait for NetBSD to include the required firmware.  They are only about 3 years behind Debian.

#### Concluding remarks ####
This install of NetBSD was to a predefined partition on a disk containing another Linux, in a computer with multiple disks booted in UEFI compatable mode.  I had to be very careful to correctly identify the partitons (/ and EFI) to be written on.

NetBSD has its own bootloader, which it wants to write to the EFI partition, in a UEFI booted computer. However, if one tried to install NetBSD to Legacy-booted computer, NetBSD would write its bootloader to the MBR, and thereby overwrite whatever was in the MBR, eg grub. So NetBSD behaves like Windows in this respect. I would not recommend trying a legacy-boot install of NetBSD unless you can give NetBSD own disk .

To do a NetBSD hard install you need to understand partitions,  and you need to read up on the various configuration choices. The installer itself is easy to use.... not graphical but it used dialog boxes. A VM install is much easier. I am glad I tried a VM install first

What I planned to do was to show that NetBSD could be setup in a way suitable for a daily work distro. The issue with amdgpu firmware is a bit of a setback. I have the following options
 -   continue to work in the hard install at VGA resolution,
 -   use a VM install which I already have, 
 -   try NetBSD in my spare computer which has Intel graphics. I am reluctant because the spare computer is Legacy boot.
 -   wait for NetBSD-11

All I have shown here is that one can, with some precautions, do a hard install of NetBSD on a Linux box, and that it may be satisfactory, depending on support for one's video card.

