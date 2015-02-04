#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <stdint.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <unistd.h>

typedef uint64_t U64;
typedef uint32_t U32;
typedef uint16_t U16;
typedef uint8_t  U8; 
typedef int64_t  I64;
typedef int32_t  I32;
typedef int16_t  I16;
typedef int8_t   I8;

typedef U8  ACCounter;                  // Size of context counters for arrays
typedef U16 ENTMAX;                      // Entry size (nKeys for each hIndex)
typedef U32 KEYSMAX;                                        // keys index bits

typedef struct{
  ENTMAX    *entrySize;                        // Number of keys in this entry
  KEYSMAX   **keys;                        // The keys of the hash table lists
  }
Hash;

typedef struct{
  ACCounter *counters;
  }
Array;

typedef struct{
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

typedef struct{
  uint8_t  verbose;
  char     *ref;
  char     *output;
  char     **tar;
  uint8_t  nTar;
  uint8_t  id;
  uint32_t nKmers;
  uint32_t nThreads;
  uint32_t context;
  uint32_t inverse;
  uint64_t *size;
  CModel   *M;
  }
Param;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// EAGLE VALUES:
#define VERSION                1
#define RELEASE                1

// SYSTEM VALUES:
#define BUFFER_SIZE            262144      
#define PROGRESS_MIN           1000000
#define DEFAULT_HELP           0
#define DEFAULT_VERBOSE        0
#define DEFAULT_IR             0
#define DEFAULT_CTX            12
#define DEF_MIN_CTX            12
#define DEF_MAX_CTX            13
#define BGUARD                 32
#define ALPHABET_SIZE          4

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

