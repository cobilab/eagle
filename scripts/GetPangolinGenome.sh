#!/bin/bash
wget https://ftp.ncbi.nlm.nih.gov/genomes/all/GCA/000/738/955/GCA_000738955.1_M_pentadactyla-1.1.1/GCA_000738955.1_M_pentadactyla-1.1.1_genomic.fna.gz
gunzip GCA_000738955.1_M_pentadactyla-1.1.1_genomic.fna.gz
mv GCA_000738955.1_M_pentadactyla-1.1.1_genomic.fna PANGOLIN.fna
