### Testing the antiX25-beta1_x64 release ###
 The work of @ProwlerGr which we looked at in the topic

https://itsfoss.community/t/antix-23-2-init-diversity-2025-remaster-edition-includes-dinit/13696

(and in several previous topics which can be found by search for 'init diversity')

has now been incorporated into an antiX official release. The beta release is now available, and there will be a final release of antix25 in a couple of months. 
This is a major step for antiX , because it marks the incorporation of 4 modern non-systemd init systems into the one .iso. 

I thought we might take a brief look at what it is like to setup one service in each of runit, s6-rc, s6-66, ad dinit. I chose to use 'setting up an NFS sever' as the task. 

#### Installing antix25-beta1 ####
As with most new distros, it is safer to start with a VM install. I used virt-manager in Void Linux, and I downloaded the `antiX-25-beta1_x64-full.iso` file. The download link is

https://sourceforge.net/projects/antix-linux/files/Testing/antiX-25/

It is 2.1GB. I checked the md5sum. 
It boots in  virt-manager . You see a screen as follows

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/antiX/antiX25.png?raw=true">
</p>


then follows a screen offering choices of init system

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/antiX/antiX251.png?raw=true">
</p>


I first chose 'normal boot', and it boots to the following screen

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/antiX/antiX252.png?raw=true">
</p>



That is live antiX. 


It installs smoothly. On reboot I get  a grub screen,

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/antiX/antiX25grub.png?raw=true">
</p>


the top line of the grun menu automatically boots whatever `/sbin/init` points to ,  in this case runit.

and then login manager screen

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/antiX/antiX253.png?raw=true">
</p>


which is nearly the same as the previous live boot, onlty this is the installed artiX. 
It is the IceWM Window Manager.

The `/etc/` directory contains config files for all 4 init systems, and /sbin contains startup files for all 4 init systems, but the actual running init process is the default 'runit'

If I go to antiX Control Centre -> Choose Startup Services  I get the 'Runit Service Manager' 

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/antiX/antiX254.png?raw=true">
</p>


and you can see there are 10 services running.
If you wanted 'runit' , that `Normal Boot` install would be OK, but you can switch it to a different init system simply by altering a pointer

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/antiX/antiX255.png?raw=true">
</p>


That symbolic link called 'init' points to runit. If you alter it to point to eg) dinit and reboot, you will get antiX with dinit as the init system.
In the  'normal boot' option, grub will always boot with the init system that the `/sbin/init` pointer points to. 

Lets now see what happens if you choose a specific init system.

##### Installing with dinit init system #####
 I use the same antiX-25-beta1_x64-full.iso but chose 'Dinit' instead of 'Normal' . From the live screen I ran the installer, and rebooted. 
 On reboot, I see that process No 1 is /sbin/init and that /sbin/init is a pointer to dinit. So it is running dinit. Strange , I expected process No 1 to be 'dinit', but it shows the pointer?
Lets try one of the commands

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/antiX/antiX256.png?raw=true">
</p>


Yes, seatd is running  and dinit can check its status.
Unfortunately the Control Centre -> System-> Choose Startup Services menu item does not work for dinit. It works for runit. I guess this is on the 'todo' list.

One can always start and stop daemons using dinit commands from the CLI. We shall see an example below.

So there is not much difference from the previous 'Normal' install.  One still gets all init systems with their config files, only the default this time is dinit instead of runit. The grub menu now defaults to dinit.


##### Small things #####
 - the antiX FAQ still shows antiX23
 - there is no  GUI to Choose Startup Services for dinit or s6-66
 - in antiX the default value of $TERM is `xterm` . This does not work well with `vi`. The default should be `gnome-256color`. It can be set with `/usr/bin/env TERM=gnome-256color /bin/bash`
 - 

##### Bigger things #####
 -  it is indeed a brave move by antiX ditching sysVinit, which was the only fully working init system. It means they will have a whole raft of ex antiX23 users who are forced to tackle a new init system at a time when the 4 init systems on offer are only partially configured in relation to the package system. There needs to be some good getting started documentation.
 -  there is a choice ahead. Either go the way Artix did and introduce subsidiary packages for each service package to supply the required service files
or 
invent some way of making servoice packages detect which init system is running and install the appropriate service files with the service package ( the way sysVinit does)
or
invent some new way unique to antiX. 
I would bet on the latter.
 -  some explanation of how the installer and grub setup the multiple init systems , and how the grub menu works, would help beginners. I tried to cover that here. 

##### Next #####
We need to look at how to start a service in each of the 4 init systems.... 2 cases
 - when the service package is installed and there are service files present
 - when ther are no service files present 
