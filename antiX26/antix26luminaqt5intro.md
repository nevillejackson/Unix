### antiX26 with Lumina desktop : the ISO ###

The Lumina Desktop Environment was developed in the BSD Unix world, and was used in PC-BSD and TrueOS . It is portable and has previously been available in various Linux distros, but its development has stalled for about 4 years and it has been withdrawn from most repos. 

I feel it is a good fit for a small DE in antiX, so I have gone to the trouble of compiling Lumina, so we can see how it performs in antiX26.

The Official Lumina website is here

https://lumina-desktop.org/features/index.html

and the source code is available here

https://github.com/lumina-desktop/lumina

What Lumina offers is a very configurable desktop, that is portable and does not depend on systemd or Dbus or Polkit. It can be configured to look like Xfce or Windows anything you prefer.

itsfoss.community/t/lumina-desktop-environment/9034

I decided that a configurable desktop with a small footprint might be useful in Antix so I compiled Lumina in antiX26, addressed some of the configuration issues, and made an .iso file with both a live antiX and an installer.

It looks like this

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/antiX26/luminastaryup.png?raw=true">
</p>


My .iso defaults to the dinit init system, but all 5 init systems are available, as in official antiX26.

The .iso is both a live antiX and an install iso. The live login is demo:demo for user and root:root for superuser. Use the `Install AntiX` icon to start the `Gazelle` Installer. Note the if you modify the live system, the modifications will be installed. 

In case someone wishes build Lumina rather than use my .iso, I include my build steps and configurations in the following replies.

I hope some may find this useful. Please inform me of any issues. 
