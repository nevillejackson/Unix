###  Summary of steps to add Xfce4 to antiX26 using turnstile session manager  ###

To install and configure Xfce in antiX do the following
 1. Install some basic packages
```
apt install xfce4
apt install xfce4-goodies
```
Some documents recommend installing lightdm, but I have found that counterproductive. I am staying with the `slimski` Login Manager.

 2. Ensure turnstiled is running
 The default antiX26 should boot with turnstiled and seatd running.
 If not they should be enabled to start at boot , using each init system.

 3.  Setup panels. 
Reboot, choose xfce in the Login Manager and configure panels
There are 4 workspaces in the top panel, but the bottom panel is almost empty.
To configure an Xfce panel in antiX, there is no menu button … you right click on the panel , and go to Panel → Panel Preferences.
There I was able to add Firefox, Thunderbird, and Thunar , and I was able to add a cpu-monitor, network-monitor, and sensor-monitor on the right hand end.
I also added the "Add Menu Item" command.

 4. Make Thunar see internal partitions
At this stage Thunar will see files, but not internal partitions. To make internal filesystems visible to Thunar, one needs to do
```
apt install thunar-volman
[put yourself in the plugdev group]
usermod -aG plugdev <username>
apt install gvfs
apt install udisks2
```
Now (after a reboot) , Thunar will be able to see internal filesystems, and they will also appear as icons on the background screen.
But, Thunar will still not be able to mount internal filesystems.... there is a permission problem. 
To fix the permissions problem mounting internal filesystems do one of the following
```
apt install lxpolkit
apt install lxsession
[this will provide a permissions popup]
```
or
```
cd /etc/polkit-1/rules.d
vi 10-enable-mount.rules
add the following lines
polkit.addRule(function(action, subject) {
    if (action.id == "org.freedesktop.udisks2.filesystem-mount-system" &&
 subject.isInGroup("storage")) {
        return polkit.Result.YES;
}});
exit and save
[put yourself in the storage group]
groupadd storage
usermod -aG storage <username>
reboot
[The polkit rule will allow mounting without the permissions popup.]
```
I chose the latter.

Then one can use  Thunar, or double click on filesystem icons, 
as well as the options which work with IceWM, ie Disk Manager or `sudo mount /dev/sdxx mountpoint`

 5. Set the mouse focus
The antiX menus do not contain xfce4-settings-manager.... I have to start it at the CLI by typing
```
xfce4-settings-manager
```
Then I get a GUI for settings and I can do the following
 - Window Manager -> Focus -> Focus follows mouse

 6.  Add some antiX programs to the menues
 There are two usefile antiX settings programs missing from the Xfce menues. They are `xfce4-settings-manager` and `antiX Control Centre`. One can add items to the menues with Applications-> Settings -> Add Menu Item, or use my `Add-desktop` icon which I have added to Panel2
It requires knowing the name of the program executable and the name of its .desktop file. For example AntiX Control Centre executable is `/usr/local/bin/antixcc.sh` and its desktop file is `antiXCC.desktop`.

 7. Add a personal background image.
Xfce has its default background image hardwired in the code. It is `xfce-x.svg`
If you wish it to boot up with a different image, the simplest way is to make a symbolic link
```
cd /usr/share/backgrounds/xfce
save the default image
mv xfce-x.svg xfce-x.orig.svg
then make a link
ln -s path-to-new-image xfce-x.svg
```
So it now looks like this
```
-rw-r--r-- 1 root root    9298 Mar  3  2025 xfce-x.orig.svg
lrwxrwxrwx 1 root root      42 May 20 21:21 xfce-x.svg -> /usr/share/backgrounds/mourne/mourne14.jpg
```
You may have to reselect the image in 'Desktop Settings' for the current background to change.

 8. Fix logout issues with slimski
If you Logout using the `Exit` icon on the top panel, only Logout works, and  you end up back at the Login Manager (slimski) screen. There you are supposed to type 'halt' or'reboot' as user and the root password, but it does not work because slimski is in 'preview' mode. 

The QUICK FIX involves bypassing slimski altogether , and halting or rebooting directly ( ie as with `poweroff` or`reboot` CLI commands) . 

The SECOND FIX involves changing slimski configuration.
Do the following
```
edit slimski.local.conf 
add the following

systemhalt_enabled  true
poweroff_enabled    true
systemhalt_cmd  /usr/sbin/poweroff
reboot_enabled      true
reboot_cmd         /usr/sbin/reboot

default_sessiontype    xfce 
```
That alone  makes use of 'halt' or 'reboot' at the slimski screen work, but the "Exit" top panel button only offers logout.

Finally, slimski needs a delay .
Go to /etc/dinit.d and edit the file slimski 
```
type = process
command = /usr/local/bin/my-slimski-wrapper.sh
logfile = /tmp/slimski.log

depends-on = seatd
```
Then write the wrapper program my-slimski-wrapper.sh
```
#!/bin/sh
sleep 8
exec /usr/bin/slimski -nodaemon -z -i
```
So slimski waits 8 seconds when started or restarted by dinit.
Similar modifications are needed for other init systems.

Now any user can halt or reboot on the slimski screen.
Logout->Logout  will drop you back to the Login Manager screen.  From there one can type 'halt or 'reboot' for the username and give the superuser password , and it will reboot or poweroff. 

If the delay is really inadequate, it may drop you back to the Login Manager screen in 'preview' mode, in which case attempts to halt or reboot will fail with a message 'systemhalt_enabled=TRUE but this action is unavailable during preview'.  In this case relogin as yourself and use `poweroff` or `reboot` at the CLI.

The above is a known issue with slimski. If it restarts while the xserver is still running it will start in preview mode.

The THIRD FIX is to make my own Exit icon which uses the antiX `desktop-session-exit` script which is found in /usr/local/bin.
Add-Menu-Item -> setup the sdd-desktop popup with Item Location set to Desktop Shortcut.  Set Item name to 'Exit antiX'. Set Item Commant to '/usr/local/bin/desktop-session-exit'. Set File Name to 'desktop-session-exit'.

This third option is the best. It allows anyone to Logout, Reboot, or Shutdown without permissions issues. 



 9. Change the default WM in slimski 
We want slimski to offer Xfce at boot up. 
Go to /etc/slimski.local.conf and change the following
```
## You can also set here the default sessiontype
# default_sessiontype     zzz-icewm
default_sessiontype     xfce
```

 10. Setup autostarted apps.
 Go to Applications -> Other -> xfce4-Settings-Manager
 Choose Session and Startup -> Application Autostart
 Tick or untick apps as required. 


