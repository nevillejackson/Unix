### Adding Trinity Desktop Environment to antiX26 ###

I installed antix26-x64-full.iso. Then, set the repo to a local mirror, did `apt update` and `apt upgrade`. Then did the following  to install Trinity DE.

 1. There are stable and rolling versions of TDE available from repositories.
I chose a stable version for Debian Trixie.
One has to configure the apt package manager
```
Create the file /etc/apt/sources.list.d/tde.list and add the following lines to it.
# TDE R14.1.x series

deb http://mirror.ppa.trinitydesktop.org/trinity/deb/trinity-r14.1.x trixie main deps

# Optional sources
deb-src http://mirror.ppa.trinitydesktop.org/trinity/deb/trinity-r14.1.x trixie main deps
```
Replace 'trixie' with your actual distribution release name (for example 'trixie'

 2. One needs to import a key
```
The best way to install GPG key is to manually download and install package trinity-keyring. The advantage is that this method of installation is not dependent on the availability of the key server. Another advantage is that the key is not integrated into the global apt keyring, but is installed as a separate file:

$ wget http://mirror.ppa.trinitydesktop.org/trinity/deb/trinity-keyring.deb
$ sudo dpkg -i trinity-keyring.deb
```
It does not matter where the file `trinity-keyring.deb` is stored. 
The actual keyring `trinity-keyring.gpg` will be installed by `dpkg` in `/usr/share/keyrings`.

 3. Install trinity
```
apt-get update
apt install tde-trinity
```
After installing the environment, it is also a good idea to install the localization packages so that you can switch the environment to your native language. For example, for Czech you would run:

```
apt-get install tde-i18n-cs-trinity
```
I stayed with the default en_US

 4. After rebooting , the Login Manager ( slimski) come up with the default still zzz-icewm.  We will reset tha tlater.
One can choose TDE using F1 key, and Trinity the starts. It looks like this
 .... tde1.png
a welcome popup, and Conky is inherited from antiX.
 One can go on throught the help screens, and one eventually arrives at 

 ... tde4.png

where one can see the lower panel with 4 workspaces (it calls them desktops) , a Menu launcher,  and various icons. There is only one panel. Its memory usage at idle is about 405M according to Conky. 

#### 5.  Konqueror or Firefox? ####
The first thing to note about Trinity is that it pushes Konqueror as both a Browser and a File Manager.  Ignore that , it is dated .  Konqueror as a browser will not cope with modern wesites. It works as a File Manager, but there are better choices.

One should use Firefox for browsing ( it is installed) , and probably Dolphin ( the package dolphin-trinity needs to be installed) as a File Manager.  

#####  5a. Adding Dolphin to Trinity #####

```
apt install dolphin-trinity
apt install polkit-agent-tde
```
It works. Dolphin is found in System -> File Manager (Dolphin). 
Unlike Thunar in Xfce in antiX, Dolphin in Trinity in antiX can see all the internal hard disk partitons. However it  does not have mount permission. The default file-manager ( zzz-FM) is the same. 

To fix this do the following
```
cd /etc/udevil
vi udevil.conf
modify the following lines to
allowed_internal_devices = *
allowed_media_dirs = /media/$USER, /run/media/$USER , /media

```
The make a polkit rule 
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
Dolphin will now mount any internal disk partiton without requiring authorisation.

When you look at the content of a file with Dolphin, it brings up a window called 'Session Chooser-Kate' and you need to click on 'New Session' and it will display the content (if it is text) in the Kate editor. When you terminate Kate, you need to delete the Session if you are just looking.


 6. To Exit, one can use the dropdown menu on background, or the Menu icon in the panel.  It gives the following choices 

 ..... tde5.png


and no matter what you choose it drops you back th the slimski Login Manager.  There one is supposed to be able to type 'halt' or 'reboot' in the user name section, and give the root password, but this does not work .... it  gives the same 'preview' error as Xfce, and all you can do is log back in again. So Trinity in antiX has the same Exit problems as Xfce. We ave to go thru all the same steps

FIRST
One can exit by typing 'poweroff' or 'reboot' at the CLI as root. 

SECOND
We need to edit `/etc/slimski.local.conf` to define the halt and reboot commands.
```
systemhalt_enabled true
poweroff_enabled true
systemhalt_cmd /usr/sbin/poweroff
reboot_enabled true
reboot_cmd /usr/sbin/reboot

[ and while there fix te default desktop ]
default_sessiontype  tde
```
That alone achieves a situation where halt/reboot work intermittantly. 
What is also needed is some delay in the startup of slimski, because it is restarting too soon and thinks it is being used in preview mode. 
We can modify the init system to introduce a delay
```
[Go to /etc/dinit.d and edit the file slimski]
type = process
command = /usr/local/bin/my-slimski-wrapper.sh
logfile = /tmp/slimski.log

depends-on = seatd
```

Then write the wrapper program my-slimski-wrapper.sh 
```
cd /usr/local/bin
vi /usr/local/bin/my-slimski-wrapper.sh
[add the lines]
#!/bin/sh
sleep 6
exec /usr/bin/slimski -nodaemon -z -i
[ make it executable]
chmod 755 my-slimski-wrapper.sh
```
So slimski waits 6 seconds when started or restarted by dinit.
Similar modifications are needed for other init systems.


Now any user can halt or reboot on the slimski screen.
Logout->Logout or Logout->Shutdown or Logout->Reboot  will drop you back to the Login Manager screen.  From there one 
can type 'halt or 'reboot' for the username and give the superuser password , an
d it will reboot or poweroff. 

If the delay is really inadequate, it may drop you back to the Login Manager scr
een in 'preview' mode, in which case attempts to halt or reboot will fail with a
 message 'systemhalt_enabled=TRUE but this action is unavailable during preview'
.  In this case relogin as yourself and use `poweroff` or `reboot` at the CLI.

The above is a known issue with slimski. If it restarts while the xserver is sti
ll running it will start in preview mode.

THIRD
There is nothing else needed.   Unlike Xfce , Trinity seems to have the necessary permissions set , so that a user does not have to become root to halt or reboot. 

If the preview problem with slimski persists, or one dislikes shutting down via slimski, one can bypass it by making one's own Exit icon using the antiX `desktop-session- exit` script which is found in /usr/local/bin.
Settings->Add-Menu-Item -> setup the add-desktop popup with Item Location set to Desktop Shortcut.  Set Item name to 'Exit antiX'. Set Item Command to '/usr/local/bin/desktop-session-exit'. Set File Name to 'desktop-session-exit'.
If you get the alert about disabling automatic applications menu answer 'no'.
The item 'Exit antiX' appears in an icon on the TDE background.
This allows anyone to Logout, Reboot or Shutdown, without slimski issues.

 7. Setup autostart apps.
In the Trinity Control Centre, if you go to TDE Components -> Service Manager, you get a list of startup services .... but none of the things on the startup desktop are there ... Things like Magnus  and Yad and the Other Desktops window. 
Some of then may be being started by antiX? 
Then I found this
"If you have the kcontrol-autostart-trinity package installed, you can easily manage apps from the control panel:"
So
```
apt-get install kcontrol-autostart-trinity
```

Now, in the Trinity Control Centre, I can go to TDE Components -> Autostart Manager
.... and I get an empty Menu .   
Apparently antiX is starting those annoying startups, not Trinity. ???

 8. Add some antiX programs to the menues.
 There is one usefile antiX settings program missing from the Trinity menues. It
 is `antiX Control Centre`. One can add items to
the menues with Settings -> Add Menu Item ` 
It requires knowing the name of the program executable and the name of its .desk
top file. For example AntiX Control Centre executable is `/usr/local/bin/antixcc.sh
.sh` and its desktop file is `antiXCC.desktop`.
It appears in the menu under whatever you specify in te Item Category box.  I used Settings.

 9. Choice of Terminal. 
The background dropdown menu offers Konsole terminal. I found it had a poor set of colour settings and would not follow the theme if I wanted 'dark' background. 
A better choice is ROXTerm ... found in Utilities -> Terminal Emulator (Rox-Term). It at least sets a comfortable grey background and readable colours.
There is also Utilities -> Terminal Emulator (uRxvt) which sets a black background by default.
To change themes go to Settings -> Trinity Control Centre.
To change an individual Konsole terminal window use Settings->Configure Konsole in the Terminal top row.
The annoying feature of Konsole is that each new Terminal you open reverts to the default black on white text colour, and a tiny font.

 10. Session Saving.
Trinity saves desktop sessions by default on exit. When you log back in, all the previous session's windows, in all 4 workspaces, reappear. 


 11. Is Trinity usable in antiX?
 I find it is OK for basic Terminal work, browsing and File Management. Some of the menu items simply do not work. The desktop is cluttered with Yad popups that I can not suppress.  If you install the apps you need , it should be workable. It is the simplest KDI-like Desktop that I could find. 

 Trinity seems to work perfectly in antiX26 with the default turnstile login daemon. There is no need to consider consolekit or elogind.

 I had one problem with graphics programs not starting in the s6-66 init system.  The message indicates Authentication failure. If this happens remove your .Xauthority file and reboot. THe system will generate a new clean .Xauthority file when it reboots. 
