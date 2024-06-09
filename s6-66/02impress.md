#### My impressions after using s6-66 for several weeks ####
In S6-66 we are looking at an init facility based on couple of seriously 
innovative academic initiatives. 

 - the `daemontools` package is a set of tools for daemon management
developed by the brilliant mathematician  D J Bernstein, ho has a n of parers on maths and computer algorithms.

 - the S6 part of s6-66  is a really thorough implementation of `daemontools` 
from `skarnet.org`.  It is this initiative which adds parallel startup . The original S6 Service Manager ( called s6-rc) added dependency management and bundles, but the new Service Manager (s6-66) does this easier. 

 - the `66` part of s6-66 ( ie the service manager) is a design revolution 
for the user interface to init systems. There is only one command ( called `66`) and there are subcommands. So it is designed like `git` or `apt` .... the modern way to build a CLIcommand. There are no scripts .... there is a user modifiable config file called a 'frontend` that sets up everything for a service... all in one place. 

So we are dealing with something that is the combined product of a number of seriously innovative people.  What you tend to get in such cases is something with a theory ( a philosophy if you like) behind it.   In contrast software from large cooporate efforts tends to be a collection of poorly integrated tools with 
inconsistent approaches.  
