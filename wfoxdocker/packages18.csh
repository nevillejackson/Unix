#! /bin/csh
foreach lib (`cat list18libsdeps.txt3`)
 apt-file -l search $lib >>& package18.txt
end
