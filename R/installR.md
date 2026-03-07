### Installing R from source download in Linux ###

In fixed release distros I always install R from source, because the distro packages are never up to date and they often do not include important libraries such as `libopenblas`. Rolling release distros usually have R up to date. 

I am putting this on the forum because, although very few forum members may be interested in R,  
it is a typical examle of the steps involved in installing a C program from source in Linux. R acrtually uses Fortran , as well as C.

#### Download ####
The sources are here

https://cran.r-project.org/

and the instructions for unpacking and installing are here

https://cran.r-project.org/doc/manuals/r-patched/R-admin.html#Getting-and-unpacking-the-sources

I downloaded R-4.5.2.tar.gz

I made a directory to work in ( /usr/local/src/R) and unpacked the download as follows

`gzip -dc R-4.5.2.tar.gz | tar xf -`

It makes a diretory `R-4.5.2`, so `cd R-4.5.2`, and I am ready to work

#### Configure ####
The `R-4.5.2` directory contains the following
```
nevj@trinity:~/R/R-4.5.2
$ ls -F
ChangeLog   config.site   COPYING  INSTALL      Makefile.fw  README  SVN-REVISION  VERSION
config.log  configure*    doc/     m4/          Makefile.in  share/  tests/        VERSION-NICK
config.out  configure.ac  etc/     Makeconf.in  po/          src/    tools/
```
There is a binary called `configure` . We use that to check that my MX26 Linux has all the packages necessary for an R build.
```
./configure >&config.out 
```

Now , a minor deviation.... I need `view` , which is part of vi, to look at the file config.out. 
There is a name clash with Midnight Commander ... I need to use `update-alternatives`
```
root@trinity:/etc/alternatives# update-alternatives --config view
There are 2 choices for the alternative view (providing /usr/bin/view).

  Selection    Path               Priority   Status
------------------------------------------------------------
* 0            /usr/bin/mcview     25        auto mode
  1            /usr/bin/mcview     25        manual mode
  2            /usr/bin/vim.tiny   15        manual mode

Press <enter> to keep the current choice[*], or type selection number: 2
update-alternatives: using /usr/bin/vim.tiny to provide /usr/bin/view (view) in manual mode
```
Now I can use `view ` to look at config.out
```
checking build system type... x86_64-pc-linux-gnu
checking host system type... x86_64-pc-linux-gnu
loading site script './config.site'
loading build-specific script './config.site'
checking for pwd... /usr/bin/pwd
checking whether builddir is srcdir... yes
checking whether ln -s works... yes
checking for ar... ar
checking for a BSD-compatible install... /usr/bin/install -c
checking for sed... /usr/bin/sed
checking for which... /usr/bin/which
checking for less... /usr/bin/less
checking for gtar... no
checking for gnutar... no
checking for tar... /usr/bin/tar
checking for tex... /usr/bin/tex
checking for pdftex... /usr/bin/pdftex
checking for pdflatex... /usr/bin/pdflatex
checking for makeindex... /usr/bin/makeindex
checking for texi2any... no
configure: WARNING: you cannot build info or HTML versions of the R manuals
checking for texi2dvi... no
checking for kpsewhich... /usr/bin/kpsewhich
......
```
There is the first missing package ..... I need to find what package supplies `texi2any` and `texi2dvi`.
There are 2 ways
 - use `apt-file` command
 - use an internet search or an AI summary


`apt-file` seems to be installed in MX26 , so 
```
root@trinity:/etc/alternatives# apt-file search texi2any
manpages-de: /usr/share/man/de/man1/texi2any.1.gz
manpages-ja: /usr/share/man/ja/man1/texi2any.1.gz
manpages-ro: /usr/share/man/ro/man1/texi2any.1.gz
texinfo: /usr/bin/texi2any
texinfo: /usr/share/info/texi2any_api.info.gz
texinfo: /usr/share/info/texi2any_internals.info.gz
texinfo: /usr/share/man/man1/texi2any.1.gz
```
There it is .... it is in texinfo package.... so is texi2dvi

Or, if I use Google 

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/R/aisearch.png?raw=true">
</p>


I get the same answer.

So lets install texinfo
```
root@trinity:/etc/alternatives# apt-get install texinfo
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
The following additional packages will be installed:
  libtext-unidecode-perl libxml-libxml-perl libxml-namespacesupport-perl libxml-sax-base-perl
  libxml-sax-perl texinfo-lib
Recommended packages:
  libxml-sax-expat-perl
The following NEW packages will be installed:
  libtext-unidecode-perl libxml-libxml-perl libxml-namespacesupport-perl libxml-sax-base-perl
  libxml-sax-perl texinfo texinfo-lib
0 upgraded, 7 newly installed, 0 to remove and 0 not upgraded.
Need to get 2,490 kB of archives.
After this operation, 13.0 MB of additional disk space will be used.
Do you want to continue? [Y/n] y
......

(frontend:7340): Gtk-WARNING **: 21:27:30.675: cannot open display: 
debconf: unable to initialize frontend: Gnome
debconf: (DISPLAY problem?)
debconf: falling back to frontend: Dialog
Replacing config file /etc/perl/XML/SAX/ParserDetails.ini with new version
Setting up texinfo (7.1.1-1) ...
Running mktexlsr. This may take some time. ... done.
Processing triggers for man-db (2.13.1-1) ...
Processing triggers for tex-common (6.19) ...
Running mktexlsr. This may take some time... done.
Processing triggers for install-info (7.1.1-1+b1) ...
root@trinity:/etc/alternatives# 
```
Not sure what those DISPLAY issues are? But it installed it. 
I already have Latex and Tex installed ,  or therr would hav ebeen more packages.


So now , view my config.out file again and find the next error
```
looking for a modern Fortran compiler
checking for gfortran... no
checking for g95... no
checking for xlf95... no
checking for f95... no
checking for fort... no
checking for ifort... no
checking for ifc... no
checking for efc... no
checking for pgfortran... no
checking for pgf95... no
checking for lf95... no
checking for ftn... no
checking for nagfor... no
checking for xlf90... no
checking for f90... no
checking for pgf90... no
checking for pghpf... no
checking for epcf90... no
checking for g77... no
checking for xlf... no
checking for f77... no
checking for frt... no
checking for pgf77... no
checking for cf77... no
checking for fort77... no
checking for fl32... no
checking for af77... no
checking whether the compiler supports GNU Fortran... no
checking whether  accepts -g... no
configure: error: No Fortran compiler found
```

So it cant find a Fortran compiler, and this time it is fatal.

The Fortran compiler in Debian based systems is `gfortran`, so install it
```
root@trinity:~# apt-get install gfortran
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
The following additional packages will be installed:
  gfortran-14 gfortran-14-x86-64-linux-gnu gfortran-x86-64-linux-gnu libgfortran-14-dev
Suggested packages:
  gfortran-multilib gfortran-doc gfortran-14-multilib gfortran-14-doc libcoarrays-dev
The following NEW packages will be installed:
  gfortran gfortran-14 gfortran-14-x86-64-linux-gnu gfortran-x86-64-linux-gnu libgfortran-14-dev
0 upgraded, 5 newly installed, 0 to remove and 0 not upgraded.
Need to get 12.6 MB of archives.
After this operation, 42.6 MB of additional disk space will be used.
....
Setting up gfortran (4:14.2.0-1) ...
update-alternatives: using /usr/bin/gfortran to provide /usr/bin/f95 (f95) in auto mode
update-alternatives: using /usr/bin/gfortran to provide /usr/bin/f77 (f77) in auto mode
Processing triggers for man-db (2.13.1-1) ...
```

So now I have to re-run `./configure` and find the next problem.
`./configure >&config.out2`
We shall not go through all thedetail again. 
Here is a list of missing packages I had to install
```
texinfo		fonts-inconsolata	texlive-fonts-extra
gfortran
libreadline8t64	libreadline-dev	readline-common	readline-doc
libx11-dev	libxau-dev	libxcb1-dev	
xorg-sgml-doctools	xtrans-dev
libxt-dev	libxmu-dev
tcl-dev		tk	tk-dev
libbz2-dev	liblzma-dev
libpcre2-dev
libcurl4-gnutls-dev	libcurlpp-dev	libcurlpp0	
gobjc
```
This list would vary with distro and if you already have stuff installed.

The `./configure` is now successful, and its output ends with the statement
```
R is now configured for x86_64-pc-linux-gnu

  Source directory:            .
  Installation directory:      /usr/local

  C compiler:                  gcc -std=gnu23  -g -O2
  Fortran fixed-form compiler: gfortran  -g -O2

  Default C++ compiler:        g++ -std=gnu++17  -g -O2
  Fortran free-form compiler:  gfortran  -g -O2
  Obj-C compiler:              gcc -g -O2 -fobjc-exceptions

  Interfaces supported:        X11, tcltk
  External libraries:          pcre2, readline, curl, libdeflate
  Additional capabilities:     PNG, JPEG, TIFF, NLS
  Options enabled:             shared BLAS, R profiling, libdeflate for lazyload

  Capabilities skipped:        cairo, ICU
  Options not enabled:         memory profiling

  Recommended packages:        yes
```
So it still needs cairo and ICU, if I want those capabilities, and it is using shared BLAS... I need OpenBLAS.... so I need more libraries and I have to rerun `./configure` with some optional parameters.
 First the libraries
```
[libjpeg-dev	libtiff-dev]	already installed
libcairo2-dev
libblas3	libblas64-3	libblas-dev	libblas64-dev
libopenblas0	libopenblas64-0
libopenblas-dev	libopenblas64-dev

```
Then I have to rerun `./configure` with options
```
./configure --prefix=/usr/local --enable-threads --enable-openmp --with-blas --with-lapack >&config.out2

.....
R is now configured for x86_64-pc-linux-gnu

  Source directory:            .
  Installation directory:      /usr/local

  C compiler:                  gcc -std=gnu23  -g -O2
  Fortran fixed-form compiler: gfortran  -g -O2

  Default C++ compiler:        g++ -std=gnu++17  -g -O2
  Fortran free-form compiler:  gfortran  -g -O2
  Obj-C compiler:              gcc -g -O2 -fobjc-exceptions

  Interfaces supported:        X11, tcltk
  External libraries:          pcre2, readline, BLAS(OpenBLAS), LAPACK(in blas), curl, libdeflate
  Additional capabilities:     PNG, JPEG, TIFF, NLS, cairo
  Options enabled:             R profiling, libdeflate for lazyload

  Capabilities skipped:        ICU
  Options not enabled:         shared BLAS, memory profiling

  Recommended packages:        yes
```
This time we are getting serious. It will use OpenBLAS and it has cairo. I decided to ignore ICU ... dont even know what it does.  So we are ready to build R. The `./configure` process builds the Makefile.

#### Building R as configured ####
I have to set compiler flags to use the openmp libraries. This is done in the file `config.site`

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/R/openmpflags.png?raw=true">
</p>


and, I rerun `./configure` .... not sure if this is necessary.

Then we can do the build... that is easy
```
make prefix=/usr/local -j6 >&make.out
```
It took 5min 9sec real time. 
The `make.out` file looks OK.
There is a check procedure
```
make check -j6 >&makecheck.out
```
It passed. 
I have to build manuals
```
make pdf
```
Then I can install
```
make prefix=/usr/local install

```

OK finished.
```
$ which R
/usr/local/bin/R
```
It is installed!
Does it run?
```
nevj@trinity:~/Rwork
$ R

R version 4.5.2 (2025-10-31) -- "[Not] Part in a Rumble"
Copyright (C) 2025 The R Foundation for Statistical Computing
Platform: x86_64-pc-linux-gnu

R is free software and comes with ABSOLUTELY NO WARRANTY.
You are welcome to redistribute it under certain conditions.
Type 'license()' or 'licence()' for distribution details.

  Natural language support but running in an English locale

R is a collaborative project with many contributors.
Type 'contributors()' for more information and
'citation()' on how to cite R or R packages in publications.

Type 'demo()' for some demos, 'help()' for on-line help, or
'help.start()' for an HTML browser interface to help.
Type 'q()' to quit R.

> sessionInfo()
R version 4.5.2 (2025-10-31)
Platform: x86_64-pc-linux-gnu
Running under: Debian GNU/Linux 13 (trixie)

Matrix products: default
BLAS/LAPACK: /usr/lib/x86_64-linux-gnu/openblas-pthread/libopenblasp-r0.3.29.so;  LAPACK version 3.12.0

locale:
 [1] LC_CTYPE=en_AU.UTF-8       LC_NUMERIC=C              
 [3] LC_TIME=en_AU.UTF-8        LC_COLLATE=en_AU.UTF-8    
 [5] LC_MONETARY=en_AU.UTF-8    LC_MESSAGES=en_AU.UTF-8   
 [7] LC_PAPER=en_AU.UTF-8       LC_NAME=C                 
 [9] LC_ADDRESS=C               LC_TELEPHONE=C            
[11] LC_MEASUREMENT=en_AU.UTF-8 LC_IDENTIFICATION=C       

time zone: Australia/Sydney
tzcode source: system (glibc)

attached base packages:
[1] stats     graphics  grDevices utils     datasets  methods   base     

loaded via a namespace (and not attached):
[1] compiler_4.5.2
> 
> 1+3
[1] 4
> 
> q()
Save workspace image? [y/n/c]: n
nevj@trinity:~/Rwork
```
So it loads, and it is using openblas-pthread.  Other options are 'openmp' and 'serial'. I can switch between them with update-alternatives.

There is more work to do  .... R has its own package system .  I need to load some packages , but that can come later. 

#### Discussion ####
I hope this gives some idea of the steps involved in a typical install of C software from source code. In its simples form it comes down to 
```
unpack
./configure
make
```
but, as you have seen here there can be complicating side issues.
There are other build systems ( eg CMake) but the above is tyoical of GNU software.

