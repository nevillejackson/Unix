### The dinit init system ###

Most home desktop users are happy with whatever `init system` their favourite Linux distro comes with by default. In many cases this will be `systemd`.  In my view `systemd` is over-complicated for a home system. There are many alternative init systems ( sysVinit, runit, OpenRC, BSD-RC, S6, 66, Busybox, dinit).  Of those, I would have recommended `OpenRC` for a simple home-usr init system , but the more recent `dinit` init system is challenging strongly.

#### Features of dinit ####
The main attributes of dinit are
 - `Dinit` allows the user to start and stop daemons ( ie services) with simple commands similar to those used by OpenRC. 
 - `Dinit` handles system startup and shutdown. 
 - `Dinit` provides a process No 1 called `/usr/bin/dinit` which forks all other processes
 - `Dinit` provides parallel startup of services and dependency management
 - `Dinit` provides service supervision, ie it automatically restarts failed daemons
 -  Dinit allows users to setup services
 
#### How to get dinit ####
The only Linux distros that come with `dinit` as the default init system are Artix and Chimera. By far the easiest of those to install is Artix, so we will use Artix here. 

I started with a download of `artix-xfce-dinit-20240823-x86_64.iso` from

https://artixlinux.org/download.php


Note, Artix has downloads with various default init systems. You need to choose the dinit version.
The install is simple... Artix uses the Calamares installer for all but its `base` iso's.
It boots to a typical  Xfce screen

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/dinit/artix1.png?raw=true">
</p>

That is my favorite Xfce background. The default background is a double helix

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/dinit/artix3.png?raw=true">
</p>
 
and I have added some things to the bottom panel there.
You may wonder what all those disk partition icons are?  Xfce seems to do that in some distros... it is obvious that my machine has a large number of partitions.

So how do I know that it is using dinit?
```
ls -F /etc/dinit.d
acpid         cryptsetup-early  lightdm       metalog     statd
avahi-daemon  cupsd             lightdm-pre   mount.d/    tty1
bluetoothd    dbus              logind        nfs-server  tty2
boot.d/       dbus-pre          lvm2          ntpd        tty3
config/       dhcpcd            lvm-monitor   rpcbind     tty4
connmand      dmeventd          lvmpolld      rsync       tty5
cronie        elogind           lvmpolld-pre  sshd        tty6
cryptsetup    haveged           mdadm         ssh-keygen  wpa_supplicant
```
There is its configuration directory. Also
```
$ ps ax | grep dinit
    1 ?        S      0:00 /usr/bin/dinit
```
There is process number 1.... it is `dinit`. 

Artix is Arch based.  You use the `pacman` package system for updates and installs. 

It is possible to install `dinit` from source into any distro.  That is not an easy option.


#### Compare dinit to runit, s6 and sysVinit ####
There is an init system comparison table here

https://wiki.gentoo.org/wiki/Comparison_of_init_systems

and some more points here

https://forum.artixlinux.org/index.php/topic,3498.0.html

 1. The first thing one notices when using `dinit` is that there only one process ( /usr/bin/dinit) that acts as an init process, accepts service management commands, and does service supervision.  Contrast that with `runit` , `s6` and other daemontools based init systems, where there is an init process plus separate supervisor processes for each and every service, plus if there is logging a separate log process for every service.. 

 2. The next thing one notices is there is just one directory (/etc/dinit.d) where
all of the service information is kept

<p align-"center">
<img src="https://github.com/nevillejackson/Unix/blob/main/dinit/artix2.png?raw=true">
</p>

The files are `service files` . We can see the content of the `sshd` service file. It is a simple list of parameters, there is no script as in `sysVinit` or `run` file as in `runit`. 
Both `sysVinit` and `runit` use a link farm... a directory full of soft links that point to the startup files of the daemons to be started. `Dinit` just has a `service file`.
The daemons required at boot time are in the `boot.d` subdirectory.

To create a `service file` in Artix/dinit, one simply installs the dinit package corresponding to the service package. For example

```
trinity:[nevj]:~$ pacman -Ss vsftpd

world/vsftpd 3.0.5-1
    Very Secure FTP daemon


world/vsftpd-dinit 20211103-4 (dinit-world)
    dinit service scripts for vsftpd
```

So to get `vsftpd` just install the above 2 packages. In Artix there are also  packages for openrc, runit and s6, to go with every service package. 


 3.The most likely thing a home user may want to do is to start or stop a daemon. The dinit commands for this are

```
dinitctl start service_name
dinitctl stop service_name
```
One can also list services
```
dinitctl list
```
and one can make a service autostart at boot
```
dinitctl enable service_name
```

Dinit, in common with s6 and systemd, allows users as well as root, to manage services That can not be done in runit or OpenRC or sysVinit.

 4. An important feature of dinit is that it automatically manages dependencies between services, so that it starts daemons in a suitable order. S6 can do that too, and so can systemd. Runit and OpenRc and SysVinit do not manage dependencies.  What proper dependency management means is that daemons can be started in parallel if they are independent.  That makes booting fast. Dinit boot time is indeed fast.... Chimera Linux (in a VM) boots in 4 seconds, Artix/dinit ( hard install)  in about 10 seconds.

#### Conclusion ####
Dinit is a suitable init system for home use. It boots fast, has simple commands, and in general leaves the user alone and does not intrude on other parts of the system. 

If you want to read more about dinit look here

https://wiki.artixlinux.org/Main/Dinit

https://davmac.org/projects/dinit/

and an interesting account from the developer of dinit here

https://davmac.wordpress.com/category/dinit/

Dinit is a different design to other init systems... it is a monolithic C++ program. All other init systems break up into a series of utilities, the worst being systemd, with s6 not far behind.  OpenRC is a script... so it is slow but has been around a while and is reliable. I would say OpenRC is the next most suitable for home use, after dinit. 

I am sure `dinit` will start to become available in other distros. Keep an eye on it, and have a try with Artix if you are interested.  
