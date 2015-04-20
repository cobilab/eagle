#!/bin/sh
# =============================================================================
# GET GRC HUMAN GENOME
for((x=1;x!=23;++x)); do wget ftp://ftp.ncbi.nlm.nih.gov/genomes/Homo_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38.p2_chr$x.fa.gz ; done
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/Homo_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38.p2_chrX.fa.gz
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/Homo_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38.p2_chrY.fa.gz
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/H_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38.p2_chrMT.fa.gz
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/H_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38.p2_unplaced.fa.gz
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/H_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38.p2_unlocalized.fa.gz
for((x=1;x!=23;++x)); do zcat hs_ref_GRCh38_chr$x.fa.gz | grep -v ">" | tr -d -c "ACGTN" > GRC$x; done
zcat hs_ref_GRCh38.p2_chrX.fa.gz | grep -v ">" | tr -d -c "ACGTN" > GRC23 ;
zcat hs_ref_GRCh38.p2_chrY.fa.gz | grep -v ">" | tr -d -c "ACGTN" > GRC24 ;
zcat hs_ref_GRCh38.p2_chrMT.fa.gz | grep -v ">" | tr -d -c "ACGTN" > GRC25 ;
zcat hs_ref_GRCh38.p2_unplaced.fa.gz | grep -v ">" | tr -d -c "ACGTN" > GRC26 ;
zcat hs_ref_GRCh38.p2_unlocalized.fa.gz | grep -v ">" | tr -d -c "ACGTN" > GRC27 ;
cat GRC* > HG;
rm -f *.fa.gz ;
rm -f GRC*
# =============================================================================
