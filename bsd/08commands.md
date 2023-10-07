## Some Basic FreeBSD Commands


The following is aimed to be an introductory overview of the most important commands in FreeBSD

It should provide a starting point for users new to the system. Here are some key commands that cover system updates, version upgrades, package management, and kernel module information:


1. **Updating FreeBSD**

... involves using the freebsd-update utility to apply security patches and system updates. Here are the steps to update FreeBSD from the command line:

`sudo freebsd-update fetch` # This command fetches the update metadata from the FreeBSD update servers

`sudo freebsd-update install` # Once the metadata is fetched, you can apply the updates. This command will download and install any available security patches and updates.

`sudo reboot` # if necessary. After the updates are installed, it's a good practice to reboot the system to ensure that any kernel updates take effect.


2. **Upgrading FreeBSD to a more recent release**

	| task | command |
	| --------------- | --------------- |
	| 1. Check your current version | `sudo uname -mrs` |
	| 2. fetch the updates available | `sudo freebsd-update fetch` |
	| 3. apply all the fixes | `sudo freebsd-update install` |
	| 4. upgrade packages | `sudo pkg-static upgrade` |
	| 5. reboot | `sudo shutdown -r now` |
	| 6. upgrade to new release | `sudo freebsd-update -r 12.0-RELEASE upgrade` (# example)	|
	| 7. final step | `sudo freebsd-update install` (# *you may have to run freebsd-update install again at the end*) |
	| 8. reboot | `shutdown -r now`	|
	| 9. upgrade the packages | `sudo pkg-static upgrade -f` |
	

3. **Further Commands**

*general remark*:

`pkg`: the package manager for FreeBSD. It's used to manage software packages on the system, including installing, updating, and removing packages.


| task | example command |
| --------------- | --------------- |
| check if a package is installed |`pkg info -l snort` |
| install a package |`pkg install snort` |
| search for a package | `pkg search snort` |
| detailed package information |`pkg search -f snort` |
| ... also package information | `pkg search -S name --exact w3m` |
| remove a package | `pkg remove snort` |
| find a file | `find /home/rosika -name 'test_txt'` |
| modules in use | `kldstat` |
| Check your current version | `uname mrs` |
| detailed information about disks | `geom disk list` |

4. **FreeBSD Handook**

The FreeBSD installer offers to install the FreeBSD Handbook during its installation process.
If for any reason you might have missed it (or didn´t want to have it installed at the time) you can install it later.

 Description: 
 
 The FreeBSD Handbook is a comprehensive guide that covers various aspects of using FreeBSD, from installation and basic system administration to advanced topics. It is well-organized and serves as an good resource for all kinds of users.
 
* `sudo pkg install en-freebsd-doc`

* `sudo pkg install dillo`
 
 Dillo is a fast, small graphical web browser ideally suited for displaying the relevant content.
 Any other graphical browser can of course be used for this task as well.
 
 If you prefer running FreeBSD from the command line (with no GUI) you can still make use of `dillo` by ssh-ing into FreeBSD using the X-forwarding feature:
 
* `ssh -X [USER]@[FreeBSD´s IP-ADDRESS]`

 example: `firejail ssh -X rosika@192.168.122.11`  # `firejail` sandboxing feature is optional of course
 
* `dillo /usr/local/share/doc/freebsd/` # in FreeBSSD VM
  
  takes you to the page from where you can start exploring the FreeBSD handbook locally.


### some interesting links:

 * https://docs.freebsd.org/en/articles/linux-users/
 * https://cloudxlab.com/assessment/displayslide/7359/bsd-syntax-of-commands
 * https://man.freebsd.org/cgi/man.cgi  # FreeBSD man pages
 * https://wiki.freebsd.org/PkgPrimer  # package management
 * https://paolozaino.wordpress.com/2021/07/05/freebsd-how-to-upgrade-freebsd-to-a-more-recent-release/
 * https://www.cyberciti.biz/faq/freebsd-hard-disk-information/ 
 * https://cooltrainer.org/a-freebsd-desktop-howto/

