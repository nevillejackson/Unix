#! /usr/bin/csh
foreach lib (`cat /home/nevj/waterfox/list18libs.txt`)
  ldd $lib >>& /home/nevj/waterfox/list18libsdeps.txt
end
