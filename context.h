#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include "defs.h"

#define ARRAY_MODE            0
#define HASH_TABLE_MODE       1
#define HASH_TABLE_BEGIN_CTX  17
#define HASH_SIZE             33554467        // first PRIME NUMBER after 2^25
#define MAX_HASH_CTX          28

typedef U8  ACCounter;                  // Size of context counters for arrays
typedef U16 ENTMAX;                      // Entry size (nKeys for each hIndex)
typedef U32 KEYSMAX;                                        // keys index bits

typedef struct
  {
  ENTMAX    *entrySize;                        // Number of keys in this entry
  KEYSMAX   **keys;                        // The keys of the hash table lists
  }
Hash;

typedef struct
  {
  ACCounter *counters;
  }
Array;

typedef struct
  {
  U32       ctx;                          // Current depth of context template
  U64       nPModels;                  // Maximum number of probability models
  U64       multiplier;
  U64       idx;
  U64       idxIR;
  U8        ir;
  Array     array;
  Hash      hash;
  U8        mode;
  }
CModel;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline void    GetIdx           (U8 *, CModel *);
inline void    GetIdxIR         (U8 *, CModel *);
CModel         *CreateCModel    (U32, U32);
void           ResetCModelIdx   (CModel *);
void           ResetIdx         (CModel *);
void           UpdateIR         (CModel *);
void           Update           (CModel *);
void           NEntries         (CModel *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
