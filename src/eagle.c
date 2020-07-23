#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <pthread.h>

#include "defs.h"
#include "mem.h"
#include "msg.h"
#include "dist.h"
#include "stats.h"
#include "buffer.h"
#include "filter.h"
#include "common.h"
#include "context.h"

Param *P; // SHARE PARAMETERS FOR THREADS

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - C H E C K   F A S T A - - - - - - - - - - - - -
//

void CheckFileIstFASTA(char *name){

  FILE *F = Fopen(name, "r");
  if(fgetc(F) != '>'){
    fprintf(stderr, "[x] Error: input requires a FASTA file!\n");
    exit(1);	  
    }
  fclose(F);  

  return;	
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - G E T   M A X   L E N G T H - - - - - - - - - - - -
//

uint64_t GetMaxLength(void){

  uint64_t x, max = 0;
  for(x = 0 ; x < P->nTar ; ++x)
    if(P->size[x] > max)
      max = P->size[x];

  return max;
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - G E N E R A T E   G N U P L O T   S H - - - - - - - - -
//

void PrintGnuplot(uint32_t min_k, uint32_t max_k){

  uint32_t x;

  FILE *OUT = Fopen("kplot.sh", "w");
  char *colors[] = { "red", "royalblue", "grey", "green", "blue", 
	             "purple", "red", "yellow", "violet", "cyan", "azure", 
		     "chartreuse", "darkgreen", "firebrick", "goldenrod", 
		     "mediumaquamarine", "olive", "orangered", "navy", 
		     "orchid", "peru" };

  fprintf(OUT, "#!/bin/sh\n");
  fprintf(OUT, "echo 'set mapping cartesian\n");
  fprintf(OUT, "set output \"%s-k-map.pdf\"\n", P->output);
  fprintf(OUT, "set view 360,0,1,1 #0,0,1,1\n");
  fprintf(OUT, "set auto\n");
  fprintf(OUT, "set tics nomirror out scale 0.75\n");
  fprintf(OUT, "set zrange [%u:%u]\n", max_k, min_k);
  fprintf(OUT, "set xrange [0:%u]\n", GetMaxLength() + 1);
  fprintf(OUT, "set yrange [0:%u]\n", P->nTar+1);
  fprintf(OUT, "set ztics 1\n");
  fprintf(OUT, "set isosamples 60\n");
  fprintf(OUT, "set hidden3d\n");
  fprintf(OUT, "unset key\n");
  fprintf(OUT, "set palette defined (");
  for(x = min_k ; x <= max_k ; ++x){ 
    fprintf(OUT, "%u \"%s\"", x, colors[x-min_k]);
    if(x != max_k)
      fprintf(OUT, ", ");
    }
  fprintf(OUT, ")\n");
  fprintf(OUT, "set zlabel \"K-mer\"\n");
  fprintf(OUT, "set ylabel \"Strain\"\n");
  fprintf(OUT, "set xlabel \"Length\"\n");
  fprintf(OUT, "splot \"%s.mink\" u 2:1:3 with points pt 5 ps 0.2 palette'",
  P->output);
  fprintf(OUT, " | gnuplot -persist\n");

  return;
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - P R I N T   K   H E A D - - - - - - - - - - - - -
//

void PrintKHead(uint32_t k){

  if(P->verbose){
    fprintf(stderr, "==========================================\n");
    fprintf(stderr, "=                 K-MER %2u               =\n", k);
    fprintf(stderr, "==========================================\n");
    }

  return;
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - R E B A T - - - - - - - - - - - - - - - - - -
//

void Rebat(uint32_t min_k, uint32_t max_k){

  uint32_t x, y, strain, pos, order;

  char *name_mink = (char *) Calloc(2048, sizeof(char));
  sprintf(name_mink, "%s.mink", P->output);
  FILE *IN = Fopen(name_mink, "r");
 
  char *name_rebat = (char *) Calloc(2048, sizeof(char));
  sprintf(name_rebat, "%s.rebat", P->output); 
  FILE *OUT = Fopen(name_rebat, "w");

  uint64_t max = GetMaxLength();

  uint32_t **matrix;
  matrix = (uint32_t **) malloc((P->nTar + 1) * sizeof(uint32_t *));
  for(x = 0 ; x < P->nTar ; ++x){
    matrix[x] = (uint32_t *) malloc((max + 1) * sizeof(uint32_t));
    for(y = 0 ; y < max ; ++y)
      matrix[x][y] = max_k;
    }

  while(fscanf(IN, "%u\t%u\t%u\n", &strain, &pos, &order) == 3){
    // fprintf(stderr, "%u : %u : %u\n", strain, pos, order);	  
    if(strain > P->nTar){
      fprintf(stderr, "Error: out of bound (Rebat)!\n");
      exit(1);
      }
    if(matrix[strain-1][pos-1] > order) matrix[strain-1][pos-1] = order;
    }

  fprintf(OUT, "# X     Y       Z\n");
  for(x = 0 ; x < P->nTar ; ++x)
    for(y = 0 ; y < max ; ++y)
      if(matrix[x][y] != max_k)
        fprintf(OUT, "%u\t%u\t%u\n\n", y, x, matrix[x][y]);

  fclose(OUT);
  fclose(IN);

  return;
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - A V E R A G E   C G   P R O F I L E - - - - - - - - -
//

void AverageCGPlot(void){

  uint32_t x, size = GetMaxLength() + 1;
  double v, *window = (double *) Calloc(size, sizeof(double));

  for(x = 0 ; x < P->nTar ; ++x){
    char *name = (char *) Calloc(2048, sizeof(char));
    sprintf(name, "%s.cg.prof", P->tar[x]);
    FILE *IN = Fopen(name, "r");

    uint32_t idx = 0;
    while(fscanf(IN, "%lf\n", &v) != EOF){
      window[idx] += (double) v;
      if(++idx == size){
	fprintf(stderr, "[x] Error: out of bounds [AverageCGPlot]!\n");
	exit(1);
        }
      }
    Free(name);
    fclose(IN);
    }

  char *name = (char *) Calloc(2048, sizeof(char));
  sprintf(name, "%s.cg.avg", P->output);

  FILE *OUT = Fopen(name, "w");
  for(x = 0 ; x < size ; ++x)
    fprintf(OUT, "%.3lf\n", window[x] / P->nTar);
  fclose(OUT);

  FILE *IN = Fopen(name, "r");
  char *name_out = (char *) Calloc(2048, sizeof(char));
  sprintf(name_out, "%s.cg.fil", P->output);
  FILE *OUT2 = Fopen(name_out, "w");
  Filter(IN, OUT2);
  fclose(OUT2);
  fclose(IN);

  if(P->plots){
    FILE *OUT3 = Fopen("CGprofileplot.sh",  "w");

    fprintf(OUT3, "#!/bin/sh\n");
    fprintf(OUT3, "echo 'reset\n");
    fprintf(OUT3, "set terminal pdfcairo enhanced font \"Verdana,12\"\n");
    fprintf(OUT3, "set output \"CGprofile.pdf\"\n");
    fprintf(OUT3, "set style line 11 lc rgb \"#808080\" lt 1\n");
    fprintf(OUT3, "set border 3 back ls 11\n");
    fprintf(OUT3, "set tics nomirror\n");
    fprintf(OUT3, "set size ratio 0.1\n");
    fprintf(OUT3, "unset grid\n");
    fprintf(OUT3, "set style line 1 lc rgb \"#0060ad\" lt 1 lw 1\n");
    fprintf(OUT3, "unset key\n");
    fprintf(OUT3, "set xlabel \"Length\"\n");
    fprintf(OUT3, "set ylabel \"CG %%\"\n");
    fprintf(OUT3, "set xrange [%u:%u]\n", 0, size);
    fprintf(OUT3, "set yrange [%u:%u]\n", 0, 1);
    fprintf(OUT3, "plot \"%s\" u 1 w lines ls 1' | gnuplot -persist\n", 
		  name_out);

    fclose(OUT3);
    if(P->verbose)
      fprintf(stderr, "[>] Shell plot with CG profile %% in CGprofileplot.sh\n");
    }

  Free(name_out);
  Free(name);

  return;
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - P R I N T   C G   P R O F I L E - - - - - - - - - -
//

void PrintCGProfile(uint32_t id){

  int v, size = 20;
  uint32_t x, idx = 0, bases = 0;
  uint32_t *window = (uint32_t *) Calloc(size +1, sizeof(uint32_t));

  char *name = (char *) Calloc(2048, sizeof(char));
  sprintf(name, "%s.cg.prof", P->tar[id]);
  FILE *OUT = Fopen(name, "w"); 
  FILE *IN  = Fopen(P->tar[id], "r"); 

  while((v = fgetc(IN)) != EOF){
    window[idx] = v;
    uint32_t ALL = 0, AT = 0, CG = 0;
    for(x = 0 ; x < size ; ++x){
      switch(window[x]){
	case 'A': case 'a': case 'T': case 't': AT++; ALL++; break;
	case 'C': case 'c': case 'G': case 'g': CG++; ALL++; break;
	default: break;
        }
      }
    if(++bases > size){
      if(AT == 0 || CG == 0 || ALL == 0)
        fprintf(OUT, "%.4lf\n", (double) 0.0);  
      else	      
        fprintf(OUT, "%.4lf\n", (double) CG / ALL);  
      } 
    if(++idx == size)
      idx = 0;    
    }

  fclose(IN);
  fclose(OUT);
  Free(window);
  Free(name);

  return;
  }


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - M I N K - - - - - - - - - - - - - - - - - -
//

void Mink_file(uint32_t first, uint32_t second, FILE *IN, FILE *OUT){

  char tmp[65535];
  unsigned x;
  while(fscanf(IN, "%u\t%s\n", &x, tmp) == 2)
    fprintf(OUT, "%u\t%u\t%u\n", first + 1, x, second);

  return;
  }

void Mink(uint32_t min_k, uint32_t max_k){

  uint64_t k, y;
  char *name_mink = (char *) Calloc(2048, sizeof(char));
  sprintf(name_mink, "%s.mink", P->output);
  FILE *OUT = Fopen(name_mink, "w");

  for(k = min_k ; k <= max_k ; ++k){
    for(y = 0 ; y < P->nTar ; ++y){
      char *name_eg = (char *) Calloc(2048, sizeof(char));
      sprintf(name_eg, "-k%u.eg", k);
      char *name_in  = concatenate(P->tar[y], name_eg);
      FILE *IN = Fopen(name_in, "r");	   
      Mink_file(y, k, IN, OUT);
      fclose(IN);
      }
    }
  fclose(OUT);

  // ./rebat 1 30436 $maxk name_mink

  return;	
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - C R E A T E   M O D E L S - - - - - - - - - - - -
//

CModels *CreatCModels(uint32_t nModels){

  uint32_t x;
  CModels *CM = (CModels *) Calloc(1, sizeof(CModels));
  CM->nModels = nModels;
  CM->M       = (CModel **) Calloc(CM->nModels, sizeof(CModel *));
  for(x = 0 ; x < CM->nModels ; ++x){
    CM->M[x]  = CreateCModel(P->min_ctx + x, P->inverse);
    CM->M[x]->id = x;
    }

  return CM;
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - M I N   M A X   C H E C K - - - - - - - - - - - -
//

void CheckMinMax(uint32_t min, uint32_t max){

  if(min > max){
    fprintf(stderr, "[x] Error: minimum k-mer is higher than maximum!\n");
    exit(EXIT_FAILURE);
    }
  return;
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - L O A D   F I L E N A M E S - - - - - - - - - - -
//

void LoadNames(char *prefix){

  uint32_t idx;	
  for(idx = 0 ; idx < P->nTar ; ++idx){
    char *name = (char *) Calloc(2048, sizeof(char));
    sprintf(name, "%s-R%u", prefix, idx + 1);
    P->tar[idx] = CloneString(name);
    free(name);
    }
  return;
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - S P L I T   F I L E S - - - - - - - - - - - - -
//

uint64_t SplitFiles(char *fn, uint8_t v){

  FILE     *IN = Fopen(fn, "r"), *OUT = NULL;
  uint64_t indexHeader = 0, indexSymbol = 0, nFiles = 0;
  char     value;
      
  if(v) fprintf(stderr, "[>] Splitting reads:\n");
  
  int header = 0;
  while((value = fgetc(IN)) != EOF){

    switch(value){
      case '>':  header = 1; 
      if(++indexHeader > 1) fclose(OUT);		 
      char name[1024];
      sprintf(name, "%s-R%u", fn, indexHeader);
      if(v) fprintf(stderr, "    [>] Splitted read .............. %s\n", name);
      OUT = Fopen(name, "w");
      continue;
      case '\n': header = 0; continue;
      default: if(header==1) continue;
      }

    fprintf(OUT, "%c", value);
    }

  fclose(OUT);
  fclose(IN);

  if(v){
    fprintf(stderr, "[>] Splitting reads done!\n");
    fprintf(stderr, "[>] Total number of reads: %u\n", indexHeader);
    }

  return indexHeader;
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - P R I N T   A V E R A G E   P L O T - - - - - - - - - - 
//

void PrintAveragePlot(Stats *S, uint32_t nKmers){

  uint32_t x;

  FILE *OUT = Fopen("AVG-data.eg", "w");
  for(x = 0 ; x < nKmers ; ++x)
    fprintf(OUT, "%u\t%.3lf\n", P->min_ctx + x, S->average[x]);
  fclose(OUT);

  FILE *OUT2 = Fopen("AVGplot.sh", "w");
  fprintf(OUT2, "#!/bin/sh\n");
  fprintf(OUT2, "echo 'reset\n");
  fprintf(OUT2, "set terminal pdfcairo enhanced font \"Verdana,12\"\n");
  fprintf(OUT2, "set output \"AVGplot.pdf\"\n");
  fprintf(OUT2, "set style line 11 lc rgb \"#808080\" lt 1\n");
  fprintf(OUT2, "set border 3 back ls 11\n");
  fprintf(OUT2, "set logscale y\n");
  fprintf(OUT2, "set tics nomirror\n");
  fprintf(OUT2, "set size ratio 1\n");
  fprintf(OUT2, "set style line 12 lc rgb \"#808080\" lt 0 lw 2\n");
  fprintf(OUT2, "set grid back ls 12\n");
  fprintf(OUT2, "set style line 1 lc rgb \"#8b1a0e\" pt 1 ps 1 lt 1 lw 3\n");
  fprintf(OUT2, "set style line 2 lc rgb \"#5e9c36\" pt 6 ps 1 lt 1 lw 3\n");
  fprintf(OUT2, "unset key\n");
  fprintf(OUT2, "set xlabel \"k-mer\"\n");
  fprintf(OUT2, "set ylabel \"Number of mRAWs\"\n");
  fprintf(OUT2, "set xrange [%u:%u]\n", P->min_ctx, P->min_ctx + nKmers - 1);
  fprintf(OUT2, "set yrange [:]\n");
  fprintf(OUT2, "plot \"AVG-data.eg\" u 1:2 w lp ls 1' | gnuplot -persist\n");
  if(P->verbose) 
    fprintf(stderr, "[>] Shell plot with AVG %% in AVGplot.sh\n");
  fclose(OUT2);

  return;
  }


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - W R I T E   W O R D - - - - - - - - - - - - -
//

void RWord(FILE *F, uint8_t *b, int32_t i, uint32_t ctx){
  uint8_t w[ctx+1], n;
  i -= ctx;
  for(n = 0 ; n < ctx ; ++n)
    w[n] = N2S(b[i+n]);
  w[ctx] = '\0';
  fprintf(F, "%s\n", w);
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - T A R G E T - - - - - - - - - - - - - - -
//
// It finds and localizes the RAWS for a certain k and returns the number
// for stats print. Also, counts the number of bases in each word.

uint32_t Target(CModel *M, Dist *D, uint32_t id_tar){
  FILE     *Reader = Fopen(P->tar[id_tar], "r");
  char     *name1  = (char *) Calloc(2048, sizeof(char));
                     sprintf(name1, "-k%u.eg", M->ctx);
  char     *name2  = concatenate(P->tar[id_tar], name1);
  FILE     *Pos    = Fopen(name2, "w");
  uint64_t nSymbols = P->size[id_tar], i = 0, raw = 0, unknown = 0;
  uint64_t n, k, idxPos, hIndex;
  int64_t  idx = 0;
  uint8_t  *wBuf, *rBuf, sym, found = 0;
  CBUF     *sBuf;

  if(P->verbose)
    fprintf(stderr, "[>] Searching target sequence %d ...\n", id_tar + 1);

  wBuf  = (uint8_t *) Calloc(BUFFER_SIZE,          sizeof(uint8_t));
  rBuf  = (uint8_t *) Calloc(BUFFER_SIZE,          sizeof(uint8_t));
  sBuf  = CreateCBuffer(BUFFER_SIZE, BGUARD);

  #ifdef PROFILE
  char *name3   = (char *) Calloc(2048, sizeof(char));
                  sprintf(name3, "%s-k%u.prof", P->tar[id_tar], M->ctx);
  FILE *Profile = Fopen(name3, "w");
  #endif

  int raw_exists = 0;
  while((k = fread(rBuf, 1, BUFFER_SIZE, Reader))){
    for(idxPos = 0 ; idxPos < k ; ++idxPos){

      #ifdef PROGRESS
      CalcProgress(nSymbols, i);
      #endif
      ++i;
      if((sym = S2N(rBuf[idxPos])) == 4){
        ++unknown;
        continue;
        }
      
      sBuf->buf[sBuf->idx] = sym;
      GetIdx(sBuf->buf+sBuf->idx-1, M); 
      if(i > M->ctx){  // SKIP INITIAL CONTEXT, ALL "AAA..."

        if(M->mode == 0){ // TABLE MODE
          if(!M->array.counters[M->idx]){ // THERE IS A RAW!
            fprintf(Pos, "%"PRIu64"\t", i-M->ctx);
            RWord(Pos,sBuf->buf, sBuf->idx, M->ctx);
	    raw_exists = 1;
            #ifdef PROFILE
            fprintf(Profile, "2\n");
            #endif
            ++raw;
            }
          else{ // NO RAW!
	    raw_exists = 0;
            #ifdef PROFILE
            fprintf(Profile, "0\n");
            #endif
            }
          }
        else{ // HASH TABLE
          found = 0;
          hIndex = M->idx % HASH_SIZE;
          for(n = 0 ; n < M->hash.entrySize[hIndex] ; n++)
            if(((uint64_t) M->hash.keys[hIndex][n]*HASH_SIZE)+hIndex == M->idx){
              found = 1;
              break;
              }
          if(found == 0){ // THERE IS A RAW
            fprintf(Pos, "%"PRIu64"\t", i-M->ctx); 
            RWord(Pos, sBuf->buf, sBuf->idx, M->ctx);
	    raw_exists = 1;
            #ifdef PROFILE
            fprintf(Profile, "1\n");
            #endif
            ++raw;
            }
          else{ // NO RAW!
	    raw_exists = 0;
            #ifdef PROFILE
            fprintf(Profile, "0\n");
            #endif
            }
          }

	if(raw_exists == 1){ // COUNTS THE BASE DISTRIBUTION OF THE RAWS
          for(n = 0 ; n < M->ctx ; n++)
	     D->C[M->id].bases[sBuf->buf[sBuf->idx-n]] += 1;
	  }

        }

      UpdateCBuffer(sBuf);
      }
    }

  fclose(Pos);
  fclose(Reader);
  ResetIdx(M);
  Free(name1);
  Free(name2);
  #ifdef PROFILE
  fclose(Profile);
  Free(name3);
  #endif
  Free(rBuf);
  Free(wBuf);
  RemoveCBuffer(sBuf);

  fprintf(stdout, "[>] Kmer %u , Read %u , mRAWs: %.4lf %% ( %"PRIu64" in "
		  "%"PRIu64" , unknown: %"PRIu64" , total: %"PRIu64" )\n",
		  M->ctx, id_tar + 1, (double) raw / (nSymbols-unknown) * 
		  100.0, raw, nSymbols-unknown, unknown, nSymbols);

  if(P->verbose)
    fprintf(stderr, "[>] Done!                     \n");  // SPACES ARE VALID!

  return raw;
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - R E F E R E N C E - - - - - - - - - - - - -
// 

void LoadReference(CModel *M){

  FILE     *Reader = Fopen(P->ref, "r");
  int64_t  k, idxPos, header = 0;
  int64_t  idx = 0;
  uint8_t  *rBuf, *sBuf, sym;
  uint64_t i = 0;
  #ifdef PROGRESS
  uint64_t size = NBytesInFile(Reader);
  #endif

  if(P->verbose == 1){
    if(P->nThreads == 0){
      fprintf(stderr, "[>] Building reference model (k=%u) ...\n", M->ctx);
      }
    else if(P->nThreads == M->id+1){
      fprintf(stderr, "[>] Building reference models ...\n");
      }
    }

  rBuf  = (uint8_t *) Calloc(BUFFER_SIZE + 1, sizeof(uint8_t));
  sBuf  = (uint8_t *) Calloc(BUFFER_SIZE + BGUARD+1, sizeof(uint8_t));
  sBuf += BGUARD;

  uint64_t skip = 0;
  while((k = fread(rBuf, 1, BUFFER_SIZE, Reader)))
    for(idxPos = 0 ; idxPos < k ; ++idxPos){
      ++i;
      #ifdef PROGRESS
      if(P->nThreads == 0)
        CalcProgress(size, i);
      else if(P->nThreads == M->id + 1) 
        CalcProgress(size, i);
      #endif
      switch(rBuf[idxPos]){
        case '>':  header = 1; skip = 0; continue;
        case '\n': header = 0; continue;  
        default: if(header==1) continue;
        }

      if((sym = S2N(rBuf[idxPos])) == 4) continue;    
      sBuf[idx] = sym;
      GetIdx(sBuf+idx-1, M);

      if(++skip > M->ctx){ // SKIP INITIAL CONTEXT, ALL "AAA..."
        Update(M);
        if(M->ir == 1){  // Inverted repeats
          GetIdxIR(sBuf+idx, M);
          UpdateIR(M);
          }
        }

      if(++idx == BUFFER_SIZE){
        memcpy(sBuf-BGUARD, sBuf+idx-BGUARD, BGUARD);
        idx = 0;
        }
      }
 
  ResetIdx(M);
  Free(rBuf);
  Free(sBuf-BGUARD);
  fclose(Reader);

  if(P->verbose == 1){
    if(P->nThreads == 0){
      fprintf(stderr, "[>] Done!                    \n");  // SPACES ARE VALID  
      }
    else if(P->nThreads == M->id+1){
      fprintf(stderr, "[>] Done!                    \n");  // SPACES ARE VALID  
      }
    }
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - T H R E A D I N G - - - - - - - - - - - - - - -
//

void *LoadRefThread(void *Par){
  CModel *M = (CModel *) Par;
  LoadReference(M);
  pthread_exit(NULL);
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - E A G L E   M A I N - - - - - - - - - - - - - - -
//

int32_t main(int argc, char *argv[]){

  char **p = *&argv;
  uint32_t n, verbose, threads, nThreads, force, out, inversions, plots, 
	   x, min, max, nKmers, idx_reads = 0, idx_model = 0;

  if(ArgsState(DEF_HELP, p, argc, "-h", "--help") || argc < 3){
    PrintMenu();
    return EXIT_SUCCESS;
    }

  if(ArgsState(0, p, argc, "-V", "--version") ){
    PrintVersion();
    return EXIT_SUCCESS;
    }

  verbose    = ArgsState (DEF_VERBOSE, p, argc, "-v",   "--verbose");
  force      = ArgsState (DEF_FORCE,   p, argc, "-f",   "--force");
  threads    = ArgsState (DEF_THREADS, p, argc, "-t",   "--threads");
  inversions = ArgsState (DEF_IR,      p, argc, "-i",   "--ignore-ir");
  out        = ArgsState (DEF_OUT,     p, argc, "-o",   "--stdout");
  plots      = ArgsState (DEF_PLOTS,   p, argc, "-p",   "--plots");
  min        = ArgsNum   (DEF_MIN_CTX, p, argc, "-min", "--minimum", 
		         MIN_HASH_CTX, MAX_HASH_CTX);
  max        = ArgsNum   (DEF_MAX_CTX, p, argc, "-max", "--maximum", 
		         MIN_HASH_CTX, MAX_HASH_CTX);

  CheckMinMax(min, max);

  if(verbose)
    fprintf(stderr, "[>] Running EAGLE v%u.%u ...\n", VERSION, RELEASE);

  nKmers = max - min + 1;
  if(threads == 1) 
    nThreads = nKmers;

  CheckFileIstFASTA(argv[argc-1]);

  P           = (Param *) Calloc(nKmers, sizeof(Param));
  P->ref      = argv[argc-2];
  P->nTar     = SplitFiles(argv[argc-1], verbose);
  P->tar      = (char **) Malloc(P->nTar * sizeof(char *));
  P->output   = CloneString(argv[argc-1]);

  LoadNames(argv[argc-1]);

  P->nKmers   = nKmers;
  P->min_ctx  = min;
  P->verbose  = verbose;
  P->plots    = plots;
  P->inverse  = inversions;
  P->nThreads = nThreads;

  if(P->verbose) PrintArgs(P);

  CModels *CM = CreatCModels(nKmers);

  Stats    *S = CreateStats (nKmers);
  Dist     *D = CreateDist  (nKmers, 4);

  P->size = (uint64_t *) Calloc(P->nTar, sizeof(uint64_t));

  for(idx_reads = 0 ; idx_reads < P->nTar ; ++idx_reads){
    FILE *Reader = Fopen(P->tar[idx_reads], "r");
    P->size[idx_reads] = NBytesInFile(Reader);
    fclose(Reader);
    }

  if(nThreads == 0){ // IF NO THREADS ==========================================

    if(P->verbose) fprintf(stderr, "[>] Using single-thread ...\n");

    for(idx_model = 0 ; idx_model < nKmers ; ++idx_model){

      LoadReference(CM->M[idx_model]);
     
      PrintKHead(idx_model + min);

      double sum = 0;
      uint64_t vx[P->nTar];
      for(idx_reads = 0 ; idx_reads < P->nTar ; ++idx_reads)
        sum += (vx[idx_reads] = Target(CM->M[idx_model], D, idx_reads));
      S->average[idx_model] = sum / (double) P->nTar;

      double sum1 = 0;
      for(idx_reads = 0 ; idx_reads < P->nTar ; ++idx_reads)
        sum1 = sum1 + pow(((double) vx[idx_reads] - S->average[idx_model]), 2);
    
      S->variance[idx_model] = sum1 / (double) P->nTar;
      S->stddev  [idx_model] = sqrt(S->variance[idx_model]);

      DeleteCModel(CM->M[idx_model]);
      }

    if(P->plots) PrintAveragePlot(S, nKmers);
    }

  else{ // IF THREADS =========================================================

    if(P->verbose) 
      fprintf(stderr, "[>] Using multi-threading to load reference ...\n");

    pthread_t t[nThreads];
  
    for(n = 0 ; n < nThreads ; ++n)
      pthread_create(&(t[n+1]), NULL, LoadRefThread, (void *) CM->M[n]);
    for(n = 0 ; n < nThreads ; ++n) // DO NOT JOIN THIS FOR WITH PREVIOUS!
      pthread_join(t[n+1], NULL);
  
    for(idx_model = 0 ; idx_model < nKmers ; ++idx_model){

      PrintKHead(idx_model + min);
 
      double sum = 0;
      uint64_t vx[P->nTar];
      for(idx_reads = 0 ; idx_reads < P->nTar ; ++idx_reads)
	sum += (vx[idx_reads] = Target(CM->M[idx_model], D, idx_reads));
      S->average[idx_model] = sum / (double) P->nTar;

      double sum1 = 0;
      for(idx_reads = 0 ; idx_reads < P->nTar ; ++idx_reads)
        sum1 = sum1 + pow(((double) vx[idx_reads] - S->average[idx_model]), 2);
    
      S->variance[idx_model] = sum1 / (double) P->nTar;
      S->stddev  [idx_model] = sqrt(S->variance[idx_model]);

      DeleteCModel(CM->M[idx_model]);
      }
      
    if(P->plots) PrintAveragePlot(S, nKmers);
    }

  if(P->verbose) fprintf(stderr, "[>] Printing overall mRAWs statistics ...\n");
  PrintStats(S, min);
  if(P->verbose) fprintf(stderr, "[>] Done!\n"); 

  if(P->verbose) 
    fprintf(stderr, "[>] Printing mRAWs nucleotide distribution ...\n");
  PrintDist(D, P->min_ctx);
  if(P->verbose) fprintf(stderr, "[>] Done!\n"); 

  if(P->verbose)
    fprintf(stderr, "[>] Printing mRAWs nucleotide CG %% ...\n");
  PrintCG(D, P->min_ctx, P->plots, P->verbose);
  if(P->verbose) fprintf(stderr, "[>] Done!\n");

  if(P->verbose)
    fprintf(stderr, "[>] Printing CG profiles ...\n");
  for(x = 0 ; x < P->nTar ; ++x)
    PrintCGProfile(x);
  AverageCGPlot();
  if(P->verbose) fprintf(stderr, "[>] Done!\n"); 

  if(P->verbose)
    fprintf(stderr, "[>] Running mink ...\n");
  Mink(P->min_ctx, P->min_ctx + nKmers - 1);
  if(P->verbose) fprintf(stderr, "[>] Done!\n");

  if(P->verbose)
    fprintf(stderr, "[>] Running projector ...\n");
  Rebat(P->min_ctx, P->min_ctx + nKmers - 1);
  if(P->verbose) fprintf(stderr, "[>] Done!\n");

  if(P->plots){
    if(P->verbose)
      fprintf(stderr, "[>] Printing SH plot ...\n");
    PrintGnuplot(P->min_ctx, P->min_ctx + nKmers - 1);
    if(P->verbose){
      fprintf(stderr, "[>] Shell plot in kplot.sh\n");
      fprintf(stderr, "[>] Done!\n");
      }
    }

  RemoveStats(S);
  RemoveDist (D);

  return EXIT_SUCCESS;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

