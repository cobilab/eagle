#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include "defs.h"

#define ARRAY_MODE            0
#define HASH_TABLE_MODE       1
#define HASH_TABLE_BEGIN_CTX  17
#define HASH_SIZE             33554467        // first PRIME NUMBER after 2^25
#define MAX_HASH_CTX          28
#define MIN_HASH_CTX          1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void           GetIdx           (U8 *, CModel *);
void           GetIdxIR         (U8 *, CModel *);
CModel         *CreateCModel    (U32, U32);
void           DeleteCModel     (CModel *);
void           ResetCModelIdx   (CModel *);
void           ResetIdx         (CModel *);
void           UpdateIR         (CModel *);
void           Update           (CModel *);
void           NEntries         (CModel *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
