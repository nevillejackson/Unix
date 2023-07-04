## Using rsync to share files between guest and host ##
The comand `rsync` is useful for keeping the contents of 2 directories
exactly the same.The 2 directories can be on separate 
physical machines on a network, or one or both of them can be on 
virtual machines on a virtual network.

`rsync` will work over any network which is setup for `ssh` 
usage.

### Example from guest to host ###
 We have a directory called Test in a guest MX system containing
some markdown files

```
nevj@mx:~/Test
$ ls
devicemount.md  kgw_2project.md  nfsmount.md.v1  rsync.md  virtiofsmount.md
intro.md        nfsmount.md      qemunbd.md      ssh.md    virtmannet.md

```
We want to maintain an exact copy of the contents of that
directory in the host system. This requires issueing an `rsync` command in the guest

```

 nevj@mx:~/Test
$ rsync -aAXvH /home/nevj/Test/* trinity:/home/nevj/Test
The authenticity of host 'trinity (192.168.32.6)' can't be established.
ECDSA key fingerprint is SHA256:xxxxxxxxxxxx.
Are you sure you want to continue connecting (yes/no/[fingerprint])? yes
Failed to add the host to the list of known hosts (/home/nevj/.ssh/known_hosts).
nevj@trinity's password: 
sending incremental file list
devicemount.md
intro.md
kgw_2project.md
nfsmount.md
nfsmount.md.v1
qemunbd.md
rsync.md
ssh.md
virtiofsmount.md
virtmannet.md

sent 19,642 bytes  received 210 bytes  2,335.53 bytes/sec
total size is 18,933  speedup is 0.95
nevj@mx:~/Test

```
Any file that is not already present in `trinity:/home/nevj/Test` 
is sent by `rsync` ( in this case all of them ).
Either the hostname (trinity) or alternatively
 the IP address can be used in the rsync statement.

### Rsync from host to guest ###
This is done in exactly the same way, except the `rsync`
 command is issued in the host.
In this case, if the hostname of the guest is used, it has to be defined 
in the `/etc/hosts` file of the host.

### Recommendation ###
Use `rsync` if you want to keep a very active directory 
updated in both host and guest. 


### LInks ###

https://www.digitalocean.com/community/tutorials/how-to-use-rsync-to-sync-local-and-remote-directories

https://www.tecmint.com/rsync-local-remote-file-synchronization-commands/

https://phoenixnap.com/kb/rsync-command-linux-examples
