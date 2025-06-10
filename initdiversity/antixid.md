#### Antix-23.2 init diversity 2025 remaster edition - includes dinit ####
Our friend @ProwlerGr from the Antix init diversity group was kind enough to send us a reminder that there is a new release of their init diversity spin which now includes the `dinit` init system ( along with sysvinit, s6-rc, s6-66, OpenRC and runit).

The announcement is here

https://www.antixforum.com/forums/topic/antix-23-2-init-diversity-2025-remaster-edition/

the .iso download is here

https://archive.org/download/antiX-23.2_init-diversity_20250510/Current/

and
The changelog from the previous edition is
```
Sysvinit:
– Bumped to 3.14-4 (backported from Trixie)
– A new metapackage sysvinit-init-diversity has been introduced to allow sysvinit to be installed alongside other inits

OpenRC
– Bumped to 0.56-1 (backported from Trixie) & modified dependencies to remove conflicts with sysv-rc
– A new metapackage “openrc-init-diversity” has been introduced to allow OpenRC to be installed alongside other inits.
– Another metapackage “openrc-init” has been introduced for those who wish to use OpenRC as the sole or default init on their system (or don’t care for other inits).

Dinit:
– Bumped to 0.19.4 (built from upstream)
– A new metapackage “dinit-init-diversity” has been introduced to allow dinit to be installed alongside other inits.
– A new metapackage “dinit-init” has been introduced for those who wish to use dinit as the sole or default init on their system (or don’t care for other inits).
– A new GUI “Dinit-Dialogbox-Manager” has been developed based on dialogbox (similar to the runit-service-manager)
– A few sample services have been packaged in a manner to handle service & package dependencies.

s6-66:
– Bumped to 66 version 0.8.0.2 (built from upstream) – This is an upgrade & incompatible to the previous 0.7.0.0 version which was present in last years spin
– A new metapackage “s6-66-init-diversity” has been introduced to allow s6-66 to be installed alongside other inits.
– Metapackage “s6-66-init” is available for those who wish to use s6-66 as the sole or default init on their system (or don’t care for other inits).
– Numerous services have been packaged in a manner to handle service dependencies – package dependencies & compilations automatically with updated maintainer scripts.

s6-rc:
– Bumped to newer versions of skalibs – execline – s6 – s6-rc – s6-linux-init – s6-linux-utils – s6-portable-utils.
– A new metapackage “s6-rc-init-diversity” has been introduced to allow s6-rc to be installed alongside other inits.
– Metapackage “s6-rc-init” is available for those who wish to use s6-rc as the sole or default init on their system (or don’t care for other inits).
– The whole ecosystem has been reconfigured & therefore incompatible with last years version due to the following reasons:
– A new optimised boot-module has been created which has dropped the majority of the previous bash scripts (from sysvinit) that were present in last years spin.
– Everything now is mostly execline (which is lean & mean to steal @anticapitalista’s line)
– The s6-db-reload script has been ported from Artix & is available to handle database compilations.
– Numerous services have been packaged in a manner to handle service dependenies – package dependencies & compilations automatically with assistance of the above tool
– The s6-rc-dialogbox-manager (former s6-rc-service-manager) has been reconfigured to utilise the same tool for compilations

Runit:
– Bumped to 2.2.0-3 (backported from Trixie)
– A new metapackage “runit-init-diversity” has been introduced to allow runit to be installed alongside other inits.
– The already available “runit-init” is there for those who wish to use runit as the sole or default init on their system (or don’t care for other inits).
– A new boot module has been created allowing for runit to be run pure without reliance on sysvinit folders or scripts
– A few services have been packaged in a manner to handle service & package dependencies.

```

There is also this comment from @ProwlerGr
```
This release is much more than just another respin as it includes a lot personal
 work from myself to port various packages and scripts from upstream & other distro’s.
I must mention that a lot of effort has been put to create a turnkey solution in
 terms of packaging the various services to handle dependencies & create the 
proper scripts to enable/start them.
```

##### Installation #####
I downloaded the file `antiX-23.2_init-diversity_x64_20250525.iso`, about 3 minutes... they have a good fast download site.
 I installed it in a VM (virt-manager), using Generic Linux 2022, 8000Mb, 4CPU's, 50 Gb virtual disk.
Virt-manager brings up the following screen where you choose which to install.

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/initdiversity/antixid2025-1.png?raw=true">
</p>

I chose the generic version ( highlighted), because I wanted an install. The others are for looking at live systems.
Look at the F1 to F7 options if you want to fiddle settings
For example F1 will tell you that there is user `demo` with passwd `demo`, and the root passwd is `root`
I took the defaults
It then produces a live Antix screen with an `Installer` button

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/initdiversity/antixid2025-2.png?raw=true">
</p>

Press the Installer button and you get a friendly graphical installer 

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/initdiversity/antixid2025-3.png?raw=true">
</p>

that is OK,  I can use all the virtual disk

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/initdiversity/antixid2025-4.png?raw=true">
</p>

I need to set the computer name, domain, locale, timezone, user name and passwd, root passwd,...
Then it finishes by installing grub in the virtual disk, updates initramfs, and says Finish.

##### Running Antix/dinit from the installed Antix #####

The installed Antix boots into a grub screen

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/initdiversity/antixid2025-5.png?raw=true">
</p>


I choose the dinit option at the bottom

and it boots to an antix Login Manager screen

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/initdiversity/antixid2025-6.png?raw=true">
</p>


Yes, the Antix login screen is B&W.... it changes to color when you login.... neat.
I used F1 to toggle the Window manager to `rox-icewm` , which is what I always use (Antix offers a choice of 13 different Window managers, but by default does not use a DE... you can install one if you want). 

Then login and I get

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/initdiversity/antixid2025-7.png?raw=true">
</p>

The IceWM screen. It has a taskbar at bottom, two workspaces by default, a menu button in the corner or on clicking backgound menu, and some icons.

Lets start a terminal and see if we can find dinit?

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/initdiversity/antixid2025-8.png?raw=true">
</p>


Yes, there it is, process number 1 is called `dinit` rather than `init`.


Dinit keeps all its system service config files in `/etc/dinit.d`. We can have a look

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/initdiversity/antixid2025-9.png?raw=true">
</p>

These files are editable. The templates are in /usr/lib/dinit.
There are not as many service files as in Artix/dinit, but more than are in Chimera/dinit. 

Dinit can also have user controlled services. The config files for user services are in `~/.config/dinit.d`.  There are no user config files in my default Antix installation. 

Lets see what services are running

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/initdiversity/antixid2025-13.png?raw=true">
</p>


There are no user services, but there are a basic set of system services. For each running service , there is a service file in `/etc/dinit.d`.

There is no `sshd` so lets install it.

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/initdiversity/antixid2025-10.png?raw=true">
</p>


It is installed... but there is no `sshd` server file, and it definitely is not running?

Maybe I need to start it... so go to Control Centre -> System -> Choose Startup Services.... and it fails .... so there is no GUI service manager for dinit yet. There are for runit and sysVinit.
 
What I need is a startup service file.... so  I will start with the simplest possible service file

```
type            = process
command         = /usr/bin/ssh-agent -D
smooth-recovery = true
depends-on      = ssh-keygen

```
So that is all there is to a dinit service file... they are not really scripts.... more like a simple config file. 
Copy that into /etc/dinit.d and see if I can start sshd with dinit commands
.... I will probably need  a ssh-keygen service file  too. 
 
```
type		= scripted
command 	= /usr/bin/ssh-keygen -A
```
Copy that in too.

Now,  lets ty to start sshd

```
# dinitctl start sshd
Service 'sshd' started
```
So it is started.  I actually had to reboot to get that to work?
Now, if we list the services

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/initdiversity/antixid2025-14.png?raw=true">
</p>

That is the end of `dinitctl list`.  There are now 2 extra services, sshd and ssh-keygen. I can also dee the daemon running with `ps`

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/initdiversity/antixid2025-15.png?raw=true">
</p>

The above only starts the sshd service for the current session. If we want the `sshd` service to start at every boot, we use
```
# dinitctl enable sshd
```
This will start the daemon immediately ( as with `dinitctl start`) and make sure it starts by default at every boot.

Notice that the `ssh-keygen` service is also started, although we did not ask for it explicitely. Dinit automatically makes sure any declared dependencies are started.

I checked, after a reboot, sshd and ssh-keygen are still running.
That is enough introduction to dinit.

##### Dinit documentation #####

There is a `Getting Started with Dinit` document here

https://github.com/davmac314/dinit/blob/master/doc/getting_started.md

and it will show you how to setup user-controlled processes.
The Artix Wiki page on dinit is also useful

https://wiki.artixlinux.org/Main/Dinit

and the source webpage from Davmac is here

https://davmac.org/projects/dinit/

and the Chimera page on service management is also a well written account

https://chimera-linux.org/docs/configuration/services

and  finally there is my own account of using dinit in Artix

https://itsfoss.community/t/the-dinit-init-system-in-artix-linux/12993

##### My assessment of the progress with dinit in init diversity spin #####

I found that the dinit version of Antix-Init-Diversity-2025 works either from an install or as a live iso. It comes with 31 services enabled, including important things like `udevd` and the networking daemon `connmand`. 

The implementation of dinit is not quite the same as in Artix, but the location of config files is the same. This is not surprising as we are comparing Debian with Arch there.

I tried starting a service which  was not present ( sshd).  It was installed but there was no service file for it. I had to write one. It was easy to setup an elementary service file, especially as I had Artix as a guide. The dinit commands to start or enable a service worked. The command to list services worked. 

From a progress point of view , the Init Diversity spin has implemented dinit and has service files for some 31 services, but other service packages ( like sshd) if installed will not install a service file. You have to write the service fiile(s) yourself. It is not difficult.  I guess before Antix release a dinit iso version they will attend to making service packages install service files when the binary is installed.

The other packages in the init diversity spin ( OpenRC, runit, s6-rc, s6-66, sysvinit) have the same package deficiency to various degrees. Sysvinit is fairly complete.

Overall it is an impressive effort, for what is really just a demonstration of alternative init systems.

##### My view on dinit #####
In the itsFOSS forum post on dinit in Artix ( see link above) I wrote

" Yes, all the others are a complicated mess, compared to dinit.
They suffer from too much partitioning of tasks.
Dinit is one blob of C++ code, and nothing else. It cant grow tentacles and invade your system ( like systemd) and it does not swamp your ps with processes (like runit and s6), and it does not fill up our filesystem with link farms( like sysvinit, OpenRC, runit). "

 After lookin at the init diversity implementation I have not changed my mind. It does use links, but they are minor. 

 Dinit does more than I have displayed here.
 The main attributes of dinit are

 -   Dinit allows the user to start and stop daemons ( ie services) with simple commands similar to those used by OpenRC.
 -  Dinit handles system startup and shutdown.
 -  Dinit provides a process No 1 called /usr/bin/dinit which forks all other processes
 -  Dinit provides parallel startup of services and dependency management
 -  Dinit provides service supervision, ie it automatically restarts failed daemons
 -  Dinit allows users to setup services

I see dinit as the simplest and best init system option for home computer users.
If you want to try dinit, use either the Antix Init Diversity spin 2025, or Artix/dinit. There is also Chimera, but it is a beta release.
I am informed that Antix will be making an Antix/dinit iso release soon.

##### Acknowledgement #####
Many thanks to @ProwlerGr  for keeping itsFOSS forum in the loop on this important progress. 


