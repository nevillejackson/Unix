### Distributions with runit preinstalled 

Several distributions come either with `runit` as the
default init system, or provide a separate .iso file for a runit version.. 
For example Antix and  Artix offer .iso files with runit as the
 init system.

All `runit` implementations keep certain directories, mostly in /etc, and these contain

 - run , supervise and log scripts for each installed service. These are typically located in a directory called `sv` ( usually /etc/sv)... one directory per service.

 - a record of the services to be started ... called `current` plus several other 
sets of services called 'default', 'previous', 'single', 'solo', etc. These are usually in /etc/runit/runsvdir)

 - startup scripts for 3 runlevels , called `1`, `2`, and `3`. These are usually in /etc/runit)

There are variations on the above.

Runit has a unique mechanism for setting up services to be started... it uses symbolic links located in a  services directory, and pointing to the relevant service in the `sv` directory. If you make a link, the service starts immediately, and also autostarts at boot. The collection of links is sometimes referred to as 
a "link farm".

#### Void Linux ####
 In Void Linux, runit is the only available init system.  So a downloaded Void .iso will contain a preinstalled runit.
Void is a suitable place to look , if you want to see runit doing everything. 

When Void  boots you see messages about runlevels fly across the screen. They are the runit runlevel scripts found in /etc/runit/1 and /etc/runit/2. You should see levels 1 and 2. Level 3 is for shutdown. 

After Void is running, you can see the init daemon
```
$ ps ax | grep runit
    1 ?        Ss     0:00 runit
```
 Yes , the init daemon is called `runit` and it is process no 1.

If you look in the `/etc` directory in Void, you find two subdirectories related to 
runit operations

 - sv subdirectory contains all the scripts to run or supervise every service that has been installed
 - runit subdirectory contains the three runlevel scripts which run at startup and shutdown, and an important subdirectory called `runsvdir` which contains all the symbolic links set by default or by the user to start services.

Void also has a symbolic link in /var
```
$ ls -l /var
....
lrwxrwxrwx  1 root root   29 Apr 14  2023 service -> ../run/runit/runsvdir/current
```
which points to the current set of symbolic links. 
Void differs from other runit distros, in that it puts this link in `/var/service` instead of in `/etc/service`.
Void users are encouraged to use /var/service to simplify setting of links 
For example

```
ln -s /etc/sv/sshd /var/service/sshd
```

will make a link in `current` which will start the `sshd` daemon.

In addition to making links, runit has the `sv` command which can get the status of a service, or start or stop it, for example

```
#sv status /var/service/*
run: /var/service/NetworkManager: (pid 1084) 4185s; run: log: (pid 1083) 4185s
run: /var/service/acpid: (pid 1038) 4185s
run: /var/service/agetty-hvc0: (pid 21437) 5s
.....
run: /var/service/sshd: (pid 1060) 4185s; run: log: (pid 1059) 4185s
....
run: /var/service/virtstoraged: (pid 1051) 4185s; run: log: (pid 1050) 4185s
run: /var/service/virtvboxd: (pid 1069) 4185s; run: log: (pid 1067) 4185s
down: /var/service/virtxend: 1s, normally up, want up; run: log: (pid 1065) 4185
```

which lists the status of all services supposed to be running. 

There is also 
`sv up <servicename>`
`sv down <servicename>`
`sv status <servicename>`
which bring a given service up or down or list its status.
The `sv` commands only work  after a service has been enabled by making a link.

When you install a service in Void ( as a Void package) the package system ( called xbps) installs the service but does not start it.  One has to follow up the package install by doing two things

 - check that the service has a subdirectory in `/etc/sv` containing at least a `run`
script and preferably also a `supervise` and a `log` script.
 - make a symbolic link to start the service.

Most Void packages will install the required entries in `/etc/sv` , but not all. 
I had to make a `run` script for `ftpd`. It is simple, here is what I used
```
#!/bin/sh
exec ftpd -D
```
all it does is start the daemon... without starting another shell to run it in ( that is what `exec` is for).  Most run scripts are that simple.

Unless you want to get into supervising daemons ( not really needed in a home
 computer) that is really all there is to setting up runit. In a server you may wish to use supervision.


The reason that `runit` is simple in Void , is that the package system is setup to work with `runit`. Installing a package that is a service usually provides all the necessary files in /etc/sv  All the user has to do to start the service is to make the necessary link.

Void has several packages relevant to `runit` in addition to the two basic packages `runit` and `runit-void` which come preinstalled

```
$ xbps-query -Rs runit
[-] rsv-1.3.3_3                    Runit sv command rewritten in rust with nic...
[*] runit-2.1.2_15                 UNIX init scheme with service supervision
[-] runit-iptables-20180616_2      Restore iptables rules on boot
[-] runit-kdump-20150226_5         Crashkernel/kdump support for runit to save...
[-] runit-nftables-20200123_2      Restore nftables rules on boot
[-] runit-swap-2.0.0_2             Script to manage swapspaces
[*] runit-void-20231124_1          Void Linux runit scripts
[-] runit-void-apparmor-20231124_1 Void Linux runit scripts - AppArmor initial...
[-] socklog-2.1.0_6                Small and secure syslogd replacement for us...
[-] sv-helper-2.0.2_2              Utilities to help administer a runit-as-pid...
[-] svctl-1_9                      Interactive controller for runit
[-] vsv-2.0.0_1                    Manage and view runit services
```
You can see that I have not installed any of these extra packages ([-]).
I tried vsv

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/runit/void1.png?raw=true">
</p>

It is neat but terminal based, not GUI. There are lots of services running in my Void, those called `virt...` are supporting virsh and virt-manager.

Learning runit is probably the  most challenging part of learning Void. In a distro like Debian one can get away with never touching the init system. In Void, because the package system only installs services, and does not enable them, you have to enable services by hand, and that means learning `runit'




#### Artix Linux ####
Artix is Arch with a choice of non-systemd init systems (dinit, OpenRC, runit or S6). You can download an iso with runit preinstalled (eg. artix-xfce-runit-20230814-x86_64.iso is what I used).
When you bring up a freshly installed Artix, you see the following runit files in 
/etc/runit


<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/runit/artix1.png?raw=true">
</p>

What you see above in /runit/runsvdir/default is the "link farm" for a number of services. They are mostly very essential services like `dbus` and the `tty's`. 

So those are the default links.  The actual services installed are in /etc/runit/sv.
You can just see the /etc/runit/sv directory at the bottom of the tree above.
The first couple of entries in /etc/runit/sv are

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/runit/artix2.png?raw=true">
</p>

There you see the entry for `acpid` along with its various subdirectories.
The full set of available services in /etc/runit/sv is

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/runit/artix3.png?raw=true">
</p>

There are more services installed, than have been activated by the "link farm"

What we have in Artix, is almost exactly the same as in Void, except

 - the `sv` subdirectory is inside the `/etc/runit` directory,
 instead of in `/etc`
 -  the service directory is in `/run/runit/service`

Now let us see what happens when we install a new package.
We shall try `vsftpd`
So look at the packages available for vsftp

```
tr~# pacman -Ss vsftpd
world/vsftpd 3.0.3-9
    Very Secure FTP daemon
world/vsftpd-dinit 20211103-3 (dinit-world)
    dinit service scripts for vsftpd
world/vsftpd-openrc 20210505-1 (openrc-galaxy)
    OpenRC vsftpd init script
world/vsftpd-runit 20200614-3 (runit-galaxy)
    Runit service script for vsftpd
world/vsftpd-s6 20230829-1 (s6-world)
    s6-rc service scripts for vsftpd
```
Well, apart from vsftpd itself, there are packages of scripts for each init
system that Artix offers.
This is interesting, Artix has solved the problem of packages needing to supply different sets of files , depending on which init system is running, by having separate packages of script files for each init system. That is neat.

So I need to install `vsftpd` and `vsftpd-runit`

```
pacman -S vsftpd
pacman -S vsftpd-runit
...
(1/1) Displaying runit service help ...
	==> Add a service:
	ln -s /etc/runit/sv/<service> /run/runit/service/
	==> Start/stop/restart a service:
	sv <start/stop/restart> <service>

```
So it displays a little helpful message about how to add the service.
So lets do it. There is a directory `/run/runit/service/` but I am going
 to ignore that suggestion and put the link directly
 in `/etc/runit/runsvdir/current`

```
ln -s /etc/runit/sv/vsftpd /etc/runit/runsvdir/current/vsftps
```
and the link appears in /etc/runit/runsvdir/current , and the ftp daemon is now running

```
# ps ax | grep vsftpd
 3000 ?        Ss     0:00 runsv vsftpd
 3001 ?        S      0:00 vsftpd -obackground=no
```
Process 3001 is the vsftpd daemon, process 3000 is the runit supervisor.
You get a `runsv` process for every service that is activated.
My Artix now supports ftp access.

Artix runit setup is almost identical to Void, except for the location 
of the `sv` directory and the `service` pointer.
What Artix brings is a a superior way of organizing the package system, so that 
it can properly support alternative init systems. The idea of having separate packages for each init system's script files for each service, is brilliant.


#### Antix Linux ####

Antix linux provides .iso files with either sysvinit or runit init systems.
I used `artix-xfce-runit-20230814-x86_64.iso`.

Antix with runit is a classic runit installation,
 exactly as described in the official runit site. 
The examples I used in the "How runit works" section were taken from Antix. There is no need to repeat them here. We might just summarize

 - the service directory is `/etc/service`, it is actually a link

 - `/etc/sv` contains the collection of run script directories

 - `/etc/runit` contains the runlevel scripts ( 1, 2, 3) and the `runsvdir`
directory which contains the current, default, previous, single and solo link
farms.

 But Antix has more than that. Anti has a GUI interface to runit. If you bring up the `Control Centre`, choose `System`, and then choose `Choose Startup Services`, you get the `Runit Service Manager`

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/runit/antix1.png?raw=true">
</p>

You can use the `Runit Service Manager` to do everything that the `sv` command will do, plus you can enable and disable logging on a per service basis. 
Antix is , as far as I know, the only distribution with  GUI interface to runit. I believe the GUI Runit Service Manager is provided by Xecure, but I was unable to verify it.

Antix also has one peculiarity. There are a small number of services, mostly very basic startup services, to be found in `/etc/init.d` directory

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/runit/antix2.png?raw=true">
</p>
 
These are sysVinit scripts, not runit run scripts. 
Runit, at least in Debian based distros, has the property that if it can not find a run script for a service in the /etc/sv directory it looks in /etc/init.d for a sysVinit script and launches that.  Antix is Debian based.

I think developers have left some of the very basic scripts used at startup in sysVinit format. You will also see there `qemu-guest-agent` and `spice-vdagent`. My Antix is a VM, and I installed these, so clearly the packages I installed had not been converted to supply a runit script directory.... they supplied a sysVinit script, so the installer put it in init.d. 
On the other hand, I also installed `ntp` from the package system, and it provided a set of ntp runit scripts in /etc/sv

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/runit/antix3.png?raw=true">
</p>
So  some parts of Antix's package system supply runit files. Antix has a compound set of repositories, some being Debian, others Antix. 

So Antix is not quite completely runit, it does not have the elegant init-aware package system that Artix has, but it does have some nice features.
