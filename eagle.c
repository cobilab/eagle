#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <pthread.h>
#include "mem.h"
#include "defs.h"
#include "common.h"
#include "context.h"

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - W R I T E   W O R D - - - - - - - - - - - -
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
void Target(Param P, uint8_t id){
  FILE     *Reader = Fopen(P.tar[id], "r");
  char     *name1  = (char *) Calloc(1024, sizeof(char));
                     sprintf(name1, "-k%u.eg", P.context);
  char     *name2  = concatenate(P.tar[id], name1);
  FILE     *Pos    = Fopen(name2, "w");
  uint64_t nSymbols = NDNASyminFile(Reader), i = 0, raw = 0, unknown = 0;
  uint32_t n, k, idxPos, hIndex, header = 0;
  int32_t  idx = 0;
  uint8_t  *wBuf, *rBuf, *sBuf, sym, found = 0;

  if(P.verbose)
    fprintf(stderr, "Searching target sequence %d ...\n", id + 1);

  wBuf  = (uint8_t *) Calloc(BUFFER_SIZE,          sizeof(uint8_t));
  rBuf  = (uint8_t *) Calloc(BUFFER_SIZE,          sizeof(uint8_t));
  sBuf  = (uint8_t *) Calloc(BUFFER_SIZE + BGUARD, sizeof(uint8_t));
  sBuf += BGUARD;

  while((k = fread(rBuf, 1, BUFFER_SIZE, Reader))){
    for(idxPos = 0 ; idxPos < k ; ++idxPos){
      #ifdef PROGRESS
      CalcProgress(nSymbols, i);
      #endif
      ++i;
      switch(rBuf[idxPos]){
        case '>':  header = 1; continue;
        case '\n': header = 0; continue;  
        default:   if(header==1) continue;
        }
      if((sym = S2N(rBuf[idxPos])) == 4){
        ++unknown;
        continue;
        }
      sBuf[idx] = sym;
      GetIdx(sBuf+idx-1, P.M); 
      if(i > P.M->ctx){  // SKIP INITIAL CONTEXT, ALL "AAA..."
        if(P.M->mode == 0){ // TABLE MODE
          if(!P.M->array.counters[P.M->idx]){ // NO MATCH!
            fprintf(Pos, "%"PRIu64"\t", i-P.M->ctx);
            RWord(Pos, sBuf, idx, P.M->ctx);
            ++raw;
            }
          }
        else{ // HASH TABLE
          found = 0;
          hIndex = P.M->idx % HASH_SIZE;
          for(n = 0 ; n < P.M->hash.entrySize[hIndex] ; n++)
            if(((uint64_t) P.M->hash.keys[hIndex][n]*HASH_SIZE)+hIndex == P.M->idx){
              found = 1;
              break;
              }
          if(found == 0){
            fprintf(Pos, "%"PRIu64"\t", i-P.M->ctx); 
            RWord(Pos, sBuf, idx, P.M->ctx);
            ++raw;
            }
          }
        }

      if(++idx == BUFFER_SIZE){
        memcpy(sBuf-BGUARD, sBuf+idx-BGUARD, BGUARD);
        idx = 0;
        }
      }
    }

  fclose(Pos);
  fclose(Reader);
  ResetIdx(P.M);
  Free(name1);
  Free(name2);
  Free(rBuf);
  Free(wBuf);
  Free(sBuf-BGUARD);

  if(P.verbose == 1)
    fprintf(stderr, "Done!                          \n");  // SPACES ARE VALID

  fprintf(stdout, "RAWs FOUND  : %.4lf %% ( %"PRIu64" in %"PRIu64" )\n", 
  (double) raw / (nSymbols-unknown) * 100.0, raw, nSymbols-unknown);  
  fprintf(stdout, "Unknown sym : %"PRIu64"\n", unknown);
  fprintf(stdout, "Total sym   : %"PRIu64"\n", nSymbols);
  if(P.nTar != id+1)
    fprintf(stdout, "------------------------------------------\n");
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - R E F E R E N C E - - - - - - - - - - - - -
void LoadReference(Param P){
  FILE     *Reader = Fopen(P.ref, "r");
  uint32_t k, idxPos, header = 0;
  int32_t  idx = 0;
  uint8_t  *rBuf, *sBuf, sym;
  #ifdef PROGRESS
  uint64_t i = 0, size = NBytesInFile(Reader);
  #endif

  if(P.verbose == 1){
    if(P.nThreads == 0){
      fprintf(stderr, "Building reference model (k=%u) ...\n", P.context);
      }
    else if(P.nThreads == P.id+1){
      fprintf(stderr, "Building reference models ...\n");
      }
    }

  rBuf  = (uint8_t *) Calloc(BUFFER_SIZE + 1, sizeof(uint8_t));
  sBuf  = (uint8_t *) Calloc(BUFFER_SIZE + BGUARD+1, sizeof(uint8_t));
  sBuf += BGUARD;

  while((k = fread(rBuf, 1, BUFFER_SIZE, Reader)))
    for(idxPos = 0 ; idxPos < k ; ++idxPos){
      ++i;
      #ifdef PROGRESS
      if(P.nThreads == 0)
        CalcProgress(size, i);
      else if(P.nThreads == P.id+1) 
        CalcProgress(size, i);
      #endif
      switch(rBuf[idxPos]){
        case '>':  header = 1; continue;
        case '\n': header = 0; continue;  
        default:   if(header==1) continue;
        }
      if((sym = S2N(rBuf[idxPos])) == 4) continue;    
      sBuf[idx] = sym;
      GetIdx(sBuf+idx-1, P.M);
      if(i > P.M->ctx){ // SKIP INITIAL CONTEXT, ALL "AAA..."
        Update(P.M);
        if(P.M->ir == 1){  // Inverted repeats
          GetIdxIR(sBuf+idx, P.M);
          UpdateIR(P.M);
          }
        }
      if(++idx == BUFFER_SIZE){
        memcpy(sBuf-BGUARD, sBuf+idx-BGUARD, BGUARD);
        idx = 0;
        }
      }
 
  ResetIdx(P.M);
  Free(rBuf);
  Free(sBuf-BGUARD);
  fclose(Reader);

  if(P.verbose == 1){
    if(P.nThreads == 0){
      fprintf(stderr, "Done!                          \n");  // SPACES ARE VALID  
      }
    else if(P.nThreads == P.id+1){
      fprintf(stderr, "Done!                          \n");  // SPACES ARE VALID  
      }
    }
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - T H R E A D I N G - - - - - - - - - - - - - - -
void *LoadRefThread(void *Par){
  Param *P = (Param *) Par;
  LoadReference(P[0]);
  pthread_exit(NULL);
  }

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - E A G L E   M A I N - - - - - - - - - - - - - - -
int32_t main(int argc, char *argv[]){
  char     **p = *&argv;
  uint32_t n, k, min, max, nKmers, nThreads = 0;
  Param    *P;

  if(ArgsState(DEFAULT_HELP, p, argc, "-h") == 1 || argc < 2){
    fprintf(stderr, "Usage: EAGLE <OPTIONS>... -r [FILE]  [FILE]:<...>    \n");
    fprintf(stderr, "                                                     \n");
    fprintf(stderr, "  -v                       verbose mode,             \n");
    fprintf(stderr, "  -a                       about EAGLE,              \n");
    fprintf(stderr, "  -t                       use multi-threading,      \n");
    fprintf(stderr, "  -i                       use inversions,           \n");
    fprintf(stderr, "  -min <k-mer>             k-mer minimum size,       \n");
    fprintf(stderr, "  -max <k-mer>             k-mer maximum size,       \n");
    fprintf(stderr, "                                                     \n");
    fprintf(stderr, "  -r [rFile]               reference file (db),      \n");
    fprintf(stderr, "                                                     \n");
    fprintf(stderr, "  [tFile1]:<tFile2>:<...>  target file(s).           \n");
    fprintf(stderr, "                                                     \n");
    fprintf(stderr, "EAGLE is a fast method/tool to compute relative MAWs.\n");
    fprintf(stderr, "The input files should be FASTA (.fa) or SEQ [ACGTN].\n");
    return EXIT_SUCCESS;
    }

  if(ArgsState(0, p, argc, "-a") || ArgsState(0, p, argc, "-V")){
    fprintf(stderr, "EAGLE %u.%u\n"
    "Copyright (C) 2015 University of Aveiro.\nThis is Free software. \nYou "
    "may redistribute copies of it under the terms of the GNU General \n"
    "Public License v2 <http://www.gnu.org/licenses/gpl.html>.\nThere is NO "
    "WARRANTY, to the extent permitted by law.\nCode written by Diogo Pratas"
    " (pratas@ua.pt) and Armando J. Pinho (ap@ua.pt).\n", RELEASE, VERSION);
    return EXIT_SUCCESS;
    }

  min = ArgsNum (DEF_MIN_CTX, p, argc, "-min", MIN_HASH_CTX, MAX_HASH_CTX);
  max = ArgsNum (DEF_MAX_CTX, p, argc, "-max", MIN_HASH_CTX, MAX_HASH_CTX);
  if(min > max){
    fprintf(stderr, "[x] Error: minimum (%u) is higher than maximum (%u)!\n",
    min, max);
    return EXIT_FAILURE;
    } 
  P = (Param *) Calloc((nKmers=max-min+1), sizeof(Param));
  P[0].ref  = ArgsString (NULL, p, argc, "-r");
  P[0].nTar = ReadFNames (P, argv[argc-1]);
  if(ArgsState(0, p, argc, "-t"))
    nThreads  = nKmers;
  for(n = 0 ; n < nKmers ; ++n){
    P[n].id       = n;
    P[n].nKmers   = nKmers;
    P[n].context  = min+n;
    P[n].verbose  = ArgsState (DEFAULT_VERBOSE, p, argc, "-v");
    P[n].inverse  = ArgsState (DEFAULT_IR, p, argc, "-i");
    P[n].nThreads = nThreads;
    if(n != 0){
      P[n].nTar = P[0].nTar;
      P[n].ref  = CloneString(P[0].ref);
      P[n].tar  = (char **) Malloc(P[n].nTar * sizeof(char *));
      for(k = 0 ; k < P[0].nTar ; ++k){
        P[n].tar[k] = CloneString(P[0].tar[k]);
        }
      }
    }

  if(P[0].verbose){
    fprintf(stdout, "===============> EAGLE v%u.%u <=============\n", 
    VERSION, RELEASE);
    PrintArgs(P);
    fprintf(stdout, "==========================================\n");
    }

  if(nThreads == 0){ // IF NO THREADS
    for(k = 0 ; k < nKmers ; ++k){
      P[k].M = CreateCModel(P[k].context, P[k].inverse); 
      LoadReference(P[k]);
      P[k].size = (uint64_t *) Calloc(P[k].nTar, sizeof(uint64_t));
      for(n = 0 ; n < P[k].nTar ; ++n){
        FILE *Reader = Fopen(P[k].tar[n], "r");
        P[k].size[n] = NDNASyminFile(Reader);
        fclose(Reader);
        }
      for(n = 0 ; n < P[0].nTar ; ++n)
        Target(P[k], n);
      DeleteCModel(P[k].M);
      if(P[0].verbose)
        fprintf(stdout, "==========================================\n");
      }
    }
  else{ // IF THREADS
    fprintf(stderr, "Using multi-threading to load reference ...\n");
    pthread_t t[nThreads];
    for(k = 0 ; k < nKmers ; ++k)
      P[k].M = CreateCModel(P[k].context, P[k].inverse); 
    for(n = 0 ; n < nThreads ; ++n)
      pthread_create(&(t[n+1]), NULL, LoadRefThread, (void *) &(P[n]));
    for(n = 0 ; n < nThreads ; ++n) // DO NOT JOIN FORS!
      pthread_join(t[n+1], NULL);
    for(k = 0 ; k < nKmers ; ++k){
      P[0].size = (uint64_t *) Calloc(P[0].nTar, sizeof(uint64_t));
      for(n = 0 ; n < P[0].nTar ; ++n){
        FILE *Reader = Fopen(P[0].tar[n], "r");
        P[0].size[n] = NDNASyminFile(Reader);
        fclose(Reader);
        }
      for(n = 0 ; n < P[0].nTar ; ++n)
        Target(P[k], n);
      DeleteCModel(P[k].M);
      if(P[0].verbose)
        fprintf(stdout, "==========================================\n");
      }
    }

  return EXIT_SUCCESS;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

