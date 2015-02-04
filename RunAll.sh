#!/bin/bash
# =============================================================================
# DOWNLOAD AND INSTALL PROGRAMS 
if false; then
  rm -fr eagle-master EAGLESRC*
  wget "https://github.com/pratas/eagle/archive/master.zip" -O EAGLESRC.zip ;
  unzip EAGLESRC.zip ;
  cd eagle-master/ 
  make cleanall ; make ;
  cp eagle mink rebat projector ../
  cd ..
  rm -fr eagle-master EAGLESRC*
else
  make cleanall ; make;
fi
# =============================================================================
# GET GRC HUMAN GENOME
if false; then
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
fi
# =============================================================================
# GET EBOVS
if true; then
  wget "www.ncbi.nlm.nih.gov/sviewer/viewer.cgi?tool=portal&sendto=on&log$=seqview&db=nuccore&dopt=fasta&sort=&val=10313991,733962878,733962903,733962926,692112628,703773102,703773112,703773128,743615855,674810554,674810552,674810549,355344222,436409439,436409339,436409389,436409419,436409279,436409289,436409299,436409319,436409309,436409329,436409269,436409349,436409359,436409409,436409379,436409399,436409369,436409429,732464081,732464090,732464099,732464108,685509613,725101076,712659283,748403407,724722348,661348605,661348595,661348615,661348625,667852489,667852500,667852510,667852521,667852531,667852542,667852552,667852562,667852572,667852582,667852592,667852604,667852614,667852625,661348635,661348645,661348655,661348665,661348675,661348685,661348695,661348705,661348715,661348725,661348735,667852635,667852646,667852656,667852666,667852676,667852686,667852696,667852706,667852716,667852726,667852736,667852747,667852757,667852767,667852778,667852788,667852798,667852809,667852819,667852830,667852840,667852851,667852861,667852872,667852882,667852893,667852903,667852914,667852924,667852934,667852945,667852957,667852967,667852978,667852988,667852999,667853009,667853020,667853030,667853041,667853051,667853062,667853072,667853082,667853093,667853103,667853113,667853124,667853134,667853145,667853155,667853166,667853176,667853186,667853197,667853207,667853218,667853228,667853239,667853249,667853259,667853270,667853280,667853291,667853301,667853311,667853322,667853332,667853343,667853353,499104232,499104240,499104248,499104256,208436385,208436395,399151316,165940954,499104197,499104205,499104213,498541194,499104221,448880171,302371213,22789222,55770807,302315369,10313991" -O RMEBOV-X
  awk '/^>/{s="RMEBOV"++d".fasta"} {print > s}' RMEBOV-X
  for((x=1; x<=158; ++x)); do cat RMEBOV$x.fasta | grep -v ">" | tr -d -c "ACGTN" > R$x; done
  rm -f RMEBOV*
fi
# =============================================================================
# RUN EAGLE
if true; then
  rm -f data2;
  ./EAGLE -v -min 11 -max 14 -i -t -r HG R1:R2:R3:R4:R5:R6:R7:R8:R9:R10:R11:R12:R13:R14:R15:R16:R17:R18:R19:R20:R21:R22:R23:R24:R25:R26:R27:R28:R29:R30:R31:R32:R33:R34:R35:R36:R37:R38:R39:R40:R41:R42:R43:R44:R45:R46:R47:R48:R49:R50:R51:R52:R53:R54:R55:R56:R57:R58:R59:R60:R61:R62:R63:R64:R65:R66:R67:R68:R69:R70:R71:R72:R73:R74:R75:R76:R77:R78:R79:R80:R81:R82:R83:R84:R85:R86:R87:R88:R89:R90:R91:R92:R93:R94:R95:R96:R97:R98:R99:R100:R101:R102:R103:R104:R105:R106:R107:R108:R109:R110:R111:R112:R113:R114:R115:R116:R117:R118:R119:R120:R121:R122:R123:R124:R125:R126:R127:R128:R129:R130:R131:R132:R133:R134:R135:R136:R137:R138:R139:R140:R141:R142:R143:R144:R145:R146:R147:R148:R149:R150:R151:R152:R153:R154:R155:R156:R157:R158
  for((x=11 ; x<=14 ; ++x));
    do
    for((y=1 ; y<=158 ; ++y));
      do
      ./mink $y $x R$y-k$x.eg
      cat R$y-k$x.eg.mink >> data2
      rm -f R$y-k$x.eg.mink ;
      done
    done
  ./rebat 158 20000 14 data2
fi
# =============================================================================
# BUILD 3D MAP
if true; then
  echo 'set mapping cartesian
  set view 80,10,1,1 #0,0,1,1
  set auto
  set zrange [14:11]
  set xrange [1:20000]   
  set ztics 1
  set isosamples 60
  set hidden3d
  unset key
  set palette defined (11 "red", 12 "brown", 13 "blue", 14 "grey")
  set zlabel "K-mer"
  set ylabel "Strain"
  set xlabel "Length"
  splot "data2" u 2:1:3 with points pt 1 palette' | gnuplot -persist
fi
# =============================================================================
