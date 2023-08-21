## Desktop Environments available in Hyperbola Linux ##
 We have seen that it is possible to start with a Display Manager and add
features so that it begins to look like a primitive DE. That is one approach, 
the other is to look for a dropin fully configured DE.
Unfortunately Hyperbola will not run any of the well known DE's, because they use 
packages like `dbus` which Hyperbola philosophy does not approve. 

 There is, however,  one  DE which avoides all of the hyperbola-banned packages,
 because it is totally  written in C++ and has no dependencies.
 A package with no dependencies can be run on any Linux distro, or even on BSD or Windows.

Lumina is not fully developed. It is not a dropin solution. However,
it is worth exploring whether Lumina can form the starting point for 
setting up a respectable DE for Hyperbola


### Getting Lumina ###
I have used Lumina previously. It is available in Void Linux, and there is 
a report 

https://github.com/nevillejackson/Unix/blob/main/voidlumina/voidlumina.pdf

of my installing and using it in Void Linux.
If you look at that report you will see that some of the
 inhouse applications which come with Lumina are really
 only place-makers, ie they are under development.
 Others , like the Insight File Manager are usable.
 However , one can always add other applications.
 The main thing about Lumina is that it is fully
 configurable from the GUI... it can be made to look like
 any of the main DE's in terms of placement of panels,
 placement of system tray, icons, and plasmids.
 Appearance (themes etc) is also fully configurable.
 It is Qt based so windowns have a Qt appearance.

The version of Lumina available in Void is 1.6.2. 
One way of getting Lumina would be to use Void to download the source code and
then compile and install it in Hyperbola. That is one of the advantages of a package with no dependencies... it can be moved anywhere.

Lumina also has its own website
https://lumina-desktop.org/
and it points to where the sources are kept, in a Github repository
https://github.com/lumina-desktop/lumina/releases
so that is another way to get Lumina. 

However Hyperbola has Lumina in its own repository. It is version 1.6.0., slightly older , but we shall try with this binary install first, simply because it is easier.
 
### Installing Lumina in Hyperbola ###
First have a look at what packages are available

```
$ pacman -Ss lumina
extra/lumina-archiver 1.6.0-5 (lumina-extra) 
    Lumina Desktop Environment - Archive Manager
extra/lumina-core 1.6.0-5 (lumina) 
    Lumina Desktop Environment - Core Components
extra/lumina-core-utils 1.6.0-5 (lumina) 
    Lumina Desktop Environment - Core Utilities
extra/lumina-fileinfo 1.6.0-5 (lumina-extra) 
    Lumina Desktop Environment - File Information Utility
extra/lumina-fm 1.6.0-5 (lumina-extra) 
    Lumina Desktop Environment - File Manager
extra/lumina-mediaplayer 1.6.0-5 (lumina-extra) 
    Lumina Desktop Environment - Streaming Media Player
extra/lumina-photo 1.6.0-5 (lumina-extra) 
    Lumina Desktop Environment - Image Viewer
extra/lumina-screenshot 1.6.0-5 (lumina-extra) 
    Lumina Desktop Environment - Screenshot Utility
extra/lumina-textedit 1.6.0-5 (lumina-extra) 
    Lumina Desktop Environment - Plaintext Editor
```

The base package is `lumina-core` but we shall install all of the above 
in order to get as full a starting point as possible, so as superuser

```
pacman -S lumina-core lumina-core-utils
pacman -S lumina-archiver lumina-fileinfo lumina-fm lumina-mediaplayer
pacman -S lumina-photo lumina-screenshot lumina-textedit
```

That is all there is to it, we just need to activate 
`exec start-lumina-desktop` in `.xinitrc` and `startx` will start it.
The default startup screen looks as follows

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/luminadefault.png?raw=true">
</p>

Battleship grey without trimmings. All it has is a tiny menu
button at the left of the black system tray at the bottom, and and RSS feed 
plasmid at the bottom right.  It needs a bit of work.

### Adding things to the default lumina desktop ###
Lets see if we can get a better background, and we need a terminal window 
to do things. 

We need to add Xterm before we start the DE, so, on the alphanumeric screen after 
login we do

```
pacman -S xterm
```

and then `startx` to start the DE. 
There is a menu for setting the Display Wallpaper.
 Right click on background, then Preferences, 
then Desktop Settings, and you get the Desktop Settings Menu

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/desktopsettings.png?raw=true"
</p>

We can see the Wallpaper button on the top right, plus there are
buttons to configure Panels, Themes, Default Applications, etc. 
Lumina is fully configurable via the GUI... one doers not need to edit config files.

Now click on Wallpaper, then '+', then choose Solid Color in the dropdoewn menu,
and you get a Select Color menu

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/selectcolor.png?raw=true">
</p>


I have setup a dark red color rgb(40,0,10) because a dark background is best for night work.
Click OK, and you have it. The other options under '+' allow Files or Directories containing images
to be used for background. So you can have a scene or whatever.  We shall look at that later

So what does it now look like? I started Xterm and some menues,
and changed the plasmid to a load monitor, and we get

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/de1.png?raw=true">
</p>

Some other icons apart from Xterm have appeared, because I already have some 
apps installed from the work on display Managers. In Lumina, most apps bring up their 
desktop icon automatically... so we have Iceweasel-UXP browser, but 
Dillo did not bring up an icon. The Insight File Manager is Lumina's own File Manager,
and is about the only one of Lumina's inbuilt packages that is 
reasonably functional. You can also see the SpaceFM File Manager,
the gthumb image viewer, and MuPDF pdf viewer. 

My Xterm is configured by its `.Xdefaults` file in my home dirtectory.
That sets the color ( again dark) and the right scrollbar. 

The menus show choosing preferences -> Desktop Settings, as described above. 
Note the system tray is at the bottom ( it can be moved) and there are no
other panels at present. The white panel at the top is virt-manager.

### Adding more applications ###
The way to use Lumina is to discard all its half-developed inbuilt apps, and install your own. That is easy in a normal distro, but in Hyperbola there is a limited range of available apps. 
I have tried to gather apps covering things most users would require
in order to show what sort of DE can be built in Hyperbola.

#### Web Browser ####
Te best available is `iceweasel-uxp`. The UXP stands for Unified XUL PLatform. XUL is a markup language , developed by Mozila for writing GUI's Firefox used to use XUL, but today it is used by Pale Moon browser and by Hyperbola.

Iceweasel is quite competent. It has tabs, bookmarks, and a menu shown below

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/iceweaselmenu.png?raw=true">
</p>

It displays the itsfoss.com site properly, but when you go to the Forum , it says 
"your browser is unsupported", and some things are missing, for example a scrollbar. 
Actually the Forum site is a good test of browsers. Discourse has stringent requirements.

The available search engines are somewhat restrictive

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/iceweaselsearch.png?raw=true">
</p>

None of the major search engines are available, the best seems to be the default SearX. What you can do is load the google ( or whatever) page, then use it to search. It seems to deal with google search results quite well, and if you access the itsFOSS Forum page via a google search, it formats normally?

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/iceweaselfoss.png?raw=true">
</p>

Why accessing a page via a google search would change the
 browser's ability to display it, is beyond me?
  However that is clearly the way to make iceweasel-uxp
 useable.

Given those shortcomings, lets have a look at Dillo

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/dillo.png?raw=true">
</p>

Dillo does not have a search engine at all. What you must  do is load the google ( or whatever) page.  Dillo will display the google search page, but it seems to have difficulty with most websites. It is plainly out of date.

#### Mailer ####
The choice is `icedove-uxp` or a simple text based mailer like `mutt`
Icedove-uxp looks just like Thunderbird. 
So I setup my email and tried it

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/icedovemail.png?raw=true">
</p>

No issues there.

#### File Manager ####
There is the inbuilt Lumina filemanager called `Insight`

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/insight.png?raw=true">
</p>

It is about the only reasonably usable Lumina App. 

A better option is `SpaceFM`, which we have already covered in the topic 
"Getting X windows into Hyperbola".

#### PDF Viewer ####

The best available is `MuPDF`. It comes in two varieties `mupdf` and `mupdf-gl`.
The latter is best choice, it uses OpenGL. There is also a command line set of pdf tools callled `mupdf-tools`.

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/mupdf.png?raw=true">
</p>

You move between pages and lines  with the keyboard, therr are no mouse buttons for navigation. The graphic quality is fine, I can read that fine text in the box. 

#### Image Viewer ####
Here we go for the old fashioned  but capable `gthumb` viewer.

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/gthumb.png?raw=true">
</p>

Hyperbola also has `Gimp`  for image manipulation, and `graphicsmagick` which is an `ImageMagick` substitute. 

#### Office ####
Hyperbola has LibreOffice... the full set calc, draw, math, base, impress, and writer. 
<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/libreoffice.png?raw=true">
</p>


 No issues there, it is as good as any distro can provide.

### Icons and Panels ###
In most cases, when you install a new package, Lumina automatically brings up an icon on the DE background. There has to be a `.desktop` file in `/usr/share/applications`, and in some cases there will be a link in `~/Desktop`. 
The package `MuPDF` came with a .desktop file but it had the line 
`NoDisplay=true`
which I had to change to 
`NoDisplay=false` 
to get an icon. 
If there are preinstalled apps, when Lumina is installed, it detects them and makes the icon. I already had Xterm, iceweasel-uxp, and SpaceFM when I installed Lumina. 

So after all the above installs, and a few others, the DE looks as follows

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/de2.png?raw=true">
</p>

We have a clutter of icons on the background. The Xterm is as before. We have 2 panels, a bottom panel which is the system tray, and a popdown upper panel ( you can just see one line of it at centre top) which has the workspace switcher and some launchers for Lumina apps. At the lower right I have replaced the RSS feed plasmid with a notepad. 
To tidy it up and to show setting up of a panel we are going to move all the LibreOffice icons to a panel on the right. So Preferences -> Desktop Settings -> Panels, and we get the panel setup window

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/panelsetup.png?raw=true">
</p>

We see 2 panels, which reflects the current setup. Each panel has been set to show the panel content (the right hand button) . The other 2 buttons set its position and properties. 
To make a new (third) panel we click `+` at the top left, and it opens a third panel subwindow. 

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/panelsetup2.png?raw=true">
</p>

We have set it to appear on the right. We now add the content

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/panelsetup3.png?raw=true">
</p>

and delete all the LibreOffice Icons, reduce the size of the remaining icons and we have a new DE

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/de3.png?raw=true">
</p>

We have all the LibreOffice icons in the right panel,
 they do not have colors, but they produce a popup 
description when you hover the mouse pointer

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/hover.png?raw=true">
</p>

That is not perfect, but it seems this is a limitation of
 Version 1.6.0 of Lumina. In the later version 1.6.2 
( which I have in Void Linux) one can get coloured icons
 in a panel. For example, from my Void/Lumina1.6.2 DE
 
<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/icon162.png?raw=true">
</p>

One still gets symbols, rather than icons, but they are coloured in Lumina 1.6.2

The remainder of the DE is OK. The upper autohide launcher panel works, again it does not have coloured icons. The lower system tray could be a better colour. The smaller remaining icons are cleaner.

### A better load monitor ###
I installed Conky. It is much better than the inbuilt Lumina 
monitor widget

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/conky.png?raw=true">
</p>

I made the right panel autohide, to keep it out of the way,
and changed the panel colour.There are a number of background widgets available 
with Conky, the one shown is a system summary.

### Other apps ###
You can see icons for

 - *Dia*  diagram constructing program
 - *Galculator* scientific calculator
 - *Scribus* desktop publishing program

### Getting a login screen ###
I am quite happy to login on an alphanumeric screen, and start the window system with `startx` and `.xinitrc`.
Some people prefer a Login Manager (also called Display Manager).
The choice in Hyperbola is SLiM, or wdm (an xdm fork).

I tried `wdm`, it simply would not work. The daemon would not start when setup with 
`rc-update add wdm` , and starting it by hand produced no error messages. I gave up on wdm. 

We will try SLiM

```
pacman -S slim
resolving dependencies...
looking for conflicting packages...

Packages (1) slim-1.3.6-8

Total Download Size:   0.89 MiB
Total Installed Size:  6.11 MiB

:: Proceed with installation? [Y/n] y
:: Retrieving packages...
 slim-1.3.6-8-x86_64      912.2 KiB   252K/s 00:04 [######################] 100%
(1/1) checking keys in keyring                     [######################] 100%
(1/1) checking package integrity                   [######################] 100%
(1/1) loading package files                        [######################] 100%
(1/1) checking for file conflicts                  [######################] 100%
(1/1) checking available disk space                [######################] 100%
:: Processing package changes...
(1/1) installing slim                              [######################] 100%
:: Running post-transaction hooks...
(1/1) Displaying hyperrc service help ...
        ==> Add a service to runlevel:
        rc-update add <service> <runlevel>
sh-5.1# rc-update add slim
 * service slim added to runlevel default
```


Reboot and we get a login screen

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/slimlogin.png?raw=true">
</p>

That is the default SLiM screen. It just has a login slot, no options to start the various Display Managers or Lumina? 
This version of `slim` seems to have a problem . When it is running , one can not logout from Lumina with the GUI buttons... even Power Off only results in a return to the Slim login manager screen. The only way to power off seems to be to use 
```
sync
halt
```
as superuser. 

After reading everything I could find on SLiM ,
 I discovered that what I thought was a problem,
 was actually a feature.
 No matter how you exit any Window Manager or DE,
 if SLiM is running, it always returns you to the SLiM
 login screen. From there you have the following options 
which can be typed into the `Username` box, followed by 
the root password in the `Password` box 
 
 - *console* - launch a terminal
 - *halt* - does a shutdown
 - *reboot* - does a reboot
 - *exit* - exits to a bash shell
 - *suspend* - suspends linux

so SLiM takes over what is normally done with the poweroff buttons in a DE. 
OK, I think I can get used to that.  It is what one would want on a multiuser computer, where only root should be able to do a shutdown.

I also discovered that SLiM does have options to choose a Window Manager or DE on its login screen. You have to hit 'F1' key to display and step thru them. 
This only works if have setup the file `$HOME/.xinitrc` properly. 
I found the following example `.xinitrc` on the Arch Wiki link listed below,
 and modified it to include a default ( ie if you do not press `F1`),
 Here is my modified `.xinitrc` file

```
#!/bin/sh
DEFAULT_SESSION=start-lumina-desktop
if [ -d /etc/X11/xinit/xinitrc.d ] ; then
 for f in /etc/X11/xinit/xinitrc.d/?*.sh ; do
  [ -x "$f" ] && . "$f"
 done
 unset f
fi
if [ -z $1 ] ; then
	exec $DEFAULT_SESSION
else
	exec $1
fi
```

With that setup I could choose to start any of 

 - *fluxbox* lumina's window manager
 - *icewm* starts icewm on login screen
 - *icewm-session* normal icewm X window manager
 - *fvwm3* latest version of fvwm window manager
 - *lumina* desktop environment 
 - *windowmaker* Unix NeXTSTEP environment

There a couple of things there that I did not deliberately install but were dragged in by other packages, for example Lumina uses Fluxbox.

There is also the file `/etc/slim.conf`.  I made some minor changes there. 
SLiM in Hyperbola comes only with the default theme. The default theme is in `/usr/share/slim/themes/default`. You can change the background putting a file `background.png` or `background.jpg` in that directory.
I had one attempt at making a custom login screen

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/hyperbola/slimcustom.png?raw=true">
</p>
 
One could improve on that. I spent more time on the login screen, than on the whole DE.

### Conclusion ###
The aim was to show that one can build a workable desktop in Hyperbola,  starting from Lumina DE and adding what one needs. 
I think we have gone close to achieving that aim. There are enough apps available to
build a a working setup and Lumina has sufficient configurability .
It would have been better to have started with
 a later version of Lumina.

The login screen is workable, but SLiM took a lot of effort to configure. 
The Lumina DE can be made as good as any of the major DE's;
 its configuration GUI is first class,
 but its inbuilt apps need work.
 That is not critical, because you can replace them with
 other apps.
 There are sufficient apps in the repositary to build a usable Office or Graphics DE. Dont know about sound or video. 

The key point is that Hyperbola really does demonstrate that one can discard  much of the bloatware that has crept into Linux, and still have a working system.





### Links ###
https://lumina-desktop.org/
.... official website

https://en.wikipedia.org/wiki/Lumina_(desktop_environment)
.... wikipedia

https://fossbytes.com/lumina-1-2-desktop-download-features
.... link has disappeared

https://github.com/lumina-desktop/lumina-docs/blob/master/luminaplugins.rst
.... details on plugins

https://github.com/lumina-desktop/lumina-docs
..... all the docs

https://github.com/nevillejackson/Unix/tree/main/voidlumina
.... NJ' Void/Lumina report

https://wiki.archlinux.org/title/SLiM
..... Arch wiki on SLiM

https://pclosmag.com/html/Issues/201305/page14.html
..... SLiM tutorial
