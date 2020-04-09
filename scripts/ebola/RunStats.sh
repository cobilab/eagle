#!/bin/bash
# =============================================================================
# RUN STATS
if true; then
rm -f averages.auxd;
size=165;
for((y=11; y<=16 ;++y)); 
  do
  echo "scale=4;`rm -f WORDS.auxd; for((x=1;x<=$size;++x)); do cat R$x-k$y.eg \
  | wc -l >> WORDS.auxd ; done; cat WORDS.auxd \
  | awk '{ sum+=$1} END {print sum}'`/$size" | bc -l >> averages.auxd;
  done
  cat averages.auxd;
fi
# =============================================================================
# BUILD 2D CURVE PLOT
if true; then
  echo 'set border 11; set logscale y; plot "averages.auxd" pt 7 notitle; set bmargin 2.5' | gnuplot -persist
fi
# =============================================================================
