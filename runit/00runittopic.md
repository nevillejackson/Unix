## Using the runit init system ##

This post reports investigations into how `runit` is implemented in various 
Linux distributions.

The goal is to learn how runit works and to find the most appropriate way of 
using runit in a home desktop situation.

The post consists of several 'chapters' each written in markdown and copied into 
this itsFOSS topic as a series of separate replies.

### Contents ###
 1. Introduction
 2. How runit works
 3. Distributions which offer runit preinstalled
  3a. Runit in Void Linux
  3b. Runit in Antix LInux
  3c. Runit in Artix Linux
 4. Distributions which offer runit as a choice during install
  4a. Runit in PeppermintOS Devuan Edition
 5. Distributions which offer runit as an optional package.
  5a. Runit inDevuan
  5b. Runit in Debian
  5c. Runit in Gentoo Linux
 6. Recommendations
