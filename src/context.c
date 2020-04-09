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

static void InitHashTable(CModel *cModel){
  cModel->hash.keys      = (KEYSMAX **) Calloc(HASH_SIZE, sizeof(KEYSMAX *));
  cModel->hash.entrySize = (ENTMAX   *) Calloc(HASH_SIZE, sizeof(ENTMAX   ));
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void InitArray(CModel *cModel){
  cModel->array.counters = (ACCounter *) Calloc(cModel->nPModels, 
  sizeof(ACCounter));
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void DeleteCModel(CModel *M){
  uint32_t k;
  if(M->mode == HASH_TABLE_MODE){
    for(k = 0 ; k < HASH_SIZE ; ++k)
      if(M->hash.entrySize[k] != 0)
        Free(M->hash.keys[k]);
    Free(M->hash.keys);
    Free(M->hash.entrySize);
    }
  else // TABLE_MODE
    Free(M->array.counters);
  Free(M);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CModel *CreateCModel(U32 ctx, U32 ir){
  CModel *M;
  U64    prod = 1, *multipliers;
  U32    n;

  M = (CModel *) Calloc(1, sizeof(CModel));

  if(ctx > MAX_HASH_CTX){
    fprintf(stderr, "Error: context is greater than %d!\n", MAX_HASH_CTX);
    exit(1);
    }
  
  multipliers  = (U64*) Calloc(ctx, sizeof(U64));
  M->nPModels  = (U64) pow(ALPHABET_SIZE, ctx);
  M->ctx       = ctx;
  M->idx       = 0;
  M->idxIR     = M->nPModels - 1;
  M->ir        = ir  == 0 ? 0 : 1;

  if(ctx >= HASH_TABLE_BEGIN_CTX){
    M->mode = HASH_TABLE_MODE;
    InitHashTable(M);
    }
  else{
    M->mode = ARRAY_MODE;
    InitArray(M);
    }
  
  for(n = 0 ; n < M->ctx ; ++n){
    multipliers[n] = prod;
    prod <<= 2;
    }

  M->multiplier = multipliers[M->ctx-1];

  Free(multipliers);
  return M;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ResetIdx(CModel *M){
  M->idx   = 0;
  M->idxIR = M->nPModels - 1;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void GetIdxIR(U8 *p, CModel *M){
  M->idxIR = (M->idxIR >> 2) + GetCompNum(*p) * M->multiplier;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void GetIdx(U8 *p, CModel *M){
  M->idx = ((M->idx - *(p - M->ctx) * M->multiplier) << 2) + *p;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void InsertKey(Hash *hash, U32 h, U64 key){
  hash->keys[h] = (KEYSMAX *) Realloc(hash->keys[h], (hash->entrySize[h] + 1) 
  * sizeof(KEYSMAX), sizeof(KEYSMAX));
  hash->keys[h][hash->entrySize[h]] = (U32) (key / HASH_SIZE);
  hash->entrySize[h]++;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void UpdateIR(CModel *M){
  U32 n;
  U64 idx = M->idxIR;

  if(M->mode == HASH_TABLE_MODE){
    U32 h = idx % HASH_SIZE;                                 // The hash index
    for(n = 0 ; n < M->hash.entrySize[h] ; n++)
      if(((U64) M->hash.keys[h][n] * HASH_SIZE) + h == idx) 
        return;
    InsertKey(&M->hash, h, idx);                           // If key not found
    }
  else
    M->array.counters[idx] = 1;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Update(CModel *M){
  U32 n;
  U64 idx = M->idx;

  if(M->mode == HASH_TABLE_MODE){
    U32 h = idx % HASH_SIZE;                                 // The hash index
    for(n = 0 ; n < M->hash.entrySize[h] ; n++)
      if(((U64) M->hash.keys[h][n] * HASH_SIZE) + h == idx) 
        return;
    InsertKey(&M->hash, h, idx);                           // If key not found
    }
  else
    M->array.counters[idx] = 1;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void NEntries(CModel *M){
  U32 max = 0, k = 0;
  for( ; k < HASH_SIZE ; ++k)
    if(max < M->hash.entrySize[k])
      max = M->hash.entrySize[k];
  fprintf(stderr, "Maximum hash entry size: %u\n", max);       
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
