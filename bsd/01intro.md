## Introduction ##
In a previous post 

https://itsfoss.community/t/communication-between-host-and-guest-vm-when-using-virt-manager/10841

we looked at using a Linux host for virt-manager with a Linux guest in the VM. There are some extra issues to consider if the guest system is not Linux. In particular virt-manager default VM settings seem to be chosen to suit Linux, and there are filesystem considerations.

Here we look at one non-Linux system. FreeBSD is a direct descendant of Berkeley Unix, whereas Linux is a totally independent Unix-like development. They both arose in the early 1990's. FreeBSD is a complete operating system, consisting of a kernel and core Unix utilities. Linux is only a kernel. They both supplement their software repositories with contributed applications, particularly GNU.

FreeBSD can be as good a desktop system as Linux. It may take a little more CLI work to setup, but it is actually more efficient and less subject to code bloat and package dependency issues. Once you have a Desktop Environnment installed, you would barely notice any difference.

What we attempt to do here is to smooth out the issues encountered in using FreeBSD in a VM. A hard install of FreeBSD is actually easier, but many people would
want to try it in a VM first. 
