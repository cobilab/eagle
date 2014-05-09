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

CModel *CreateCModel(uint32_t ctx, uint32_t ir) 
  {
  CModel    *cModel;
  uint64_t  prod = 1, *multipliers;
  uint32_t  n;

  cModel = (CModel *) Calloc(1, sizeof(CModel));

  if(ctx > MAX_CTX)
    {
    if(ctx == 32)
      fprintf(stderr, "Warning: context 32 is reserved to Magic Johnson.\n");
    fprintf(stderr, "Error: context cannot be greater than %d\n", MAX_CTX);
    exit(1);
    }
  
  multipliers           = (uint64_t *) Calloc(ctx, sizeof(uint64_t));
  cModel->nPModels      = (uint64_t) pow(ALPHABET_SIZE, ctx);
  cModel->ctx           = ctx;
  cModel->idx           = 0;
  cModel->idxIR         = cModel->nPModels - 1;
  cModel->ir            = ir  == 0 ? 0 : 1;

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

inline void GetIdxIR(uint8_t *p, CModel *M)
  {
  M->idxIR = (M->idxIR >> 2) + GetCompNum(*p) * M->multiplier;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline void GetIdx(uint8_t *p, CModel *M)
  {
  M->idx = ((M->idx - *(p - M->ctx) * M->multiplier) << 2) + *p;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void InsertKey(Hash *hash, unsigned hIndex, uint64_t key)
  {
  hash->keys[hIndex] = (KEYSMAX *) Realloc(hash->keys[hIndex],
  (hash->entrySize[hIndex] + 1) * sizeof(KEYSMAX), sizeof(KEYSMAX));

  hash->keys[hIndex][hash->entrySize[hIndex]] = key;
  hash->entrySize[hIndex]++;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void UpdateIR(CModel *cModel)
  {
  unsigned   n;
  uint64_t   idx = cModel->idxIR;

  if(cModel->mode == HASH_TABLE_MODE)
    {
    unsigned hIndex = idx % HASH_SIZE;                       // The hash index

    for(n = 0 ; n < cModel->hash.entrySize[hIndex] ; n++)
      if(cModel->hash.keys[hIndex][n] == idx)                  // If key found
        return;

    InsertKey(&cModel->hash, hIndex, idx);                 // If key not found
    }
  else
    {
    cModel->array.counters[idx] = 1;
    }
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Update(CModel *cModel)
  {
  unsigned   n;
  uint64_t   idx = cModel->idx;

  if(cModel->mode == HASH_TABLE_MODE)
    {
    unsigned hIndex = idx % HASH_SIZE;                       // The hash index

    for(n = 0 ; n < cModel->hash.entrySize[hIndex] ; n++)
      if(cModel->hash.keys[hIndex][n] == idx)                  // If key found
        return;

    InsertKey(&cModel->hash, hIndex, idx);                 // If key not found
    }
  else
    {
    cModel->array.counters[idx] = 1;
    }
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void NEntries(CModel *cModel)
  {
  uint32_t max = 0, k;
  for(k = 0 ; k < HASH_SIZE ; ++k)
    if(max < cModel->hash.entrySize[k])
      max = cModel->hash.entrySize[k];
  fprintf(stderr, "Maximum hash entry size: %u\n", max);       
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
