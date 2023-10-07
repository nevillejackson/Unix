## FreeBSD Desktop environments ##
FreeBSD has all the main Desktop Environments ... KDE, Gnome, Mate, Xfce, Cinnamon, LXQT are officially supported. The documentation for installing and starting these is here

https://docs.freebsd.org/en/books/handbook/desktop/

There are also packages for Budgie and Lumina DE's, and the
 Enlightenment, ICEwm, Fluxbox, i3, and Windowmaker
 Window Managers

We are going to look at Xfce as an example.

## Installing xfce ##
It is necessary to have a working X11 ( or Wayland) before attempting to install a desktop environment. 

The available version of xfce is 4.18. To install

```
pkg install xfce
```

There are two additional requirements over those needed for X11. Xfce requires `dbus`. Dbus is present in the Freebsd install, but it is not enabled. To enable it  we do

```
edit /etc/rc.conf
add the line
dbus_enable="YES"
```

One  also has to mount the /proc file system . Make an entry in `/etc/fstab` as folows

```
edit /etc/fstab
add the line
proc     /proc     procfs     rw    0  0
```

Reboot and you are ready to start xfce.
The idea of getting X11 installed and working first is to ensure
that this DE install goes smoothly.

## Starting xfce ##
The xfce package does not install a Display Manager.  You login at the prompt and type

```
startxfce4
```
It will start up and the default screen looks as follows

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/bsd/xfce.png?raw=true">
</p>

We have changed the background to a FreeBSD image.

From here on it works the same as any Linux with Xfce. 
