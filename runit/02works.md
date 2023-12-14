### How runit works ###

Runit consists of a directory structure in `/etc` plus a number of daemon processes. If you look at the daemons running in any Linux system with `runit` you will see something like the  following

```
$ ps ax | grep run
    1 ?        Ss     0:01 runit
 1394 ?        Ss     0:00 runsvdir -P /etc/service log: ..................................................................................................................................................................................................................................................................................00:00:00.000 [INFO] [seatd/seat.c:39] Created VT-bound seat seat0 00:00:00.000 [INFO] [seatd/seatd.c:194] seatd started .
 1444 ?        Ss     0:00 runsv ssh
 1445 ?        Ss     0:00 runsv getty-tty3
 1446 ?        Ss     0:00 runsv seatd
 1447 ?        Ss     0:00 runsv getty-tty6
 1448 ?        Ss     0:00 runsv getty-tty4
 1449 ?        Ss     0:00 runsv udevd
 1450 ?        Ss     0:00 runsv getty-tty5
 1451 ?        Ss     0:00 runsv getty-tty1
 1452 ?        Ss     0:00 runsv getty-tty2
 1453 ?        Ss     0:00 runsv ntp
 1454 ?        Ss     0:00 runsv connman
 1455 ?        Ss     0:00 runsv dbus
 1462 ?        Ss     0:00 runsv cups
 1463 ?        Ss     0:00 runsv slimski
 1473 ?        S      0:00 svlogd -tt /var/log/runit/connman
 1475 ?        S      0:00 svlogd -tt /var/log/runit/ssh
```

 We can identify 4 differnet daemons here

 - `runit`  - is process #1 . It replaces the process called `init`  which is used
    in sysVinit and OpenRC. It is the first process to run in user space once the kernel is up and going. It executes various scripts which start other user-space processes, so all other user-space processes are chidren of the `runit` process.

 - `runsvdir` - watches a directory called `/etc/service` which contains files used to setup a service. Inside `/etc/service` each service has a subdirectory containing a run script and other things. If `runsvdir` finds a new subdirectory ( ie a new service) it starts a `runsv` process to manage that service. That is about all `runsvdir` does. It is a `cockatoo` daemon.

 - `runsv`  - there is one of these for every service started by `runsvdir`. You can see 14 `runsv` processes in the above example.
 Each `runsv`starts a service, and waits for commands issued
 by the `sv` utility, which can stop/start a running service or query its status.
 If there is logging, `runsv` starts the logging daemon.
 - `svlogd` - if a service has logging set up, there  will be a `svlogd` process in addition to the `runsv` process, for that service. In the above example there are two log daemons running, for connman and ssh services. Logging is setup by putting a log script alongside the run script in /etc/service subdirectory.

The only command `runit` has in addition to the 4 daemons, is the `sv` utility command referred to above. Everything else is information stored in its directory structure.

It is common practice, but not essential, to put all the service subdirectories with their run and log scripts not in `/etc/service` but in a `staging directory` where they can be edited and tested before enabling. If this is done `/etc/service` becomes a symbolic link to the `staging directory` , for example

```
$ ls -l /etc/service
lrwxrwxrwx 1 root root 22 Oct 19  2022 /etc/service -> runit/runsvdir/current
```
so here the staging directory is `/etc/runit/runsvdir/current`
and, 
this in turn is commonly also a set of links, for example

```
nevj@antixvm:/etc/runit/runsvdir
$ ls -l current
lrwxrwxrwx 1 root root 7 Jul  5 20:45 current -> default
nevj@antixvm:/etc/runit/runsvdir
$ ls -l default
total 0
lrwxrwxrwx 1 root root 16 Oct 19  2022 connman -> /etc/sv/connman/
lrwxrwxrwx 1 root root 13 Jul 10 18:07 cups -> /etc/sv/cups/
lrwxrwxrwx 1 root root 13 Oct 19  2022 dbus -> /etc/sv/dbus/
lrwxrwxrwx 1 root root 22 Oct 19  2022 getty-tty1 -> ../../../sv/getty-tty1
lrwxrwxrwx 1 root root 18 Oct 19  2022 getty-tty2 -> /etc/sv/getty-tty2
lrwxrwxrwx 1 root root 18 Oct 19  2022 getty-tty3 -> /etc/sv/getty-tty3
lrwxrwxrwx 1 root root 18 Oct 19  2022 getty-tty4 -> /etc/sv/getty-tty4
lrwxrwxrwx 1 root root 18 Oct 19  2022 getty-tty5 -> /etc/sv/getty-tty5
lrwxrwxrwx 1 root root 18 Oct 19  2022 getty-tty6 -> /etc/sv/getty-tty6
lrwxrwxrwx 1 root root 12 Jul 10 08:06 ntp -> /etc/sv/ntp/
lrwxrwxrwx 1 root root 14 Oct 19  2022 seatd -> /etc/sv/seatd/
lrwxrwxrwx 1 root root 16 Oct 19  2022 slimski -> /etc/sv/slimski/
lrwxrwxrwx 1 root root 11 Nov  4 15:43 ssh -> /etc/sv/ssh
lrwxrwxrwx 1 root root 14 Oct 19  2022 udevd -> /etc/sv/udevd/
```
in this case `current` is the same as `default`, but what you see is a whole heap of symbolic links, one to each service to be started, and they all point to the subdirectories in the `etc/sv` directory which contain the run script and others  for tahe  service. 

So this set of links is an elegant way of controlling two things

 - choosing an appropriate set of services to start depending on whether linux is 
in multiuser or single user ( ie recovery ) mode, or others like current versus default versus previous, and solo which is special.

 - choosing which services to start in a particular run mode.

This approach is sometimes referred to as a "link farm" , but that term also has another connotation in relation to web site links. 
Used in a filesystem, it is a powerful technique that allows switching between similar alternatives.

That is all there is to how runit works, it relies on 4 daemons and a `staging directory` containing sets of run scripts, accessed via a link farm. The only user interface is the `sv` command, which communicates with the `runsv` daemons.

The only remaining thing is to look at an example run script. Here is the script for `cupsd` 

```

nevj@antixvm:/etc/sv/cups
$ cat run
#!/usr/bin/env /lib/runit/invoke-run
set -e

NAME="cups"
DAEMON=/usr/sbin/cupsd

# Exit service if DAEMON is not installed
if [ ! -x $DAEMON ]; then
	exit 161
fi

#~ # Prepare cups run folder
#~ mkdir -p /run/cups/certs
#~ [ -x /sbin/restorecon ] && /sbin/restorecon -R /run/cups

exec 2>&1

exec $DAEMON -f
```
So all it does is define the daemon name, divert STDERR to STDOUT, and execute the daemon.  Nothing like the complex structured sysVinit scripts. A `runit` run script does one thing, run the daemon. That is characteristic of runit, it breaks the job up into a number of singleminded tasks.
