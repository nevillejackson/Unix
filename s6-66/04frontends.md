#### Setting up a frontend file ####
Each service requires a 'frontend' file. This is how services are configured.
Root frontend files are in `/etc/66/service`.  User frontend files are in `~/.66/service`. 
When the system is installed, there will be some root frontend files present in `/etc/66/service`. These are useful as examples.

Ne services will require a 'frontend file' . We give a simple example of installing and starting a new service 'vsftpd'.
```
apt install vsftpd
```
this makes sure the vsftpd binary is present. 
We need to make a file called `vsftpd` in /etc/66/service containing
```
[main]
@type=classic
@description="vsftpd daemon"
@version=0.0.1
@user=(root)

[start]
@execute=(
    /usr/sbin/vsftpd
)
```
'classic' means it is a traditional daemon, ie not a 'oneshot' or a 'module'
this service can be started by root only
the binary is at `/usr/sbin/vsftpd`

This is a new frontend.  It has to be parsed once
```
66 parse vsftpd`
```
Then it can be started
```
66 start vsftpd
```
Acrually, if a parse is needed, `66 start ...` will do the parse before proceeding with the start.
If there is a mistake in the frontend, and you need to edit it, you then need to do
```
66 reconfigure vsftpd
```
which will re-parse it and re-start it.

That is all there is to starting a simple daemon. 
There are more complicated things, such as setting environment variables, and defining dependencies between services. 
