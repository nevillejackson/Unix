### Making a  nostalgic configuration of NetBSD in virt-manager ###

I grew up with Berkeley Unix ( 4.2BSD on a Motorola 68010 processor in a mini-computer).
This is a bit of a nostalgia trip. The nearest thing I can find to that today on a PC is NetBSD. The oldest PC Unix is 386BSD. It can be found in archives, but is not distributed today. So NetBSD is the oldest surviving PC Unix, closlely followed by FreeBSD, (but  FreeBSD has morphed). NetBSD is the closest thing to original Berkeley Unix.

NetBSD downloads are available here

```
https://www.netbsd.org/
```

It is available for an amazing range of hardware.


#### Install ####

I used `NetBSD-10.0-amd64.iso` and installed it in virt-manager (QEMU/KVM)a running in a Void Linux host.

The install is a simple stepwise process with dialog screens. You can see all the steps here

```
https://www.netbsd.org/docs/guide/en/chap-exinst.html#exinst-system-configuration
```

I did a full install to a virtual disk, but I neglected to configure the network in the configuration menu. That led to some difficulties later on . 
The install is fast, and when finished you reboot and get a display like the following


<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd1.png?raw=true">
</p>


Choose [1] and it boots to a console login

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd2.png?raw=true">
</p>


Then login and you get a shell prompt

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd3.png?raw=true">
</p>


That is an `sh` shell prompt. The other options are `csh` and `zsh`.  That is right, no `bash` although `sh` is a good approximation.  For a real Berkekely experience I should be using 'csh' as the interactive shell.

There is an X window system, called CTWM, which is a modified TWM. You can start it with `startx`

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd4.png?raw=true">
</p>


So you get a terminal window ( a uxterm with no scrollbar) and there are 4 workspaces ( lower right). There is a menu that comes with a right click on background.

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd5.png?raw=true">
</p>


That is it, fairly basic, no taskbar or icons. I can work with that.


#### Configure network ####

Now lets see if we can communicate with it. 
First shock... it does not have `ip` command.... it uses `ifconfig`

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd6.png?raw=true">
</p>


That interface `vioif0` is the bridge to the virt-manager host. 

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd7.png?raw=true">
</p>


I chose the `virti0` device model in `virt-manager`  for the NIC . The default `e1000` did not work with NetBSD for me.

Notice that `vioif0` has no IP address. Most Linuxes installed in virt-manager automatically setup an IP address for the virtual machine... my NetBSD does not.... because I did not ask for network setup at install time. 
So, having neglected to do it at install time, I have to now do it by hand.


It needs to have dhcpcd running. It is there 
```
$which dhcpcd
/sbin/dhcpcd
```
but not running
```
ps ax | grep dhcp
nothing
```
So, how do I start a daemon in NetBSD? 

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd8.png?raw=true">
</p>


That starts dhcpcd for the current session. Notice `vioif0` now has an IP number 192.168.122.160. I can ping the host.

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd9.png?raw=true">
</p>


 To make it start at boot I have to make some settings in `/etc/rc.conf`
```
ifconfig_vioif0="up"
dhcpcd=YES
```

Now, reboot and test

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd10.png?raw=true">
</p>


OK, the interface is up and has an IP number.
Check the routing with `route show`

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd11.png?raw=true">
</p>


All OK. 

For those who might want to bring the interface up with a static IP the commandds are
```
ifconfig vioif0 inet 192.168.122.99 submask 255.255.255.0 
route add default 192.168.122.1
```
I dont recommend it. 
Learn from my oversight.... when you install NetBSD configure the network at install time. 

#### Please can I have a scrollbar on my Xterm ####
My original access to Berkekey Unix was an alphanumeric terminal... ie a CRT screen that displayed characters , but not graphics. So, no Window Manager, only one window which displayed the last 24 lines.

So today's NetBSD with a Window Manager is a luxury by comparison... I can have multiple windows, multiple workspaces, the only thing I miss is the Xterm does not have a scrollbar.
That is easily fixed. 
The file that controls Xterm layout is called `.Xresources`.  It needs to be in your home directory.
I have a generic .Xresources file . It has a lot of lines about colours , followed by

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd12.png?raw=true">
</p>


Put that in the home directory and reboot. 
You can see the scrollbar on the right
It slides with middle button and steps up with right button or down with left button. It keeps 4096 lines. 
I can live with that.

#### What is the filesystem like ####
NetBSD uses by default the Berkeley Fast File System (FFS)

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd12a.png?raw=true">
</p>


FFS is basis of Linux ext filesystems. Linux can read files from an FFS filesystem if it has kernel modules supporting UFS. UFS is the modern name for FFS.

Lets have a look at my root filesystem

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd13.png?raw=true">
</p>


First notable difference there is no boot directory. The NetBSD binary is called `netbsd`, and it is in the root directory.. It is 28Mb.  Not large .  By comparison Void Linux is 124Mb for the initramfs and 13Mb for vmlinuz. BSD does not use an initramfs. Its boot mechanism is simpler. 

There is a file `boot` which is binary, and another file `boot.cfg`

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd14.png?raw=true">
</p>


which seems to be instructions for grub. ( Like /etc/default/grub in Linux)

There is another directory /kern 

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd15.png?raw=true">
</p>


which seems to contain kernel-relevant stuff that would normally be in /etc

Otherwise , the structure seems to be fairly standard, and would be familiar to Linux users.  The X11 code is in `/usr/X11R7`. 
The search path is a little unusual


<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd16.png?raw=true">
</p>


That PATH definition is in `~/.profile`

In `/usr/mdec` there is a collection of boot files for both efi and mbr. 



#### The package system ####
The original Berkekey Unix did not have a package system. You got what came with the distribution. There was internet, but it only supported email and ftp. So one could download source code ( Fortran or C usually) from  sites like Netlib, and compile it , like in Gentoo today only without the help of portage.


So  packaging in NetBSD is not part of my nostalgic journey. It is newfangled stuff. 
The NetBSD package installing software is described here


```
https://www.netbsd.org/docs/guide/en/chap-boot.html#chap-boot-pkgsrc
```

There are 19980 packages available as binaries. 
You can also build packages from source.

```
https://www.netbsd.org/docs/pkgsrc/using.html
```

The package facility is called `pkgsrc`, but that is not a command.... the commands are `pkg_add`, `pkg_delete`, `pkg_info`, ... 


##### Using binary packages #####
One has to tell NetBSD where the package server is. In a Debian-based Linux you would define that in `/etc/apt/sources.list`.  NetBSD does it differently. It defines an environment variable called `PKG_PATH`

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd17.png?raw=true">
</p>


Well, I followed the book, but that did not work

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd18.png?raw=true">
</p>


So I can ping the site. What could be wrong? Have a look with the browser

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd19.png?raw=true">
</p>


Oh, stupid me, those names in capitals `OPSYS/ARCH/VERSIONS` are generics. I need `NetBSD/x86_64/10.0`.

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd20.png?raw=true">
</p>


Well that is progress.  It found the site. Packages are .tgz files.  Looks like I need to do it as root , or maybe make some directories , like `usr/pkg`

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd21.png?raw=true">
</p>


OK, it works as root. 
So NetBSD is like FreeBSD, in that it keeps added packages in a separate directory. FreeBSD uses `/usr/local` but NetBSD uses `/usr/pkg`. The merit of this is that packages do not interfere with system upgrades. 

`pkgin` is the recommended tool for updating packages, and can also be used for searching

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd22.png?raw=true">
</p>


Yes, NetBSD has the package `a2ps` used for print formatting. It is not installed. 

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd23.png?raw=true">
</p>


Quite a bit of fuss, but it did the install. 

To delete a package `pkgin remove PKGNAME` or `pkg_delete PKGNAME`.
Version control seems to be a matter of setting the PKG_PATH environment variable.

One last thing, the definition of `PKG_PATH` needs to be added to the file `~/.profile`, for each user including root, so that it is defined at each login. 

##### Building packages from source code #####
If you build a package from source, you can compile code and install it the same way you would for a program written by yourself. There are no restrictions. 

Most distros provide a procedure for dealing with source code packages in a standardised way. In NetBSD the build utility for source code is called `pkgsrc`. 

```
https://www.netbsd.org/docs/software/packages.html
```

It supports most BSD distros, and Linux, ( ie you can use pkgsrc in distros other than NetBSD). It is different to the Gentoo portage system or the FreeBSD ports system.

To get pkgsrc for the first time one can download a tar file with ftp and unpack it in the /usr directory.

```
# ftp ftp://ftp.NetBSD.org/pub/pkgsrc/pkgsrc-2025Q2/pkgsrc.tar.gz
#
# tar -xvf pkgsrc.tar.gz -C /usr
```
This creates a directory called `/usr/pkgsrc`, which is where all the source code of packages will be stored. The ftp download took some time.

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd24.png?raw=true">
</p>


What you get in `/usr/pkgsrc` is a huge directory structure with almost nothing in it except a few manifest files. You dont get any code until you compile a package from source.  The `pkgsrc` directory can be updated by ftp-ing a new tar file or by using CVS.
```
# cd /usr/pkgsrc
# cvs update -dP
```

Now , lets try and build a package from source. 
You can browse the online package index here

```
https://cdn.netbsd.org/pub/pkgsrc/packages/NetBSD/x86_64/10.0_2025Q2/All/
```

We shall try to build `7-zip-24.09.tgz `
Go to the `7-zip` subdirectory in `pkgsrc`
and type 'make'
followed by `make install`

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd25.png?raw=true">
</p>


Well I did not know that would interfere, try again

```
# unset PKG_PATH
# make
```
I get pages of configure and compiler output... it hunts down all the dependencies, including `make` itself. It ends like this

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd26.png?raw=true">
</p>


It says it installed the binary in `/usr/pkg/bin`.  I cant find it there? 
Do some searching

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd27.png?raw=true">
</p>


Looks like it renamed the binary `7zz`.  I cant be sure, there is no man page for either `7-zip` or `7zz`? 
I can check my looking at the Makefile 

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd28.png?raw=true">
</p>


Yes, it installs the binary to `/usr/pkg/bin/7zz`.  Confusing?
If I type `7zz` I get a 7-zip usage printout. That confirms it.

After finishing one can do
```
# make clean
# make clean-depends
```
to tidy up leftovers from the compilation.


There is more to `pkgsrc`. It can be configured.
The config file is `/etc/mk.conf`. It is not present in the system as installed. It allows setting the compiler name, compiler flags, linker flags, and package specific build options.  A bit like USE flags in Gentoo.


#### Updating the system ####
System updates are separate from the package system. The documentation is here

```
https://www.netbsd.org/docs/guide/en/chap-upgrading.html
```

There are several methods of doing a binary upgrade
 -  The `sysinst` method.  You start with a .iso file and invoke the installer, but choose `Upgrade NeBSD` instread of install. It basically does all the install steps except partitioning.
 - The `sysupgrade` method. This does an inline upgrade of a running system. It requires intervention only for changes to config files.

I have NetBSD-10.0. We are going to try an inline upgrade to NetBSD-10.1


The program sysupgrade is in `pkgsrc/sysutils/sysupgrade`.  It needs to be installed, so another source code install.

```
# cd /usr/pkgsrc/sysutils/sysupgrade
# unset PKG_PATH
# make
.... lots of compiler output
# make install
# which sysupgrade
/usr/pkg/sbin/sysupgrade
```

Now  I reboot, do a backup, then use the `sysupgrade` command

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd29.png?raw=true">
</p>


Lots of output, including some questions about overwriting config files.

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd30.png?raw=true">
</p>



There is some problem with postinstall checks

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd31.png?raw=true">
</p>


It seems all fixes passed. So reboot and see if it works.

It boots and I can login. 
I had better change `PKG_PATH` in `.profile` to 10.1
and, test the binary package system.... yes it still works. 
I had better set a hostname
```
# hostname -s netbsdvm
```
The network connection still works.
I think that was a successful upgrade. 

##### Finding packages #####
The list of packages available as binaries ( for my system) is here

```
https://cdn.netbsd.org/pub/pkgsrc/packages/NetBSD/x86_64/10.0_2025Q2/All/
```

or it can be searched with `pkgin`

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd32.png?raw=true">
</p>


The list of packages available as source ( for my system) is here

```
https://cdn.netbsd.org/pub/pkgsrc/packages/NetBSD/x86_64/10.0_2025Q2/All/
```

or one can search the directory tree in `/usr/pkgsrc`.
The latter list seems to contain all the packages also available as binaries. 
Things like firefox browser, thunderbird mailer and xfce DE are  available . 
I am happy with CTWM nostalgic WM.


##### Mail in BSD #####
Of course, one can install a modern mail agent like Thunderbird. 
What I am interested in is the original BSD `mail` program
It is present, but requires some configuration
 - a hostname has to be set
 - directories need to be added to `/var/spool/postfix`
   specifically `maildrop` and `public` and their ownership and group needs to be as shown

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd33.png?raw=true">
</p>

 - the `postfix` service needs to be running

Given that, I can do
 
<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd34.png?raw=true">
</p>


You terminate the typed message with `Ctrl D`. When you say `mail` it displays all unread messages. You can step through them with `+` and `-`. 
You can pipe a message to `mail` from another program, it is compartable with all text processing utilities. 

That is internal mail. You can also configure it to receive network mail.

I worked with `mail` for years. I believe it is still around in Linux, but have never tried. It was just interesting to go back to it, in the present context. 


##### Virtual Memory #####
Unix, including SysVUnix and BSD, was one of the first operating systems to implement virtual memory ... the system of dividing physical memory into pages and swapping pages between real memory and disk swap space, while  all the time making it look to the user as if they had a large continuous slab of memory. 

We take it for granted today, but it made multi-user  systems feasable and allowed programs requiring large amounts of memory to run.

BSD still has the `vmstat` utility which shows how much memory is in use and whether there is pageing activity.  Some Linuxes still have it. 

My NetBSD in VM has 4Gb of emulated physical ram and a swap space. Lets see if we can make it swap
```
cd /usr/pkgsrc/www/netscape
unset PKG_PATH
make
```
Compiling Netscape may make it swap

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netscape34.png?raw=true">
</p>


Well it has page faults but no 'pi' or 'po'.  So it is accessing pages already in memory, but not swapping them to disk.  It has a lot of free memory. I thought compiling would take more than that?


The package is 'netscape-20040123nb4' . Netscape discontinued in 2007, so this must be an old package.
It seems to be downloading firefox 138.0.4? Must be some dependency there.

I reduced it to 1Gb of ram. 

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd35.png?raw=true">
</p>


Lots of page faults, lots of pages freed ( it is discarding pages from finished processes) , 5 reactivated pages, but still no swapping.

Reduce it to 128Mb of ram.

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/netbsd/netbsd36.png?raw=true">
</p>


At last, some real pageing activity, the 'pi' and 'po' columns are active. 
It is amazing how little ram NetBSD will run in. 
I remember having a 486 with 128Mb of ram , and it ran FreeBSD-1.x quite happily, including TWM and Netscape.  I thought a modern BSD might need more ram than that.

I will increase the ram now, and let it finish compiling Netscape.


##### The Netscape browser #####
Why Netscape? Well, this is a nostalgia trip. Netscape is the first browser I ever used, and I used it in FreeBSD and SunOS in the 1990's. Mozilla Firefox is a descendent of Netscape Navigator. 

I abandoned the compile and installed the binary
```
#pkgin install netscape-200401234nb4
```
It installed 52 packages ( including firefox-138.04).... lots of dependencies.
It was a total letdown... it ran firefox instead. Looks nothing like the original Netscape. 


##### Discussion #####
This is not a review of what NetBSD can do today.... it is an attempt  to wind NetBSD back to what I remember BSD was like in the 1990's. 
So dont judge NetBSD by what you see here.  It is a capable modern OS. 

I might try and have a look at modern NetBSD, but that is another topic.

What did I achieve from my nostalgic windback?  Apart for some personal satisfaction at seeing old things, there is something to be gained from the contrast. 
It was more difficult to configure things in early BSD. 

I also learnt how to drive NetBSD... it was an effective way of doing that. The package system is different to any other Linux/BSD and takes some practice to get things working. The update system seems very efficient.   The package and update systems are separate... not like Debian where `apt` does it all. That is a BSD feature ( FreeBSD is the same). It is because BSD keeps the system away from package installs.  BSD treats the utilities as part of the OS, while Linux treats utilities as just another addon package. 

I have no intention of using my nostalgic creation. It was the journey, not the end point , that meant something to me.  
