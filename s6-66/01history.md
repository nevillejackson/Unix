#### Brief history of init systems ####
Some of the things a running Linux system does are outside of the kernel. These are either
 - packages run in 'user space' , or
 - services ( also called daemon processes) 
Traditionally , after the kernel starts, it starts one process( called `init`) , 
which consults some config files, which tell it what other daemon processses
to start. 
Traditional daemon processes are slightly different to user-launched processes in that they are orphaned ( ie not owned by a shell). That mens they dont die, if you exit the shell... traditional daemons run forever.... at least in theory.
In practice, they die unexpectedly, and have to be manually restarted.
That is the reality in a traditional init system like RC, sysVinit, or OpenRC.

At some point, it was decided that daemons need to be managed. That means that instead of fixing bugs in the daemon, we automatically restart it when it fails.
A whole spectrum of new init systems emerged -- Upstart, Systemd, Runit, S6, Dinit, Shepherd. 

Systemd has dominated in Linux, but not in BSD. It is the default in Debian, OpenSUSE, RHEL, and Arch.
Systemd has been criticized for violating Unix philosophy. It is large and a big learning curve. Systemd is like traditional init systems in that there is only one process running for each daemon started. A single process manages all running daemons. 
Systemd is unlike traditional init systems in that it has parallel startup, users as well as root can manage services, and it has ways of managing multiple services as a group. (Correct me here if needed.... I do not know systemd well).

The Runit and S6 init systems are different. They are both based on a package called 'daemontools', which manages daemons by having a separate supervise process ( in addition to the daemon process itself) running for each daemon. There may also be a logging process for each daemon, and a supervise process for the logging process.. In this system , daemons are not orphaned, they belong to the supervise process.  

Runit has supervision and parallel startup, , but not tools for service management( ie starting and stopping services) -- you do it by hand, and no management of dependencies..

S6 has supervision plus service management, and it manages dependencies.. The original service manager ( called S6-rc) has been available for some time, but has a few non-friendly concepts (such as compiling the database and writing execline scripts ) which make it clumsy to use. S6-rc has `bundles` which allow serveics to be grouped and managed as a unit.

 There is a new service manager  for S6 ( called S6-66) which has  an elegant design and is much more user friendly. S6-66 avoids compiling a database, and it has a 'frontend' file instead of an execline script. 66 has 'trees', which are like bundles, and it allows users to manage services, as well as root.

This topic is about s6-66, but the background is important. In S6-66 we are looking at a facility with a substantially different philosophy to either systemd or traditional init systems.
