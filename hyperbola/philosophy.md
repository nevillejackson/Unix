## What can we learn from Hyperbola Linux? ##
There have been 3 topics in which I have examined

 - how to install Hyperbola

https://itsfoss.community/t/hyperbola-linux-install/10938

 - how to get X windows and a Window Manager running in Hyperbola

https://itsfoss.community/t/getting-x-windows-into-hyperbola-linux/10950

 - how to get a Desktop Environment in Hyperbola

https://itsfoss.community/t/can-hyperbola-linux-support-a-desktop-environment/11061

These 3 steps are things that are generally taken for granted in most Linux distributions. Why so much difficulty in Hyperbola?

The answer is that there are a large number of packages which are not present in Hyperbola, for reasons related to the philosopy of the Free Software Foundation and the GNU Free Software Movement.

https://wiki.hyperbola.info/doku.php?id=en:start#if_hyperbola_is_a_fully_free_distribution_is_it_following_the_gnu_free_system_distribution_guidelines_gnu_fsdg

So the thing that most characterizes Hyperbola is the list of things that are missing.  Lets have a look at these.


## What packages are missing from Hyperbola Linux? ##
There is a full list of available packages here

https://www.hyperbola.info/packages/

and a complete list of packages-not-included here

https://wiki.hyperbola.info/doku.php?id=en:philosophy:incompatible_packages

The missing packages are systemd, dbus, openssl, bluez, xorg, gtk-doc, libcanberra, avahi, zstd, protobuf, libatasmart, java/openjdk, rust, sudo, gettest, mono, nodejs, npm, pip, conan, cargo, flatpak, chromium, qt-webengine, youtube-dl/yt-dlp, electron, midori, gp, cdrtools, kodi, pulseaudio, libhandy, imagemagick, directx-headers, directx-shader-compiler, redis, syslog-ng, libsecret, polkit, udisks2, gvfs, pam, retroarch/libretro, qt-location, php, sbc.

Those are Arch package names, I believe. The full reasons for omitting each package  are given in the above link. For nearly all of them there is a reasonable fully free alternative, eg it uses OpenRC instead of systemd, xenocara instead of Xorg, graphicsmagick instead of imagemagick. 

To give  some idea of the amount of code removed, I recorded the RAM used by Hyperbola and some other distros

Distro |    Ram Used(Mb)
---|---:
Hyperbola as installed |   76
Hyperrbola/IceWM      |   108
Hyperbola/LuminaDE   |    145
Antix/IceWM          |    168
MX21/Xfce              |  610
Debian12/Gnome    |      1412
Mint21/Cinnamon     |    1315

Those are all as installed, without added software. Hyperbola is smaller than Antix, but it is more than 10x smaller than Mint or Debian.

The question of whether Hyperbola can function as a normal desktop  with so many things missing has been answered by the previous 3 topics. There remains the question of what is the point of struggling to do things in this totally free way?

## The real message behind Hyperbola ##
If you study the list of missing packages carefully, and look at the reasons, you will find that it is not just a matter of Licencing and Open Source. There are issues of software quality. Many of the packages on the banned list are the result of coorporate projects which tends to introduce unreasonable complexity and to compromise free software goals. Some are languages (go and rust) and some are package managers (pip and cargo) which tend to undermine the integrity of the main Linux package manager. 

So what they are saying is that the goals of some of these rejected packages tend to compromise the goals of a fully free Linux or a clean coded Linux, that the implementation is poor quality, and that there is unreasonable code bloat. 

So the real message is this. Modern Linux has accumulated a good deal of 'rubbish-ware' and it is time to pause development and conduct a 'pruning' phase. 

## The future for Hyperbola ##
Hyperbola have announced that future releases will use a BSD kernel, instead of LinuxLibre. So we may have our first GNU/BSD distro. That in itself will be interesting, but it does not change the arguments presented above, which are mainly about non-GNU packages, not the kernel.

There is no suggestion that an average Linux user should install Hyperbola and expect it to be better than any other distro. It is not for that, it is for the future.

## Links ##
https://itsfoss.com/hyperbola-linux-review/

https://www.hyperbola.info/

https://wiki.hyperbola.info/doku.php?id=en:manual:beginner_installation

https://wiki.hyperbola.info/doku.php?id=en:philosophy:incompatible_packages

https://www.hyperbola.info/todo/xenocara-migration/


