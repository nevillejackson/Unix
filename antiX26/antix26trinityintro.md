This is my implementation of antiX26 with Trinity Desktop (TDE)  with turnstile login daemon and slimski Login Manager.

It complements the turnstile implementation of Xfce Desktop documented here

https://itsfoss.community/t/antix26-with-xfce-the-iso/15925

    I have spent some time deciding on the best way to implement DE's in antiX26.. I have concluded that the best option is to retain both turnstile and slimski , because that is minimal disturbance of the default antiX26, and all the WM’s and init systems continue to work. 

   Trinity requires some extra work compared to Xfce. The Dolphin File Manager needs to be installed and configured.

I have made an .iso file , and it is available at sourceforge

https://sourceforge.net/projects/antix26-with-a-de/files/


It corrects the partition mounting and reboot/halt issues with raw Trinity, and have provided an altenate File Manager Dolphin.

The following replies contain some howto info … for those DIY enthusiasts and as documentation.

I would like to acknowledge assistance from @berninghausen and @ProwlerGr .

I hope that the Trinity version may be helpful to someone wanting to start in antiX with a  Qt-based DE, instead of the default WM’s.
Trinity is based on a fork of KDE3 and it includes its own forked copy of Qt, so it is independent of Qt versions.

antiX with Trinity has a slightly smaller memory ( ie ram) footprint than antiX with Xfce. The figures are

```
Ram usage at idle as reported by Conky in a 4Mb VM
 - antiX26 + zzz-IceWM                  230-240 M
 - antiX26 + Xfce ( turnstile version)  520-530 M
 - antiX26 + Xfce ( consolekit version) 500-510 M
 - antiX26 + Trinity ( with turnstile)  440-450 M
```


Please report any issues.
