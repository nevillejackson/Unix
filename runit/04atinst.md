### Distributions which provide runit as a choice at install time ###

Some distributions provide a choice of init system in their installer.
This means that they package several init systems into  one .iso file, then set an option in the installer 
to choose one or more init systems.

#### PeppermintOS Devuan Edition ####

The Devuan edition of Peppermint uses the `calamares` installer, and it offers a choice of 
sysVinit, runit, or OpenRC.
I used `PeppermintOS-devuan_64_xfce.iso` and chose `runit` as the init system.
After the install completed it booted with the following processes executing

```
$ ps ax | grep run
    1 ?        Ss     0:00 runit
 1573 ?        Ss     0:00 runsvdir -P /etc/service log: ...........................................................................................................................................................................................................................................................................................................................................................................................................
 1959 ?        Ss     0:00 runsv getty-tty6
 1960 ?        Ss     0:00 runsv getty-ttyS0
 1961 ?        Ss     0:00 runsv getty-tty3
 1962 ?        Ss     0:00 runsv getty-tty5
 1963 ?        Ss     0:00 runsv getty-tty4
 1964 ?        Ss     0:00 runsv getty-tty1
 1965 ?        Ss     0:00 runsv getty-tty2
 1966 ?        Ss     0:00 runsv default-syslog
```

So `runit` is process #1, `runsvdir` is watching /etc/service, and there are 8 runsv processes supervising 
several getty's and syslog. No other services are running

In `/etc` we have

 - `init.d` directory containing sysvinit scripts for lots of services
 - `sv` directory containing subdirectories and run scrips for the getty's and syslog 
services listed above.
 - `runit` directory containing 

```
nevj@trinity:/etc/runit$ ls -F
1*  2*  3*  ctrlaltdel*  rc.shutdown  README@  runsvdir/
```

 - `runit/runsvdir`  directory containing

```
nevj@trinity:/etc/runit$ ls -lF runsvdir
total 20
lrwxrwxrwx 1 root root   27 Nov 18 21:23 current -> /etc/runit/runsvdir/default/
drwxr-xr-x 2 root root 4096 Nov 20 20:50 default/
drwxr-xr-x 2 root root 4096 Feb 10  2023 default.run/
drwxr-xr-x 3 root root 4096 Nov 18 21:23 single/
drwxr-xr-x 2 root root 4096 Nov 18 21:23 solo/
drwxr-xr-x 2 root root 4096 Feb 10  2023 svmanaged/
```

and the links are in `default`

```
nevj@trinity:/etc/runit$ ls -lF runsvdir/default
total 0
lrwxrwxrwx 1 root root 22 Nov 18 21:23 default-syslog -> /etc/sv/default-syslog/
lrwxrwxrwx 1 root root 22 Feb 10  2023 getty-tty1 -> ../../../sv/getty-tty1/
lrwxrwxrwx 1 root root 18 Nov 18 21:23 getty-tty2 -> /etc/sv/getty-tty2/
lrwxrwxrwx 1 root root 18 Nov 18 21:23 getty-tty3 -> /etc/sv/getty-tty3/
lrwxrwxrwx 1 root root 18 Nov 18 21:23 getty-tty4 -> /etc/sv/getty-tty4/
lrwxrwxrwx 1 root root 18 Nov 18 21:23 getty-tty5 -> /etc/sv/getty-tty5/
lrwxrwxrwx 1 root root 18 Nov 18 21:23 getty-tty6 -> /etc/sv/getty-tty6/
lrwxrwxrwx 1 root root 19 Nov 18 21:23 getty-ttyS0 -> /etc/sv/getty-ttyS0/
```

So the runit installed in Peppermint is quite minimal.  Only the fundamental 
getty and syslog services are under runit script control.  All the other 
services are still sysVinit scripts.... you can see them in /etc/init.d

```
nevj@trinity:~$ ls -F /etc/init.d
alsa-utils*              keyboard-setup.sh*            rc.local*
apparmor*                killprocs*                    rcS@
avahi-daemon*            kmod*                         README@
binfmt-support*          lightdm*                      reboot*
bluetooth*               live-config                   rmnologin*
bootlogd*                lm-sensors*                   rsyslog*
bootlogs*                mountall-bootclean.sh*        saned*
bootmisc.sh*             mountall.sh*                  sendsigs*
brightness*              mount-configfs*               single*
checkfs.sh*              mountdevsubfs.sh*             smartmontools*
checkroot-bootclean.sh*  mountkernfs.sh*               spice-vdagent*
checkroot.sh*            mountnfs-bootclean.sh*        stop-bootlogd*
console-setup.sh*        mountnfs.sh*                  stop-bootlogd-single*
cron*                    networking*                   sudo*
cups*                    network-manager*              umountfs*
cups-browsed*            ntpsec*                       umountnfs.sh*
dbus*                    openbsd-inetd*                umountroot*
elogind*                 plymouth*                     urandom*
eudev*                   plymouth-log*                 uuidd*
halt*                    procps*                       vsftpd*
hostname.sh*             pulseaudio-enable-autospawn*  x11-common*
hwclock.sh*              rc@
```

All those executable files ( marked with *) are sysvinit scripts).
They  wer all present at install time, except `vsftpd` which I installed as a test. 
All these services are running. It is not clear to me whether `runit` automatically executes 
all scripts in `/etc/init.d`, or whether it is triggered in some way.

There is a link `/etc/init.d/rc` which points to  `/lib/init/rc` which is a script that starts and stops servics according to runlevel. 
There is also a link `/etc/init.d/rcS` which points to a script `/lib/init/rcS` which 
again is a script and it executes `/etc/init.d/rc'.
This is sysVinit stuff. I dont understand it, but I think starting and stopping of services in
Peppermint is still under control of sysVinit. Runit only provides process #1
and a few basic services.
At some point during startup, the runit process #1 must launch these sysVinit scripts

So Peppermint Devuan with runit is a hybrid. The package system only supplies sysVinit scripts.
They have not dealt with the problem of package system support for runit.

