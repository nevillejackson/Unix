### Getting started with Obarun and s6-66 ###
 The only official released distro which has S6 with the 66
 Service Manager is Obarun. The init diversity spins
 ( Antix, MX, Devuan) have it but they are spins, ie trial systems not official releases.  Void has S6 and 66 available as packages but the versions are old.

The name 'Obarun' comes from 'OpenBox Arch RUNit'. '66' was originally developed for Runit, then changed to S6.
Obarun is based on Arch.  There are two downloads available here

https://web.obarun.org/index.php?id=74

obarun-2024.05.04-x86_64.iso       875Mb
obarun-JWM-2024.05.04-x86_64.iso   1.4Gb

#### Install ####

I took the JWM release, and installed it in a qemu/KVM VM under virt-manager. 
On loading the .iso into the VM, it boots into a live system with the JWM window manager.
You see a boot screen
<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/obarun/obarun1.png?raw=true"
</p>
then when you boot, you get a console login prompt
p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/obarun/obarun2.png?raw=true"
</p>
You login as user 'oblive' with password 'toor' ( 'root' is also password 'toor')
and you get a jwm screen
p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/obarun/obarun3.png?raw=true"
</p>

So it finds an internet connection, and it has an icon 'Obarun 
installer', which the guidelines tell you to ignore. .....
 For some complicated reason the way to start an install is to 

 - open a terminal ( the choice is Xterm or Terminator )
 - `sudo /usr/bin/obarun-install`

The installer will start. The install documentation is here

https://wiki.obarun.org/en/distribution/installation/obarun-install

This install document shows every menu. I will just emphasize 2 points

 - if you do not choose a Desktop Environment in the Main Menu, you will get the default which is no DE at all, just a minimal install with a console
 - at the end, if you do not get the reboot message, do not try
 to go back or reboot. Start agian from the .iso file. Some modern installers allow you to backtrack , but the Obarun installer does not allow this.

Whan you reboot you will get a grub menu ( assuming you chose grub as the bootloader) and obarun will boot.  Login this time as the user you defined in the install ( not as root) . `startx` will bring up the JWM DE. 

JWM has `xterm` as its default terminal. Unless you supply a .Xresources file, `xterm` has no scrollbar. It is easier to install Terminator
```
pacman -S terminator
```
It will be added to the menu, and it gets an icon on the lower panel ( great - first thing I ever want is a terminal). Terminator has a scrollbar, but it is the same (black) color as the background, so is invisible, but it works. 

It is a good idea to do an update
```
pacman -Syu
```
I actually did a second install, chose the minimum DE, then added IceWM . It is more work doing it that way, but IceWM is nicer, for example Terminator in IceWM has a visible scrollbar. I start IceWM with `startx` instead of a login screen. 

 The default shell in Obarun is `zsh`. Zsh is  derived from 'sh'.It has some of the features of `bash` and `csh`.  It is used in MacOS. The config file for `zsh` is `.zshrc`. Obarun also has `bash` and `sh` , I presume because they are needed for scripts.

We are now ready to start looking at `66`.


#### What services are present in an Obarun install ####
The first thing you notice about 66 in Obarun is that there is 
nothing in `/etc/66/service`. No frontends at all! 
Nothing in `~/.66.service` either. 
This is different to the init-diversity spins ... they provide 
some example frontends.

But, there are some services running.  In the JWM install there
 is

```
% ps ax | grep s6
    1 ?        S      0:00 s6-svscan -t 0 -L 256 /run/66/scandir/0
  214 ?        S      0:00 s6-supervise scandir-log
  215 ?        S      0:00 s6-supervise 66-shutdownd
  216 ?        S      0:00 s6-supervise fdholder
  217 ?        S      0:00 s6-supervise oneshotd
  218 ?        S      0:00 s6-supervise boot@system:tty-earlier@tty12
  219 ?        Ss     0:00 s6-log -bpd3 -- 1 T /run/66/log/0
  220 ?        Ss     0:00 s6-ipcserverd -1 -- s6-ipcserver-access -v0 -E -l0 -i data/rules -- s6-sudod -t 30000 -- /usr/libexec/66-oneshot --
  245 ?        S      0:00 s6-supervise boot@system:udevd-log
  246 ?        S      0:00 s6-supervise boot@system:udevd
  247 ?        S      0:00 s6-supervise boot@system:tty-rc@tty1
  248 ?        S      0:00 s6-supervise boot@system:tty-rc@tty2
  307 ?        Ss     0:00 s6-log -d3 n3 s1000000 /run/66/log/udevd
  743 ?        S      0:00 s6-supervise dbus
  744 ?        S      0:00 s6-supervise dbus-log
  745 ?        S      0:00 s6-supervise consolekit
  746 ?        S      0:00 s6-supervise consolekit-log
  747 ?        S      0:00 s6-supervise openntpd
  748 ?        S      0:00 s6-supervise openntpd-log
  749 ?        S      0:00 s6-supervise connmand
  750 ?        S      0:00 s6-supervise connmand-log
  751 ?        S      0:00 s6-supervise wpa_supplicant
  752 ?        S      0:00 s6-supervise wpa_supplicant-log
  773 ?        Ss     0:00 s6-log -d3 n3 s1000000 /var/log/66/consolekit
  777 ?        Ss     0:00 s6-log -d3 n3 s1000000 /var/log/66/dbus
  781 ?        Ss     0:00 s6-log -d3 n3 s1000000 /var/log/66/connmand
  782 ?        Ss     0:00 s6-log -d3 n3 s1000000 /var/log/66/openntpd
  817 ?        Ss     0:00 s6-fdholderd -1 -i data/rules
  824 ?        S      0:00 s6-supervise scandir@nevj:svscan@nevj-log
  825 ?        S      0:00 s6-supervise scandir@nevj:svscan@nevj
  834 ?        Ss     0:00 s6-log -d3 n3 s1000000 /var/log/66/scandir@nevj:svscan@nevj
  837 ?        Ss     0:00 s6-log -d3 n3 s1000000 /var/log/66/wpa_supplicant
  861 ?        Ss     0:00 s6-svscan -d 3 -t 0 -L 256 /run/66/scandir/1000
  863 ?        S      0:00 s6-supervise fdholder
  864 ?        S      0:00 s6-supervise oneshotd
  866 ?        Ss     0:00 s6-ipcserverd -1 -- s6-ipcserver-access -v0 -E -l0 -i data/rules -- s6-sudod -t 30000 -- /usr/libexec/66-oneshot --
  957 ?        Ss     0:00 s6-fdholderd -1 -i data/rules
  964 ?        S      0:00 s6-supervise dbus@nevj
  965 ?        S      0:00 s6-supervise dbus@nevj-log
  976 ?        Ss     0:00 s6-log -d3 n3 s1000000 /home/nevj/.66/log/dbus@nevj
```
the system services plus dbus, concolekit, openntpd, connmand, wpa_supplicant, and 
user services are supported for user 'nevj'.

In my Minimal install with IceWM there is

```
% ps ax | grep s6
    1 ?        S      0:00 s6-svscan -t 0 -L 256 /run/66/scandir/0
  243 ?        S      0:00 s6-supervise scandir-log
  244 ?        S      0:00 s6-supervise 66-shutdownd
  245 ?        S      0:00 s6-supervise fdholder
  246 ?        S      0:00 s6-supervise oneshotd
  247 ?        Ss     0:00 s6-log -bpd3 -- 1 T /run/66/log/0
  250 ?        Ss     0:00 s6-ipcserverd -1 -- s6-ipcserver-access -v0 -E -l0 -i data/rules -- s6-sudod -t 30000 -- /usr/libexec/66-oneshot --
  251 ?        S      0:00 s6-supervise boot@system:tty-earlier@tty12
  274 ?        S      0:00 s6-supervise boot@system:udevd-log
  275 ?        S      0:00 s6-supervise boot@system:udevd
  276 ?        S      0:00 s6-supervise boot@system:tty-rc@tty1
  277 ?        S      0:00 s6-supervise boot@system:tty-rc@tty2
  334 ?        Ss     0:00 s6-log -d3 n3 s1000000 /run/66/log/udevd
  744 ?        Ss     0:00 s6-fdholderd -1 -i data/rules
  751 ?        S      0:00 s6-supervise dhcpcd
  752 ?        S      0:00 s6-supervise dhcpcd-log
  759 ?        Ss     0:00 s6-log -d3 n3 s1000000 /var/log/66/dhcpcd
```

just the system services and dhcpcd. 

So where is '66' getting its frontends for all these services?
It would seem Obarun is using the immutable reference frontends 
in `/usr/share/66/service`

```
% ls /usr/share/66/service
 boot/        connmand     dhcpcd                    'wpa_supplicant-wired@'
 boot-user/   consolekit   openntpd                  'wpa_supplicant@'
 scandir/     cupsd        wpa_supplicant
 user/        dbus        'wpa_supplicant-nl80211@'
```

those are in the JWM install, and

```
% ls /usr/share/66/service
 boot/   dhcpcd          'wpa_supplicant-nl80211@'  'wpa_supplicant@'
 user/   wpa_supplicant  'wpa_supplicant-wired@'
```

the Minimal install has only dhcpcd and wpa-supplicant, plus the system services in the 'boot' subdirectory.

So Obarun comes with some services. If you wanted to modify these , you would add overriding frontends in `/etc/66/service`. If you wanted extra services you would also add frontends to `/etc/66/service` or for user services `~/.66/service`.

#### A small services project ####
In order to get a feel for some of the things '66' can do, we shall try to setup
a system in which various groups of services can be switched on and off as required. The idea is that not all daemons need to be running all the time.... that is inefficient  and slows the system. 
So lets imagine a small home Linux system in which the user 
sometimes uses `ssh` or `ftp` network connections, and sometimes 
uses bluetooth.  We want to be able to  activate or deactivate 
these in any combination.

##### Packages and frontends #####
The first step is to install packages and write frontends for the required services. We will use the JWM install. 
The reguired packages are ...
For connections:

 - openssh
 - sshd-66serv
 - vsftpd
 - sshfs

For Bluetooth:

 - bluez
 - bluez-mesh
 - pulseaudio-bluetooth
 - pulseaudio


The required frontends are ...
For connections:

 - sshd
 - vsftpd 
 
For Bluetooth:

 - bluetoothd
 - bluetooth-meshd


Get the packages.

The connection packages:
`openssh` is already there. 
`pacman -S sshd-66serv` 
adds `sshd` to `/usr/share/66/service`, so Obarun has an
 official 66-aware package for `sshd` and it installs in 
the system area.

`pacman -S vsftpd` , on the other hand, installs `/usr/bin/vsftpd`
, but does not make a frontend in either `/usr/share/66/service` or `/etc/66/service`.  
So we need to make a frontend, I can steal the one from my work 
on Antix-init-diversity-spin

```
[main]
@type=classic
@description="vsftpd daemon"
@version=0.0.1
@user=(root)

[start]
@execute=(
    /usr/sbin/vsftpd
)
```
just need to change /usr/sbin/vsftpd to /usr/bin/vsftpd for Obarun.

`pacman -S sshfs` 
installs /usr/bin/sshfs.... it has no daemon of its own, it uses sshd, so no frontend needed. 

The bluetooth packages:

`pacman -S bluez`
does not install any binary in /usr/bin? It has a daemon called `bluetoothd`, which seems to be in /lib/bluetooth/bluetoothd?   (what a strange place?)
Bluetoothd has config files in `/etc/bluetooth`.
So we setup a frontend for bluetoothd

```
[main]
@type=classic
@description="bluetoothd daemon"
@version=0.0.1
@user=(root)

[start]
@execute=(
    /usr/lib/bluetooth/bluetoothd
)
```

Now we install the other bluetooth package
`pacman -S bluez-mesh`
It also has a config file in `/etc/bluetooth` and its binary for the daemon is also in `/lib/bluetooth`. 
So we setup another frontend for `bluetooth-meshd`

```
[main]
@type=classic
@description="bluetooth-meshd daemon"
@version=0.0.1
@user=(root)
@depends = (bluetoothd)

[start]
@execute=(
    /usr/lib/bluetooth/bluetooth-meshd
)
```

##### Setup services in trees using `66` #####
We want to make 2 trees called `connect` and `blue`, each
 containing the relevant daemons

```
66 -t connect enable sshd
66 -t connect enable vsftpd

66 -t blue enable bluetoothd
66 -t blue enable bluetooth-meshd
```

That is all, now we hav a tree structure like this

```
Ob66jwm# 66 tree status
Name        : boot
Current     : no
Enabled     : no
Allowed     : root
Groups      : boot
Depends     : None
Required by : None
Contents    : boot@system:system-hostname boot@system:mount-run
              boot@system:populate-run boot@system:mount-tmp
              boot@system:populate-tmp boot@system:mount-proc
              boot@system:mount-sys boot@system:populate-sys
              boot@system:mount-dev boot@system:mount-pts
              boot@system:mount-shm boot@system:populate-dev
              boot@system:mount-cgroups boot@system:mount-branch
              boot@system:system-hwclock boot@system:modules-kernel
              boot@system:system-random boot@system:modules-system
              boot@system:system-sysctl boot@system:udevd-log
              boot@system:udevd boot@system:udevadm
              boot@system:system-fontnkey boot@system:system-fsck
              boot@system:mount-fstab boot@system:system-branch
              boot@system:mount-rw boot@system:local-time
              boot@system:local-authfiles boot@system:local-sethostname
              boot@system:local-loop boot@system:local-tmpfiles
              boot@system:local-dmesg boot@system:tty-earlier@tty12
              boot@system:mount-netfs boot@system:local-branch
              boot@system:runtime-branch boot@system:canopy
              boot@system:tty-rc@tty1 boot@system:tty-rc@tty2 boot@system

Name        : blue
Current     : no
Enabled     : no
Allowed     : root
Groups      : admin
Depends     : None
Required by : None
Contents    : bluetoothd bluetoothd-log bluetooth-meshd bluetooth-meshd-log

Name        : global
Current     : no
Enabled     : yes
Allowed     : root
Groups      : admin
Depends     : None
Required by : session
Contents    : dbus-log dbus consolekit-log consolekit openntpd-log
              openntpd connmand-log connmand wpa_supplicant-log
              wpa_supplicant

Name        : connect
Current     : no
Enabled     : no
Allowed     : root
Groups      : admin
Depends     : None
Required by : None
Contents    : sshd sshd-log vsftpd vsftpd-log

Name        : session
Current     : no
Enabled     : yes
Allowed     : root
Groups      : admin
Depends     : global
Required by : None
Contents    : scandir@nevj:setenv@nevj scandir@nevj:svscan@nevj
              scandir@nevj:svscan@nevj-log scandir@nevj
              boot-user@nevj:mount-run@nevj boot-user@nevj

```

That all looks OK... every daemon is in the correct tree.

Now, before we boot and allow the `enable` to have effect, we 
had better `start` them , to test if the frontends are 
functional

```
66 -t connect start sshd
66 -t connect start vsftpd

66 -t blue start bluetoothd
66 -t blue start bluetooth-meshd
```

Yes , they all start without error.
So time to boot and test

##### What our project can do #####
 When we reboot we see that neither the 'connect' or 'blue' trees are enabled, and none of their daemons are running. 
 Now. lets say we want some connectivity. We can start the
 'connect' tree

```
66 tree start connect
```

Now both `sshd` and `vsftpd` are running, so the system will accept incoming ssh or sftp or sshfs or ftp connections. 
I tested it, the VM will accept an `ssh` login from the host system. 
There is some issue with ftp

```
$ ftp 192.168.122.36
Connected to 192.168.122.36.
500 OOPS: vsftpd: both local and anonymous access disabled!
```

We shall look into that. `ftp` is defined in /etc/services.
 What I need is to enable local users in `/etc/vsftpd.conf`. 
 Then 

```
tree stop connect
tree start connect
```

That makes the daemon read the new config file, then from the 
host

```
[nevj@trinity ~]$ ftp 192.168.122.36
Connected to 192.168.122.36.
220 (vsFTPd 3.0.5)
Name (192.168.122.36:nevj): 
331 Please specify the password.
Password: 
230 Login successful.
.....
```

So that is OK too.

Now we may want to stop that and start the bluetooth service

```
66 tree stop connect
66 tree start blue
```

Now sshd and vsftpd are not running, but bluetoothd and
 bluetooth-meshd are running, so we can use bluetooth connections. Not sure how to test that in a VM.

That is enough demonstration. Obviously we can start both the connect and blue trees, so that all daemons are running, or we can go back to none at all.
So what we have  is a system that is really 4 different combinations of services, which are switchable. It is a bit like `runlevels` in sysVinit, but much simpler to implement. I believe `systemd` can be configured to do this, but I do not know how? S6-rc can do it with `bundles` but it is more difficult to setup. Runit and Dinit can not do it. 

#### User services ####
Everything above was done as root.  
Obarun has the facility to allow userd to manage services. 
What Obarun provides is a system tree called `session'

```
Ob66jwm# 66 tree status
Name        : session
Current     : no
Enabled     : yes
Allowed     : root
Groups      : admin
Depends     : global
Required by : None
Contents    : scandir@nevj:setenv@nevj scandir@nevj:svscan@nevj
              scandir@nevj:svscan@nevj-log scandir@nevj
              boot-user@nevj:mount-run@nevj boot-user@nevj
```

The tree `session` contains various services that are needed for user 'nevj'  to be able to manage user services. This tree is enabled so it sets up the user environment for user nevj at boot time. 
o
If you look at the trees available to nevj

```
nevj@Ob66jwm ~ % 66 tree status
Name        : global
Current     : no
Enabled     : yes
Allowed     : nevj
Groups      : user
Depends     : None
Required by : None
Contents    : xdg-user-dirs dbus@nevj-log dbus@nevj
```

you see only nevj's global tree ( which is different from the system global tree). 
Nevj can add services to that tree , or can start other trees.
For example I tried to shift all of the work done above by root with the `connect` and `bluez` trees to nevj's control. 
This involved copying the required frontends from 
`/etc/66/service` to `~nevj/.66./service` , and then 
redefining the `connect' and `blue` trees for nevj's 
environment

```
nevj@Ob66jwm ~ % ls /home/nevj/.66/service
bluetooth-meshd  bluetoothd  vsftpd
```

and editing then to change the user from root to nevj.

Now setup the connect and blue trees again under nevj

```
66 -t connect enable sshd
parse: fatal: unable to find service frontend file of: sshd
```

OK, user nevj can not utilize the frontend for `sshd` that is 
found in `/usr/share/66/service`, the way root could.
 We will have to make a user copy

```
cp /usr/share/66/service/sshd /home/nevj/.66/service
```

and we need to edit the frontend to change te user name.
Now, try again to enable `sshd`

```
66 -t connect enable sshd
```

it works, and now nevj has a tree called connect with sshd in it. Note this is totally separate from root's connect tree, we perhaps should have used a different name.
Now enable the other services

```
66 -t connect enable vsftpd
66 -t blue enable bluetoothd
66 -t blue enable bluetooth-meshd
```

Nevj now has both `connect` and `blue` trees, with appropriate services.
Lets reboot, and see if we can make it work for nevj

```
66 tree start connect
```

Both sshd and vsftpd are now running.
But I can not ssh or ftp into the VM?  That may be something to do with the fact that these two services should be run as system services? I think they were a bad choice for a user service?

Try bluetooth

```
66 tree start blue
```

Both bluetoothd and bluetooth-meshd are running.
I probably cant test that in a VM either. 

That is enough to demonstrate user services in `66` , 
but it was a poor choice of services. 


#### Some final notes on Obarun ####
 To halt Obarun you must use 

```
66 halt
```

No other method, will work. You cant use the GUI (Exit and Shutdown) , you cant become root and do `shutdown -h now`  or `halt`, you cant use the `Shutdown or Reboot` buttons on the VM. That is because `66` controls the startup and shutdown processes, like any init system should, but the normal shutdown methods listed above are not mapped to `66 halt` , so they do not work.
After doing `66 halt` the VM does not stop running. You have to use `Force Off` on the VM Menu.  
No doubt this behaviour can be tidied up.

Obarun is fast. It boots in about 10 sec to a login prompt. When you login it automatically runs startx which brings up JWM. No Login Manager, no need for it, just a waste of time. 

One can obviously use any DE that is available in the Arch repository with Obarun. I used `JWM` because it is default and simple. I did test setting up `IceWM`, it works. This is not about DE's. 

The writing of frontends to setup a service will disappear as `66 is adopted
by regular distros.   It will eventually work like sysVinit or systemd in Debian, where you just install a package, and all the work configuring and starting a daemon is done for you.  Then you would hardly notice which init system is being used, unless you wanted to stop some service.

One might get the impression from my topics on the init-diverdsitry-spins 
https://itsfoss.community/t/summary-of-s6-66-in-antix-init-diversity-spin/12149
that using S6 with `66` is difficult. That is true while learning, but we have seen here that, once learnt, `66` is simple to use. There is no doublt that `66` could replace `systemd`. It has all the same functionality, a better intellectual basis, and is not tied to Linux or glibc ( it can work with BSD or musl ). I hope it gets a foothold somewhere.


Obarun is obviously setup to demonstrate `66`.  It is a fine Arch based distro . The init-diversity-spins have extended `66` to the Debian family. There is an unofficial port of `66` to Void Linux. It would be nice to see a port to BSD next. 


#### Acknowledgement ####
I appreciate the assistance given by @Obarun and @ProwlerGR in learning to use `66'.


#### Links ####

http://web.obarun.org/

https://wiki.obarun.org/

https://www.skarnet.org/software/

https://tiagojsilva.github.io/en/unixlike/meme/arch_manjaro/2021-12-02_obarun-install/
(this link is dated 2022)

https://www.antixforum.com/forums/topic/antix-s6-66-sysv-runit/
(this link is dated 2019)

