### Configuring Lumina Desktop in antiX ###

#### 1. Panels ####
 Go to Applications -> Settings -> Desktop Configution -> Panels
 It allows 2 Panels , they can be Top and Bottom , or either side.
 I setup the bottom panel for system tray plus Applications and Workspace Switcher. The workspace switcher is nice , in Lumina you can move windows between workspaces dragging them off the side of the screen.

 My top panel is frequently used apps... Console Window ("Roxterm"), Console Window ("uRxvt"), Screenshot, Insight File Manager, Web Browser ("Firefox ESR").

#### 2. Icons ####
 These default to the left side. I removed File Manager and added my own "Exit antiX" icon, which overcomes the reboot and shutdown problem with slimski.

Applications -> Settings -> Add Menu Item
Item Name: Exit antiX
Item Icon: cross-red.png
Item Category: System
Item Command: /usr/local/bin/desktop-session-exit
Item Location: Desktop Shortcut
File Name: desktop-session-exit
OK
An Icon should appear. 
I also added a Rox-Term icon.

#### 3. AntiX Control Centre ####
Lumina does not pick this up in its menus. I had to add it
Applications -> Settings -> Add Menu Item
Item Name: AntiX Control Centre
Item Icon: antix-logo.png
Item Category: System
Item Command: /usr/local/bin/antixcc.sh
Item Location: Application
File Name: antiXCC
OK
A menu item should appear under Applications -> System
It also appears as an icon?  Not sure why.
One could also now add it to the upper Panel.

The default icon size  is too large. To adjust right click on an icon -> Modify Item -> Decrease Item Sizes .  It change all icons, not just the one clicked on.

#### 4. Attempt to fix slimski preview bug ####
If you use Applications -> Leave -> (Restart or Poweroff) 
it logs you out to the slimski Login Manager screen, where you are supposaed to be able to enter "halt" ( or "reboot") as a user name and give the root password. It fails ... slimski gives a "preview error message" 

What is happening is that slimski is restarting too quickly, before the `xserver` has terminated, so it thinks you want preview mode ( preview mode is for debugging in slimski).

To try and counter this we can put a delay in the slimski startup.
This can be done in slimski.local.conf
```
[disable the command]
#atlogin_cmd      exec /usr/bin/bash -login /etc/X11/Xsession "%sessiontype"
[and substitute]
atlogin_cmd  sleep 3 &&  exec /usr/bin/bash -login /etc/X11/Xsession "%sessiontype"
```
That makes a delay in the X session startup
I also use the init system to put a delay in slimski startup.

Go to /etc/dinit.d and edit the file slimski
```
type = process
command = /usr/local/bin/my-slimski-wrapper.sh
logfile = /tmp/slimski.log

depends-on = seatd
```
Then write the wrapper program my-slimski-wrapper.sh
```
cd /usr/local/bin
vi my-slimski-wrapper.sh
[edit in thefollowing lines]
#!/bin/sh
sleep 3
exec /usr/bin/slimski -nodaemon -z -i
[then make it executable]
chmod 755 my-slimski-wrapper.sh
```
So slimski waits 3 seconds when started or restarted by dinit.
Similar modifications are needed for other init systems.



That achieves 'half' a fix. 
If the delays are really inadequate, it may still drop you back to the Login Manager screen in ‘preview’ mode, in which case attempts to halt or reboot will fail with a message ‘systemhalt_enabled=TRUE but this action is unavailable during preview’.  

It also helps to edit slimski.local.conf to define the halt and reboot commands
```
systemhalt_enabled true
poweroff_enabled true
systemhalt_cmd /usr/sbin/poweroff
reboot_enabled true
reboot_cmd /usr/sbin/reboot

[ and while there fix the default desktop ]
```
For some reason I can not fathom, it is necessary to make these changes in /etc/slimski.conf.... in theory it should not be needed , but the making of an .iso is flawed if this is not done.

If this all fails, use the "Exit AntiX" icon. It always works because it bypasses slimski.

#### 5. Set Autostart Apps ####
 By default Lumina starts nothing . The two things that appear .... Other Desktops, and Conky .... are started by antiX.
 There is a menu item Applications -> Settings -> Edit AntiX Startup File.
It allows editing of the file `~/.desktop-settings/startup`

#### 6. File Manager and access to internal disk partitions ####
The `Insight File Manager` which comes with Lumina is workable.  It can see mounted partitions, but it can not detect or mount unmounted internal partitions. 
Therefore we have to look at providing an alternative File Manager

First we can try making devices visible to antiX
```
[put yourself in the plugdev group]
usermod -aG plugdev <username>
apt install gvfs
apt install udisks2
```
That should make internal disks and usb drives visible.
After reboot, internal partitions are visible to zzz-FM, but not to Insight. 
Therefore Insight is of limited use. 
We need to get Dolphin File Manager ( I chose Dolphin rather than Thunar because it is Qt based).

```
apt install dolphin

```
 That also installs `dolphin-data` and libraries `libkf6*` and `libqt6*`
An entry appears in Applications ->System -> Dolphin, and it starts.
It detects all internal unmounted partitons, but can not mount them or view the contents. The message is
```
An error occurred while accessing 'ArtixDinit', the system responded: Cannot request authentication for this action. The PolicyKit authentication system appears to be not available.: Not authorized to perform operation
```
When I check, polkitd is installed, as well as various libraries, but the daemon `polkitd` is not running before I start Dolphin, but it is running after I start Dolphin. Lumina does not start it because Lumina dies not use polkit, but Dolphin starts it. 
So I just need a polkit rule.

Make a polkit rule
```
cd /etc/polkit-1/rules.d
vi 10-enable-mount.rules
[add the following lines
// Allow udisks2 to mount devices without authentication
// for users in the "wheel" group.
polkit.addRule(function(action, subject) {
    if ((action.id == "org.freedesktop.udisks2.filesystem-mount-system" ||
         action.id == "org.freedesktop.udisks2.filesystem-mount") &&
        subject.isInGroup("storage")) {
        return polkit.Result.YES;
    }
});
```
That allows any user in the "storage" group to mount files.
Make sure user is in the "storage" group
```
groupadd storage
usermod -aG storage <username>
reboot
```
After reboot , Dolphin now appears as an icon and its menu entry has shifted to Applications -> Utility -> File Manager ("Dolphin").
That must be a Lumina feature. Other DE's dont automatically make icons or reshuffle menus.
Dolphin will now mount any internal disk partiton you click on,  without requiring authorisation.
That is the way a file manager ought to behave.
I think that solves the File Manager issue. 
I will put it in the top panel.

#### 7. Using Lumina ####
Lumina, in its present form with no active development, is a usable desktop. It has a small memory footprint, similar to Xfce and Trinity, and it has a more modern Qt feel than Trinity.  It is the most easily configured of the 3 DE's I have looked at for antiX
 
One issue I found is that conky starts when lumina is started, then dies. 
It can be started again at the CLI, but it dies every time you click on an icon or the background ? When you click on the workspace changer , it sometimes  starts again.? It appears this is a common conky issue, but none of the suggested fixes work for conky with lumina?

The most annoying thing, for me, about using Lumina, is that I have been unable to set 'Focus follows mouse'.  That is the traditional Unix mouse action. I miss it. Setting Applications -> Settings -> Desktop Configuration -> Window Manager Settings -> Focus Policy to 'Active Mouse Focus' seems to do nothing... it stays on 'Click to Focus'.

RoxTerm works well with `vi`, but uRxvt does not.

The default File Manager is set to Insight. If you want to be able to mount internal partitios in the File Manager, set it to Dolphin. Applications -> Settings -> Desktop Configuration -> Desktop Defaults -> Applictions -> File Manager. Choose File Manager ("Dolphin") in the list of applications.
