#!/bin/sh
# =============================================================================
# BUILD 3D MAP
echo 'set mapping cartesian
set view 360,0,1,1 #0,0,1,1
set auto
set zrange [14:11]
set xrange [1:30500]   
set ztics 1
set isosamples 60
set hidden3d
unset key
set palette defined (11 "red", 12 "brown", 13 "blue", 14 "grey")
set zlabel "K-mer"
set ylabel "Strain"
set xlabel "Length"
splot "data2" u 2:1:3 with points pt 1 palette' | gnuplot -persist
# =============================================================================
