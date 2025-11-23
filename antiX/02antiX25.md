#### Starting and stopping services(daemons) when the init system service files are present in /etc. ####

All init systems require one or more files ( called service files) which describe how to start and stop each daemon. The format and layout of these files varies greatly between init systems. Users of `sysVinit` will recall the files in /etc/init.d` which are basically scripts... one script per daemon.  Service files for the 4 init systems we have in `artiX25` are located as follows
 - runit service files are in /etc/sv and /etc/runit and /var/service(or /etc/runit/runsvdir/default)
 - S6-Rc service files are in /etc/s6-rc and /etc/s6-linux-init
 - s6-66 service files are in /etc/66 (admin added files) , /usr/share/66 ( supplied system files) and ~/.66 (user added files)
 - dinit service files are in /etc/dinit.d (admin added files) , /usr/share/dinit (suplied system files) and ~/.config/dinit.d ( user service files).

In old fashioned init systems such as `sysVinit` service files typically came with the package that installed the service. So one simply installed the package and the service was started ( ie made to run)  and enabled ( ie made to start at every boot). 

That is only partly true for the 4 more modern init systems available in `antiX25`. Some packages ( eg sshd, cupsd, connmand, bluetoothd,...) have been provided ( preinstalled not with their packages) ,  but others ( eg vsftpd, nfsd, ...) are not present, and the user is required to write  service file(s) before attempting to start the service daemon. 

Here we are going to look at the simplest case.... how to start and stop a daemon in each of the 4 init systems, when the service files are present.

##### Runit service managenment #####
We will use sshd as an example, the package is installed, but sshd is not running at boot
One needs to find the name of the service ( it is not necessarily the same as the name of the daemon). Look in /etc/sv....we see `ssh` not `sshd` ... that is the service name. One can check with `cat ssh/run` ... you will see the run script....yes the script starts `sshd`.

Next one needs to check in `/etc/runit/runsvdir/current` to see if the ssh service is currently controlled by runit. It is not. Another way to check is to use the Control Centre -> Runit Service Manager GUI. It does not list `ssh`, but it can be used to add it.

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/antiX/runitaddservice.png?raw=true">
</p>



then it appears in the list

....  runitserviceadded.png
<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/antiX/runiterviceadded.png?raw=true">
</p>


and can now be seen as a file in `/etc/runit/runsvdir/current` ( actually it is a symlink pointing to `/etc/sv/ssh`

From here , one can either use the Runit Service Manager GUI to start/stop/enable/diable the ssh service, or one can use CLI commands as follows
To start or stop 
```
sv up ssh
sv down ssh
```
To check its status
```
sv status ssh
```
To enable ( ie make it start at boot)
```
ln -s /etc/sv/ssh /etc/runit/runsvdir/default/
```
Yes runit uses links to enable services. Once a service is linked, it will automatically start immediatelt,  it will always start at boot, and if it stops it will be restarted. That is called 'supervision' of services. Runit has supervision.
What we did `adding` a servie with the GUI is equivalent to making the above symlink.

That is all there is to it. The biggest headache with runit is that every distro puts its service files in different locations. There is no standard layout. 



To disable a service, remove the symlink. 


##### s6-rc service management #####
s6-rc is the original S6 service manager.
We shall again use sshd as an example. The package is installed.... it is the exact same package that we saw was installed using runit. AntiX separates packages from init system service files. So a package only needs to be installed once... it will work for all init systems.  That is good design.  Artix does that too. 

As with runit, one needs to check what is the service name.  Look in /etc/s6-rc/sv. We see two service files `sshd-log` and `sshd-srv`. S6 runs two supervisor processes called `s6-supervise sshd-log` and `s6-supervice sshd-srv` as well as the daemon itself. ( Contrast to runit which runs one supervisor process called `runsv sshd` plus the daemon). So the service name is `sshd`. 

The s6-rc GUI is not in Control Center, it is in Applications -> Preferences -> s6-rc Dialogbox Manager. 

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/antiX/s6-rcdialogboxmanager.png?raw=true">
</p>


In this case sshd-log and sshd-srv ar in the GUI .... we dont need to add them. ```
 ps ax | grep sshd
  259 ?        S      0:00 s6-supervise sshd-log
  274 ?        S      0:00 s6-supervise sshd-srv
  956 ?        Ss     0:00 s6-log -d3 -b -- n3 s2000000 T /var/log/sshd
 1015 ?        Ss     0:00 sshd: /usr/sbin/ss e
```
So all we need to do is look at the service management commands
To start/stop sshd

```
s6-rc -u change sshd
s6-rc -d change sshd
```
the 'u' and 'd' stand for up and down.

Enabling a service in s6-rc is more complicated.  You have to add it to a "runlevel" bundle. A bundle is a group of services. THere is a bundle called 'default' which is useful for adding services which we wish to satart at boot time. To add sshd to it 
```
s6-rc-bundle add default sshd
```
Then to ensure the change is loaded next boot
```
s6-rc-compile
```
Yes, you have to manually recompile the database.

To stop a service temporarily
```
s6-rc-bundle stop sshd
```
and you dont recompile , so after a boot the service will be back.

I am not sure how much of that can be done in the GUI. It seems to have add, restart, and stop buttons.

##### s6-66 service management #####
s6-66 is S6 with theoriginal service manager replace by 66.
We shall again use sshd as an example. The package in installed. The service files are in /etc/66 (admin added system files) and /usr/share/66/service (supplied systm files) and ~/.66 (user service files). The service is running and it is a system service.... if it was a user service the service files would be in ~/.66.

```
$ ps ax | grep sshd
  786 ?        S      0:00 s6-supervise sshd-log
  787 ?        S      0:00 s6-supervise sshd
  877 ?        Ss     0:00 /usr/bin/s6-log -d3 n3 s1000000 /var/log/66/sshd
  926 ?        Ss     0:00 sshd: /usr/sbin/sshd -D -e -f /etc/ssh/sshd_config [listener] 0 of 10-100 startups
```

Compared to s6-rc, s6-66 avoids compiling a database, and it has a ‘frontend’ file instead of an execline script. 66 has ‘trees’, which are like bundles, and it allows users to manage services, as well as root.

There is a `global` tree which is the default and is present at install time, and is always enabled ( ie it starts automatically at boot time).  New services default to the `global` tree , unless another tree is specified. 

There is a `boot` tree which contains various services needed when the system boots. There is a tree called ‘session’ which containes modules which setup an environment in which a user can manage services.
Other user-defined trees can be created. A service can only belong to one tree.
Any type of service can belong to a tree, even a module service.

There is a directory `/run/66/scandir/…` containing, for each user, the compiled version of the frontend for each running service, plus a number of status files. For root it is /run/66/scandir/0/…, for the first user it is /run/66/scandir/1000/… There is a special admin command to manage scandirs.
The root scandir is automatically created. A user scandir must be initialised with the 66 scandir ... command, before any user services can be started.

We can look at the trees in the default setup ( as installed) 

```
nevj@antix1:~
$ 66 tree status
Name        : global
Current     : no
Enabled     : yes
Allowed     : nevj
Groups      : user
Depends     : None
Required by : None
Contents    : dbus@nevj-log dbus@nevj
```
A user can only see the `global` tree.

```
root@antix1:~# 66 tree status
Name        : global
Current     : no
Enabled     : yes
Allowed     : root
Groups      : admin
Depends     : None
Required by : session
Contents    : seatd-log seatd dhcpcd-log dhcpcd acpid-log acpid cron-log
              cron haveged-log haveged alsa-log alsa alsa-restore
              dbus-log dbus sshd-log sshd ufw bluetoothd-log bluetoothd
              connmand-log connmand slimski-log slimski cupsd-log cupsd

Name        : boot
Current     : no
Enabled     : no
Allowed     : root
Groups      : boot
Depends     : None
Required by : None
Contents    : boot@system boot@system:system-hostname boot@system:mount-run
              boot@system:mount-tmp boot@system:populate-sys
              boot@system:mount-pts boot@system:mount-shm
              boot@system:populate-dev boot@system:mount-cgroups
              boot@system:system-sysctl boot@system:tty-earlier@tty12
              boot@system:populate-run boot@system:populate-tmp
              boot@system:mount-branch boot@system:system-hwclock
              boot@system:system-random boot@system:udevd
              boot@system:udevd-log boot@system:udevadm
              boot@system:system-fontnkey boot@system:system-fsck
              boot@system:system-branch boot@system:mount-rw
              boot@system:mount-swap boot@system:local-loop
              boot@system:local-sethostname boot@system:local-time
              boot@system:local-authfiles boot@system:local-rc
              boot@system:local-dmesg boot@system:local-branch
              boot@system:runtime-branch boot@system:canopy
              boot@system:tty-rc@tty2 boot@system:tty-rc@tty1

Name        : session
Current     : no
Enabled     : yes
Allowed     : root
Groups      : admin
Depends     : global
Required by : None
Contents    : scandir@nevj:svscan@nevj scandir@nevj:svscan@nevj-log
              scandir@nevj boot-user@nevj:mount-run@nevj boot-user@nevj

```

Root can also see the `boot` tree and the `session` tree.

Notice that sshd is in the global tree... therefore it will start at boot.

Here are some things we can do with 66 commands. 

Start/stop a service immediately

```
66 start sshd
66 stop sshd
```

Enable a service ( this will put it in theglobal tree)
```
66 enable sshd
```

Disable a service ( this will prevent it starting at boot)

```
66 disable sshd
```

Show status of a service

```
root@antix1:~# 66 status sshd
Name                   : sshd
Version                : 0.0.2
In tree                : global
Status                 : enabled, up (pid 926 pgid 926) 2152 seconds
Type                   : classic
Description            : ssh daemon
Part of                : None
Notify                 : 0
Max death              : 5
Earlier                : 0
Source                 : /usr/share/66/service/sshd
Live                   : /run/66/scandir/0/sshd
Dependencies           : sshd-log
Required by            : None
Optional dependencies  : None
Start script           : 
                         #!/usr/lib/execline/bin/execlineb -P
                         
                             /usr/lib/execline/bin/foreground { if { test -e /etc/ssh/sshd_not_to_be_run } 66 stop sshd }
                         
                             /usr/lib/execline/bin/foreground { bash -c "if id -u sshd >/dev/null 2>&1 ; then exit 0 ; else /sbin/useradd -r sshd -s /bin/nologin >/dev/null 2>&1 ; fi" }
                         
                             /usr/lib/execline/bin/foreground { mkdir -p /run/sshd }
                         
                             /usr/lib/execline/bin/foreground { chmod 0755 /run/sshd }
                         
                             /usr/lib/execline/bin/foreground { /bin/ssh-keygen -A }
                         
                             /usr/lib/execline/bin/foreground { touch /var/log/lastlog }
                         
                             /usr/lib/execline/bin/foreground { chgrp utmp /var/log/lastlog }
                         
                             /usr/lib/execline/bin/foreground { chmod 664 /var/log/lastlog }
                              execl-cmdline -s { /usr/sbin/sshd ${Args} }
Stop script            : 
                         None
Environment source     : /etc/66/conf/sshd/0.0.2
Environment file       : 
                         environment variables from: /etc/66/conf/sshd/0.0.2/.sshd
                         Args=!-D -e ${ArgsConfFile}
                         ArgsConfFile=!-f /etc/ssh/sshd_config

Environment ImportFile : None
StdIn                  : s6log:/run/66/scandir/0/fdholder
StdOut                 : s6log:/var/log/66/sshd
StdErr                 : inherit:/var/log/66/sshd
Logger name            : sshd-log
Logger file            : 
66-execute: info: Starting service: sshd
Server listening on 0.0.0.0 port 22.
Server listening on :: port 22.
66-execute: info: Starting service: sshd
Server listening on 0.0.0.0 port 22.
Server listening on :: port 22.
```
Status gives a mountain of information.

There is a lot more than that to s6-66. The concept of user-controlled services is new to me and I do not yet have a grasp of why one would need user-controlled daemons.  The way to use trees to manage a large set of services in a server computer something I have not mastered. 

Beware reading older web-sites on 66 . 66 is evolving and the command syntax has changed. Older sites may have 66-start `<servicename>`, but the modern form is `66 start <servicename>` .... ie it isnow like git... 66 is the command and start is a subcommand. 

We were fortunate to get some contributions and assistance on this forum from thedeveloper of 66, @oberun (Eric Vidal). See this topic

https://itsfoss.community/t/s6-init-system-with-s6-66-service-manager-in-antix-init-diversity-spin/11984

66 has its own demonstration distro, called Obarun Linux

https://web.obarun.org/

s6-66 is definitely easier to use than s6-rc, and it does not sacrifice any of the sophisticated service mamnagement abilities. S6 is definitely a competitior with systemd. Their capabilities are similar.



####e Dinit service management #####
Dinit is different to runit and S6. Dinit does not have separate supervising and logging processes for each daemon. Those functions are performed, for all daemons by a single process called `dinit` which also acts as process no 1 ( the init process). What you see running is
```
$ ps ax
  PID TTY      STAT   TIME COMMAND
    1 ?        S      0:00 /lib/dinit/dinit
    2 ?        S      0:00 [kthreadd]
.....
```
plus one process for each daemon. For example
```
 601 ?        Ss     0:00 /usr/libexec/bluetooth/bluetoothd
  602 ?        Ss     0:00 /usr/sbin/cupsd -f
  603 ?        Ss     0:00 /usr/bin/slimski -nodaemon -z -i
  617 ?        S      0:00 /usr/bin/dbus-daemon --system --nofork --nopidfile --
  628 ?        S      0:00 sshd: /usr/sbin/sshd -D -e -f /etc/ssh/sshd_config [l
```
There you see 5 daemons runnning. 
In Antix25 the daemon sshd starts at boot by default and its service file is in /usr/share/dinit/services.... so it is a system wide service suplied with the distro. Its service file is simple
```
type = process
command = /etc/dinit.d/scripts/sshd

depends-on = loginready
```
No script at all, just a declaration... the binary to start is called sshd and it depends on a process called loginready.

The command that controls services in dinit is called `dinitctl`. It has subcommands like git. 

To start/stop a daemon such as sshd ...
```
dinitctl start sshd
dinitctl stop sshd
```

To check service status. Dinit in AntiX25 does not yet have a GUI service manager . You have to use CLI as follows to check on services.
```
dinitctl status sshd
```

To enable a service so it will start at boot
```
dinitctl enable sshd
```

That is all. Dinit is simple , but not without capability. It can bundle services, like s6-66, it allows users to setup services, it provides servce supervision, it does parallel startup of services., but not without capability. It can bundle services (like in s6-66), it allows users to setup services, it provides servce supervision, it does parallel startup of services.


##### Discussion #####
I have tried to illustrate the simple task of starting and stopping services in each of the 4 init systems, and, I have tried to add a few explanatory comments along the way.

The result is a messy article, but I hope it will help people to see that init systems are nothing to be afraid of. 

There is far more to it , of course. We need to move on to what one needs to do if a service file is not available for a particular service

