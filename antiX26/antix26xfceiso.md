### Summary of steps to prepare antiX26 with Xfce for making an .iso file ###

To make an .iso file for use as an install medium, one needs to setup certain special files before using the AntiX 'ISO Snapshot' tool.

 1.  Include the install program.
On a live antiX you see an Install icon, but it does not appear in an installed system. The program is there , it is called `/sbin/minstall`. 
To add an icon do the following
```
Look at a live antiX26 ( eg in a VM) and copy out the file `/usr/share/applications/minstall.desktop'
Boot antiX and copy/paste the `minstall.Desktop` file into `~/Desktop`
```

 2. Setup `/etc/skel`
In making an 'Install' iso, one's home directory and other sensitive files in /etc are not copied. To control what the .iso has in the home directory of its 'demo' user do the following
Files placed in `/etc/skel` will be placed in the /home/demo directory of the .iso file 
so
```
mkdir -p /etc/skel/.config/xfce4
cp -r ~/.config/xfce4/ /etc/skel/.config
chown -R root:root /etc/skel/.config/xfce4
cp -r .local /etc/skel
cp -r Desktop /etc/skel
cp -r .desktop-session /etc/skel
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
mkdir /usr/local/share/live-files/files/etc/polkit-1
mkdir /usr/local/share/live-files/files/etc/polkit-1/rules.d
cp /etc/polkit-1/rules.d/10-enable-mount.rules /usr/local/share/live-files/files/etc/polkit-1/rules.d
```

 4. Run the antiX 'ISO Snapshot' tool from the menu.
It will write scratch files in your home directory, and it puts the .iso file in /home/snapshot.

 5. Unfortunately steps 3 and 4. do not work for the case where the target .iso is a distribution .iso without persistance ... ie for making my installation .iso. 
It is necessary to fix this by hand
First loop mount the .iso file
```
mount -o loop snapshot-20260612_2011.iso /mnt
```
Then unsquash the file /mnt/antiX/linuxfs
```
unsquashfs -d /home/nevj/tmp /mnt/antiX/linuxfs
Parallel unsquashfs: Using 12 processors
222668 inodes (215181 blocks) to write

[===========================================================|] 437849/437849 100%

created 194364 files
created 20051 directories
created 28232 symlinks
created 0 devices
created 46 fifos
created 0 sockets
created 26 hardlinks
```
Second, transfer the required files from /usr/local/share/live-files/files/etc to /etc , in the temporary copy in ~/tmp
```
cd /home/nevj/tmp
cp usr/local/share/live-files/files/etc/slimski.conf etc
cp usr/local/share/live-files/files/etc/slimski.local.conf etc
cp -r usr/local/share/live-files/files/etc/dinit.d/slimski etc
cp -r usr/local/share/live-files/files/etc/polkit-1/rules.d etc
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
```

Fifth ... make a new file `linuxfs.info` file
```
du -sb ~nevj/tmp 
vi linuxfs.info
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
  -o ../antix26-xfce-turnstile-x86_64.iso \
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
xorriso 1.5.6 : RockRidge filesystem manipulator, libburnia project.

Drive current: -outdev 'stdio:../antix26-xfce-turnstile.iso'
Media current: stdio file, overwriteable
Media status : is blank
Media summary: 0 sessions, 0 data blocks, 0 data,  138g free
xorriso : WARNING : -volid text does not comply to ISO 9660 / ECMA 119 rules
Added to ISO image: directory '/'='/media/nevj/Void-musl/home/nevj/tmp2'
xorriso : UPDATE :    1017 files added in 1 seconds
xorriso : UPDATE :    1017 files added in 1 seconds
xorriso : NOTE : Copying to System Area: 432 bytes from file '/usr/lib/ISOLINUX/isohdpfx.bin'
libisofs: NOTE : Automatically adjusted MBR geometry to 1019/122/32
libisofs: NOTE : Aligned image size to cylinder size by 68 blocks
xorriso : UPDATE :  0.46% done
xorriso : UPDATE :  1.29% done
xorriso : UPDATE :  3.15% done
xorriso : UPDATE :  5.92% done, estimate finish Mon Jun 15 20:20:29 2026
.....
xorriso : UPDATE :  99.89% done
ISO image produced: 994544 sectors
Written to medium : 994544 sectors at LBA 0
Writing to 'stdio:../antix26-xfce-turnstile.iso' completed successfully.
```
Warnings are OK, but check for errors.
The consolekit case is similar


The completed, custom install ISO will be generated in the parent directory as antix26-xfce-turnstile.iso.


 6. Test the .iso file in a VM, and then written to a USB flash drive.

