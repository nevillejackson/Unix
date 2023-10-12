### login
## How to log in to FreeBSD VM

This chapter aims to describe some different ways of logging in to FreeBSD VM.
Logging into a FreeBSD guest in virt-manager or via SSH is an important aspect of managing and using the guest VM. Here are the steps for the various methods.

* with virt-manager screen

* via console
	with `ssh` commands and
	with `virsh` commands
	


### Logging into a FreeBSD Guest in virt-manager:


1. Start virt-manager: 

    Open virt-manager on your Linux host. You should find it in your applications menu or run it from the terminal.

2. Select the FreeBSD VM:

     In the virt-manager interface, locate your FreeBSD virtual machine in the list of available VMs. Click on it to select it.

3. Open the Console: 

    Right-click on the VM and select "Open" or "Open Console." This will open a graphical console window displaying the VM's boot process.

4. Login:

     When the VM has finished booting, you'll see the FreeBSD login prompt in the console window. Enter your FreeBSD username and password to log in.
     

### Logging into a FreeBSD Guest via SSH and virsh


"virsh" is a command-line utility that is part of the libvirt toolkit.

Libvirt is an open-source API and management toolset for managing virtualization technologies like KVM (Kernel-based Virtual Machine), QEMU, Xen, and others. Virsh is a powerful command-line tool for managing and interacting with virtual machines managed by libvirt.

Here are some of the *key functions*:

1. Managing Virtual Machines:
 
     "virsh" allows you to start, stop, pause, resume, and control the power state of virtual machines.

2. Inspecting Virtual Machine Status:

     You can use `virsh list --all` to check the status (running, paused, etc.) of virtual machines.

3. Creating and Deleting Virtual Machines:

     You can create new virtual machines, clone existing ones, and delete virtual machines.

4. Modifying Virtual Machine Configurations:
 
     `virsh dumpxml` enables you to modify or inspect the XML configuration of virtual machines.

5. Snapshot Management: 

    You can create and manage snapshots of virtual machines using `virsh snapshot-create` and related commands.

... and many more tasks.


For Linux hosts with deb package management you may install it with:

`sudo apt-get install libvirt-daemon-system virtinst`


**Using FreeBSD from the console login with virsh and ssh commands.**


1. List All Installed VMs:

        `virsh list --all`
        
    This command displays a list of all virtual machines, including their statuses (running, paused, etc.).

2. Start the FreeBSD VM:

        `virsh start freebsd13.1`  # freebsd13.1 is an example, so...
        
    ... replace freebsd13.1 with the actual name of your FreeBSD VM.
        This command initiates the booting process of the specified virtual machine.

3. Get the VM's IP Address:

        `virsh domifaddr freebsd13.1`
        
    This command provides information about the network interfaces of the virtual machine, including its IP address.

4. SSH into the FreeBSD VM:

        `ssh username@vm_ip_address`
        
    Replace username with your FreeBSD username and vm_ip_address with the IP address obtained from step 3.
    Use this command to log in to your FreeBSD VM via SSH.

5. Shutdown the FreeBSD VM:  # final step, when you´re done using FreebSD VM

        `virsh shutdown freebsd13.1`
        
   This command initiates the shutdown of the virtual machine.


### Some additional remarks


It´s always a good idea to take a snapshot of the virtual machine before applying any major changes to it.

Here `virsh` may come in handy again. 
freebsd13.1 is taken as an example again.

* `virsh snapshot-list freebsd13.1` # lists all available snapshots for the specific VM
* `virsh snapshot-create-as --domain freebsd13.1 --name "after_latest_upgdate"` # `--name`: put your personal remark here
* `virsh snapshot-revert freebsd13.1 after_latest_update` # revert to a previous working state
* `virsh snapshot-delete --domain freebsd13.1 --snapshotname after_latest_update`  # delete snapshot


### Some links


* https://libvirt.org/manpages/virsh.html
* https://linuxconfig.org/how-to-create-snapshots-of-qemu-kvm-guests
* https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/virtualization_deployment_and_administration_guide/sect-managing_guest_virtual_machines_with_virsh-managing_snapshots
* https://access.redhat.com/solutions/1597713






