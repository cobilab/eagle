#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include "mem.h"
#include "defs.h"
#include "common.h"
#include "context.h"
#include "paint.h"

//TODO: USE DEFAULT ENLARGE = SUSAMPLE
//TODO: hide N's from plot

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - S U B S A M P L E - - - - - - - - - - - -

void SetSubsample(Parameters *P, uint64_t max)
  {
  if(P->sub != -1)
    return;  

  if(max < DEFAULT_SAMPLE_RATIO)
    { 
    P->sub = 1;
    return;
    }

  P->sub = max / DEFAULT_SAMPLE_RATIO;
  }


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - T A R G E T - - - - - - - - - - - - - - -

void Target(Parameters *P, CModel *cModel, uint8_t id, Painter *Paint, FILE 
*Plot)
  {
  FILE        *Reader  = Fopen(P->tar[id], "r");
  #ifdef STREAM
  char        *name1   = concatenate(P->tar[id], ".str");
  FILE        *Writter = Fopen(name1, "w");
  #endif
  #ifdef POSITIONS
  char        *name2     = concatenate(P->tar[id], ".pos");
  FILE        *Positions = Fopen(name2, "w");
  #endif
  uint64_t    nSymbols = NDNASyminFile(Reader), i = 0, init, repeated = 0,
              unknown = 0;
  uint32_t    n, k, idxPos, hIndex;
  int32_t     idx = 0;
  uint8_t     *writterBuffer, *readerBuffer, *symbolBuffer, sym, region, 
              found;

  if(P->verbose)
    fprintf(stderr, "Searching target sequence %d ...\n", id + 1);

  if(id != 0)
    Paint->cx += DEFAULT_WIDTH + DEFAULT_SPACE;

  writterBuffer = (uint8_t  *) Calloc(BUFFER_SIZE,          sizeof(uint8_t));
  readerBuffer  = (uint8_t  *) Calloc(BUFFER_SIZE,          sizeof(uint8_t));
  symbolBuffer  = (uint8_t  *) Calloc(BUFFER_SIZE + BGUARD, sizeof(uint8_t));
  symbolBuffer += BGUARD;

  init   = 0;
  region = NO_MATCH_REGION;
  while((k = fread(readerBuffer, 1, BUFFER_SIZE, Reader)))
    {
    for(idxPos = 0 ; idxPos < k ; ++idxPos)
      {
      sym = DNASymToNum(readerBuffer[idxPos]);
      if(sym == 4) 
        {
        if(i % P->sub == 0)
          Rect(Plot, Paint->width, GetPoint(1+P->ptN), Paint->cx,
          Paint->cy + GetPoint(i+1), GetRgbColor(LEVEL_HUE_N));

        // TODO: Stop block or erase them from positions file

        #ifdef STREAM
        writterBuffer[idxPos] = N_SYMBOL;
        #endif

        ++i;
        #ifdef PROGRESS
        CalcProgress(nSymbols, i);
        #endif
        ++unknown;
        continue;
        }
      symbolBuffer[idx] = sym;

      GetIdx(symbolBuffer+idx-1, cModel);            

      #ifdef STREAM
      writterBuffer[idxPos] = UNIQUE_SYMBOL;
      #endif
      if(cModel->mode == 0)  // TABLE MODE
        {
        if(cModel->array.counters[cModel->idx] != 0)  // WORD FOUND
          {
          #ifdef STREAM
          writterBuffer[idxPos] = MATCH_SYMBOL;
          #endif
          ++repeated;

          if(i == 0)
            region = MATCHED_REGION;

          if(region == NO_MATCH_REGION)  // NO REGION FOUND
            {
            #ifdef POSITIONS
            fprintf(Positions, "%"PRIu64":%"PRIu64"\n", init + 1, i);
            #endif
            region = MATCHED_REGION;
            }
          }
        else  // NO WORD FOUND
          {
          if(i % P->sub == 0)
            Rect(Plot, Paint->width, GetPoint(1+P->pt), Paint->cx,
            Paint->cy + GetPoint(i+1), GetRgbColor(LEVEL_HUE));

          #ifdef STREAM
          writterBuffer[idxPos] = UNIQUE_SYMBOL; 
          #endif
          if(region == MATCHED_REGION)  // THERE WAS A REGION
            {
            region = NO_MATCH_REGION;
            init   = i;
            }
          }
        }
      else  // HASH TABLE
        {
        found = 0;
        hIndex = cModel->idx % HASH_SIZE;
        for(n = 0 ; n < cModel->hash.entrySize[hIndex] ; n++)
          if(((uint64_t) cModel->hash.keys[hIndex][n] * HASH_SIZE) + hIndex == 
          cModel->idx)
          //if(cModel->hash.keys[hIndex][n] == cModel->idx)
            {
            #ifdef STREAM
            writterBuffer[idxPos] = MATCH_SYMBOL;
            #endif
            ++repeated;

            if(i == 0)
              region = MATCHED_REGION;

            if(region == NO_MATCH_REGION)  // NO REGION FOUND
              {
              #ifdef POSITIONS
              fprintf(Positions, "%"PRIu64":%"PRIu64"\n", init + 1, i);
              #endif
              region = MATCHED_REGION;
              }

            found = 1;
            break;
            }

        if(found == 0) // NO WORD FOUND
          {
          if(i % P->sub == 0)
            Rect(Plot, Paint->width, GetPoint(1+P->pt), Paint->cx,
            Paint->cy + GetPoint(i+1), GetRgbColor(LEVEL_HUE));
          #ifdef STREAM
          writterBuffer[idxPos] = UNIQUE_SYMBOL;
          #endif
          if(region == MATCHED_REGION)  // THERE WAS A REGION
            {
            region = NO_MATCH_REGION;
            init   = i;
            }
          }
        }

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
    #ifdef STREAM
    fprintf(Writter, "%s", writterBuffer);
    fflush(Writter);
    #endif
    }

  #ifdef POSITIONS
  if(region == NO_MATCH_REGION)
    fprintf(Positions, "%"PRIu64":%"PRIu64"\n", init + 1, i);
  #endif

  #ifdef STREAM
  fclose(Writter);
  Free(name1);
  #endif
  #ifdef POSITIONS
  fclose(Positions);
  Free(name2);
  #endif
  Chromosome(Plot, Paint->width, GetPoint(P->size[id]), Paint->cx, Paint->cy);
  ResetIdx(cModel);
  Free(readerBuffer);
  Free(writterBuffer);
  Free(symbolBuffer-BGUARD);
  fclose(Reader);

  fprintf(stderr, "Found: %"PRIu64" unknown symbols.\n", unknown);
  fprintf(stderr, "Repeated: %.4lf %% (%"PRIu64" in %"PRIu64")\n", (double) 
  repeated / (nSymbols-unknown) * 100.0, repeated, nSymbols-unknown);  

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

      GetIdx(symbolBuffer+idx-1, cModel);
      Update(cModel);
      if(cModel->ir == 1)                                  // Inverted repeats
        {
        GetIdxIR(symbolBuffer+idx, cModel);
        UpdateIR(cModel);
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
 
  ResetIdx(cModel);
  Free(readerBuffer);
  Free(symbolBuffer-BGUARD);
  fclose(Reader);

  if(P->verbose == 1)
    {
    if(cModel->mode == HASH_TABLE_MODE)
      NEntries(cModel);
    fprintf(stderr, "Done!                          \n");  // SPACES ARE VALID  
    }

  return cModel;
  }

  
//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - E A G L E   M A I N - - - - - - - - - - - - - - -

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
    fprintf(stderr, "Usage: Eagle <OPTIONS>... -r [FILE]  [FILE]:<...>   \n");
    fprintf(stderr, "                                                    \n");
    fprintf(stderr, "  -v                       verbose mode             \n");
    fprintf(stderr, "  -c  <ctx>                context size model       \n");
    fprintf(stderr, "  -i                       use inversions           \n");
    fprintf(stderr, "  -ea <pts>                enlarge absent           \n");
    fprintf(stderr, "  -en <pts>                enlarge N's              \n");
    fprintf(stderr, "  -s  <sub>                sub-sample               \n");
    fprintf(stderr, "  -o  <oFile>              output map file          \n");
    fprintf(stderr, "                                                    \n");
    fprintf(stderr, "  -r  [rFile]              reference file (database)\n");
    fprintf(stderr, "                                                    \n");
    fprintf(stderr, "  [tFile1]:<tFile2>:<...>  target file(s)         \n\n");
    return EXIT_SUCCESS;
    }

  fprintf(stderr, "===============> Eagle v1.0 <=============\n");
  
  P->model        = (ModelPar *) Calloc(1, sizeof(ModelPar));
  P->verbose      = ArgsState  (DEFAULT_VERBOSE  , p, argc, "-v" );
  P->model[0].ir  = ArgsState  (DEFAULT_IR       , p, argc, "-i" );
  P->model[0].ctx = ArgsNumber (DEFAULT_CTX      , p, argc, "-c" );
  P->pt           = ArgsNumber (0                , p, argc, "-ea");
  P->ptN          = ArgsNumber (0                , p, argc, "-en");
  P->sub          = ArgsNumber (DEFAULT_SUBSAMPLE, p, argc, "-s" );
  P->ref          = ArgsString (NULL             , p, argc, "-r" );
  P->output       = ArgsString ("plot"           , p, argc, "-o" );
  P->nTar         = ReadFNames (P, argv[argc-1]);
  if(P->verbose) 
    {
    PrintArgs(P);
    fprintf(stderr, "==========================================\n");
    }

  refModels = LoadReference(P);

  char     *name       = concatenate(P->output, ".svg");
  FILE     *Plot       = Fopen(name, "w");
  char     backColor[] = "#ffffff";
  Painter  *Paint;
  uint64_t max = 0;

  P->size = (uint64_t *) Calloc(P->nTar, sizeof(uint64_t));
  for(n = 0 ; n < P->nTar ; ++n)
    {
    FILE *Reader = Fopen(P->tar[n], "r");
    P->size[n] = NDNASyminFile(Reader);
    if(P->size[n] > max)
      max = P->size[n];
    fclose(Reader);
    }

  SetScale(max);
  SetSubsample(P, max);
  if(P->verbose == 1)
    fprintf(stderr, "Subsample ratio: %u\n", P->sub);

  Paint = CreatePainter(GetPoint(max), backColor);
  PrintHead(Plot, (2 * DEFAULT_CX) + (((Paint->width + DEFAULT_SPACE) * 
  P->nTar) - DEFAULT_SPACE), Paint->size + EXTRA);
  Rect(Plot, (2 * DEFAULT_CX) + (((Paint->width + DEFAULT_SPACE) *
  P->nTar) - DEFAULT_SPACE), Paint->size + EXTRA, 0, 0, backColor);

  for(n = 0 ; n < P->nTar ; ++n)
    Target(P, refModels, n, Paint, Plot);

  PrintFinal(Plot);
  Free(name); 

  return EXIT_SUCCESS;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

