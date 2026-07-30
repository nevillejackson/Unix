### Adding Lumina Desktop (Qt5 Version) to antiX26 ###

 1. Install antiX26 from `antiX-26_x64-full.iso`

 2. Choose a local repo 
Control Centre -> Repo Manager
choose `http://mirror.aarnet.edu.au/pub/mxlinux/packages`

 3. Update ... always updatea fresh install
```
apt update
apt upgrade
```

 4. Pull Lumina source code from my forked Lumina repo
```
apt install git
```

 5. Configure git
```
 git config --global user.name "<yourname>"
 git config --global user.email <youremail>
 git config --global core.editor vi
 git config --global color.vi true

```

 6. Make a work directory and initialize git there
```
mkdir /usr/local/src/LuminaQt5
cd /usr/local/src/LuminaQt5
git init
```

 7. Pull my repo to the local work directory
```
git pull https://github.com/nevillejackson/Lumina
remote: Enumerating objects: 47403, done.
remote: Counting objects: 100% (654/654), done.
remote: Compressing objects: 100% (204/204), done.
remote: Total 47403 (delta 580), reused 450 (delta 450), pack-reused 46749 (from
 4)
Receiving objects: 100% (47403/47403), 114.34 MiB | 7.25 MiB/s, done.
Resolving deltas: 100% (33471/33471), done.
From https://github.com/nevillejackson/Lumina
 * branch              HEAD       -> FETCH_HEAD
```
The work directory now contains
```
$ ls
build-info               dev-tools      LICENSE         port-files
daily_use_tips.md        diffReport.sh  luminaKeys.txt  README.md
DEPENDENCIES.md          i18n-projects  lumina.pro      ROADMAP
DeveloperGuidelines.txt  icon-theme     mkport.sh       src-qt5
```
The source code is in src-qt5
The version is Lumina 1.6.3


 8. Define origin as remote using http
```
git remote add origin https://github.com/nevillejackson/Lumina
git remote -v
origin	git@github.com:nevillejackson/Lumina (fetch)
origin	git@github.com:nevillejackson/Lumina (push)
```
This is not essential. It is in case I want to push Lumina code changes back into my forked repo.

 9.  Compiling Lumina
Instructions for this are in the README.md file on the site
`https://github.com/lumina-desktop/lumina/`

 9a. My forked Lumina uses Qt5. It contains Qt project files ( which are like Makefiles). So the first step is to run
```
apt install qt5-qmake
apt install build-essential
qmake PREFIX=/usr/local
Info: creating stash file /home/nevj/LuminaQt5/.qmake.stash
Project MESSAGE: Build OS Info: Linux, x86_64, #2 SMP PREEMPT_DYNAMIC Sun Dec 28 20:03:05 EST 2025
Project MESSAGE:  - Detected Qt Version: 5.15.15
Project MESSAGE: Build Settings Loaded: Linux -Debian
```
that creates a normal Unix Makefile.
 
 9b. Then run
```
make >&make.out
```
to compile Lumina and save the output.
That will fail with the message
```
Project ERROR: Unknown module(s) in QT: core gui network widgets x11extras multimedia multimediawidgets concurrent svg quick qml
```
Antix has some Qt5 packages installed , but not enough to compile Lumina. The following packages need to be installed

```
for qml
apt install qtdeclarative5-dev
for core
apt install libqt5core5t64
apt install libqt5gui5t64
apt install libqt5network5t64
apt install libqt5sql5t64
for gui
apt install libqt5gui5t64
for network
apt install libqt5network5t64
for widgets
apt install libqt5widgets5t64
for x11extras
apt install libqt5x11extras5 libqt5x11extras5-dev
for multimedia
apt install qml-module-qtmultimedia libqt5multimedia5 libqt5multimediawidgets5 qtmultimedia5-dev
for  multimediawidgets
apt install libqt5multimediawidgets5 qtmultimedia5-dev
for concurrent
libqt5concurrent5t64
for svg
apt install libqt5svg5 libqt5svg5-dev 
for quick 
apt install libqt5quick5 qml-module-qtquick2 qml-module-qtquick-window2 qml-module-qtquick-controls2 qml-module-qtquick-layouts

```
Some of those will be present in default antiX26.
Then re-run `make` and get the following error
```
In file included from ../libLumina/LuminaX11.cpp:7:
../libLumina/LuminaX11.h:26:10: fatal error: xcb/xcb_ewmh.h: No such file or directory
   26 | #include <xcb/xcb_ewmh.h>
      |          ^~~~~~~~~~~~~~~~
compilation terminated.

```
After installing `libxcb-ewmh-dev` and  `libxcd-ewmh2`
and rerunning `make` , it reports another missing .h file.
After about 16 make attempts, the list of packages I had to install to supply missing `.h` files is

```
libxcb-ewmh-dev libxcb-ewmh2
libxcb-util0 libxcb-util-dev
libxcb-icccm4-dev
libxcb-image0-dev
libxcb-shm0-dev
libxcb-composite0 libxcb-composite0-dev
libxcb-render0-dev libxcb-shape0-dev libxcb-xfixes0-dev
libxcb-damage0-dev
libxcb-dpms0 libxcb-dpms0-dev
libxdamage-dev
libxfixes-dev
libqt5waylandclient5
qt5dxcb-plugin
gir1.2-gudev-1.0 libbrotli-dev libbz2-dev libevdev-dev libexpat1-dev
  libfontconfig-dev libfreetype-dev libgudev-1.0-dev libinput-dev libmtdev-dev
  libpng-dev libwacom-dev libxkbcommon-dev qtbase5-private-dev
libxcursor-dev libxrender-dev
libxcb-cursor-dev
libdbusextended-qt5-1 libdbusextended-qt5-dev libdbusmenu-qt5-2
  libdbusmenu-qt5-dev
libpam0g-dev
```
The packages whose names end in '-dev' are the ones that supply '.h' files. 
The corresponding binary package must also be present.
Lumina should now compile with `make`. It takes a few minutes.

 10. Installing Lumina
Do the following as root
```
make install >&install.out
```
It will do `make install` in all the subdirectories.
It installs binaries in /usr/local/bin
```
$ ls /usr/local/bin
aCSTV.sh                           login-config-antix
add-desktop                        lte
add-key                            lthemeengine
alsa-set-default-card              lthemeengine-sstest
amz                                lumina-archiver
android-device-usb-connect.sh      lumina-config
ansi-bars                          lumina-desktop
ansi-colors                        lumina-fileinfo
ansi-tput                          lumina-fm
antix                              lumina-info
antiX-acoustic-colours             lumina-mediaplayer
antixccgrub.sh                     lumina-open
antixcc.sh                         lumina-photo
antiX-cli-cc                       lumina-pingcursor
antix-cloud                        lumina-screenshot
antiX-current-kb-layout            lumina-search
antiX-equaliser-toggle             lumina-textedit
antiX-FileManager.sh               lumina-xconfig
antix_firewall_toggle              magicrescue-a26df-wrapper

desktop-session-wallpaper          squmount
desktop_tool.py                    start-lumina-desktop
detect-init.sh                     start-t
```
All those binaries `lumina-*` plus the start script `start-lumina-desktop`
And, in `/usr/share/applications you will find
```
$ ls /usr/local/share/applications
lthemeengine.desktop        lumina-mediaplayer-pandora.desktop
lumina-archiver.desktop     lumina-photo.desktop
lumina-config.desktop       lumina-screenshot.desktop
lumina-fileinfo.desktop     lumina-search.desktop
lumina-fm.desktop           lumina-support.desktop
lumina-info.desktop         lumina-textedit.desktop
lumina-mediaplayer.desktop  lumina-xconfig.desktop
```
 Reboot and see if it makes it into the Login Manager (slimski).
No, F1 cant toggle to lumina... more setup required.

Try from a console,`startx /usr/local/bin/start-lumina-desktop` ... it starts lumina. That proves lumina install is OK ... what has to be configured is s the Login Manager(slimski)>

 Antix uses an apt-hook to trigger including a new DE in the slimski login manager. That did not work because I installed lumina with `make install` outside of apt.
So I had to do it by hand.

Edit /etc/slimski.local.conf
```
sessiontypes rox-fluxbox,zzz-fluxbox,minimal-fluxbox,fluxbox,rox-icewm,zzz-icewm,minimal-icewm,icewm,rox-jwm,zzz-jwm,minimal-jwm,jwm,herbstluftwm,lumina

default_sessiontype     lumina

```
Reboot again.
Now the Login Manager (slimski) finds lumina when toggled with F1 key. It boots to its default desktop 

.... luminadefault.png

with Conky and the Other Desktops  window from antiX.
One can start a RoxTerm terminal by Applications -> Utility -> Console Window ("RoxTerm") .  I need to make a Roxterm icon. 

Applications -> Leave -> Poweroff returns you to the Login Manager. From there using 'halt' for the username should power the system off , but it does not.  Same problem as with Xfce and Trinity desktops. It is a known slimski issue. You need to relogin and use the `poweroff` CLI command as root. 

In `/usr/share/xsessions`  you will find `Lumina-DE.desktop`


 11. In a previous attempt 
https://itsfoss.community/t/antix25-beta-adding-the-lumina-desktop/15261
I had to make some source code modifications. 
That is not necessary. I think it was caused by some missing library. Lumina 1.6.3 compiles without error providing all dependencies are present. 

 12. Missing utilities.  
I have deliberately omitted starting the Lumina Desktop Utilities ... Insight File Manager,  File Archiver, Scientific Calculator, Text Editor, MultiMedia Player, Screenshots, PDF Viewer, Multi-Monitor Management. These are not sufficiently developed to be useful.  They are present in the Applications Menu, and as binaries in /usr/local/bin.
One should choose from the range of portable utilities available in the Debian Repo, and install those. In general one should stick with Qt based utilities for compatability with Lumina, but GTK based apps can be installed.

 The Lumina Core Utilities  ... lumina-config, lumina-xconfig and lumina-search are present, and are useful.

 13. Qt5 dependence.
The official Lumina 1.6.3 depends on Qt5 (5.15.15).   
There is an unofficial spin of Lumina that uses Qt6

https://github.com/7b7b/7b7b

That may be a path forward if Qt5 disappears sometime in the future.
