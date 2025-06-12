#  Secure Linux Remote Access with AnyDesk (Unattended Mode)

Hi everyone,  

This is a joint post by @nevj and @Rosika.

[quote="Neville Jackson, username:nevj"]
[/quote]

and

[quote="Rosika Schreck, username:Rosika"]
[/quote]

 Neville (@nevj) and Rosika (@Rosika) recently experimented with secure unattended remote access using AnyDesk on Linux. We thought it might help others, so here’s a detailed guide based on our setup. Feedback welcome!
 
Note: 
 
In this tutorial, we demonstrate the setup using Rosika accessing Neville's remote machine as an example. However, the same method can easily be applied if you wish to configure unattended remote access to your own machine.



### 1. Why AnyDesk?

AnyDesk offers a simple, secure, and cross-platform solution for remote desktop access. Unlike SSH or VPN setups, AnyDesk does not require port forwarding or paid VPN services. 
It also supports file transfer and graphical sessions out of the box.

In this tutorial, we’ll show how **Neville** (host in Australia) granted **Rosika** (client in Germany) secure unattended access to his Debian machine using AnyDesk (with some extra security considerations in mind).



### 2. Setup Overview

####  Host Machine (Remote — Neville’s Computer)

- **OS:** Debian on bare metal  
- **AnyDesk:** Installed normally: `anydesk.service` is running  
- **User Preparation:**
  - A **dedicated user account** was created for remote access (e.g., `rosika`)
  - This user:
    - Has **no `sudo` or root privileges**
    - Belongs to minimal groups (only their own)
- **Anydesk Setup:**
  - AnyDesk was enabled for **Unattended Access**.
    Go to Settings -> Security and click the box´s 'Enable Unattended Access'
    <p align="center">
<img src="https://github.com/Rosika2/new_projects/blob/main/unattended_access_tick1.png?raw=true">
</p>

  - Set a **strong password**
  
    <p align="center">
<img src="https://github.com/Rosika2/new_projects/blob/main/set_PW.png?raw=true">
</p>

  - Neville provided the following credentials to Rosika:

    1. AnyDesk ID
    2. Unattended access password
    3. Username of the new account
    4. Password for that user account

- _(Optional)_: `fail2ban` was tested, but AnyDesk’s internal precautions appear sufficient.



#### Client Machine (Rosika’s Virtual Machine)

- **Host OS:** Linux Lite  
- **Guest OS (VM):** Debian 11 “bullseye”  
- **Virtualization:** KVM/QEMU with `virt-manager`  
- **Sandboxing:**  
  Started `virt-manager` inside Firejail:
  `firejail virt-manager`
  Within Debian VM: `firejail anydesk`
  
This ensures that the AnyDesk process running inside the Debian VM is sandboxed thus adding an extra layer of protection for the Linux Lite host and the Debian VM:


### 3.  Logging In via Unattended Access


- Launch AnyDesk inside the Debian VM.
- Enter the remote AnyDesk ID.
- Click Connect.
- When prompted, enter the Unattended Access password.
- You’re connected to Neville’s desktop session as a limited user.


  
  <p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/anydesk/2enter_target_address.png?raw=true">
</p>

  
  <p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/anydesk/2authorization_request.png?raw=true">
</p>

  <p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/anydesk/2Nevilles_login_screen.png?raw=true">
</p>

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/anydesk/2Nevillles_desktop.png?raw=true">
</p>

  
  
### 4.  File Transfer with AnyDesk  


- Click the “File Transfer” button in the AnyDesk toolbar.
- A dual-pane file manager opens.
- Drag and drop files between client and host.
- File access is limited to the permissions of the remote (non-privileged) user.


   
   <p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/anydesk/2anydesk_data_transfer_button.png?raw=true">
</p>
<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/anydesk/2anydesk_data_transfer_interface.png?raw=true">
</p>

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/anydesk/2data_transfer_success.png?raw=true">
</p>


### 5. Security Notes


- No ssh ports or VPN needed.  Anydesk has its own ports.
- Remote user has no sudo/root privileges, unless granted.
- Clipboard sharing disabled unless manually started (e.g. via spice-vdagent)
- No shared folders between VM and host
- Firejail + KVM VM = Strong isolation for the Linux Lite host
- AnyDesk implements rate-limiting for failed login attempts


### 6. Why Not SSH or VPN?

While *SSH* is secure with key-based authentication, it requires:

- Port forwarding
- Dynamic DNS or static IP setup
- Firewall configuration
- Additional software and maintenance

With *AnyDesk*:

- Remote GUI access is quick and beginner-friendly
- No open ports = less exposure
- Great for helping friends/family or remote troubleshooting


### Conclusion

Remote access doesn’t have to be complex. 
With AnyDesk and a few precautions (non-privileged users, sandboxing, and VM isolation), you can 
enable safe, unattended Linux access — without exposing SSH or needing a commercial VPN.


### Further Reading/Resources:

1. Anydesk Official Documentation – Unattended Access:
   https://support.anydesk.com/knowledge/unattended-access
2. Note on supported operating systems: 
   https://support.anydesk.com/knowledge/supported-operating-systems
3. Firejail Documentation:
   https://firejail.wordpress.com/documentation-2/
4. Virt-Manager User Guide (for managing KVM/QEMU VMs):
   https://virt-manager.org/
5. Install and Use Qemu on Ubuntu:
    https://itsfoss.com/qemu-ubuntu/


### Addendum

As far as file transfer is concerned you may have noticed in the respective screenshot that the file
`für_sudo.txt` (which I selected for the action) resides in the folder `/home/rosika2/.anydesk/AnyDesk` on my system.

I deliberately put it there, as my firejailed instance of `anydesk` restricts access to most folders.
So it´s a firejail thing and it has nothing to do with `anydesk` itself.
If you run `anydesk` the non-sandboxed way there´ll be no need to shuffle files around.


### Important Note

#### Wayland vs Xorg Compatibility

At the time of writing, AnyDesk on Linux requires the host machine (the machine being remotely accessed) to use Xorg/X11 for incoming connections.



This is because:
 
- Xorg allows AnyDesk to access the screen contents for remote control
- Wayland, the newer display protocol, has stricter security and doesn’t yet allow screen sharing in the same way. In fact it does not have the networking capabilities of X11.


If your host system uses Xorg (which is the case for most current Debian, Linux Lite, and many other distros), AnyDesk works fine for remote access.
If the host switches to Wayland (more common on some newer distros), AnyDesk may not support incoming connections.

If you encounter problems in the future:

 Check which display server you're using run: `echo $XDG_SESSION_TYPE`.
 It should return `x11` for Xorg or `wayland`.

Many login screens allow you to select an Xorg session if needed.
