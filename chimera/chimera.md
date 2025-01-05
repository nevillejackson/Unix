#### Chimera Linux in virt-manager with Plasma and Wayland ####

Want to try something really different? 
Chimera Linux have just released new images as of 4/12/24.

https://chimera-linux.org/

Chimera is a new distro build from scratch using
 - Linux kernel 6.12
 - core tools from FreeBSD
 - apk package system from Alpine
 - Clang/LLVM toolchain
 - Musl C library
 - Gnome and Plasma desktops with Wayland &/or X11
 - multiple architectures - Intel/AMD, ARM AArch64, POWER, and RISC-V
 - the `dinit` init system
 - 
A real mix. One could debate whether it is Linux with a BSD userland or BSD with a Linux kernel. 
The real point is it is an independent distro like Solus and Void ... it does not belong to any family.

The downloads are available here

https://repo.chimera-linux.org/live/latest/

I used `chimera-linux-x86_64-LIVE-20241204-plasma.iso`

There is extensive documentation available here

https://chimera-linux.org/docs/

##### My install experience #####

The download was simple. Go to

https://repo.chimera-linux.org/live/latest/

and click on the image you want . I chose `chimera-linux-x86_64-LIVE-2024-plasma.iso`. 
There are also `gnome` and `base`.
Base is without any DE. If you do not want Gnome or Plasma, you can start with `base` and add any of `enlightenment`, `sway`, `wayfire`, `labwc`, `pekwm`, `icewm`, or `kde1`

I did a VM install using virt-manager. The only additional hardware needed ( over the default) was a USB Mouse, and USB keyboard. 
It looks as follows 

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeraplasma.png?raw=true">
</p>


That is a typical barebones KDE desktop.

Now the big shock!   There is no install script. You have to do it yourself using Linux CLI, but, the instructions in the help document found here

https://chimera-linux.org/docs/installation

are excellent. 

###### Install steps #####
Starting with the live system
 1. Open a terminal... it is called Konsole

 2. Partition the disk. In my case the virtual disk ( /dev/vda)
    I used the KDE Partiton Manager .... it looks like Gparted with a few 
    modifications. I made a GPT partition table and 3 partitions...
    EFI, bios-grub, and /.  

<p align-"center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimerapartitions.png?raw=true">
</p>

 3. Make a place in the live system to mount the install partition (vda3)
    and mount it.
```
mkdir /media/root
mount /dev/vda3 /media/root
```

 4. Also mount the EFI partiton
```
mkdir -p /media/root/boot/efi
mount /dev/vda1 /media/root/boot/efi
```

 5. Use the `chimera-bootstrap` program to copy all the system files to the install directory
```
chimera-bootstrap -l /media/root
```

 6. Chroot into the install directory
```
chimera-chroot /media/root
```

 7. Do an update. You have to use the `apk` package system
```
apk update
apk upgrade --available
```
It  used 14Mb of extra disk space.

 8. Make an /etc/fstab file. Chimera has a special program to do that
```
genfstab / >> /etc/fstab
```

 9. Make a new initramfs
```
update-initramfs -c -k all
```

 10. Add grub. I originally intended to do an UEFI boot so I did
```
apk add grub-x86_64-efi
```

 11. Install grub
```
mkdir -p /boot/efi
mount /dev/vda1 /boot/efi
grub-install --efi-directory=/boot/efi \
  --target=x86_64-efi
```
That failed ????  I think because I had virt-manager set to bios-boot.
So I change to
```
apk add grub-i386-pc
grub-install /dev/vda
```
That was OK

 12. Setup a user and change root password
```
useradd nevj
passwd nevj
usermod -a -G wheel,kvm nevj
passwd root
```

 13. Give it a hostname
```
echo chimeraplasma >> /etc/hostname
```

 14. Setup timezone
```
ln -sf /usr/share/zoneinfo/Australia/Sydney /etc/localtime
```

 15. Shutdown and reboot
```
reboot
```
Unfortunalely it does not boot, I get this
```
grub>
```
That means I forgot something..... after `grub-install` I should have done `update-grub` to construct the `grub.cfg` file.
So I have to figure out how to boot from the grub> prompt
First I need to know disk and partition names

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeragrub.png?raw=true">
</p>

So the root filesystem is, in grub notation, (hd0,gpt3) and in linux notation /dev/vda3. Now do this
```
grub> set root=(hd0,gpt3)
grub> linux /boot/vmlinuz=-6.12.5-0-generic root=/dev/vda3
grub> initrd /boot/initrd.img-6.12.5-0-generic
grub> boot
```
and it boots.
I get to a console prompt

<p align="center"> 
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeraconsoleprompt.png?raw=true">
</p> 


 16. Login at the console
Now the fun starts. It should look like this

<p align="center"> 
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeraconsolelogin.png?raw=true">
</p> 


but several times I have had to fiddle with virt-manager hardware configuration, adding or deleting usb-keyboard, to make it accept my login name and password.

 17. Start the DE
I have to look in /bin to find what to use to start Plasma

<p align="center"> 
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimerastartde.png?raw=true">
</p> 


The script I need is
```
/bin/startplasma-wayland
```
Run that  and I get the DE

<p align="center"> 
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeraplasmakonsole.png?raw=true">
</p> 


I have a DE with  Konsole terminal window.
So, the install is successful, I now have to configure.

###### Configuring Chimera ######
 1. Getting a network connection
There are no interfaces configured. The Konqueror browser is present but it cannot communicate with the internet. 
I need to find something to configure interfaces. There is nothing in the System settings menu. A search with `apk` gives

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeraapknet.png?raw=true">
</p>


So both `dhcpcd` and `NetworkManager` are present, but neither is running. 
I decide to use `dhcpcd`, so I have to use `dinit` to start the `dhcpcd` daemon
```
# dinitctl enable dhcpcd
Service 'dhcpcd' has been enabled.
Service 'dhcpcd' started.
#
```
Simple as that. I dont need to configure it.... `dhcpcd` finds the virtual machine gateway and sets up the link with dhcp.

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeranetworked.png?raw=true">
</p>


Now the browser works.

 2. Getting a login manager
The console login is a little inconvenient, especially when the VM hardware needs to be fiddled. I can avoid it by setting up a login manager.
The login manager `sddm` is already installed. 

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimerasddm.png?raw=true">
</p>


There seem to be several packages... all installed.
 `dinit` seems to be configured like in Artix.... a management package for
 each daemon ( service).
The Chimera documentation tells me to do this

```
# dinitctl enable sddm
```
and as soon as I do that, the sddm Login Manager screen appears.

chimerasddmscreen.png
<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimerasddmscreen.png?raw=true">
</p>


but when I then login, I get a blank display. If I CtrlC I get back to the login manager, and from there I can sutdown and reboot. Then after reboot I get the Login Manager screen and the login works.... I drop straight into Plasma.
Now it is starting to look like a normal distro.

 3. Communication.
I can ssh or sftp into the host. 
If I want to ssh into the Chimear VM I will need to  start `sshd`
That is easy 
```
# dinitctl enable sshd
```
That is all. The packages `openssh`  and `openssh-dinit` were already installed.

 4. Personalizing the Plasma screen
Under System Settings there is a Quick Settings menu where I can choose my favourite wallpaper and set a dark theme

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeraplasmacustom.png?raw=true">
</p>


and I can set my normal 'focus follows mouse' setting.

I have never done anything elaborate with Plasma, and I have never had Wayland before, so I cant really test it.  It is Plasma version 6.2.4 KDE Frameworks Version 6.9.0 Qt version 6.8.1.
The window manager is Kwin version 6.2.4

 5. Copy/paste
Hang on, I cant copy/paste in and out of the VM?
I need `spice-vdagent`  installed

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeraspiceinstall.png?raw=true">
</p>

OK, it is there now?

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeraspiceinstalled.png?raw=true">
</p>


I still need to start the daemon

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeraenablespice-vdagentd.png?raw=true">
</p>


after a reboot I get

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeraspicerunning.png?raw=true">
</p>


That looks right , but I still cant copy/paste in and out of the VM. 
I think spice may not work properly with Wayland.... it needs Xorg server. 
I install `xwayland` , and `xserver-xorg` .

Then I get a choice of DE's on the login screen

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimerax11.png?raw=true">
</p>


and if I choose Plasma(X11) copy/paste works, but if I choose Plasma(Wayland) it does not work. So spice-vdagent does not work with wayland yet, not even with xwayland installed..
The other choice (Weston) gives me a blank screen with a Wayland terminal and nothing else!

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeraweston.png?raw=true">
</p>


Copy/paste does not work there either.

There were lots of reboots testing the above, and I have to say Chimera is the fastest booting VM I have ever encountered.  Dinit is a fast simple init system. Shutown is less than 1 second, boot is about 3 seconds.


##### Notes on Chimera's mix of features #####
Why would anyone take core utilities from BSD, a package system from Alpine, the LLVM toolchain, the MUSL library ( alternative to glibc) and the almost unused 'dinit' init system, and integrate them into a distro using a modern Linux kernel?
There is only one answer. It is way too much work to be a fun topic or a challenge. It has to be because the Chimera team really believe each of the above choices is the best available. So why not make a distro that combines the best ov every world.
Let us look at each of these features and try and see why it might be considered superior.


######  The BSD core utilities ######
We are talking about the basic Unix CLI commands. Things like cat, chmod, chroot, cp, dd, du, df, echo, ln, ls, mv, pr, rm, sort, tee, uniq, wc, and lots more, plus larger programs like awk, grep, sed, tar that were in the original Unix. 
In BSD they come as part of the OS. In Linux  they are addon packages, usually from GNU. 

Let us look at the simple command 'cat' . Its Linux man page starts like this
```
NAME
       cat - concatenate files and print on the standard output

SYNOPSIS
       cat [OPTION]... [FILE]...

DESCRIPTION
       Concatenate FILE(s) to standard output.

       With no FILE, or when FILE is -, read standard input.

       -A, --show-all
              equivalent to -vET

       -b, --number-nonblank
              number nonempty output lines, overrides -n

       -e     equivalent to -vE

       -E, --show-ends
              display $ at end of each line

       -n, --number
              number all output lines

       -s, --squeeze-blank
              suppress repeated empty output lines

       -t     equivalent to -vT

      -T, --show-tabs
              display TAB characters as ^I

       -u     (ignored)

       -v, --show-nonprinting
              use ^ and M- notation, except for LFD and TAB

       --help display this help and exit

       --version
              output version information and exit

```
However the BSD version is like this

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/bsdmancat.png?raw=true">
</p>


First thing to notice.... the BSD version of cat has no long options with a double minus ( ie like --show-all).
There are some other subltle differences, not discernable from the man pages
 - BSD utilities are stable. The flags do not change over time
 - the BSD code is tidy and readable. GNU code is infamously  cluttered and hard to read.
 - the BSD licence is more permissive, there is no copyleft provision.

One does not have to go to Chimera or BSD to use the BSD utilities.  Debian has a package called 'bsdutils' .

https://packages.debian.org/sid/bsdutils 

So why did Chimera choose BSDutils?  I think the above points make it clear. 


###### APK package system ######
The acronym APK stands for 'Alpine Package Keeper'. Apk is used in Alpine Linux. Apk packages have the `.apk` filename extension, and this is not to be confused with Android packages, which also have the `.apk` filename extension.

Apk is documented here

https://wiki.alpinelinux.org/wiki/Alpine_Package_Keeper

and there are hints about using it here

https://docs.alpinelinux.org/user-handbook/0.1a/Working/apk.html

We have already used it in this document to do system updates, and to add the grun package.  It is easy to learn .... simpler than the familiar Debian apt system. 

There is a directory `/etc/apk`

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeraapk2.png?raw=true">
</p>


and the subdirectory repositories.d contains

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeraapk1.png?raw=true">
</p>


so Chimera has its own repository for `.apk` files. 
It also uses flatpaks, which are accessible via the KDE Discover app.

The file `world` contains a list of all installed packages

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeraapk3.png?raw=true">
</p>



The file 'arch' records the architecture. 
```
$cat arch
x86_64
$
```
Chimera is available for x86_64, ppc64, ppc64le, aarch64, riscv64 architectures.

The Chimera apk repository is dependent on Alpine. When Alpine has a new release, the repository path will change. You can edit `/etc/apt/repositories.d/00-repo-main.list` and run `apk upgrade --available` , to shift to a newer repo. 
Downgrading packages to older versions is not supported.

You can find what is available in Alpine packages here

https://pkgs.alpinelinux.org/packages

When it comes to using packages, Chimera is essentially the same as using Alpine. 




####### The Clang/LLVM toolchain #######
The Linux kernel included in Chimera is built with the Clang/LLVM toolchain and the MUSL libc library.  That makes it somewhat different from most Linux kernels which are built with gcc, binutils, and glibc.

More on kernel building here

https://docs.kernel.org/kbuild/llvm.html

We need to understand what these new compiler tools are and why they might be better than the traditional tools.

 - Clang is a new C compiler. It provides a C and C++ language frontend for LLVM. It supposedly supports all the extensions and features of gcc, while remaining fast and light.
 - LLVM is a whole set of compilers and toolchain programs. So it encompasses all the build tools used for the compile, link and execute steps in making a program run file.  It is more than that, it is designed to make programs portable across architectures, which it achieves by compiling to a standardized assembler language for a virtual machine.  It facilitates graphics code with OpenGL. It supports C, C++, Fortran, Julia, Rust, and several other languages. 

The best way to learn about this toolchain is to try compiling some user programs.

 
I have a small C program called `folli.c`
```
$ ls
Makefile  folli.c  folli.h  folli.scr
```
It consists of one `.c` file , a header file `folli.h`, a script to run it `folli.scr` and a Makefile to compile it.
The Makefile is simple
```
# Makefile for folli.c 
SRC = folli.c
INCLUDESRC = folli.h 
OBJ = folli.o
CC = gcc
HOMEDIR = .
BINDIR = .
SRCDIR = $(HOMEDIR)/Folli
MANDIR = .
LIBS =  -lm  
CFLAGS = -v  -g  -static

folli:	$(OBJ) 
	gcc  -o folli  $(OBJ)  $(LIBS) 
test:	folli.scr
	sh -ex folli.scr >&out
clean:
	rm *.o 
folli.o :      folli.h 
```
The above is as it stands for a compile in Void Linux ( or most GNU/Linuxes) .
It will need modification in Chimera.

So lets see if we can find the tools in Chimera. 

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeratools.png?raw=true">
</p>


Nothing there!. No make, no compiler,....
So it does not install with build tools by default.
We will have to find and install them with `apk`
Search for make

chimeramake.png
<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeramake.png?raw=true">
</p>


So it is available but not  installed ... `bmake` is the BSD version `gmake` is the GNU version.
So lets try bmake

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimerabmake.png?raw=true">
</p>


Now we need to clean up the makefile

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeramakefile.png?raw=true">
</p>


and try to run it

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimerabmake2.png?raw=true">
</p>

 
So, obviously, I needto install clang

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeraclang.png?raw=true">
</p>


notice it installs `llvm-binutils` and `musl-devl` as well 
Now try again

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimerafolli1.png?raw=true">
</p>


Clearly clang wants a type specifier in the C code. In Void Linux with gcc you get a warning. Clang is more strict.
o I add the `int` type to the `main()` statement in the C code, and

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimerafollicompile.png?raw=true">
</p>


That is a successful compile.  `folli.o` , the object file is present, and there is an executable called `folli`.
Lets see if it runs

```
$ sh ./folli.scr
 number of primary sites 2000000
 So/P ratio   3.00
 growthrate - slope of log_wt/log_age line  4.30345
 growth intercept - of log_wt/log_age line 0.0000020170
 follicle initiation rate - increase per timeincrement per cm sq 450.0000
 time of start of primary follicle initiation period       64
 time of start of secondary original follicle initiation period       86
 number of founder cells at time zero 100000000.0
 average number of cells per p,so,sd follicle  35.000   33.000   30.000 


 time     foll  pfoll   sofoll   sdfoll diffoundcel  foundcel  wght surfarea folirate celbrate    d    pd   sod   sdd    sd  rat
between P and So periods
 61         0        0         0         0           0   107054834     97.2    190.3    85633  0.07055      0     0      0      0      0   0.0
 between P and So periods

.... snip  .....

299  27069651  2171768   6543389  18354494   842578546           0  90914.0  18196.9  8188583  0.01439   1488   119    360   1009   1368  11.5
  out  of founder cells 
300  27069651  2171768   6543389  18354494   842578546           0  92229.7  18372.0  8267400  0.01434   1473   118    356    999   1355  11.5
  adult S+P density per cm sq =     2572
 adult P density per cm sq =       206
 adult So density per cm sq =      622
 adult Sd density per cm sq =     1744
 adult S density per cm sq =     2365
 adult S/P ratio =     11.5
 p interval = 14
 so interval = 18
$ 
```
 It runs successfully, the output is correct. What it is doing is modelling the development of wool follicles.  

The executable file is an ELF file

```
$ file folli
folli: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib/ld-musl-x86_64.so.1, BuildID[sha1]=bc6367d94b8572a9fdeadf1628fc9b6e389b764a, with debug_info, not stripped
$ 
$ ls -l folli
-rwxr-xr-x  1 nevj nevj 17752 Jan  1 16:32 folli

```

Slightly different from the one made in Void with the GNU compiler toolchain

```

$ file folli
folli: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=a80de8ce5929bfd25492ab505c8d1277b5c44b11, for GNU/Linux 3.2.0, with debug_info, not stripped

ls -l ~/Folli/folli
-rwxr-xr-x 1 nevj nevj 28344 Dec 31 21:29 /home/nevj/Folli/folli
```

Notice it is dynamically linked to MUSL libraries.
Notice also the executable file produced with `clang` is dramatically smaller than that produced with `gcc`?
That is strange....the object files (folli.o) are similar in size
```
gcc
ls -l ~/Folli/folli.o
-rw-r--r-- 1 nevj nevj 15992 Dec 31 21:29 /home/nevj/Folli/folli.o

clang
ls -l folli.o
-rw-r--r--  1 nevj nevj 18544 Jan  1 16:32 folli.o
```
I looked at them with `nm` , they have the same symbols, but the clang one has some extra symbols. 

That is on very elementary look at Clang/LLVM/musl. I must go on and try a more complicated build. I need to also try Fortran under LLVM. But not here.

There is not much to say about MUSL. It is documented here

https://musl.libc.org/

It has static linking, which is important in embedded applications. Both muls and glibc are implementations of the standard C library.



###### The `dinit` init system ######
There is an easy introduction to `dinit` in the Chimera documentation

https://chimera-linux.org/docs/configuration/services

The web page is here

https://davmac.org/projects/dinit/

and the github page

https://github.com/davmac314/dinit

also provides good documentation.

Dinit provides an init process, service management, and supervision. It can launch multiple services in parallel, and that makes it fast. Boot time for Chimera in my VM is about 3 seconds. It is written in C++ and is compatable across BSD, MasOS, and Linux.

Dinit is considered to be part of the `daemontools` family of init systems ( daemontools, runit, s6, 66, dinit), but unlike the others it does not start a separate process to supervice each daemon. There is one supervision process called `dinit-monitor` that monitors all services.  there is also one process called `dinit` that replaces init as process No. 1. , and manages shutdowns and reboots. As such, it is really the simplest of all init systems except BSD RC. 


Dinit is easier to use than runit. Its service management commands are similar to OpenRC. It is a small learning curve ... half an hour will get you there.


##### Problems #####
 1. Sometimes if I click on the Menu button in the bottom panel, I get a completely blank background

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/chimera/chimeraproblem.png?raw=true">
</p>


and the only way out is a shutdown and reboot. It happens with both X11 and Wayland. I am not sure if this is a VM issue or a Chimera issue.

Further searching reveals that it is an X11 issue.  The problem only appeared in Wayland after I added the xwayland package. So I removed the xwayland package, and the problem disappears in Wayland/Plasma, but it is still there in X11/Plasma.

So I went looking for parts of X11 not installed and found the package
`xserver-xorg-xephyr` which is concerened with "X server outputting to a window on a pre-existing display". Installing that package fixed the problem in X11/Plasma.

So now I went and reinstalled the `xwayland` package  and Wayland/Plasm still works ( no blank screen on touching menu button)
So I think the problem is solved. It was caused by starting with the Plasma/Wayland, then adding X11 and forgetting to add needed components. 

 2. Spice-vdagent does not seem to work in Plasma/Wayland, but it works in Plasma/X11.
  In both Wayland and X11, the required daemons are running
```
$ ps ax | grep spice
  499 ?        Ssl    0:00 /usr/bin/spice-vdagentd -x
  969 ?        Ssl    0:00 /usr/bin/spice-vdagent
```
I suspect that vdagent does not communicate properly with Wayland, because Wayland has no server daemon for it to communicate with. Even the presence of `xwayland` is not enough for vdagent to work in PLasma/Wayland.
This i a `spice-vdagent` problem.  It would happen with any Wayland DE. Not unique to Chimera.

##### Conclusion #####

My overall feeling is that Chimera is a well planned attempt to create a distro that uses the best bits and pieces from a number of areas.
In particular the `apk` package system is at least as good as Debian's `apt` and is less cluttered, and the `dinit` init system is fast and simple.

The Clang/LLVM/MUSL toolchain worked for me as a user, and there does not seem to be anything wrong with the Linux kernel compiled with it.

I have no idea what the driver situation is, because I used a VM, but it is a Linux kernel so it should have all the driver modules every Linux kernel has. 

The big hurdle for new users is the absence of an install script.  You just use commands in the live Chimera. The documentation is great. Anyone could follow the steps, and Chimera help by providing a couple of critical utilities
`chimera-bootstrap` , `chimera-chroot` and `genfstab`. Dont be scared off by the install.

Chimera uses musl libc. That means that proprietary apps that require glibc will not work as package installs or downloaded binaries. They will work if containerized ( eg flatpaks). The benefit of MUSL is cleaner code and a better toolchain for developers.

Chimera is itself available as a container image `chimeralinux/chimera` in Docker Hub. It should make a nice lightweight container image, similar to AlpineLinux. I have not tested the container image.

Chimera only started in 2021, and it did not reach an Alpha Release until 2023. This current release is a Beta.  Give it a chance. It is on the way to becoming a mature, independent distro with an interesting combination of sources. 

