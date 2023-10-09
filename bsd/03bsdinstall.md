## Installing FreeBSD in a virt-manager virtual machine. ##

FreeBSD comes with numbered fixed releases. The present release is called 
FreeBSD-13-STABLE, and there have been minor releases following that called 
FreeBSD 13.0, 13.1, and 13.2. We chose to use FreeBSD 13.2-RELEASE.

There is also FreeBSD 14-CURRENT which is bleeding edge and rolling release.
FreeBSD-13-STABLE is a semi-fixed release model,  and the minor releases are 
simple fixed releases. 

An important consideration in installing BSD is choice of filesystem. The 
traditional FreeBSD filesystem has always been Unix File System (UFS) which has been extended to UFS2. The other native filesystem ( ie suited to use as the root filesystem) is Z File System (ZFS). We chose to use UFS2.

Other filesystems ( eg ext2/3/4,...) have support in the FreeBSD kernel, but should not be used for the root filesystem. 

The official FreeBSD install instructions are here

https://docs.freebsd.org/en/books/handbook/bsdinstall/

One can install from a .img file or a .iso file. We chose to use

`FreeBSD-13.2-RELEASE-amd64-disc1.iso`

which is a CD sized .iso containing the basic system, and relying on network downloads for additional material such as Xorg, and a DE.

Virt-manager can install directly from an `.iso` file, so we pointed a New Virtual Machine to the `.iso` file, setup 40Gb of file space for the `.qcow2` file, left all the virt-manager settings at their default values, and told it to boot the `.iso`.

It starts with a menu which offers the choice of Install, Shell, or Live System. We chose Install. 

After that follow a series of dialog windows, much like a non-graphical Debian install.  The full install process is described in the FreeBSD Handbook, Chapter 2

https://docs.freebsd.org/en/books/handbook/bsdinstall/

There follows a litany of familiar steps
 - keymap
 - set hostname
 - choose optional components - we accepted the default components and added ports
 - networking
 - disk space - we chose auto UFS. The other choices are auto ZFS and Manual.
 - partitioning  - we used guided partitioning and alowed it to use the entire virtual disk. We chose to use a GPT partition table. It made a freebsd-boot partition, a freebsd-ufs '/' partition and a freebsd-swap partition.
 - fetching distribution files. It downloads selected components - base.txz, kernel.txz, lib32.txz, portx.txz
 - root passwd, NIC's, choose a mirror, set time zone, 
 - enable services. We chose sshd, moused, ntpd, dumpdev
 - hardening options. We ignored
 - Users
 - Final check, we added Install FreeBSD Handbook, then let it install
 - Reboot

It boots and you get a login console on the `virt-manager` screen. 
There are no issues. The default `virt-manager` settings work. One can login and work with the CLI on the console screen. 

That is all there is to FreeBSD.  The kernel is in `/boot/kernel/kernel` and the Berkeley Unix core utilities are in `/bin`  and `/usr/bin`. Any packages added later, will go in `/usr/local`.

There is no X window system, and no desktop environment. We shall see in later sections how to add those.

If you look in the `/etc` directory, you may find a file `rc.conf`.

```
cat /etc/rc.conf
hostname="freebsd132vm"
ifconfig_vnet0="DHCP"
ifconfig_vtnet0_ipv6="inet6 accept_rtadv"
sshd_enable="YES"
moused_enable="YES"
ntpd_enable="YES"
# Set dumpdev to "AUTO" to enable crash dumps. "NO" to disable
dumpdev="AUTO"
```
 FreeBSD uses the RC method  to start services. It has a process called `init`, which is the BSD `init` command, but all that it does is run the `/etc/rc` script. The whole init system is controlled by `rc` scripts.  There is no systemd, or OpenRC, or Runit or SysVinit.... no init system at all.  The whole startup process is controlled by a script, which the user can modify.

One command you may need to use is `ifconfig`. FreeBSD does not have `ip`...`ifconfig` is its predecessor. It will tell you the IP number of the `vtnet0` interface which is the bridge to the host system.

FreeBSD comes with the original BSD `vi` editor ( not vim). It also has `ee` (Easy Editor) which is like nano. One can, of course, add other editors as packages.


