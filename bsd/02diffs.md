## Linux and BSD: differences and similarities 

### Some differences


FreeBSD and Linux are both Unix-like operating systems, but they have some key differences. Here's a general overview of the main differences between BSD (specifically FreeBSD) and Linux. The list is not meant to be exhaustive by any means.

       
1. *Licensing*:

    FreeBSD uses the BSD license, which is rather permissive.
    Linux uses the GNU General Public License (GPL), which seems to have more stringent requirements for sharing modifications and distributing software.   
        
2. *Userland*:

    FreeBSD's userland tools often come from the FreeBSD project itself or are adapted from other BSD systems.
    Linux distributions can vary significantly in terms of userland tools.
        E.g.: Popular Linux distributions like Ubuntu use the GNU userland tools.    
        
3. *Package Management*:

    FreeBSD uses the Ports Collection and the pkg package manager for software installation and management.
    Linux distributions have different package management systems like APT (Debian/Ubuntu), RPM (Red Hat/CentOS), and others.
        
4. *Filesystem*:

    FreeBSD supports the UFS (Unix File System) and ZFS (Zettabyte File System) as its primary filesystems.
    Linux has a wider range of filesystem options, including ext4, XFS, Btrfs, and also supports ZFS. 
        
5. *Jails vs. Containers*:

    FreeBSD uses a feature called "Jails" for OS-level virtualization and isolation.
    Linux has a variety of containerization technologies, e.g with Docker, LXC (Linux Containers).   
        
6. *Community and Development*:

    FreeBSD has a more centralized development model, with a Core Team overseeing the project.
    Linux has a more distributed development model, with many different organizations and individuals contributing to the kernel and distributions.
        
7. *Hardware Support*:

    Linux typically has broader hardware support, thanks to its large developer and user base.
    FreeBSD has good support for a wide range of hardware, but it may not support bleeding-edge hardware as quickly as Linux. 
        
8. *additional remarks*:

    The whole FreeBSD base system is located in the Unix filesystem in '/' and '/usr'. All extra installed packages go into '/usr/local'. 	Thus BSD keeps the base system separate in the file system.
   
    Linux does not do this. Added packages in Linux go into /usr mixed in with the base system.    
    /usr/local in Linux is used for software added manually by the user, outside of the package system.        
     Thus Linux separates what the package system controls, BSD separates both the base OS and the packages. 	        
        
        
### Some similarities


There are also some areas where BSD and Linux share similarities or have common features.
Here are some key points.

1. *Unix-like Architecture*:     

 Both BSD and Linux are Unix-like operating systems, which means they are similar with respect to the principles of the Unix operating system.
 Some examples: use of shells, filesystems, and a variety of core utilities.
    
2. *Kernel*:

  Both Linux and BSD kernels are monolithic.
  Linux allows for various kernel modules and customizations.   
  The two kernels are totally different code, but behave very much the same.    
        
3. *modules in FreeBSD kernel*:

 FreeBSD kernel does support modules, and they can be added dynamically  .  
 Instead on `modprobe` to load a module, FreeeBSD has the command `kldload`.        
    	
4. *Multi-User and Multi-Tasking*:

   Both BSD and Linux support multi-user and multi-tasking environments so that multiple users can run processes at the same time. 	
     
5. *Command Line Interface (CLI)*:

  Both provide a command-line interface, which is important for system administration and automation tasks.
		
6. *Open Source*: 

  BSD and Linux are both open-source operating systems, which means their source code is freely available for modification and (re-)distribution.	
    		
7. *Networking*: 

  Both systems are commonly used as network servers and routers. They support standard networking protocols and services.
    	
8. *Package Management*: 

  While the package management systems differ between BSD and various Linux distributions, they both have package managers for software installation and updates. FreeBSD uses the Ports Collection and pkg, while Linux distributions use package managers like APT, RPM,YUM, or DNF. 	  

9. *Security Features*: 

  BSD and Linux share many security features, such as user and group permissions and firewall capabilities.
    	
10. *POSIX Compliance*:

  Both share the POSIX (Portable Operating System Interface for Unix) standards, which provides compatibility with many Unix-based software applications.
     	
11. *Virtualization*: 

  Both support various virtualization technologies, including KVM/QEMU, VirtualBox, and others for hosting virtual machines.  
    

    	
    	
### Further reading

* https://www.zenarmor.com/docs/linux-tutorials/freebsd-vs-linux

* https://www.howtogeek.com/190773/htg-explains-whats-the-difference-between-linux-and-bsd/

* https://www.fosslinux.com/43260/linux-vs-bsd.htm

* https://helpdeskgeek.com/linux-tips/bsd-vs-linux-the-basic-differences/

* https://en.wikipedia.org/wiki/Unix-like

* https://docs.freebsd.org/en/books/handbook/kernelconfig/
