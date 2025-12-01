### Installing Xfce in antiX25 ###

AntiX offers a number of Window Managers, the best , in my opinion, being IceWM or JWM. However some users may prefer a conventional DE. There are several popular DE's available as packages
 - Xfce4 (package name xfce4)
 - Gnome ( package name gnome-classic)
 - KDE5 ( packages kde-plasma-desktop, kde-full, kde-standard)
 - Cinnamon ( package name cinnamon)
 - Mate ( package mate-core)
 - Budgie
 - LXDE
 - LXQT

I am going to try first with Xfce, becaus ethat is my familiar desktop.

The instructions, kindly researched for me by @Rosika, are 
```
apt install xserver-xorg xfce4 xfce4-goodies lightdm lightdm-gtk-greeter
```
"LightDM as your default display manager should be chosen.
And after a reboot, you should see the LightDM login screen.
Select Xfce from the session options and you should be able to log in with your credentials."

Well let us see how it works.
 
```
root@antixdinit:/home/nevj# apt install xfce4
The following package was automatically installed and is no longer required:
  libspdlog1.15
Use 'apt autoremove' to remove it.

Installing:
  xfce4

Installing dependencies:
  exo-utils                      thunar
  libgarcon-1-0                  thunar-data
  libgarcon-common               xfce4-appfinder
  libgarcon-gtk3-1-0             xfce4-helpers
  libgtk-layer-shell0            xfce4-panel
  libpolkit-gobject-1-0          xfce4-pulseaudio-plugin
  libpolkit-gobject-elogind-1-0  xfce4-session
  libthunarx-3-0                 xfce4-settings
  libxfce4panel-2.0-4            xfdesktop4
  libxfce4ui-utils               xfdesktop4-data
  libxfce4windowing-0-0          xfwm4
  libxfce4windowing-common

Suggested packages:
  thunar-archive-plugin     gvfs-backends  xfce4-power-manager
  thunar-media-tags-plugin  xfce4-goodies  fortune-mod

Recommended packages:
  gvfs                   desktop-base      | logind             upower
  policykit-1-gnome      mate-polkit       light-locker         colord
  | polkit-1-auth-agent  tango-icon-theme  | xfce4-screensaver  xiccd
  thunar-volman          xfce4-notifyd     | xscreensaver
  tumbler                xorg              | mate-screensaver
  udisks2                default-logind    systemd-sysv

Summary:
  Upgrading: 0, Installing: 24, Removing: 0, Not Upgrading: 0
  2 not fully installed or removed.
  Download size: 6,467 kB
  Space needed: 36.2 MB / 25.0 GB available

Continue? [Y/n] 

.......

Processing triggers for libc-bin (2.41-12) ...
Processing triggers for runit-antix (2.2.0-3.0antix6) ...
Errors were encountered while processing:
 dinit-service-ufw
 dinit-service-haveged
Available xsessions have changed
Updating Slimski /etc/slimski.local.conf
desktop-session-update-wm-menus: Writing file: /usr/share/desktop-session/wm-menus/fluxbox-wm-menu
desktop-session-update-wm-menus: Writing file: /usr/share/desktop-session/wm-menus/icewm-wm-menu
desktop-session-update-wm-menus: Writing file: /usr/share/desktop-session/wm-menus/jwm-wm-menu
desktop-session-update-wm-menus: Writing file: /usr/share/desktop-session/wm-menus/RAW-wm-menu
Writing Menu: fluxbox
Writing Menu: jwm
Writing Menu: icewm
Error: Sub-process /usr/bin/dpkg returned an error code (1)
```
Well there are errors, but I think it did the install.
Now lightdm
```
root@antixdinit:/home/nevj# apt install lightdm lightdm-gtk-greeter
The following package was automatically installed and is no longer required:
  libspdlog1.15
Use 'apt autoremove' to remove it.

Installing:
  lightdm  lightdm-gtk-greeter

Installing dependencies:
  elogind            libelogind0             libpam-elogind
  libelogind-compat  liblightdm-gobject-1-0

Suggested packages:
  accountsservice  upower  xserver-xephyr

Recommended packages:
  desktop-base

Summary:
  Upgrading: 0, Installing: 7, Removing: 0, Not Upgrading: 0
  2 not fully installed or removed.
  Download size: 1,638 kB
  Space needed: 5,856 kB / 25.0 GB available

Continue? [Y/n] 

......

Processing triggers for libc-bin (2.41-12) ...
Processing triggers for runit-antix (2.2.0-3.0antix6) ...
Processing triggers for man-db (2.13.1-1) ...
Processing triggers for dbus (1.16.2-2.0nosystemd3) ...
Processing triggers for hicolor-icon-theme (0.18-2) ...
Errors were encountered while processing:
 dinit-service-ufw
 dinit-service-haveged
 elogind
 libpam-elogind:amd64
 lightdm
Available xsessions have changed
Updating Slimski /etc/slimski.local.conf
desktop-session-update-wm-menus: Writing file: /usr/share/desktop-session/wm-menus/fluxbox-wm-menu
desktop-session-update-wm-menus: Writing file: /usr/share/desktop-session/wm-menus/icewm-wm-menu
desktop-session-update-wm-menus: Writing file: /usr/share/desktop-session/wm-menus/jwm-wm-menu
desktop-session-update-wm-menus: Writing file: /usr/share/desktop-session/wm-menus/RAW-wm-menu
Error: Sub-process /usr/bin/dpkg returned an error code (1)
root@antixdinit:/home/nevj# 
```
Same again, errors but I think it installed lightdm
During the install I had to answer a question.... whether to use slimski or lightdm.  I chose lightdm

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/antiX/lightdm.png?raw=true">
</p>


Reboot and see what we get 

We get the slimski screen ??

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/antiX/slimski.png?raw=true">
</p>

 but it  has the option 'xfce' if I toggle it with F1

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/antiX/slimskixfce.png?raw=true">
</p>


So, login with that and we get

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/antiX/xfce1.png?raw=true">
</p>



OK, there is an error, and a strange magnifier window, but the interesting thing is that menu of Window Managers. 
I 'ticked' Xfce and said OK.  I simply got the same xfce screen again.

So, open a terminal window and see what is going on. .... First thing, connmand is not running

```
junk.connman
junk.connman.dinit
```

It runs and stops immediately. 
Without connmand I have no communication with the host. 
So, serarch with google for the error message
```
https://www.antixforum.com/forums/topic/unable-to-create-an-interface-to-connman-on-the-system-bus/
https://www.antixforum.com/forums/topic/connman-cant-connect-to-wifi/
https://www.antixforum.com/forums/topic/i-get-a-cmst-error-everytime-i-boot-into-my-system/
https://www.antixforum.com/forums/topic/cmst-critical-error/
https://antixlinux.com/connman-not-connecting/
```
It seems amtiX has been plagued with this connman issue for years, and in my case it IS a connman issue, not an Xfce issue, but it only appears when Xfce is running.... if I use IceWM there is no issue, connman works. 

Lets try some of the recommended fixes
 - a start of connmand does not work ... it just stops again
 - /etc/connman/main.conf has
   `#PreferredTechnologies = `
   Replace this with
   `PreferredTechnologies = ethernet,wifi`
 Then `dinitctl start connmand` and it hangs for a minute and interface eth0 coes up, then it times out and eth0 goes down again.  
 - /etc/connman/main.conf has
   `#OnlineCheckMode = one-shot`  and that is the default
   replace with
   `OnlineCheckMode = continuous`
  Same .... connmand starts then times out

Nothing works. My solution is to abandon connman , and use ifupdown. 
To do that we disable connmand
```
dinitctl disable connmand
```
and then we can either use `ceni` or we can edit the file `/etc/network/interfaces` by hand. I did the latter
```
[nevj@trinity ~]$ cat junk.ceni
root@antixdinit:/etc/network# cat interfaces
# interfaces(5) file used by ifup(8) and ifdown(8)
# Include files from /etc/network/interfaces.d:
source /etc/network/interfaces.d/*

auto eth0
iface eth0 inet dhcp
```
Then reboot , and eth0 interface comnes UP... no problem.
I still get the message about `cmst error`, but that can be ignored.

I now have a working Xfce in antiX.  It is really minimal... has a terminal,  a File Manager and an Application Finder. 
I need to add some more Xfce4 packages, but that is trivial. We have demonstrated that one can install Xfce in antiX25.




