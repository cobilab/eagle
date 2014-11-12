#!/bin/bash
./DNAiProfile1 -v -k 4 -k 8 -k 12 1/3 -k 15 1/3 -w 5 -o INFO R15
echo 'set style line 1 lc rgb "#8b1a0e" pt 1 ps 1 lt 1 lw 2 # --- red
set style line 11 lc rgb "#808080" lt 1
set border 3 back ls 11
set tics nomirror
unset key
plot "INFO" with lines linestyle 1' | gnuplot -persist
