##### Setting up Anydesk for Unattended Access #####
  Assuming the Anydesk client is installed  in the remote machine, all one needs to do is go to 
Settings -> Security
unlock it 
and click the box 'Enable Unattended Access'

unattendedbox.png
<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/anydesk/unattendedbox.png?raw=true">
</p>


it will then offer a menu to set a password.

unattendedsetpassword.png
<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/anydesk/unattendedsetpassword.png?raw=true">
</p>


You should set a very strong password ( eg 12 digits with a mix of lower and upper case, numbers, and special characters).  This password is your ony protection against unwanted access. There is no 2 factor authentication like in normal attended mode. 

Before you leave the unattended machine, make sure it is
n , and the anydesk daemons are running

```
$ ps ax | grep any
    667 ?        Ssl    0:04 /usr/bin/anydesk --service
   1183 ?        Sl     0:03 /usr/bin/anydesk --tray
   3242 ?        Sl     0:02 /usr/bin/anydesk

```
You do not have to run the anydesk interactive window. For security you should log out but not shut down.
