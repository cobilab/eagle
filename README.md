#EAGLE#

<br>
<p align="center"><img src="/logo.png" 
alt="EAGLE" width="350" height="100" border="0" /></p>
<br>

Search for **relative absent words (RAW)** in genomic sequences using a reference sequence. Currently, EAGLE runs on command line linux environment. It reports into files the absent words, in a k-mer range size, as well as the associated positions. EAGLE ca run in a multi-threading mode to minimize computation times.

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

## EXECUTION

### Run EAGLE

Run EAGLE using:

<pre>
./EAGLE -v -min 11 -max 14 -i -r SEQ1 SEQ2
</pre>

for search RAWs with size 11 to 14 that are absent from SEQ1 and present in a specific genomic sequence (SEQ2).

### Run EAGLE on 157 Ebola virus using human as reference

For a current outbreak example, simply run:

<pre>
. RunAll.sh
</pre>

It will download, filter and find RAWs with an output 3D plot.
Nevertheless, make sure that you have gnuplot installed on your Linux box. 
If not use:

<pre>
sudo apt-get install gnuplot
</pre>

to install.
The output will be a 3D interactive plot, such as the following example:

![ScreenShot](/ebola.png)

## PARAMETERS

To see the possible options type
<pre>
./EAGLE
</pre>
or
<pre>
./EAGLE -h
</pre>
These will print the following options:
<pre>
<p>
Usage: EAGLE &#60OPTIONS&#62 ... -r [FILE]  [FILE]:&#60...&#62

  -v                       verbose mode,            
  -a                       about EAGLE,            
  -t                       use multi-threading,            
  -i                       use inversions,           
  -min &#60k-mer&#62             k-mer minimum size,           
  -max &#60k-mer&#62             k-mer maximum size,              
                                                    
  -r  [rFile]              reference file (database),
                                                    
  [tFile1]:&#60tFile2&#62:&#60...&#62  target file(s).</p>         
</pre>

## CITATION ##

On using this software/method please cite:

Raquel M. Silva, Diogo Pratas, Luisa Castro, Armando J. Pinho, Paulo J. S. G. Ferreira (2014). Three minimal sequences found in Ebola virus genomes and absent from human DNA (Submitted).

## ISSUES ##

For any issue let us know at [issues link](https://github.com/pratas/eagle/issues).

## LICENSE ##

GPL v2.

For more information:
<pre>http://www.gnu.org/licenses/gpl-2.0.html</pre>


