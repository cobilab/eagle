#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <time.h>
#include "mem.h"
#include "defs.h"
#include "common.h"
#include "context.h"

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - T A R G E T - - - - - - - - - - - - - - -

void Target(Parameters *P, CModel *cModel, uint8_t id)
  {
  FILE        *Reader  = Fopen(P->tar[id], "r");
  char        *name    = concatenate(P->tar[id], ".ea");
  FILE        *Writter = Fopen(name, "w");
  uint64_t    nSymbols = NDNASyminFile(Reader), i = 0, unique = 0;
  uint32_t    k, idxPos;
  int32_t     idx = 0;
  uint8_t     *writterBuffer, *readerBuffer, *symbolBuffer, sym;

  if(P->verbose)
    fprintf(stderr, "Searching target sequence %d ...\n", id + 1);

  writterBuffer = (uint8_t  *) Calloc(BUFFER_SIZE,          sizeof(uint8_t));
  readerBuffer  = (uint8_t  *) Calloc(BUFFER_SIZE,          sizeof(uint8_t));
  symbolBuffer  = (uint8_t  *) Calloc(BUFFER_SIZE + BGUARD, sizeof(uint8_t));
  symbolBuffer += BGUARD;

  while((k = fread(readerBuffer, 1, BUFFER_SIZE, Reader)))
    {
    for(idxPos = 0 ; idxPos < k ; ++idxPos)
      {
      sym = DNASymToNum(readerBuffer[idxPos]);
      if(sym == 4) continue;
      symbolBuffer[idx] = sym;

      GetPModelIdx(symbolBuffer+idx-1, cModel);            
      if(cModel->array.counters[cModel->pModelIdx] == 0)
        {
        writterBuffer[idxPos] = 45;
        ++unique;
        }
      else
        writterBuffer[idxPos] = 88;

      if(++idx == BUFFER_SIZE)
        {
        memcpy(symbolBuffer - BGUARD, symbolBuffer + idx - BGUARD, BGUARD);
        idx = 0;
        }

      ++i;
      #ifdef PROGRESS
      CalcProgress(nSymbols, i);
      #endif
      }
    fprintf(Writter, "%s", writterBuffer);
    fflush(Writter);
    }

  fclose(Writter);
  Free(name);
  ResetCModelIdx(cModel);
  Free(readerBuffer);
  Free(writterBuffer);
  Free(symbolBuffer-BGUARD);
  fclose(Reader);

  fprintf(stderr, "Uniqueness: %.3lf %% (%"PRIu64" in %"PRIu64")\n", (double) 
  unique / nSymbols * 100.0, unique, nSymbols);  

  if(P->verbose == 1)
    fprintf(stderr, "Done!                          \n");  // SPACES ARE VALID 
  }


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - R E F E R E N C E - - - - - - - - - - - - -

CModel *LoadReference(Parameters *P)
  {
  FILE      *Reader = Fopen(P->ref, "r");
  uint32_t  k, idxPos;
  int32_t   idx = 0;
  uint8_t   *readerBuffer, *symbolBuffer, sym;
  CModel    *cModel;
  #ifdef PROGRESS
  uint64_t  i = 0, size = NBytesInFile(Reader);
  #endif

  if(P->verbose == 1)
    fprintf(stderr, "Building reference model ...\n");

  readerBuffer  = (uint8_t *) Calloc(BUFFER_SIZE + 1, sizeof(uint8_t));
  symbolBuffer  = (uint8_t *) Calloc(BUFFER_SIZE + BGUARD+1, sizeof(uint8_t));
  symbolBuffer += BGUARD;
  cModel        = CreateCModel(P->model[0].ctx, P->model[0].ir);

  while((k = fread(readerBuffer, 1, BUFFER_SIZE, Reader)))
    for(idxPos = 0 ; idxPos < k ; ++idxPos)
      {
      sym = DNASymToNum(readerBuffer[idxPos]);
      if(sym == 4) continue;
      symbolBuffer[idx] = sym;

      GetPModelIdx(symbolBuffer+idx-1, cModel);
      cModel->array.counters[cModel->pModelIdx] = 1;
      if(cModel->ir == 1)                                  // Inverted repeats
        {
        GetPModelIdxIR(symbolBuffer+idx, cModel);
        cModel->array.counters[cModel->pModelIdx] = 1;
        }

      if(++idx == BUFFER_SIZE)
        {
        memcpy(symbolBuffer - BGUARD, symbolBuffer + idx - BGUARD, BGUARD);
        idx = 0;
        }
      #ifdef PROGRESS
      CalcProgress(size, ++i);
      #endif
      }
 
  ResetCModelIdx(cModel);
  Free(readerBuffer);
  Free(symbolBuffer-BGUARD);
  fclose(Reader);

  if(P->verbose == 1)
    fprintf(stderr, "Done!                          \n");  // SPACES ARE VALID  

  return cModel;
  }

  
//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - E A G L E   M A I N - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int32_t main(int argc, char *argv[])
  {
  char        **p = *&argv;
  CModel      *refModels;
  uint32_t    n;
  Parameters  *P;

  P = (Parameters *) Malloc(1 * sizeof(Parameters));
  if((P->help = ArgsState(DEFAULT_HELP, p, argc, "-h")) == 1 || argc < 2)
    {
    fprintf(stderr, "                                                    \n");
    fprintf(stderr, "Usage: Eagle [OPTIONS]... -r [FILE]  [FILE]:[...]   \n");
    fprintf(stderr, "                                                    \n");
    fprintf(stderr, "  -v                       verbose mode             \n");
    fprintf(stderr, "  -c  <ctx>                context size model       \n");
    fprintf(stderr, "  -i                       use inversions           \n");
    fprintf(stderr, "  -r  <rFile>              reference file (database)\n");
    fprintf(stderr, "                                                    \n");
    fprintf(stderr, "  <tFile1>:<tFile2>:<...>  target file(s)         \n\n");
    return EXIT_SUCCESS;
    }

  fprintf(stderr, "===============> Eagle v1.0 <=============\n");
  
  P->model        = (ModelPar *) Calloc(1, sizeof(ModelPar));
  P->verbose      = ArgsState  (DEFAULT_VERBOSE, p, argc, "-v");
  P->model[0].ir  = ArgsState  (DEFAULT_IR     , p, argc, "-i");
  P->model[0].ctx = ArgsNumber (DEFAULT_CTX    , p, argc, "-c");
  P->ref          = ArgsString (NULL, p, argc, "-r");
  P->nTar         = ReadFNames (P, argv[argc-1]);
  if(P->verbose) 
    PrintArgs(P);

  refModels = LoadReference(P);

  for(n = 0 ; n < P->nTar ; ++n)
    Target(P, refModels, n);

  return EXIT_SUCCESS;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

