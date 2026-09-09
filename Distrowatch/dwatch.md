#### Distrowatch hit counts and rankings ####
  There are two sorts of data available on distrowatch
 - counts of hits per day to the web page of each distro
   `https://distrowatch.com/dwres.php?resource=popularity`
 - project rankings. These are voluntary rantings on a scale of 1 to 10 by users.
    They report average rating and the number of people offering ratings, for each distro
   `https://distrowatch.com/dwres.php?resource=ranking&sort=votes`

The hits counts are not ideal, as has been often discussed elsewhere, because 
they do not necessarily reflect whether a person is using a distro. 
One would , however,  expect that if a person is motivated enough to do a ranking,  
then they are likely to be using the distro. So the ratings count may relate to actual usage. 

The numbers of users doing ratings reflect something similar to the 'hits' list ... the top 5 distros sorted on number of votes are
 - Mint
 - MX-Liux
 - openSUSE
 - Debian
 - CachyOS

The average ratings are mysterious. If one sorts the data in order of average rationg ( for distroxs with >100 votes) the top 5 distros are
 - Artix
 - Arch
 - Void
 - TrueNAS

I wanted to see if these data could reveal anything else about how people rate Linux distros.
I downloaded the table data into R, and found a number of interesting features.

##### Histograms of hits per day data #####
A histogram shows how hits counts vary across the whole population of Linux and BSD distros.
The hits table has 4 entries ... counts averaged ove last 1 month, last 3 months, last 6 months and last 9 months.
I did a histogram for each

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/Distrowatch/hpd.png?raw=true">
</p>

The counts have been transformed to logarithms ( I used logs to base 10 so one can easily relate them to actual counts ... eg 2.0 is $10^2$ which is a count of 100.

There is one notable feature  ... the 1 month histogram has 2 peaks , one at counts of 10-50, the other at counts of 80-100.  The lower peak gradually disappears as one averages over more months. 
There is some temporary phemnomenon in the last 1 month data. What this points to is that the last 12 months data is the most reliable.

 - Slackware
 and Mint is No 10.  So users of those 5 'niche' distros really love what they have 
and give a high ranking.


##### Histograms of ratings data #####
There were ratings data for 468 distros.  I presume those missing had zero votes and therefor no rating. 
I did a histogram for Average Rating and for NumberofVotes.

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/Distrowatch/rating.png?raw=true">
</p>


There very few low ratings, and there are 100 distros with only one vote. The disdtribution of reting means is skewed. The counts have been log transformed, as above for hits.

We have 2 variables so we should look at if they are correlated.

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/Distrowatch/ratvot.png?raw=true">
</p>

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/Distrowatch/ratvotad.png?raw=true">
</p>



There is a pattern... distros with a high number of votes always receive a high average rating and ar emostly 'Active".
So the top left corner of the scatterplot is unoccupied.   Distros with a low number of votes can receive any rating.
Dead distros ( discontinued or dormant) tend to have less votes, but follow the same pattern.  So the scatterplot is not disrupted by including "dead" distros.


If we look at the "Family" distros belong to ( eg Debian and derivatives is one Family) we see the same pattern for all families

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/Distrowatch/ratvotf.png?raw=true">
</p>


The only thing I can see is that the Void  family does not receive any low Mean Ratings. All the other families are spead everywhere in thesame triangular pattern. I suspect that is due to the type of users Void and Gentoo attract.

##### Relation between Mean Rating and Hits per Day #####
 We can merge the hits and ratings datafiles and see if hits and ratings relate.

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/Distrowatch/rathits.png?raw=true">
</p>


Hits and ratings are two different things. Distros with a high number of hits per day get a high rating, but distros with a low number of hits per day can get any rating.

##### Relation between  No of Ratings and Hits per Day #####
No of hits and No of ratings are correlated

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/Distrowatch/nrathits.png?raw=true">
</p>


So rating counts and hit counts measure the same thing , probably 'interest' or 'popularity'.


##### Do Mean Ratings differ between Families? #####
There seem to be small differences

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/Distrowatch/ratbox.png?raw=true">
</p>


In reading a boxplot, the heavy line is the mean, the box represents 95% of values, the whiskers represent ... and the circle points represent outliers.
The Debian family has a lot of outliers with low ratings. Void and BSD families get the highest mean ratings
The differences are not large. Family has little to do with it.

##### Does number of ratings vary between Families? #####
Fedora, Debian and BSD families seem to attract more ratings.

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/Distrowatch/noratbox.png?raw=true">
</p>


##### Does hits per day vary between Families? #####
Only Fedora fanily seems to differ in mean hits per day.

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/Distrowatch/hpdbox.png?raw=true">
</p>


##### Conclusion #####
There is not much in it. Which family a distro belongs to does not seem to explain much of the variation in hits or ratings. 

I did discover something . If you consider a rating to be a score, you can calculate the total score across all scorers by 'RatingsMean * No of Ratings', and this total score correlates with number od Hits per Day.

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/Distrowatch/totalscore.png?raw=true">
</p>


It should be a log/log plot.

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/Distrowatch/logtotalscore.png?raw=true">
</p>


So Ratings does not contribute much over Hits per Day, when totalled.
If we look at families in this plot

<p align="center">
<img src="https://github.com/nevillejackson/Unix/blob/main/Distrowatch/logtotalscorefamily.png?raw=true">
</p>


there is still nothing of note. 

So my final recommendation ... if you want to look at distrobox rankings , just look at hits per day, the ratings do not add anything.
