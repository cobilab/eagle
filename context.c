#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "defs.h"
#include "mem.h"
#include "common.h"
#include "context.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void InitHashTable(CModel *cModel)
  {
  cModel->hash.keys      = (KEYSMAX **) Calloc(HASH_SIZE, sizeof(KEYSMAX *));
  cModel->hash.entrySize = (ENTMAX   *) Calloc(HASH_SIZE, sizeof(ENTMAX   ));
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void InitArray(CModel *cModel)
  {
  cModel->array.counters = (ACCounter *) Calloc(cModel->nPModels, 
  sizeof(ACCounter));
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CModel *CreateCModel(U32 ctx, U32 ir) 
  {
  CModel *cModel;
  U64    prod = 1, *multipliers;
  U32    n;

  cModel = (CModel *) Calloc(1, sizeof(CModel));

  if(ctx > MAX_HASH_CTX)
    {
    fprintf(stderr, "Error: context is greater than %d!\n", MAX_HASH_CTX);
    exit(1);
    }
  
  multipliers       = (U64*) Calloc(ctx, sizeof(U64));
  cModel->nPModels  = (U64) pow(ALPHABET_SIZE, ctx);
  cModel->ctx       = ctx;
  cModel->idx       = 0;
  cModel->idxIR     = cModel->nPModels - 1;
  cModel->ir        = ir  == 0 ? 0 : 1;

  if(ctx >= HASH_TABLE_BEGIN_CTX)
    {
    cModel->mode = HASH_TABLE_MODE;
    InitHashTable(cModel);
    }
  else
    {
    cModel->mode = ARRAY_MODE;
    InitArray(cModel);
    }
  
  for(n = 0 ; n != cModel->ctx ; ++n)
    {
    multipliers[n] = prod;
    prod <<= 2;
    }

  cModel->multiplier = multipliers[cModel->ctx-1];

  return cModel;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ResetIdx(CModel *cModel)
  {
  cModel->idx   = 0;
  cModel->idxIR = cModel->nPModels - 1;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline void GetIdxIR(U8 *p, CModel *M)
  {
  M->idxIR = (M->idxIR >> 2) + GetCompNum(*p) * M->multiplier;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline void GetIdx(U8 *p, CModel *M)
  {
  M->idx = ((M->idx - *(p - M->ctx) * M->multiplier) << 2) + *p;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void InsertKey(Hash *hash, U32 hIndex, U64 key)
  {
  hash->keys[hIndex] = (KEYSMAX *) Realloc(hash->keys[hIndex],
  (hash->entrySize[hIndex] + 1) * sizeof(KEYSMAX), sizeof(KEYSMAX));

  hash->keys[hIndex][hash->entrySize[hIndex]] = (U32) (key / HASH_SIZE);
  hash->entrySize[hIndex]++;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void UpdateIR(CModel *cModel)
  {
  U32 n;
  U64 idx = cModel->idxIR;

  if(cModel->mode == HASH_TABLE_MODE)
    {
    U32 hIndex = idx % HASH_SIZE;                            // The hash index
    for(n = 0 ; n < cModel->hash.entrySize[hIndex] ; n++)
      if(((U64) cModel->hash.keys[hIndex][n] * HASH_SIZE) + hIndex == idx) 
        return;
    InsertKey(&cModel->hash, hIndex, idx);                 // If key not found
    }
  else
    cModel->array.counters[idx] = 1;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Update(CModel *cModel)
  {
  U32 n;
  U64 idx = cModel->idx;

  if(cModel->mode == HASH_TABLE_MODE)
    {
    U32 hIndex = idx % HASH_SIZE;                            // The hash index
    for(n = 0 ; n < cModel->hash.entrySize[hIndex] ; n++)
      if(((U64) cModel->hash.keys[hIndex][n] * HASH_SIZE) + hIndex == idx) 
        return;
    InsertKey(&cModel->hash, hIndex, idx);                 // If key not found
    }
  else
    cModel->array.counters[idx] = 1;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void NEntries(CModel *cModel)
  {
  U32 max = 0, k = 0;
  for( ; k < HASH_SIZE ; ++k)
    if(max < cModel->hash.entrySize[k])
      max = cModel->hash.entrySize[k];
  fprintf(stderr, "Maximum hash entry size: %u\n", max);       
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
