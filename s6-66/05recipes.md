#### Some examples of tasks and how to do it with `66` ####
Everyone needs a recipe book.  This is not comprehensive .... just gives a taste of how easy `66` is to use.

##### Operating as root user #####

Start a service immediately
```
66 start <servicename>
```

Enable a service, so it will start at boot time.
```
66 enable <servicename>
```

Look at the status of a service
```
66 status <servicename>
```

Look at the available trees
```
66 tree status
```

To make a new tree you simply start a service specifying that tree
```
66 -t <newtree> start <servicename>
```
That will also add a service to an existing tree

To start a tree
```
66 tree start <treename>
```
ie, the same as starting a single service. All services within that tree will start if they have been enabled. You can use this to selectively start part of a tree by disabling unwanted services. If you want the tree to start automatically at boot 
you have to enable the tree
```
66 tree enable <treename>
```


##### Operating as non-root user #####
Before any normal user can use '66' , root has to setup a module which establishes 
the user environment. For example
```
66 parse boot-user@nevj
66 configure -e nano boot-user@nevj
edit the config ( eg set window manager)
66 reconfigure boot-user@nevj
always needed after a config is changed
```
there will then be a new tree called 'session'
```
root@antix-init-diversity:~# 66 tree status

Name        : session
Current     : no
Enabled     : no
Allowed     : root
Groups      : admin
Depends     : None
Required by : None
Contents    : scandir@nevj:setenv@nevj scandir@nevj:svscan@nevj
              scandir@nevj:svscan@nevj-log scandir@nevj
              boot-user@nevj:mount-run@nevj boot-user@nevj
```
and it contains various module services which setup user 'nevj's' environment

Now, each time ytyou boot you need root to do
```
66 start boot-user@nevj
```
and that sets up nevj's environment.

Now user 'nevj' can use all the same `66` commands as in above examples for root.
User 'nevj' has his  own global tree, which is separate from root's global tree, and user 'nevj' can not see or manipulate the system trees.
Otherwise everything is the same as operating as root.
