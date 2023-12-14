### Recommendations ###
  I found the following

 - The best implementations of runit are Void and Artix. Antix is a close second.
 - If you want a GUI controlled init system use Antix
 - If you want to try runit , try a system with it pre-installed - either 
Void or Artic or Antix.
 - It is important that a distro provides package support for runit. Just providing a runit package is inadequate. Every package that installs a daemon should 
come with a runit script. 
 - the best package system design for using multiple init systems in Artix. 
 - the fact that Debian now makes a runit package available is interesting.  They have backed down on the systemd only approach.  They have not adapted the package system.
 - Devuan is supposed to support multiple init systems, but it has a long way to go to provide something as good as the Artix package system. Same applies to Peppermint Devuan Edition.
 - Gentoo has the best runit documentation. It spells out all the limitations.
 - Runit has some merit over sysVinit. It boots faster, and its scripts are simpler.

