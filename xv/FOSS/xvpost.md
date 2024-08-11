### Resurrecting xv .. the original Linux image viewer ###

I have some old image manipulation software which uses the `xv` image viewer . It is not available in Debian , Arch or Void repos, so I decided to track down the source code and compile it. 

I obtained the source code from here

https://github.com/haegar/xv

It is beautiful clean coded C written by John Bradley back in the 1990's.  
It compiles virtually without modification on a modern 64 bit system, with one exception

The routine `xvpng.c`  makes calls to the PNG image library 
`libpng` which has changed its API since `xv` was written. 
Considerable modification of `xvpng.c` was needed. 
Otherwise, it is an easy compile, and I have a binary ( called `xv`) which seems to run correctly.

Here is what it looks like, if you type `xv` with no parameters you get

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/xv/FOSS/xv1.png?raw=true">
</p>

and you then have to right click on that window to get the control menu

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/xv/FOSS/xv2.png?raw=true">
</p>

and from the control menu you can use `Load`  to load an image

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/xv/FOSS/xv3.png?raw=true">
</p>

I have selected a .png image in my ~/Pictures folder.
If I press OK , I get the image displayed

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/xv/FOSS/xv4.png?raw=true">
</p> 


However that is the pedestrian way to use it.  If you type 
`xv imagename`
it immediately brings up the image display menu. 
and, if you want the control menu , right click on the image display. 

Some things the control menu can do

 - crop
 - rotate and mirror
 - filtering algorithms
 - add text to image
 - put image on the root window
 - report image info
 - resize the display window
 - change color modes.
 - save image to various formats

 I have tested reading the following image file types

 - pbm
 - pgm
 - png
 - rast ( sun rasterfile)
 - tiff
 - jpeg

they all work for me. 

#### Getting `xv` ####

If you would like to try `xv` there are several options

1. Download my x86-64 binary from my github repo

https://github.com/nevillejackson/Unix/tree/main/xv

You will find  there all the source code files (*.c and *.h), plus a file called 'xv' which is an x86_64 binary. It is 1.08Mb. If you download it in raw mode, change its permissions to execute (`chmod 755 xv`), and place it somehere like ~/bin or /usr/local/bin, it should execute when you type `xv`.
As always, if you download any binary, virus scan it.
`xv` does have dependencies. Here is the complete list of libraries needed for startup
```
$ ldd ./xv
	linux-vdso.so.1 (0x00007ffe11b85000)
	/usr/lib/x86_64-linux-gnu/libgtk3-nocsd.so.0 (0x00007f72e1119000)
	libtiff.so.6 => /usr/lib/x86_64-linux-gnu/libtiff.so.6 (0x00007f72e1069000)
	libjpeg.so.62 => /usr/lib/x86_64-linux-gnu/libjpeg.so.62 (0x00007f72e0fd6000)
	libpng16.so.16 => /usr/lib/x86_64-linux-gnu/libpng16.so.16 (0x00007f72e0fa0000)
	libz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x00007f72e0f81000)
	libX11.so.6 => /usr/lib/x86_64-linux-gnu/libX11.so.6 (0x00007f72e0abe000)
	libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f72e09df000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f72e07fe000)
	libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f72e0f7a000)
	libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f72e0f75000)
	libwebp.so.7 => /usr/lib/x86_64-linux-gnu/libwebp.so.7 (0x00007f72e078c000)
	libzstd.so.1 => /usr/lib/x86_64-linux-gnu/libzstd.so.1 (0x00007f72e06d0000)
	liblzma.so.5 => /lib/x86_64-linux-gnu/liblzma.so.5 (0x00007f72e06a1000)
	libLerc.so.4 => /usr/lib/x86_64-linux-gnu/libLerc.so.4 (0x00007f72e0609000)
	libjbig.so.0 => /usr/lib/x86_64-linux-gnu/libjbig.so.0 (0x00007f72e0f62000)
	libdeflate.so.0 => /usr/lib/x86_64-linux-gnu/libdeflate.so.0 (0x00007f72e05e3000)
	/lib64/ld-linux-x86-64.so.2 (0x00007f72e1124000)
	libxcb.so.1 => /usr/lib/x86_64-linux-gnu/libxcb.so.1 (0x00007f72e05b9000)
	libstdc++.so.6 => /usr/lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f72e0200000)
	libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f72e0599000)
	libXau.so.6 => /usr/lib/x86_64-linux-gnu/libXau.so.6 (0x00007f72e0f5b000)
	libXdmcp.so.6 => /usr/lib/x86_64-linux-gnu/libXdmcp.so.6 (0x00007f72dfe00000)
	libbsd.so.0 => /usr/lib/x86_64-linux-gnu/libbsd.so.0 (0x00007f72e0583000)
	libmd.so.0 => /usr/lib/x86_64-linux-gnu/libmd.so.0 (0x00007f72e0576000)

```
Those would be present in most full Linux installs ( my Debian and MX have all these present) , but if you have some minimal Linux install, some of these libraries may be missing and would need to be installed.

2. Compile `xv` yourself. 
You may want to do this if you want `xv` for a different architecture. 
You could start with my source code from my github repo

https://github.com/nevillejackson/Unix/tree/main/xv

or you could start from the original source from which I obtained code
https://github.com/haegar/xv

or there is one other site which keeps `xv` code

http://www.trilon.com/xv/downloads.html

and they also have a binary i386 architecture in an rpm file ( I have no idea how old this binary is).

3. There is a snap download available

https://snapcraft.io/xv

I have not tried it

4. OpenSUSE has xv as a package

https://software.opensuse.org/package/xv

#### Documentation ####
`xv` comes with a very comprehensive manual. It is in the `doc` subdirectory on my github site

https://github.com/nevillejackson/Unix/tree/main/xv/docs

The file is `xvdocs.pdf`.
When you try to view it  on Github, it says it cant read the .pdf file, but if you download it ( go to ..., then Download) it will download to your Downloads directory, and a display will start using your local .pdf viewer. 


