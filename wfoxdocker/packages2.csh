#! /bin/csh
apt-file  --fixed-string -l search --from-file libfiles.txt  >>packages2.txt
