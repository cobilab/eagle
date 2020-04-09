#!/bin/bash
rm -f CORONAS.fa
idArray=("NC_045512" "NC_004718.3" "NC_019843.3" "NC_006577.2" "NC_005831.2" "NC_006213.1" "NC_002645.1"); 
# NC_045512   SARS-CoV-2
# NC_004718.3 SARS
# NC_019843.3 MERS
# NC_006577.2 HKU
# NC_005831.2 NL63
# NC_006213.1 OC43
# NC_002645.1 229E
for id in ${idArray[@]}; do
  echo "Downloading $id ...";
  efetch -db nucleotide -format fasta -id "$id" >> CORONAS.fa;
done
gto_fasta_split_reads < CORONAS.fa;
mv out1.fasta SARS-CoV-2.fa
mv out2.fasta SARS.fa
mv out3.fasta MERS.fa
mv out4.fasta HKU.fa
mv out5.fasta NL63.fa
mv out6.fasta OC43.fa
mv out7.fasta 229E.fa
