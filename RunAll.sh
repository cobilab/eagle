#!/bin/bash
# =============================================================================
# DOWNLOAD AND INSTALL PROGRAMS
rm -fr eagle-master EAGLESRC*
wget "https://github.com/pratas/eagle/archive/master.zip" -O EAGLESRC.zip ;
unzip EAGLESRC.zip ;
cd eagle-master/ 
make cleanall ; make ;
cp eagle mink rebat projector ../
cd ..
rm -fr eagle-master EAGLESRC*
# =============================================================================
# GET GRC HUMAN GENOME
for((x=1;x!=23;++x)); do wget ftp://ftp.ncbi.nlm.nih.gov/genomes/Homo_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38_chr$x.fa.gz ; done
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/Homo_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38_chrX.fa.gz
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/Homo_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38_chrY.fa.gz
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/H_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38_chrMT.fa.gz
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/H_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38_unplaced.fa.gz
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/H_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38_unlocalized.fa.gz
for((x=1;x!=23;++x)); do zcat hs_ref_GRCh38_chr$x.fa.gz | grep -v ">" | tr -d -c "ACGTN" > GRC$x; done
zcat hs_ref_GRCh38_chrX.fa.gz | grep -v ">" | tr -d -c "ACGTN" > GRC23 ;
zcat hs_ref_GRCh38_chrY.fa.gz | grep -v ">" | tr -d -c "ACGTN" > GRC24 ;
zcat hs_ref_GRCh38_chrMT.fa.gz | grep -v ">" | tr -d -c "ACGTN" > GRC25 ;
zcat hs_ref_GRCh38_unplaced.fa.gz | grep -v ">" | tr -d -c "ACGTN" > GRC26 ;
zcat hs_ref_GRCh38_unlocalized.fa.gz | grep -v ">" | tr -d -c "ACGTN" > GRC27 ;
cat GRC* > HG;
rm -f *.fa.gz ;
rm -f GRC*
# =============================================================================
# GET EBOVS
wget "www.ncbi.nlm.nih.gov/sviewer/viewer.cgi?tool=portal&sendto=on&log$=seqview&db=nuccore&dopt=fasta&sort=&val=667853353,661348595,661348605,661348615,661348625,661348635,661348645,661348655,661348665,661348675,661348685,661348725,661348715,661348705,661348695,667853343,667853332,667853322,667853311,667853301,667853291,667853280,667853270,667853259,667853249,667853239,667853228,667853218,667853207,667853197,667853186,667853176,667853166,667853155,667853145,667853134,667853124,667853113,667853103,667853093,667853082,667853072,667853062,667853051,667853041,667853030,667853020,667853009,667852999,667852988,667852978,667852967,667852957,667852945,667852934,667852924,667852914,667852903,667852893,667852882,667852872,667852861,667852851,667852840,667852830,667852819,667852809,667852798,667852788,667852778,667852767,667852757,667852747,667852736,667852726,667852716,667852706,667852696,667852686,667852676,667852666,667852656,667852646,667852635,667852625,667852614,667852604,667852592,667852582,667852572,667852562,667852552,667852542,667852531,667852521,667852510,667852500,667852489,661348735" -O RMEBOV-X
awk '/^>/{s="RMEBOV"++d".fasta"} {print > s}' RMEBOV-X 
wget "ftp://ftp.ncbi.nlm.nih.gov/genomes/Viruses/Bundibugyo_ebolavirus_uid51245/NC_014373.fna" -O RMEBOV100.fasta
wget "ftp://ftp.ncbi.nlm.nih.gov/genomes/Viruses/Reston_ebolavirus_uid15006/NC_004161.fna" -O RMEBOV101.fasta
wget "ftp://ftp.ncbi.nlm.nih.gov/genomes/Viruses/Sudan_ebolavirus_uid15012/NC_006432.fna" -O RMEBOV102.fasta
wget "ftp://ftp.ncbi.nlm.nih.gov/genomes/Viruses/Tai_Forest_ebolavirus_uid51257/NC_014372.fna" -O RMEBOV103.fasta
wget "ftp://ftp.ncbi.nlm.nih.gov/genomes/Viruses/Zaire_ebolavirus_uid14703/NC_002549.fna" -O RMEBOV104.fasta
for((x=1; x<=104; ++x)); do cat RMEBOV$x.fasta | grep -v ">" | tr -d -c "ACGTN" > R$x; done
rm -f RMEBOV*
# =============================================================================
# RUN EAGLE
rm -f data2;
for((x=12 ; x<=15 ; ++x));
  do
  ./eagle -v -c $x -i -ea 1 -r HG R1:R2:R3:R4:R5:R6:R7:R8:R9:R10:R11:R12:R13:R14:R15:R16:R17:R18:R19:R20:R21:R22:R23:R24:R25:R26:R27:R28:R29:R30:R31:R32:R33:R34:R35:R36:R37:R38:R39:R40:R41:R42:R43:R44:R45:R46:R47:R48:R49:R50:R51:R52:R53:R54:R55:R56:R57:R58:R59:R60:R61:R62:R63:R64:R65:R66:R67:R68:R69:R70:R71:R72:R73:R74:R75:R76:R77:R78:R79:R80:R81:R82:R83:R84:R85:R86:R87:R88:R89:R90:R91:R92:R93:R94:R95:R96:R97:R98:R99:R100:R101:R102:R103:R104 ;
  for((y=1 ; y<=104 ; ++y));
    do
    mv R$y.pos R$y-$x.pos ;
    ./projector R$y-$x.pos
    rm -f R$y-$x.pos ;
    ./mink $y $x R$y-$x.pos.proj
    rm -f R$y-$x.pos.proj ;
    cat R$y-$x.pos.proj.mink >> data2
    rm -f R$y-$x.pos.proj.mink ;
    done
  done
  ./rebat data2
# =============================================================================
# BUILD 3D MAP
echo 'set mapping cartesian
set view 80,10,1,1 #0,0,1,1
set auto
set zrange [14:12]
set xrange [1:20000]   
set ztics 1
set isosamples 60
set hidden3d
unset key
set palette defined (12 "brown", 13 "blue", 14 "grey")
set zlabel "K-mer"
set ylabel "Strain"
set xlabel "Length"
splot "data2" u 2:1:3 with points pt 1 palette
' | gnuplot -persist
# =============================================================================
