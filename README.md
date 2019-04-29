<a href="https://zenodo.org/badge/latestdoi/19451188"><img src="https://zenodo.org/badge/19451188.svg" alt="DOI"></a>
[![Build Status](https://travis-ci.org/pratas/eagle.svg?branch=master)](https://travis-ci.org/pratas/eagle)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](LICENSE)

<p align="center"><img src="imgs/logo.png" alt="EAGLE" height="80" border="0" /></p>

<p align="justify">Search for <b>relative absent words (RAWs)</b> in genomic sequences using a reference sequence. Currently, EAGLE runs on a command line environment. It reports into files the absent words, in a k-mer range size, as well as the associated positions. EAGLE can run in a <b>multi-thread mode</b> to minimize computation times.</p>

## INSTALLATION ##

<p align="justify">Cmake is needed for installation (http://www.cmake.org/). You can download it directly from http://www.cmake.org/cmake/resources/software.html or use an appropriate packet manager. In the following instructions we show the procedure to install, compile and create the RAWs: </p>

### STEP 1

Download, install and resolve conflicts.

#### Linux 
<pre>
#sudo apt-get install cmake
git clone https://github.com/pratas/eagle.git
cd eagle/src/
cmake .
make
</pre>
Alternatively, you can install (without cmake and only for linux) using
<pre>
git clone https://github.com/pratas/eagle.git
cd eagle/src/
mv Makefile.linux Makefile
make
</pre>

#### OS X
Install brew:
<pre>
ruby -e "$(curl -fsSL https://raw.github.com/Homebrew/homebrew/go/install)"
</pre>
only if you do not have it. After type:
<pre>
brew install cmake
brew install wget
brew install gcc48
wget https://github.com/pratas/eagle/archive/master.zip
unzip master.zip
cd eagle-master/src/
cmake .
make
</pre>
With some versions you might need to create a link to cc or gcc (after the *brew install gcc48* command), namely
<pre>
sudo mv /usr/bin/gcc /usr/bin/gcc-old   # gcc backup
sudo mv /usr/bin/cc /usr/bin/cc-old     # cc backup
sudo ln -s /usr/bin/gcc-4.8 /usr/bin/gcc
sudo ln -s /usr/bin/gcc-4.8 /usr/bin/cc
</pre>
In some versions, the gcc48 is installed over /usr/local/bin, therefore you might need to substitute the last two commands by the following two:
<pre>
sudo ln -s /usr/local/bin/gcc-4.8 /usr/bin/gcc
sudo ln -s /usr/local/bin/gcc-4.8 /usr/bin/cc
</pre>

#### Windows

<p align="justify">In windows use cygwin (https://www.cygwin.com/) and make sure that it is included in the installation: cmake, make, zcat, unzip, wget, tr, grep (and any dependencies). If you install the complete cygwin packet then all these will be installed. After, all steps will be the same as in Linux.</p>

## EXECUTION

As an example, the objective is to find minimal absent words (RAWs) that appear in a E. coli and not in the assembled (GRC) human chromosome 18, for k-mer sizes between 9 and 13 (including inverted words).

### Get data

EAGLE accepts fasta (http://en.wikipedia.org/wiki/FASTA_format) and seq (ACGTN characters) formats. Therefore, data might be downloaded using a graphical interface or by wget and after decompressed. Above we use wget:

<pre>
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/H_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38_chr18.fa.gz ;
gunzip hs_ref_GRCh38_chr18.fa.gz ;
mv hs_ref_GRCh38_chr18.fa C18.fa ;
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/Bacteria/Escherichia_coli_K_12_substr__DH10B_uid58979/NC_010473.fna ;
mv NC_010473.fna ECOLI.fna ;
</pre>

### Run EAGLE

Run EAGLE using:

<pre>
./EAGLE -v -t -min 9 -max 13 -i -r C18.fa ECOLI.fna
</pre>

It will create files with prefix "ECOLI.fna" followed by the suffix "-k9.eg". The number 9 stands for k = 9. The "-k10.eg" stands for k = 10 and so on. Each file contains the respective(s) RAWs for each k along with the respective positions (the content is ordered by positions). If a file is empty it means that there are no RAWs, nevertheless the running output in the console should indicate something as "RAWs FOUND : 0.0000 % ( 0 in 4753180 )" (for k=8).

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
                                                    
  -r  [rFile]              reference file (db),
                                                    
  [tFile1]:&#60tFile2&#62:&#60...&#62  target file(s).         

EAGLE is a fast method/tool to compute relative MAWs.
The input files should be FASTA (.fa) or SEQ [ACGTN].</p>
</pre>

### Options meaning

| Parameters          | Meaning                                                     |
|---------------------|:------------------------------------------------------------|
| -h                  | It will print the parameters menu (help menu)                                        |
| -v                  | It will print progress information such as number of MAWs, etc.    |
| -a                  | It will print the EAGLE version number, license type and authors.                    |
| -t                  | It will use multiple-threading. The number of threads will be equal to the maximum k-mer less the minimum k-mer. The time to accomplish the task will be much lower, although it will use more memory (memory from each model is cumulative).                   |
| -i                  | Inverted words (reverse complemented) will also be considered. |
| -min &#60;k-mer&#62;   | Size of the minimum k-mer (word size). Possible interval [1;28]. Contexts above 16 will be handled with a hash-table, where the implementation is approximately linear in memory relatively to the size of the sequence. |
| -max &#60;k-mer&#62;   | Size of the maximum k-mer (word size). Possible interval [1;28]. Contexts above 16 will be handled with a hash-table, where the implementation is approximately linear in memory relatively to the size of the sequence. |
| -r [refFile]        | The reference filename. Accepted sequence alphabet [A,C,G,T,N]. |
| [tarFile]           | The target filename(s). For multiple file usage separate by ":". Example: Virus1:Virus2:virus3. Accepted sequence alphabet [A,C,G,T,N]. |


## CITATION ##

On using this software/method, please cite:

Raquel M. Silva, Diogo Pratas, Luísa Castro, Armando J. Pinho & Paulo J. S. G. Ferreira. Bioinformatics (2015): btv189.
DOI: [10.1093/bioinformatics/btv189] (http://doi.org/10.1093/bioinformatics/btv189).

## ISSUES ##

For any issue let us know at [issues link](https://github.com/pratas/eagle/issues).

## LICENSE ##

GPL v3.

For more information:
<pre>http://www.gnu.org/licenses/gpl-3.0.html</pre>


