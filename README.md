#EAGLE#

![ScreenShot](/logo.png)

Search for **relative absent words (RAW)** in genomic sequences using a reference sequence.
It builds an image with patterns reporting the absent words regions as well as report the position into a file.

## INSTALLATION ##

Simply run the following instructions at a Linux terminal:

<pre>
wget https://github.com/pratas/eagle/archive/master.zip
unzip master.zip
cd eagle-master
make cleanall 
make
</pre>

Note that these instructions will install EAGLE and several minimal programs to 
cope with specific needs, such as 3D visualization on Gnuplot.

### Run 99 Ebola virus using human as reference

Simply type:

<pre>
. RunAll.sh
</pre>

Nevertheless, make sure that you have gnuplot installed on your Linux box. 
If not use:

<pre>
sudo apt-get install gnuplot
</pre>

to install.


