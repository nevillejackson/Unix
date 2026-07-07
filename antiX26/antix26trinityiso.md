### Summary of steps to prepare antiX26 with Trinity for making an .iso file ###

To make an .iso file for use as an install medium, one needs to setup certain special files before using the AntiX 'ISO Snapshot' tool.

 1.  Include the install program.
On a live antiX you see an Install icon, but it does not appear in an installed system. The program is there , it is called `/sbin/minstall`. 
To add an icon do the following
```
Look at a live antiX26 ( eg in a VM) and copy out the file `/usr/share/applications/minstall.desktop'
Boot antiX and copy/paste the `minstall.Desktop` file into `~/Desktop`

```
When you boot my .iso live, you will see two install icons .... one called "Custom antiX26 Install"  .... ignore that , it uses Konqueror. The other is called "Install AntiX" .. use that one ... it uses the antiX Gazelle installer.

 2. Setup `/etc/skel`
In making an 'Install' iso, one's home directory and other sensitive files in /etc are not copied. To control what the .iso has in the home directory of its 'demo' user do the following
Files placed in `/etc/skel` will be placed in the /home/demo directory of the .iso file 
so
```
cd ~nevj
cp -r .config /etc/skel
cp -r .local /etc/skel
cp -r Desktop /etc/skel
cp -r .desktop-session /etc/skel
cp -r .trinity /etc/skel
cd /etc/skel
mkdir Downloads Documents Music Pictures Videos Templates
```
Those are the necessary non-personal files that will make the 'demo' xfce in the .iso boot up the same as my own configured xfce install.

 3. Setup `/usr/local/share/live-files/`
To control what the .iso file has in its `/etc` directory over and above the default for a fresh install, one has to add what is required to `/usr/local/share/live-files/files/etc`
In the present case all that is needed is slimski.conf, slimski.local.conf , and dinit.d/slimski.
```
cp /etc/slimski.conf /usr/local/share/live-files/files/etc
cp /etc/slimski.local.conf /usr/local/share/live-files/files/etc
mkdir /usr/local/share/live-files/files/etc/dinit.d
cp /etc/dinit.d/slimski /usr/local/share/live-files/files/etc/dinit.d/slimski
mkdir /usr/local/share/live-files/files/etc/init.d
cp /etc/init.d/slimski /usr/local/share/live-files/files/etc/init.d/slimski
mkdir /usr/local/share/live-files/files/etc/sv
cp -r /etc/sv/slimski /usr/local/share/live-files/files/etc/sv/slimski
mkdir /usr/local/share/live-files/files/etc/polkit-1
mkdir /usr/local/share/live-files/files/etc/polkit-1/rules.d
cp /etc/polkit-1/rules.d/10-enable-mount.rules /usr/local/share/live-files/files/etc/polkit-1/rules.d
cp -r /etc/chrony /usr/local/share/live-files/files/etc
```

 4. Run the antiX 'ISO Snapshot' tool from the menu.
It will write scratch files in your home directory, and it puts the .iso file in /home/snapshot.

 5. Unfortunately steps 3 and 4. do not work for the case where the target .iso is a distribution .iso without persistance ... ie for making my installation .iso. 
It is necessary to fix this by hand
I used Void linux to do this step ( saves installing things in antiX)
First loop mount the .iso file
```
mount -o loop snapshot-20260707_1726.iso /mnt
```
Then unsquash the file /mnt/antiX/linuxfs
```
unsquashfs -d /home/nevj/tmp /mnt/antiX/linuxfs
Parallel unsquashfs: Using 12 processors
267748 inodes (262702 blocks) to write

[==========================================================-] 530450/530450 100%

created 238543 files
created 23503 directories
created 29129 symlinks
created 0 devices
created 50 fifos
created 0 sockets
created 26 hardlinks

```
Second, transfer the required files from /usr/local/share/live-files/files/etc to /etc , in the temporary copy in ~/tmp
```
cd /home/nevj/tmp
cp usr/local/share/live-files/files/etc/slimski.conf etc
cp usr/local/share/live-files/files/etc/slimski.local.conf etc
cp -r usr/local/share/live-files/files/etc/dinit.d/slimski etc/dinit.d
cp -r usr/local/share/live-files/files/etc/init.d/slimski etc/init.d
cp -r usr/local/share/live-files/files/etc/sv/slimski etc/sv
cp -r usr/local/share/live-files/files/etc/polkit-1/rules.d/10-enable-mount.rules etc/polkit-1/rules.d
cp -r  usr/local/share/live-files/files/etc/chrony etc

```

Third ... copy all files from the original .iso to a second working directory
```
mkdir ~nevj/tmp2
cd /mnt
cp -r * ~nevj/tmp2
```

Fourth ... re-squash the edited unsquashed `linuxfs` folder , replacing the temporary copy in ~nevj/tmp2
```
cd  ~nevj/tmp2
cd antiX
rm linuxfs
mksquashfs ~nevj/tmp ~nevj/tmp2/antiX/linuxfs -comp xz -b 1024k -always-use-fragments
Parallel mksquashfs: Using 12 processors
Creating 4.0 filesystem on /home/nevj/tmp2/antiX/linuxfs, block size 1048576.
[==========================================================|] 235508/235508 100%

Exportable Squashfs 4.0 filesystem, xz compressed, data block size 1048576
	compressed data, compressed metadata, compressed fragments,
	compressed xattrs, compressed ids
	duplicates are removed
Filesystem size 2195751.77 Kbytes (2144.29 Mbytes)
	29.61% of uncompressed filesystem size (7415736.20 Kbytes)
Inode table size 2067360 bytes (2018.91 Kbytes)
	21.33% of uncompressed inode table size (9693897 bytes)
Directory table size 2522052 bytes (2462.94 Kbytes)
	35.72% of uncompressed directory table size (7061348 bytes)
Xattr table size 78 bytes (0.08 Kbytes)
	97.50% of uncompressed xattr table size (80 bytes)
Number of duplicate files found 60735
Number of inodes 291227
Number of files 238543

....  many lines skipped .....

```

Fifth ... make a new file `linuxfs.info` file
```
du -sb ~nevj/tmp 
cd ~nevj/tmp2/antiX
vi linuxfs.info
[edit in the result of the du command]
```

Sixth ... make a new `linuxfs.md5` file
```
md5sum linuxfs
vi linuxfs.md5
[edit in the new md5sum]
```

 Seventh .... Build the final bootable ISO
Use xorriso to master the new bootable ISO for distribution. Make sure `xorriso` and `isolinux` [in void 'syslinux'] are installed. 

```
cd ~nevj/tmp2

sudo xorriso -as mkisofs \
  -V "Custom_antiX26_Install" \
  -o ../antix26-trinity-x86_64.iso \
[ -o ../antix26-xfce-consolekit-x86_64.iso \]
  -isohybrid-mbr /usr/lib/ISOLINUX/isohdpfx.bin \
[in void -isohybrid-mbr /usr/lib/syslinux/isohdpfx.bin \]
  -c boot/isolinux/boot.cat \
  -b boot/isolinux/isolinux.bin \
  -no-emul-boot -boot-load-size 4 -boot-info-table \
  -eltorito-alt-boot \
[  -e boot/uefi/efiboot.img \]
  -e boot/grub/efi.img \
  -no-emul-boot -isohybrid-gpt-basdat \
  .

```
The bracketed line `[ -e boot/uefi/efiboot.img \]` does not work for antiX26 ... the file is not there. That means the .iso will not do an UEFI boot from a usb drive .... it will do a grub boot, on either a UEF-bios computer or a  Legacy-bios computer.
The output from above is
```
GNU xorriso 1.5.6.pl02 : RockRidge filesystem manipulator, libburnia project.

Drive current: -outdev 'stdio:../antix26-trinity-x86_64.iso'
Media current: stdio file, overwriteable
Media status : is blank
Media summary: 0 sessions, 0 data blocks, 0 data, 86.0g free
xorriso : WARNING : -volid text does not comply to ISO 9660 / ECMA 119 rules
Added to ISO image: directory '/'='/home/nevj/tmp2'
xorriso : UPDATE :    1018 files added in 1 seconds
xorriso : UPDATE :    1018 files added in 1 seconds
xorriso : NOTE : Copying to System Area: 432 bytes from file '/usr/lib/syslinux/isohdpfx.bin'
libisofs: NOTE : Automatically adjusted MBR geometry to 1021/138/32
libisofs: NOTE : Aligned image size to cylinder size by 863 blocks
xorriso : UPDATE :  8.45% done
xorriso : UPDATE : Thank you for being patient. Working since 2 seconds.
xorriso : UPDATE : Thank you for being patient. Working since 3 seconds.
xorriso : UPDATE : Thank you for being patient. Working since 4 seconds.
xorriso : UPDATE : Thank you for being patient. Working since 5 seconds.
xorriso : UPDATE : Thank you for being patient. Working since 6 seconds.
xorriso : UPDATE : Thank you for being patient. Working since 7 seconds.
xorriso : UPDATE : Thank you for being patient. Working since 8 seconds.
xorriso : UPDATE : Thank you for being patient. Working since 9 seconds.
xorriso : UPDATE : Thank you for being patient. Working since 10 seconds.
xorriso : UPDATE : Thank you for being patient. Working since 11 seconds.
xorriso : UPDATE : Thank you for being patient. Working since 12 seconds.
xorriso : UPDATE : Thank you for being patient. Working since 13 seconds.
xorriso : UPDATE : Thank you for being patient. Working since 14 seconds.
xorriso : UPDATE : Thank you for being patient. Working since 14 seconds.
xorriso : UPDATE : Thank you for being patient. Working since 15 seconds.
ISO image produced: 1127184 sectors
Written to medium : 1127184 sectors at LBA 0
Writing to 'stdio:../antix26-trinity-x86_64.iso' completed successfully.

```
Warnings are OK, but check for errors.


The completed, custom install ISO will be generated in the parent directory as antix26-trinity-x86_64.iso.


 6. Test the .iso file in a VM, and then written to a USB flash drive.

