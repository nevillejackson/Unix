### Things to do in antiX26 (not DE related) ###


 1. Setup Network Time Protocol
There are 2 alternative packages for ntp
 - chrony
 - ntpsec
We are going to use `chrony`, so
```
apt install chrony
```

For the dinit init system:
There is no service package `dinit-service-chrony`, so we have to setup a servic
e file in /etc/dinit.d
```
cd /etc/dinit.d
vi chrony
[add the following lines]
type = process
command = /sbin/chronyd -d
depends-on = loginready
logfile = /var/log/chrony-dinit.log
```
Start and enable the service
```
dinitctl start chrony
dinitctl status chrony
dinitctl enable chrony
```
or use the GUI under Control Centre -> Services
Verify that everything is working with 
```
chronyc tracking
```
If the displayed time is out by a lot ( eg 1 hour ) do 
```
dpkg-reconfigure tzdata
```
and check that the local time zone is set correctly.


For the runit init system:
There is a package `runit-service-chrony`  , but we shall do it by hand...
Configure chronyd as follows
```
cd /etc/sv
mkdir chronyd
cd chronyd
vi run
[edit in the folowing lines]
#!bash
exec /sbin/chronyd -d
[save from vi]
chmod 755 run
```
Then make a link
```
cd /etc/runit/runsvdir/default
ln -s /etc/sv/chronyd chronyd
```
The daemon should be running.

For the sysVinit init system:
The chrony package installs a service file in /etc/init.d. 
You do not need to do anything.
Chronyd should startuo at boot.

For the s6-rc init system:
There is no service package for chronyd in s6-rc.
This is very difficult. s6-rc had config is another whole topic.

For the s6-66 init system:
For s6-66 there is a config package `66-service-chronyd`, so all we need do is install the config package
```
apt install 66-service-chronyd
```
The daemon should be running 
```
ps ax | grep chron
 7197 ?        S      0:00 s6-supervise chronyd-log
 7198 ?        S      0:00 s6-supervise chronyd
 7205 ?        Ss     0:00 /usr/bin/s6-log -d3 n3 s1000000 /var/log/66/chronyd
```




 2. DNS lookup failure
 AntiX used `connmand` in place of NetworkManager.  Check that `connmand` is running`. If running, you need to use `connmanctl` to make sure connman uses dhcp on the correct interface and has nameservers.  Try the following
```
connmanctl services
[make sure your interface's service-name is at top of the list ... note  the name]
connmanctl config <service-name> --ipv4 dhcp
connmanctl config <service-name> --nameservers 1.1.1.1 8.8.8.8
```
Test that DNS is working by pinging something.

 




