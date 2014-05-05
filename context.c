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

void FreeCModel(CModel *cModel)
  {
  Free(cModel->array.counters);
  Free(cModel);
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
    fprintf(stderr, "Error: context cannot be greater than %d\n", MAX_CTX);
    exit(1);
    }
  
  multipliers           = (uint64_t *) Calloc(ctx, sizeof(uint64_t));
  cModel->nPModels      = (uint64_t) pow(ALPHABET_SIZE, ctx);
  cModel->ctx           = ctx;
  cModel->pModelIdx     = 0;
  cModel->pModelIdxIR   = cModel->nPModels - 1;
  cModel->ir            = ir  == 0 ? 0 : 1;
  InitArray(cModel);
  
  for(n = 0 ; n != cModel->ctx ; ++n)
    {
    multipliers[n] = prod;
    prod <<= 2;
    }

  cModel->multiplier = multipliers[cModel->ctx-1];

  return cModel;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ResetCModelIdx(CModel *cModel)
  {
  cModel->pModelIdx   = 0;
  cModel->pModelIdxIR = cModel->nPModels - 1;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline void GetPModelIdxIR(uint8_t *p, CModel *M)
  {
  M->pModelIdxIR = (M->pModelIdxIR >> 2) + GetCompNum(*p) * M->multiplier;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline void GetPModelIdx(uint8_t *p, CModel *M)
  {
  M->pModelIdx = ((M->pModelIdx - *(p - M->ctx) * M->multiplier) << 2) + *p;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
