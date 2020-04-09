#include "dist.h"
#include "mem.h"
#include "common.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Dist *CreateDist(uint32_t size, uint32_t nBases){ // size = nKmers, nBases = 4
 
  uint32_t x;
  Dist *D     = (Dist *) Calloc(1, sizeof(Dist));
  D->size     = size;
  D->C        = (Counts *) Calloc(size, sizeof(Counts));
  for(x = 0 ; x < size ; ++x){
    D->C[x].nBases = nBases; 	  
    D->C[x].bases  = (uint64_t *) Calloc(nBases, sizeof(uint64_t));
    }

  return D;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void PrintDist(Dist *D, uint32_t min_ctx){

  int x;
  fprintf(stderr, "+--------------------------------------------------------+\n");
  fprintf(stderr, "|             mRAWs nucleotide distribution              |\n");
  fprintf(stderr, "+--------+-----------+-----------+-----------+-----------+\n");
  fprintf(stderr, "|  kmer  |     A     |     C     |     G     |     T     |\n");
  fprintf(stderr, "+--------+-----------+-----------+-----------+-----------+\n");
  for(x = 0 ; x < D->size ; ++x)
    fprintf(stderr, "| %6u | %9u | %9u | %9u | %9u |\n", x + min_ctx,
    D->C[x].bases[0], D->C[x].bases[1], D->C[x].bases[2], D->C[x].bases[3]);		  
  fprintf(stderr, "+--------+-----------+-----------+-----------+-----------+\n");

  return;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void PrintCG(Dist *D, uint32_t min_ctx, uint32_t p, uint32_t v){

  int x;
  uint64_t tot, at, cg;
  double at100 = 0, cg100 = 0;
  FILE *OUT = Fopen("CG-data.eg", "w");
  fprintf(stderr, "+--------------------------------------------------+\n");
  fprintf(stderr, "|              mRAWs AT/CG distribution            |\n");
  fprintf(stderr, "+--------+--------------------+--------------------+\n");
  fprintf(stderr, "|  kmer  |         AT         |         CG         |\n");
  fprintf(stderr, "+--------+--------------------+--------------------+\n");
  for(x = 0 ; x < D->size ; ++x){
    tot = D->C[x].bases[0]+D->C[x].bases[1]+D->C[x].bases[2]+D->C[x].bases[3];
    at  = D->C[x].bases[0]+D->C[x].bases[3];
    cg  = D->C[x].bases[1]+D->C[x].bases[2];
    if(at != 0) at100 = (double) at/tot*100;
    if(cg != 0) cg100 = (double) cg/tot*100;
    if(at == 0 || cg == 0)
    fprintf(stderr, "| %6u | %10u [--.-%%] | %10u [--.-%%] |\n", x + min_ctx,
    at, at100, cg, cg100);
    else
    fprintf(stderr, "| %6u | %10u [%3.1lf%%] | %10u [%3.1lf%%] |\n", x + min_ctx,
    at, at100, cg, cg100);
    if(at != 0 && cg != 0)
      fprintf(OUT, "%u\t%.3lf\t%.3lf\n", x + min_ctx, at, cg);
    }
  fclose(OUT);
  fprintf(stderr, "+--------+--------------------+--------------------+\n");

  FILE *OUT2 = Fopen("CGplot.sh",  "w");
  FILE *IN   = Fopen("CG-data.eg", "r");
  if(p){
    fprintf(OUT2, "#!/bin/sh\n");
    fprintf(OUT2, "echo 'reset\n");
    fprintf(OUT2, "set terminal pdfcairo enhanced font \"Verdana,12\"\n");
    fprintf(OUT2, "set output \"CGplot.pdf\"\n");
    fprintf(OUT2, "set style line 11 lc rgb \"#808080\" lt 1\n");
    fprintf(OUT2, "set border 3 back ls 11\n");
    fprintf(OUT2, "set tics nomirror\n");
    fprintf(OUT2, "set size ratio 1\n");
    fprintf(OUT2, "set style line 12 lc rgb \"#808080\" lt 0 lw 2\n");
    fprintf(OUT2, "set grid back ls 12\n");
    fprintf(OUT2, "set style line 1 lc rgb \"#8b1a0e\" pt 1 ps 1 lt 1 lw 3\n");
    fprintf(OUT2, "set style line 2 lc rgb \"#5e9c36\" pt 6 ps 1 lt 1 lw 3\n");
    fprintf(OUT2, "set key bottom right\n");
    fprintf(OUT2, "set xlabel \"k-mer\"\n");
    fprintf(OUT2, "set ylabel \"Distribution (%%)\"\n");
    fprintf(OUT2, "set xrange [%u:%u]\n", min_ctx, min_ctx + D->size - 1);
    fprintf(OUT2, "set yrange [%u:%u]\n", 0, 100);
    fprintf(OUT2, "plot \"CG-data.eg\" u 1:2 t \"AT\" w lp ls 1, "
                  " \"CG-data.eg\" u 1:3 t \"CG\" w lp ls 2 ' | gnuplot -persist\n");
    
    if(v) fprintf(stderr, "[>] Shell plot with CG %% in CGplot.sh\n");	  
    }
  fclose(IN);
  fclose(OUT2);

  return;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void RemoveDist(Dist *D){

  int x;	
  for(x = 0 ; x < D->size ; ++x)	
    Free(D->C[x].bases);	
  Free(D->C);	
  Free(D);

  return;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
