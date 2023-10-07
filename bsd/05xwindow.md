## Getting an X11 window system into FreeBSD ##
While FreeBSD does not come with any type of graphical terminal support, it can be added as a package or a port. We start with adding the X11 Window System.  We need to have X11 ( or Wayland) installed and configured, before we think about installing any Desktop Environment.

### Preparing virt-manager to support X11 ###
X11 requires some extra settings in virt-manager. These should be set before X11 is installed

 - EvTouch USB Graphics Tablet
   Ensure that this is present... it is usually there by default under Tablet
   If not present use
   Add Hardware -> Input -> EvTouch USB Graphics Tablet

 - USB Mouse
   This is not present be default ... add it with
   Add Hardware -> Input -> USB Mouse

### Installing the X11 Window System ###
The full documentation of X11 for Freebsd is here

https://docs.freebsd.org/en/books/handbook/x11/

The X11 metapackage is called `Xorg`  and it consists of 182 packages. Its installation is simple

```
pkg add xorg
```
This installs X11 in `/usr/local`. The binaries are in `/usr/local/bin` and the config files are in `/usr/local/etx/X11`

It is recommended that the user belong to the `video` group , so

```
pw groupmmod video -m username
```

In a VM one does not have to worry about graphics card drivers.
We did not install any extra fonts for X11, but that is covered in the Handbook X11 Chapter. 

The following additional packages are required for Xorg

```
pkg install utouch-kmod
pkg install xf86-input-evdev
```

and the `utouch` kernel module needs to be loaded at boot time. This is done by

```
edit /boot/loader.conf
add the line
utouch-load="YES"
```
Reboot , and you are ready to start Xorg

### Starting X11 ###
There is no login manager to automatically start X11. One starts it with 

```
startx
```

at the CLI. This executes a script at `/usr/local/etc/X11/xinit/xinitrc`, which starts the primitive `twm` Window Manager and opens 3 `xterm` windows.
Twm looks like this

<p align="center">
<img src=/common/Foss/bsd/twm.png">
</p>

My `xterm` windows are showing coloured because I have a custom `.Xresources` file in my home directory. The default will show a white background and no scrollbar, like the `login` window. 

`twm` is quite usable, if all you want is an `xterm`. The point of starting it here is to check that X11 is working properly. 
So do you have keyboard input and a mouse cursor?
If you did not specify `moused` as a service to be enabled during the install of FreeBSD, you will need to do it now, so

```
edit the file /etc/rc.conf
and add the line
moused_enable="YES"
```
then reboot and try again.

There may be some difficulty exiting from 'twm' in a VM. One can always become root and do `sync` and `halt`, or one can use the `shutdown` button in virt-manager.



