#ifndef DIST_H_INCLUDED
#define DIST_H_INCLUDED

#include <stdio.h>
#include "defs.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

typedef struct{
  uint64_t  *bases;
  uint32_t  nBases;
  }
Counts;

typedef struct{
  Counts    *C;
  uint32_t  size;
  }
Dist;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Dist      *CreateDist  (uint32_t, uint32_t);
void      PrintDist    (Dist *, uint32_t);
void      PrintCG      (Dist *, uint32_t, uint32_t, uint32_t);
void      RemoveDist   (Dist *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

