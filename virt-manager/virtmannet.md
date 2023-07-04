## Virt-manager network setup ##

We assume that virt-manager has been installed
 in a Linux host, and 
at least one guest Linux system has been installed 
and is running.

The first thing to learn with virt-manager is how to find the settings.
Under View on the QEMU/KVM Window, there are two options

 - Console  which means show the running guest console
 - Details which means show the settings
 
You can toggle between them.
You can change the settings for a guest VM at any time, they are not fixed when it is installed. It is best to change settings with the guest not running. If you change settings with the guest running, they do not apply until the next boot.

### Default network settings ###
The guest VM is by default connected to the host system by a network bridge.  If we look at the host network devices with the `ip` command  we see

```
 $ ip addr 
 ..... 
 5: virbr0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000
    link/ether 52:54:00:78:7c:5d brd ff:ff:ff:ff:ff:ff
    inet 192.168.122.1/24 brd 192.168.122.255 scope global virbr0
       valid_lft forever preferred_lft forever      
```
  That device `virbr0` is a virtual bridge between the host and guest systems. The guests under virt-manager ( there can be more than one) are on a virtual network with address 192.168.122.0. The bridge is address 192.168.122.1 on that network (as seen by the host).  The host can route packets to that network via the bridge interface, in exactly the same way as it routes packets to an  ethernet port or other interfaces.
  
  To find the IP address of a guest on the above virtual network, one has to use the `ip` command in the guest.
  
``` 
  $ ip addr
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host 
       valid_lft forever preferred_lft forever
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP group default qlen 1000
    link/ether 52:54:00:59:a0:69 brd ff:ff:ff:ff:ff:ff
    inet 192.168.122.45/24 brd 192.168.122.255 scope global dynamic noprefixroute eth0
       valid_lft 2932sec preferred_lft 2932sec
    inet6 fe80::994e:fcd3:91f3:6f30/64 scope link noprefixroute 
       valid_lft forever preferred_lft forever
```

We see 2 interfaces, `lo` which is the guest itself, and `eth0` which is the guest's interface to the virtual network. We can see now that our guest has IP address 192.168.122.45 on the virtual network.

We had to go into the guest to find its IP address, because virt-manager by default uses NAT (Network Address Translation) which means that one can not obtain the IP address of the guest from anywhere outside the guest. 

One can operate virt-manager with just a bridge and no NAT. To do that got to View -> Details -> Add Hardware -> Network and change the Network Source setting from `Virtualnetwork default:NAT` to `Bridge Device`. Without NAT, one can scan the virtual network from the host with a tool like `nmap` and find the guest IP addresses. That is about the only difference. We recommend using virt-manager with the default NAT setting. 

From the host one can ping the guest using its IP number,
 and one can ping the bridge using IP=192.168.122.1.

From the guest one can ping the host using either the hostname, 
or the IP number of any of the hosts interfaces,
 and one can ping the bridge using IP=192.168.122.1.
The guest does not know the host's hostname, the name is 
unresolved in the guest, but the ping packet is routed to the host where
 it gets resolved.


### Using hostnames ###
While one can communicate with a guest in virt-manager 
using its IP address, it is more convenient to use hostnames. To do that we just need to set up hostnames in the file `/etc/hosts`
To achieve that , we need to add an entry to the file 
`/etc/hosts` in the host system

```
$ cat /etc/hosts
# IP-number         Domain-name    Hostname
......
192.168.32.6		trinity.colrose.com.au	trinity
192.168.32.7		mary.colrose.com.au	mary
192.168.122.45      mxvm.colrose.com.au       mxvm

```
The entry `mxvm` is the hostname of the guest system.
 The host system's hostname is `trinity` and `mary` is another 
comuter on the local network.


With this setup one can do, from the host

```
$ ping mxvm
PING mxvm.colrose.com.au (192.168.122.45) 56(84) bytes of data.
64 bytes from mxvm.colrose.com.au (192.168.122.45): icmp_seq=1 ttl=64 time=0.199 ms
```

From the guest, one can always ping the host 

```
$ ping trinity
PING trinity.colrose.com.au (192.168.32.6) 56(84) bytes of data.
64 bytes from trinity.colrose.com.au (192.168.32.6): icmp_seq=1 ttl=64 time=0.246 ms
```

because the guest will route the unresolved ping packed to the host
as explained above.
From the guest, one can also ping any other computer or 
device on the local net  ( such as `mary`), and any 
internet site that the host can access. 

We use `ping` here just to show that communication is established.  
If ping is successful, we can proceed with data sharing.

### Recommendation ###
When you setup virt-manager, get familiar with its virtual network. 
Everything you do to share data between host and guest, or between multiple guests, depends on understanding the virtual network.

### Links ###

https://wiki.libvirt.org/VirtualNetworking.html

https://gulraezgulshan.medium.com/virtual-networking-in-linux-b1abcb983e72




   
  
  
  
  
