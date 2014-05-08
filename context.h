#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include "defs.h"

typedef uint8_t   ACCounter;            // Size of context counters for arrays
typedef uint16_t  ENTMAX;                // Entry size (nKeys for each hIndex)
typedef uint64_t  KEYSMAX;                                  // keys index bits

typedef struct
  {
  ENTMAX          *entrySize;                  // Number of keys in this entry
  KEYSMAX         **keys;                  // The keys of the hash table lists
  }
Hash;

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
  uint64_t        idx;
  uint64_t        idxIR;
  uint8_t         ir;
  Array           array;
  Hash            hash;
  uint8_t         mode;
  }
CModel;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline void    GetIdx                (uint8_t *, CModel *);
inline void    GetIdxIR              (uint8_t *, CModel *);
void           ResetCModelIdx        (CModel *);
CModel         *CreateCModel         (uint32_t, uint32_t);
void           ResetIdx              (CModel *);
void           UpdateIR              (CModel *);
void           Update                (CModel *);
void           NEntries              (CModel *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
