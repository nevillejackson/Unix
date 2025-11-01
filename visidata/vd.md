### Using VisiData as a Spreadsheet Alternative ###
Visidata is an Open Source GPL Licensed data tool. You can use it to enter or inspect or modify column-stored data, the same way you would in a spreadsheet. Its home page is here

https://www.visidata.org/

and there is a quality tutorial here

https://jsvine.github.io/intro-to-visidata/

#### Why would one use VisiData rather than LO Calc? ####
 - the display looks like a spreadsheet
 - it fits in with the inter-usability of Linux utilities
 - it uses Python for writing functions. Pythn is vastly superior to VBA.
 - it can handle millions or rows of data
 - it handles 64 data formats, including csv, json and Excel.
 - it does graphics
 - it works across Linux, OS/X and Win
 - there is good documentation
 - it has some vim-like keybindings

#### Installing VisiData ####
In Debian based distros it is available as a package. There seem to be no dependencies.
```
# apt-get install visidata
.....
Need to get 197 kB of archives.
.....
The following NEW packages will be installed:
  visidata
.....
After this operation, 981 kB of additional disk space will be used.
.....
Preparing to unpack .../visidata_2.11-1_all.deb ...
Unpacking visidata (2.11-1) ...
Setting up visidata (2.11-1) ...
Processing triggers for man-db (2.11.2-2) ...
```

If you really must have the latest version, the Github site is here

https://github.com/saulpw/visidata


#### Getting started ####

We might mention the cheat sheet here

https://jsvine.github.io/visidata-cheat-sheet/en/

I have a little .csv test file
```
"Rowno","Tagpros","Slideno","Tagarmid","Dpccperfoll" 
"1"," A8000"," 431/80  "," 71E4012",51.85 
"2"," B0660"," 006/85  "," 71E4533",85.7 
"3"," A7994"," 424/10/1"," 72E4030",124.05
"4"," A8010"," 428/80  "," 72E4078",36.4
```
 To read that into VisiData
```
vd dpcc -f csv
```
Visidata uses file extensions to identify file types. If the file extension is missing ( like here) you have to tell it the filetype with `-f csv`.  Simpler to use extensions,
With that comand I get a terminal window like this

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/visidata/vd1.png?raw=true">
</p>


Lets say I want to edit the data value '36.4' in the bottom row.
I move to that row, and across to the last column...

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/visidata/vd2.png?raw=true">
</p>


Then I enter 'e', the new value, then enter..... done. 

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/visidata/vd3.png?raw=true">
</p>


VisiData does not write on the current file. To save use `Ctrl s filename` ... give the saved file a new name
```
Then to quit `q`
Both files will be there.
I can move around between cells with arrow keys ( or with vi's h,j,k,l keys)

```


#### Make a new table and enter some data ####
I can start am empty table with
```
vd new.vd
```
I get one cell

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/visidata/vd4.png?raw=true">
</p>


Lets say I have some rainfall data .... 2 columns ... Date and Rainfall in mm.
I need to rename the first column, so  '^' , and I get the opportunity to type in 'Date' as its heading.  I need to set the column type to 'data' so '@'.
Then I need to add a column , so 'za' and it lets m name it 'Rainfall', and I can set its type to floating point with '%'.

Now it looks like this

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/visidata/vd5.png?raw=true">
</p>


Now, how can I add some rows of data?

I might save first 
```
Ctrl s new.vd
and enter a filename ... 'new.vd' is mine
```
 I can create some blank rows with 
```
ga 5
```
makes 5 blank rows, then
I can move to the first column, second row , press 'e', and enter 22/10/25 for Date and 15.0 for Rainfall.
When I start editing it changes the table to

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/visidata/vd6.png?raw=true">
</p>


and I cant see how to get back to the normal table?
That is a failure. 
If I have to edit in every value with 'e' , it would simpler to do it in an editor, then read  the file into 'vd'. 
Maybe 'vd' is not meant for data entry?

#### A larger dataset ####
I have a blank separated .csv file where each row is a sheep and columns are either ID's or data such as dermal papilla cell count, bodyweight, woolweight, etc.
First try
```
vd --csv-delimiter=" " dpcc.csv
```
reads it OK, but it adds a column of row numbers at column 1 and gets the headers out of wack. I fix that by adding an extra header to the file. Then it looks like

<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/visidata/dpcc.png?raw=true">
</p>


That is correct now. 
Lets see what we can do with these data
Try to plot DpccperFollicle against wool weight(cww and cww2) and fibre diameter(Diam).
Here are the steps
 - choose the dpcc column
 - % to make it float
 - ! to set it  as the X axis
 - choose the Cww column
 - % to make it numeric
 - also make the cww and cww2 columns numerig
 - g..

and I get

.... dpccgraph.png
<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/visidata/dpccgraph.png?raw=true">
</p>


OK, there is a relationship of dpcc with fibre diameter, but not with wool weight. I am not impressed with either
 - the strange set of key commands
 - the graph quality

but it is quite automatic.

#### Can VisiData work with a pipe ####
Yes VisiData will work with othertools in a pipeline
```
df | vd
```
gives

.... vdpipe.png
<p align="center">
<img  src="https://github.com/nevillejackson/Unix/blob/main/visidata/vdpipe.png?raw=true">
</p>


That is what I meant by "it fits in with the inter-usability of Linux utilities".It is designed to work at the terminal.

#### Conclusion ####
 It may be usefile, especially for converting data file formats, but I am finding it irritating and limited. Maybe others can see some opportunities here?
