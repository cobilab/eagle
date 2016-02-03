#!/bin/bash
###############################################################################
# MINIMAL SEQUENCES FOUND IN ZIKA VIRUS GENOME AND ABSENT FROM HUMAN DNA (GRC)
###############################################################################
# PARAMETERS ==================================================================
INSTALL=1;
DOWNLOAD=1;
EAGLE=1;
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
cp goose/scripts/Down* .
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
rm -fr HS-GENOME REPORT-SPLIT;
for((x=1; x<28; ++x));
  do 
  cat HS$x | grep -v ">" | tr -d -c "ACGT" >> HS-GENOME;
  done
perl DownloadZika.pl
(./goose-splitreads < zika.fa ) &> REPORT-SPLIT;
NZIKAS=`cat REPORT-SPLIT | tail -n 1 | awk '{print $1;}'`;
for((x=1; x<=$NZIKAS ; ++x));
  do
  #cat zika.fa | grep -v ">" | tr -d -c "ACGT" > ZIKA-GENOME;
  echo "cat out$x.fa";
  done
fi
###############################################################################
if [[ "$EAGLE" -eq "1" ]]; then
./EAGLE -v -i -t -min 11 -max 15 -r HS-GENOME ZIKA-GENOME
fi
# cat ZIKA-GENOME-k12.eg
#===============================================================================
