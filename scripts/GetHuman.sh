#!/bin/bash
#rm -f GCF_000001405.39_GRCh38.p13_genomic.fna.gz
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/Homo_sapiens/current/GCF_000001405.39_GRCh38.p13/GCF_000001405.39_GRCh38.p13_genomic.fna.gz 
gunzip GCF_000001405.39_GRCh38.p13_genomic.fna.gz
mv GCF_000001405.39_GRCh38.p13_genomic.fna HS.fna
