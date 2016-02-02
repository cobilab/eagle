#!/bin/bash
# PARAMETERS ==================================================================
INSTALL=1;
DOWNLOAD=1;
###############################################################################
if [[ "$INSTALL" -eq "1" ]]; then
rm -fr goose-* EAGLE goose/ eagle/ ;
# GET GOOSE ===================================================================
git clone https://github.com/pratas/goose.git
cd goose/src/
make
cd ../../
cp goose/src/goose-* .
cp goose/scripts/Get* .
# GET EAGLE ===================================================================
git clone https://github.com/pratas/eagle.git
cd eagle/
cmake .
make
cp EAGLE ../
cd ../
fi
###############################################################################
if [[ "$DOWNLOAD" -eq "1" ]]; then
. GetHumanParse.sh
rm -fr HS-GENOME;
for((x=1; x<28; ++x));
  do 
  cat HS$x | grep -v ">" | tr -d -c "ACGT" >> HS-GENOME;
  done
wget  --trust-server-names -q \
ftp://ftp.ncbi.nlm.nih.gov/genomes/Viruses/Zika_virus_uid36615/NC_012532.fna \
-O ZIKA.fna
cat ZIKA.fna | grep -v ">" | tr -d -c "ACGT" > ZIKA-GENOME;
fi
###############################################################################
./EAGLE -v -t -min 11 -max 15 -r HS-GENOME ZIKA-GENOME
#===============================================================================
