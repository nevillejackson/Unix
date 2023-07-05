 This is a joint post by @nevj and @Rosika

[quote="Neville Jackson, username:nevj"]
[/quote]

and

[quote="Rosika Schreck, username:Rosika"]
[/quote]

It reports on our collaborative investigations into the virt-manager package which uses qemu/KVM to run virtual machines inside a host Linux system. 
In particular we have looked at various ways of communicating between a guest Linux system running in a VM and its host Linux system.

Our post consists of several 'chapters' each written in markdown and copied into itsFOSS  as a series of separate replies.

## Contents ##
 1. virt-manager networking setup
 2. Thunar, ssh, scp, and sftp
 3. virtiofs filesystem mounts
 4. nfs filesystem mounts
 5. qemu-nbd mounts
 6. device mounts
 7. rsync

 We hope this may be useful to someone who has used virt-manager but 
has not yet conquered data sharing . 
 This is also an experiment in using Markdown
 to prepare a post offline.
Please let us know of any issues.
