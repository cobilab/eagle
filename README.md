<br>
<div align="center">
  
[![Build Status](https://travis-ci.org/cobilab/eagle.svg?branch=master)](https://travis-ci.org/cobilab/eagle)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](LICENSE)
[![Speed](https://img.shields.io/static/v1.svg?label=Ultra-Fast&message=High%20speed%20performance&color=green)](#)
[![Release](https://img.shields.io/static/v1.svg?label=Release&message=v2.1&color=orange)](#)
[![TinyURL](https://img.shields.io/static/v1.svg?label=TinyURL&message=EAGLE&color=blue)](https://tinyurl.com/eagle-tool)

</div>
<br>
<p align="center"><img src="imgs/logo.png" alt="EAGLE" height="90" border="0" /></p>
<br>
<p align="justify">EAGLE is a program to map <b>minimal Relative Absent Words (mRAWs)</b>. EAGLE identifies and localizes the mRAWs contained in a range size of k-mers, running on a command-line environment with multi-threads to minimize computation times. It contains extensions to estimate CG distributions and create automatic plots (Gnuplot). It works on FASTA data without size limitations.</p>

### Installation ###

<p align="justify">CMake must be installed to compile EAGLE. CMake can be downloaded from the CMake webpage (http://www.cmake.org/) or by an appropriate packet manager. The following instructions show the procedure to install and compile EAGLE manually: </p>

```
git clone https://github.com/pratas/eagle.git
cd eagle/src/
cmake .
make
```

External dependencies to download, align and visualize the data requiere conda installation.

Steps to install conda:
```
wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
bash Miniconda3-latest-Linux-x86_64.sh
```
Additional instructions can be found here:
```
https://docs.conda.io/projects/conda/en/latest/user-guide/install/linux.html
```
To install the dependencies using conda:
```
conda install -c cobilab gto --yes
conda install -c bioconda tabix --yes
conda install -c bioconda bowtie2 --yes
conda install -c bioconda samtools --yes
conda install -c bioconda entrez-direct --yes
conda install -c bioconda/label/cf201901 entrez-direct --yes
```

### Run EAGLE ###

Run EAGLE using:

<pre>
./EAGLE -v -t -min 11 -max 14 -p -r Human.fna SARS-CoV-2.fa
</pre>

### Parameters ###

To see the possible options type
<pre>
./EAGLE
</pre>
or
<pre>
./EAGLE -h
</pre>
These will print the following options:
```
                                                                        
             ███████╗  █████╗   ██████╗  ██╗      ███████╗              
             ██╔════╝ ██╔══██╗ ██╔════╝  ██║      ██╔════╝              
             █████╗   ███████║ ██║  ███╗ ██║      █████╗                
             ██╔══╝   ██╔══██║ ██║   ██║ ██║      ██╔══╝                
             ███████╗ ██║  ██║ ╚██████╔╝ ███████╗ ███████╗              
             ╚══════╝ ╚═╝  ╚═╝  ╚═════╝  ╚══════╝ ╚══════╝              
                                                                        
NAME                                                                    
      EAGLE v2.1 2015-2020                                            
      Efficient computation of minimal Relative Absent Words (mRAWs).   
                                                                        
AUTHORS                                                                 
      D. Pratas and J. M. Silva.                                    
                                                                        
SYNOPSIS                                                                
      ./EAGLE [OPTION]... [FILE] [FILE]                                 
                                                                        
SAMPLE                                                                  
      Run: ./EAGLE -v -F -i -min 11 -max 16 human.fa SARS-CoV2.fa       
                                                                        
DESCRIPTION                                                             
      Localization and quantification of minimal Relative Absent Words. 
                                                                        
      -h,  --help                                                       
           usage guide (help menu).                                     
                                                                        
      -V,  --version                                                    
           display program and version information.                     
                                                                        
      -f,  --force                                                      
           force mode. Overwrites old files.                            
                                                                        
      -v,  --verbose                                                    
           verbose mode (more information).                             
                                                                        
      -t,  --threads                                                    
           does NOT use threads if flag is set (slower).                
                                                                        
      -i,  --ignore-ir                                                  
           does NOT use inverted repeats if flag is set.                
                                                                        
      -o,  --stdout                                                     
           write to standard output.                                    
                                                                        
      -p,  --plots                                                      
           print Shell code to generate plots (gnuplot).                
                                                                        
      -min [NUMBER],  --minimum [NUMBER]                                
           k-mer minimum size (usually 10).                             
                                                                        
      -max [NUMBER],  --maximum [NUMBER]                                
           k-mer maximum size (usually 16).                             
                                                                        
      [FILE]                                                            
           Input FASTA reference (e.g. human genome) -- MANDATORY.      
           The content of this file will be loaded in the models.       
                                                                        
      [FILE]                                                            
           Input FASTA target (e.g. multi SARS-CoV2) -- MANDATORY.      
           The mRAWs will be mapped on the content of this file.        
                                                                        
COPYRIGHT                                                               
      Copyright (C) 2015-2020, IEETA/DETI, University of Aveiro.        
      This is a Free software, under GPLv3. You may redistribute        
      copies of it under the terms of the GNU - General Public          
      License v3 <http://www.gnu.org/licenses/gpl.html>. There          
      is NOT ANY WARRANTY, to the extent permitted by law.

```


### Citation ###

Version 2.1:
 * Article under Review


version 1.0:
 * R. M. Silva, D. Pratas, L. Castro, A. J. Pinho & P. J. S. G. Ferreira. Bioinformatics (2015): btv189.
[URL](http://doi.org/10.1093/bioinformatics/btv189).

### Issues ###

For any issue let us know at [issues link](https://github.com/pratas/eagle/issues).

### License ###

GPL v3.

For more information:
<pre>http://www.gnu.org/licenses/gpl-3.0.html</pre>


