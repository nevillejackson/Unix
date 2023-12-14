## Introduction ##

The init system in Linux is responsible for two important tasks
 - starting the first process after the kernel is running
 - supervising daemon processes ( often called services)  - eg. starting and stopping a service, service logs, restarting a failed service.

The official `runit` site is here
http://smarden.org/runit/

Runit is a modern implementation of and old-fashioned set of init tools called 
`daemontools`. 

https://wiki.gentoo.org/wiki/Daemontools

Runit is an alternative to sysvinit or systemd. Runit can be used on its own, or in conjunction with OpenRC. It is a lot easier to switch between `runit` and `sysvinit` than between `runit` and `systemd`.

The best way to learn `runit` is to use a distro that comes with runit preinstalled. Runit is not set up in exactly the same way in all distros, so we are going to look at several distros with preinstalled `runit` , and later look at some distros where `runit` can be added as a package.

