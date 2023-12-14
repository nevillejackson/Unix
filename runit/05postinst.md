### Distributions in which runit is available a package ###

Some distributions provide a `runit` package, but do not have it as the default init
system. In this case it is up to the user 
to install a runit package and to configure runit. If the runit package is replacing sysVinit, or OpenRC, the changeover from `init` to `runit` is quite easy. If the runit package is to replace systemd, the changeover is much more complicated and involves some additional transitional packages.

In any case, the amount of support for runit provided by the package system will determine how effectively a swapover to runit can be implemnented.

#### Devuan ####
Devuan comes only with sysVinit. It provides packages for OpenRC, S6  and runit. There is no init option in the installer, if the install is from a .iso file, but I believe the netinstall provides an init choice.

When you install Devuan from the .iso file, you get the following in /etc

 - an `/etc/init.d` directory containing all the sysVinit scripts.
 - an `/etc/sv` directory containing only the runit run and log files for `acpid`
 - a `/etc/runit` directory containing only the `runsvdir` subdirectory, which in turn contains only a `default` subdirectory containing only one link which points to 
`/etc/sv/acpid`

and you get the `init` process as process #1.
In other words, you get sysVinit fully configured, plus an example setup for runit
which configures only the `acpid` service.

Now what happens if you install  `runit` as a  package on top of sysVinit?

```
$ apt-cache search runit
dh-runit - debhelper add-on to handle runit runscripts
golang-github-soundcloud-go-runit-dev - Go library wrapping runit service status
mini-httpd-run - Small HTTP server (Runit integration)
runit - system-wide service supervision
runit-helper - dh-runit implementation detail
runit-init - system-wide service supervision (as init system)
runit-run - service supervision (systemd and sysv integration)
runit-services - UNIX init scheme with service supervision (services)
runit-systemd - transitional package for runit-systemd users
socklog-run - system and kernel logging services - runit services
init-system-helpers - helper tools for all init systems
live-config-runit - Live System Configuration Components (runit backend)
```

Which package? Well I installed `runit-init` because I wanted runit to be process #1 
and to supervise services. This dragged in several other packages, runit, runit-helper, fgetty, getty-run, and sysuser-helper. It also removed some packages, live-config, live-config-sysvinit, refractasnapshotbox, refractasnapshotgi, sysvinit-core. I assume the removed packages are all to do with sysvinit..

After this, my Devuan has the following
 
 - /etc/init.d still contains a large number of sysVinit scripts. Exactly the same as before.
 - /etc/sv now has the following

```
$ cd /etc
nevj@trinity:/etc$ ls sv
acpid         dbus            exim4       getty-tty4   mdadm
acpi-fakekey  dbus.dep-fixer  getty-hvc0  getty-tty5   rsyslog
anacron       default-syslog  getty-tty1  getty-tty6   slim
cron          dhclient        getty-tty2  getty-ttyS0  svlogd
cups          elogind         getty-tty3  getty-ttyv0  wicd
```

 - /etc/runit now has
 
```
$ ls runit
1  2  3  ctrlaltdel  rc.shutdown  README  runsvdir

```

 - /etc/service is now a link to /etc/runit/runsvdir/current
 - /etc/runit/runsvdir/current has the following link

```
nevj@trinity:/etc$ ls -l /etc/runit/runsvdir/current
lrwxrwxrwx 1 root root 27 Dec  1  2021 /etc/runit/runsvdir/current -> /etc/runit/runsvdir/default
``` 

and /etc/runit/runsvdir/default has
```
c$ ls -l /etc/runit/runsvdir/default
total 0
lrwxrwxrwx 1 root root 13 Dec  1  2021 acpid -> /etc/sv/acpid
lrwxrwxrwx 1 root root 20 Nov  8 22:35 acpi-fakekey -> /etc/sv/acpi-fakekey
lrwxrwxrwx 1 root root 15 Nov  8 22:35 anacron -> /etc/sv/anacron
lrwxrwxrwx 1 root root 12 Nov  8 22:35 cron -> /etc/sv/cron
lrwxrwxrwx 1 root root 12 Nov  8 22:35 cups -> /etc/sv/cups
lrwxrwxrwx 1 root root 12 Nov  8 22:35 dbus -> /etc/sv/dbus
lrwxrwxrwx 1 root root 22 Nov  8 22:35 dbus.dep-fixer -> /etc/sv/dbus.dep-fixer
lrwxrwxrwx 1 root root 22 Nov  8 21:46 default-syslog -> /etc/sv/default-syslog
lrwxrwxrwx 1 root root 16 Nov  8 22:35 dhclient -> /etc/sv/dhclient
lrwxrwxrwx 1 root root 15 Nov  8 22:35 elogind -> /etc/sv/elogind
lrwxrwxrwx 1 root root 22 Mar 10  2019 getty-tty1 -> ../../../sv/getty-tty1
lrwxrwxrwx 1 root root 18 Jun 20  2021 getty-tty2 -> /etc/sv/getty-tty2
lrwxrwxrwx 1 root root 18 Jun 20  2021 getty-tty3 -> /etc/sv/getty-tty3
lrwxrwxrwx 1 root root 18 Jun 20  2021 getty-tty4 -> /etc/sv/getty-tty4
lrwxrwxrwx 1 root root 18 Jun 20  2021 getty-tty5 -> /etc/sv/getty-tty5
lrwxrwxrwx 1 root root 18 Jun 20  2021 getty-tty6 -> /etc/sv/getty-tty6
lrwxrwxrwx 1 root root 19 Nov  8 21:42 getty-ttyS0 -> /etc/sv/getty-ttyS0
lrwxrwxrwx 1 root root 13 Nov  8 22:35 mdadm -> /etc/sv/mdadm
lrwxrwxrwx 1 root root 15 Nov  8 22:35 rsyslog -> /etc/sv/rsyslog
lrwxrwxrwx 1 root root 12 Nov  8 22:35 slim -> /etc/sv/slim
```

in other words, a link to every service in /etc/sv.

 The services in /etc/sv duplicate some of the services in /etc/init.d,
 but by no means all. What this means is that some of the services have
 been configured for runit while the remainder have been left as sysVinit scripts,
 which the runit process will execute if it does not find a run file in /etc/sv. 

 So what I end up with is a hybrid system.  I suspect that there are still many
 packages that do not provide runit support.

 One might conclude that Devuan has not completely solved the alternate init system 
problem.  It goes a long way by removing systemd, and providing alternate init system 
packages, but its package system has not been adapted to deal properly with
 init alternatives. This is not surprising, as Devuan depends on Debian for most
 of its package system.




#### Debian ####
Debian comes only with systemd, but Debian (since Debian 11) does provide a runit package , plus some transitional packages for converting from systemd. There is also a procedure for doing a `chmod` during the install and installing runit there. 
The packages provided are

```

nevj@debianvm:~$ apt-cache search runit
dh-runit - debhelper add-on to handle runit runscripts
runit-helper - dh-runit implementation detail
djbdns-conf - programs to create service directories
mini-httpd-run - Small HTTP server (Runit integration)
getty-run - runscripts to supervise getty processes
runit - system-wide service supervision
runit-init - system-wide service supervision (as init system)
runit-run - service supervision (systemd and sysv integration)
runit-systemd - transitional package for runit-systemd users
runit-services - UNIX init scheme with service supervision (services)
libs6-2.11 - small and secure supervision software suite (shared library)
libs6-dev - small and secure supervision software suite (development files)
s6 - small and secure supervision software suite
s6-doc - small and secure supervision software suite (documentation)
snooze - run a command at a particular time
socklog - system and kernel logging services - binaries
socklog-run - system and kernel logging services - runit services
```

The instructions for doing a runit conversion are here

https://lecorbeausvault.wordpress.com/2022/02/07/debian-switching-init-system-easily-openrc-sysvinit-runit/

I have not tried it.  It would seem to be very complicated, particularly the 
package system issues.  It would be much simpler to start from Devuan and not have the systemd transitional problems.

It is worth noting that Devuan seem to have backtracked on the systemd only
decision, and now provide several alternative init systems. They still have some way to go to provide package system support for these alternatives.




#### Gentoo ####


 Gentoo comes with the `OpenRC` init system. You can add `systemd` or `runit`
 by emerging packages.
 I left my Gentoo install with `OpenRC`, and I blocked it from installing any package requiring systemd by setting `USE= -systemd` in `/etc/portage/make.conf`
 and by putting `sys-apps/systemd` in `/etc/portage/package.mask/package.mask`

 OpenRC is not much more than a friendly frontend to `sysVinit`. 
 It has 3 commands `rc-status`, rc-update`, and `rc-service` and it has the `runlevel` concept of sysVinit. 

 My Gentoo, as installed, has the following
 
  - `init` is process #1
  - `/etc/init.d` contains sysVinit scripts for all the services
  - there are no runit directories like sv, or runit, or service.
  - `/etc/runlevels` contains subdirectories boot, default, nonetwork,
   shutdown, and sysinit, each of which  contains a set of symbolic links 
   to scripts in `/etc/init.d`

  
  Now what happens if I emerge the runit package?
Lets find it first

```
nevj@mary ~ $ emerge --search runit

[ Results for search key : runit ]
Searching...
*  sys-process/runit
      Latest version available: 2.1.2-r1
      Latest version installed: [ Not Installed ]
      Size of files: 109 KiB
      Homepage:      http://smarden.org/runit/
      Description:   A UNIX init scheme with service supervision
      License:       BSD
```

Now lets see wjhat it would do if I installed it

```
nevj@mary ~ $ emerge --ask --pretend sys-process/runit

These are the packages that would be merged, in order:

Calculating dependencies... done!
Dependency resolution took 1.47 s (backtrack: 0/20).

[ebuild  N     ] sys-process/runit-2.1.2-r1  USE="-static"
```

Nothing very dangerous there, it does not remove anything , so lets try it


```
mary /home/nevj # emerge --ask sys-process/runit

These are the packages that would be merged, in order:

Calculating dependencies... done!
Dependency resolution took 1.38 s (backtrack: 0/20).

[ebuild  N     ] sys-process/runit-2.1.2-r1  USE="-static" 

Would you like to merge these packages? [Yes/No] yes

>>> Verifying ebuild manifests

>>> Emerging (1 of 1) sys-process/runit-2.1.2-r1::gentoo
>>> Downloading 'https://mirror.aarnet.edu.au/pub/gentoo/distfiles/09/runit-2.1.2.tar.gz'
--2023-12-09 21:25:01--  https://mirror.aarnet.edu.au/pub/gentoo/distfiles/09/runit-2.1.2.tar.gz
Resolving mirror.aarnet.edu.au... 2001:388:30bc:cafe::beef, 202.158.214.106
Connecting to mirror.aarnet.edu.au|2001:388:30bc:cafe::beef|:443... connected.
HTTP request sent, awaiting response... 200 OK
Length: 110916 (108K) [application/x-gzip]
Saving to: ‘/var/cache/distfiles/runit-2.1.2.tar.gz.__download__’

/var/cache/distfile 100%[===================>] 108.32K  --.-KB/s    in 0.1s    

2023-12-09 21:25:01 (834 KB/s) - ‘/var/cache/distfiles/runit-2.1.2.tar.gz.__download__’ saved [110916/110916]

 * runit-2.1.2.tar.gz BLAKE2B SHA512 size ;-) ...                         [ ok ]
>>> Unpacking source...
>>> Unpacking runit-2.1.2.tar.gz to /var/tmp/portage/sys-process/runit-2.1.2-r1/work
>>> Source unpacked in /var/tmp/portage/sys-process/runit-2.1.2-r1/work
>>> Preparing source in /var/tmp/portage/sys-process/runit-2.1.2-r1/work/admin/runit-2.1.2/src ...
>>> Source prepared.
>>> Configuring source in /var/tmp/portage/sys-process/runit-2.1.2-r1/work/admin/runit-2.1.2/src ...
>>> Source configured.
>>> Compiling source in /var/tmp/portage/sys-process/runit-2.1.2-r1/work/admin/runit-2.1.2/src ...
make -j4 
   ..........
   lots of compiling
   ..........
>>> Source compiled.
>>> Test phase [not enabled]: sys-process/runit-2.1.2-r1

>>> Install sys-process/runit-2.1.2-r1 into /var/tmp/portage/sys-process/runit-2.1.2-r1/image/
>>> Completed installing sys-process/runit-2.1.2-r1 into /var/tmp/portage/sys-process/runit-2.1.2-r1/image/

 * Final size of build directory: 2444 KiB (2.3 MiB)
 * Final size of installed tree:   736 KiB

strip: x86_64-pc-linux-gnu-strip --strip-unneeded -N __gentoo_check_ldflags__ -R .comment -R .GCC.command.line -R .note.gnu.gold-version
   /bin/chpst
   /sbin/runit
   /sbin/runit-init
   /bin/runsv
   /bin/runsvchdir
   /bin/runsvdir
   /bin/sv
   /bin/svlogd
   /sbin/utmpset

>>> Installing (1 of 1) sys-process/runit-2.1.2-r1::gentoo
 * The links to services runit will supervise are installed
 * in /etc/service.
 * If you need multiple runlevels, please see the documentation
 * for how to set them up.
 * 
 * To make sure sv works correctly in your currently open
 * shells, please run the following command:
 * 
 * source /etc/profile
 * 

>>> Recording sys-process/runit in "world" favorites file...

>>> Completed (1 of 1) sys-process/runit-2.1.2-r1::gentoo

 * Messages for package sys-process/runit-2.1.2-r1:

 * To make sure sv works correctly in your currently open
 * shells, please run the following command:
 * 
 * source /etc/profile
 * 

 * GNU info directory index is up-to-date.

 * IMPORTANT: 4 config files in '/etc' need updating.
 * See the CONFIGURATION FILES and CONFIGURATION FILES UPDATE TOOLS
 * sections of the emerge man page to learn how to update config files.
mary /home/nevj # exit
exit
Script done.
mary /home/nevj # 
```


So what do we have now in /etc?

  -  `/etc/sv` exists and contains the following


```
  nevj@mary /etc $ ls sv
  
getty-tty1  getty-tty2  getty-tty3  getty-tty4  getty-tty5  getty-tty6
```

  in other words , just the getty processes that support terminals
  
   -  `etc/runit` contains

    
```
    nevj@mary /etc $ ls runit
1  2  3  ctrlaltdel
```
   
   in other words, just the 3 runlevels that runit uses
   
    - `/etc/service` contains

```
    nevj@mary /etc $ ls -l service
total 0
lrwxrwxrwx 1 root root 18 Dec  9 21:25 getty-tty1 -> /etc/sv/getty-tty1
lrwxrwxrwx 1 root root 18 Dec  9 21:25 getty-tty2 -> /etc/sv/getty-tty2
lrwxrwxrwx 1 root root 18 Dec  9 21:25 getty-tty3 -> /etc/sv/getty-tty3
lrwxrwxrwx 1 root root 18 Dec  9 21:25 getty-tty4 -> /etc/sv/getty-tty4
lrwxrwxrwx 1 root root 18 Dec  9 21:25 getty-tty5 -> /etc/sv/getty-tty5
lrwxrwxrwx 1 root root 18 Dec  9 21:25 getty-tty6 -> /etc/sv/getty-tty6
```
  in other words , links to the runit services in sv

   - everything else is still in init.d

```
   nevj@mary /etc $ ls init.d
agetty                 hwclock            runsvdir
binfmt                 ip6tables          s6-svscan
bluetooth              iptables           savecache
bootlogd               keymaps            save-keymaps
bootmisc               killprocs          save-termencoding
cgroups                kmod-static-nodes  seedrng
chronyd                local              sshd
consolefont            localmount         swap
cups-browsed           loopback           swclock
cupsd                  modules            sysctl
dbus                   mount-ro           sysfs
devfs                  mtab               sysklogd
device-mapper          net.lo             systemd-tmpfiles-setup
dhcpcd                 netmount           systemd-tmpfiles-setup-dev
display-manager        net-online         termencoding
display-manager-setup  nullmailer         transmission-daemon
dmcrypt                numlock            udev
dmesg                  osclock            udev-settle
elogind                pciparm            udev-trigger
fsck                   procfs             vsftpd
functions.sh           pydoc-3.11         wpa_supplicant
git-daemon             pydoc-3.12         xdm
gpm                    root
hostname               rsyncd
```
  
  in other words, they are all still sysVinit scripts

   - openrc is still present, and its commands still work
   
   - process #1 is still init.... not runit.

   Now what happens if I reboot?


```
nevj@mary /etc $ ps ax | grep init
    1 ?        Ss     0:00 init [3]
```

Process #1 is still init.... runit did not take control of the init process?
Why?

There it is in the dmesg output

```
[    1.396489] Run /init as init process
[    1.396551]   with arguments:
[    1.396552]     /init
[    1.396553]   with environment:
[    1.396553]     HOME=/
[    1.396554]     TERM=linux
[    1.396554]     BOOT_IMAGE=/boot/vmlinuz-6.6.0-gentoo-x86_64
```

So, time to read the manual.
Gentoo has a wiki page on runit

https://wiki.gentoo.org/wiki/Runit

and a wiki talk page

https://wiki.gentoo.org/wiki/Talk:Runit

both are really essential reading if you want to implement runit in Gentoo.
It seems there are several ways of using runit

  - as init and RC with OpenRC
  - as init and RC without OpenRC
  - as PID 1 but not RC
  - as service supervisor but not init

I will try the third case .... replace init with runit but let OpenRC handle all the services.  I edit `/etc/runit/1` and insert the line

```
RUNLEVEL=S /sbin/rc default
```

Then reboot, and  .... it does not work. 
OK, maybe I have to tell the kernel to use runit-init.... so add

```
init=/sbin/runit-init
```

 to the linux line in the grub menu, let it boot,
and, ...it boots with runit as PID #1, and there is also a `runsvdir` process running, and a `runsv` process for each of getty1 to getty6, but no services and no logs. Also I only have a console login.... Xfce will not start.... not surprising if there are no services.

I will try to start some services. I edit `/etc/runit/1` and insert the line

```
RUNLEVEL=S /sbin/rc default
```

This should use rc to start the default set of daemons, which are

```
$ rc-status default
Runlevel: default
 sysklogd       
 wpa_supplicant
 dhcpcd       
 vsftpd      
 chronyd    
 dbus      
 cupsd    
 netmount
 display-manager  
 local           
 
```
 
 It might help to have a display-manager and dbus. 
 Reboot, again with the modified CMDLINE , and this time it works. 

```
 nevj@mary ~ $ ps ax | grep run
    1 ?        Ss     0:00 runit
 2474 ?        Ss     0:00 runsvdir -P /etc/service log: ...........................................................................................................................................................................................................................................................................................................................................................................................................
 2477 ?        Ss     0:00 runsv getty-tty5
 2478 ?        Ss     0:00 runsv getty-tty3
 2479 ?        Ss     0:00 runsv getty-tty6
 2480 ?        Ss     0:00 runsv getty-tty4
 2481 ?        Ss     0:00 runsv getty-tty2
 2482 ?        Ss     0:00 runsv getty-tty1
 2500 tty7     Ssl+   0:01 /usr/bin/X :0 -seat seat0 -auth /var/run/lightdm/root/:0 -nolisten tcp vt7 -novtswitch
 2749 ?        S      0:00 /usr/bin/dbus-daemon --config-file=/usr/share/defaults/at-spi2/accessibility.conf --nofork --print-address 11 --address=unix:path=/run/user/1000/at-spi/bus_0
 2820 ?        Sl     0:00 xfce4-terminal --geometry=80x24 --display :0.0 .................
```

So runit still only supervises the getty processes, but all the default services are running under control of OpenRC

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/runit/gentoo1.png?raw=true">
</p>

Now, I dont want to have to edit the grub menu every time I boot it, 
so I need to set the kernel boot parameter in `/etc/default/grub`

```
# Boot with runit instead of sysvinit
GRUB_CMDLINE_LINUX="init=/sbin/runit-init"
```

Then, after much soul searching, I find that I have to run 
`grub-mkconfig -o /boot/grub/grub.cfg`
in Gentoo, and then run `update-grub` in Debian, with os-prober enabled, because Debian controls my grub, and os-prober needs to see the updated grub.cfg in Gentoo. 


After that It boots Gentoo with runit automatically.

There is still one issue, if I have Gentoo with runit as process #1, it does not halt or reboot or shutdown.  This is documented. One needs to do a shutdown with 
`runit-init 0`
and to do a reboot with 
`runit-init 6`

We now come to the difficult business of using `runit` to control services, either with or without OpenRC.
I am not going to attempt this, for the simple reasion that it is not 
properly supported by the Gentoo package system - Gentoo packages 
provide sysVinit scripts , not runit scripts. 

There are some third party runit scripts available 

https://github.com/powerman/powerman-overlay

and

https://github.com/powerman/powerman-overlay/blob/master/power-misc/runit-scripts/runit-scripts-1.5.0.ebuild

but they are not in the Gentoo repo, one has to obtain them from Github.The other option would be to write your own scripts.
It is not worth the trouble. 

So, concluding remarks for Gentoo are

 - it supplies a runit package
 - it is easy to use runit as the init process
 - it is not easy to use runit to manage services
 - the Gentoo runit documentation is excellent. It tells you exactly 
 what the situation is and how to go about doing things.
