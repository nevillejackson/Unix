#! /bin/csh
foreach lib (`cat libfiles.txt`)
 apt-file --fixed-string -l search $lib >>& package.txt
end
