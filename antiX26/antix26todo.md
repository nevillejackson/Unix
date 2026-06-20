### Things to do in antiX26 (not xfce related) ###


 1. Setup Network Time Protocol
There are 2 alternative packages for ntp
 - chrony
 - ntpsec
We are going to use `chrony`, so
```
apt install chrony
```
There is no service package `dinit-service-chrony`, so we have to setup a servic
e file in /etc/dinit.d
```
cd /etc/dinit.d
vi chrony
[add the following lines]
type = process
command = /usr/sbin/chronyd -d
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

The daemon `chronyd` needs to be set to start at boot in each init system.


 2. DNS lookup failure
 AntiX used `connmand` in place of NetworkManager.  Check that `connmand` is running`. If running, you need to use `connmanctl` to make sure connman uses dhcp on the correct interface and has nameservers.  Try the following
```
connmanctl services
[make sure your interface's service-name is at top of the list ... note  the name]
connmanctl config <service-name> --ipv4 dhcp
connmanctl config <service-name> --nameservers 1.1.1.1 8.8.8.8
```
Test that DNS is working by pinging something.

 




