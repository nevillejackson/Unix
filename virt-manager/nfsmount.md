## NFS mount of a host filesystem to a guest mountpoint ##
NFS stands for Network File System. It is one of the oldest
and most reliable ways of sharing files between machines
on a network. 
It works just as well on a virtual network, as on a 
physical network. 

### Setting up the host to share a filesystem ###
The host system must have the NFS server daemon (commonly called nfsd) running 
for NFS to be able to export a host directory to the guest system. 
Exporting consists of making a directory available to be mounted by 
the guest system.
 
_In a Debian host_, or a derivative, the NFS server package is 'nfs-kernel-server' 
and the client packages are 'nfs-common', 'libnfs13',
 and 'libnfsidmap2'. 
Installing 'nfs-kernel-server' with `apt` should automatically 
start the server daemon.

_In an Ubuntu host_, or a derivative,...

_In a  Void Linux host_ NFS requires  the 
package `nfs-utils` and one should check that the 
service `nfs-server` is running

```
[nevj@trinity ~]$ ps ax | grep nfs
 1052 ?        Ss     0:00 runsv nfs-server
 1318 ?        I<     0:00 [nfsiod]
 1549 ?        S      0:00 [nfsd]
```
and it also requires that the services `statd`, rcpbind'  
and 'netmount' are running. In Void Linux these may have to be 
started by hand.

To share a filesystem it has to be listed in the file 
`/etc/exports` as follows

```
/common   192.168.122.45(rw,no_subtree_check,no_root_squash)
```
will allow the directory `/common` in the host
  to be mounted by the VM guest at 192.168.122.45
Changes to `/etc/exports` have to be followed by

```
exportfs -a
```
You only need to do `exportfs -a` once, it is permanent.
You can check that it has worked by

```
showmount -e localhost
Export list for localhost:
/common 192.168.122.45
```


### Setting up the guest to do an NFS mount ###
To do an nfs mount in the guest, one should first check 
that nfs is installed, and that the nfs client 
(called nfsiod) is running.

``` 
ps ax | grep nfs
   1327 ?        I<     0:00 [nfsiod]
   2983 pts/0    S+     0:00 grep nfs

```
That is OK so we can do a mount

```
mount -t nfs 192.168.32.6:/common /mnt/common

df -H
Filesystem            Size  Used Avail Use% Mounted o.....
......
192.168.32.6:/common  1.2T  111G  975G  11% /mnt/common
```

That is all that is needed.
The IP address used in the above example can be any 
interface available in the host. In my case there was a choice of

 - the static port 192.168.32.6
 - the DHCP port used for internet connection
 - the virtual bridge port virbr0 which is 192.168.122.1

They all work. 
If one configures `/etc/hosts` in the guest,
 one can use the hostname instead of IP address.
DNS is not required.
So NFS is different from `ssh` in that it listens
 on all interfaces. Ssh is commonly restricted to
 _not_ listen to the internet port.

If you require an nfs mount every time you start the guest VM, 
put the following in `/etc/fstab`


```
192.168.32.6:/common /mnt/common nfs rw,hard 0 0
```

## Reverse option ##
It would be possible to configure a VM guest system as 
an NFS server and to make the host act as a client. This
would make a guest filesystem mountable by the host. 
This backwards-sharing may be less useful and it does not 
correspond to what is commonly meant by a shared folder.

## Recommendation ##
Use NFS mounts if you want what is commonly called
 a `shared folder`. That is to share large amounts 
of data between host and guest systems.
Use nfs mounts if you want the mount to be made
 automatically every time you start the guest VM.
 
### Links ### 

https://linuxize.com/post/how-to-mount-an-nfs-share-in-linux/

https://docs.voidlinux.org/config/network-filesystems.html


