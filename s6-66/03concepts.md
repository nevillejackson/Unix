#### Concepts used by 66 ####
 It is important to understand the underlying ideas.

 -  66 -- the name of the command that performs all Service Management actions ( except setting up a frontend file for a service). `66` has subcommands ( like `git`) and there is a help menu displayed by `66 -h` or `66 <subcommand> -h`

 - service -- a process that runs automatically in background. The process is often called a daemon. Daemons in S6 ( and runit) are not orphaned the way they are in other init systems ... they are owned by a supervise process which persists.

 - frontend -- a user-written file providing information ( for setting up a service)  from which 66 constructs and stores an execline script and other details. For root frontends are in '/etc/66/service' and for normal users frontends can be in '~/.66/service` or '/etc/66/service/user'. Frontends need to be accessed once with the `66 parse` command, and if they are altered need to be re-accessed with the `66 reconfigure` command.

 - service type -- classic, oneshot or module. Describes the way a service is used
   - classic service -- a typical persistent daemon ( called longrun in s6-rc)
   - oneshot service -- a service that performs one action then dies. ( for example a mount)
   - module service -- a service ( or a bundle of services) whose actions can be modified at runtime by setting the environment. The names of services of type module end in '@'

 - instantiated -- refers to the mechanism by which service actions can me modified at runtime. Any type of service can be instantiated. A module service is always instantiated. Instantiated services have an '@' in their name ( eg tty@) and use an '@I' in the frontend as a placemarker for a value substituted at runtime. 

 - tree -- a collection of services ( like a bundle in s6-rc) which
 can be managed together by one `66 tree ...` command.
 There is a `global` tree which is the default and is present at install time.
 The 'global' tree is always enabled - ie it starts automatically at boot time.
 New services default to the global tree unless another tree is specified.
 There is a 'boot' tree which contains various services needed  when the system boots. 
 There is a tree called 'session' which containes modules which setup an environment in which a user can manage services.
 Other user-defined trees can be created. A service can only belong to one tree.
 Any type of service can belong to a tree, even a module service.
 A tree can not be nested within another tree.

 - enable/disable -- something that is 'enabled' starts automatically at boot 
time. A single service can be enabled. A tree can be enabled. 

 - stop/start -- start means launch a service immediately. A tree can be started immediately. When a tree started, only the services within it that are enabled 
will start. 

 - scandir -- a directory ( /run/66/scandir/...) containing, for each user, the compiled version of the frontend for each running service, plus a number of status files. For root it is /run/66/scandir/0/..., for the first user it is /run/66/scandir/1000/.... There is a special admin command to manage scandirs. 
The root scandir is automatically created.  A user scandir must be initialised with the `66 scandir ...` command, before any user services can be started. 

 - boot@system -- a special module that creates a tree called 'boot' that containes all the services needed for the system to boot properly. 

 - boot-user@username -- a special module that creates a tree called 'session' that contains modules which setup an environment in which a user can manage user owned services.

