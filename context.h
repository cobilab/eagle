#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include "defs.h"

typedef unsigned short ACCounter;       // Size of context counters for arrays

typedef struct
  {
  ACCounter       *counters;
  }
Array;

typedef struct
  {
  unsigned        ctx;                    // Current depth of context template
  ULL             nPModels;            // Maximum number of probability models
  uint64_t        multiplier;
  uint64_t        pModelIdx;
  uint64_t        pModelIdxIR;
  uint8_t         ir;
  Array           array;
  }
CModel;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void           FreeCModel            (CModel *);
inline void    GetPModelIdx          (uint8_t *, CModel *);
inline void    GetPModelIdxIR        (uint8_t *, CModel *);
void           ResetCModelIdx        (CModel *);
CModel         *CreateCModel         (uint32_t, uint32_t);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
