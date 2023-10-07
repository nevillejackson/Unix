## Using FreeBSD as a guest VM in virt-manager ##

  This is a joint post by @nevj and @Rosika
 [quote="Neville Jackson, username:nevj"]
 [/quote]
 and
 [quote="Rosika Schreck, username:Rosika"]
 [/quote]

It reports our investigations into  running the virt-manager package in a Linux host and a guest FreeBSD system in a VM. 

Our post consists of several 'chapters' each written in markdown and copied into itsFOSS as a series of separate replies.

### Contents ###
 1. Introduction 
 2. Where FreeBSD differs from Linux
 3. Installing FreeBSD in a virt-manager VM
 4. Using FreeBSD from the console login ... virsh and ssh or virt-manager screen
 5. Getting a Window System into FreeBSD
 6. FreeBSD desktop environments
 7. Communication between Linux host and FreeBSD guest
    7a. File transfers between Linux host and FreeBSD guest - scp, sftp, rsync
    7b. File sharing between Linux host and FreeBSD guest - mounts and filesystem types.
    7c. Copy/paste and spice-vdagent
 8. FreeBSD commands
We hope this may be useful to Linux users who would like to try BSD in a VM.
Please let us know of any issues.
