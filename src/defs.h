#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <stdint.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <unistd.h>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// RUNNING COMPILATION PROPERTIES :: COMMENT OR UNCOMMENT 

#define PROGRESS // DISPLAY % OF PROGRESS
#define PROFILE  // DISPLAY A PROFILE WITH EXISTS OR NOT

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
  U32       id;
  }
CModel;

typedef struct{
  CModel    **M;
  uint32_t  nModels;
  }
CModels;

typedef struct{
  uint8_t   verbose;
  uint8_t   force;
  uint8_t   plots;
  uint8_t   stdout;
  char      *output;
  char      *ref;
  char      **tar;
  uint64_t  nTar;
  uint32_t  nKmers;
  uint32_t  nThreads;
  uint32_t  min_ctx;
  uint32_t  inverse;
  uint64_t  *size;
  }
Param;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define VERSION                2
#define RELEASE                1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define BUFFER_SIZE            65535    
#define PROGRESS_MIN           10000
#define DEF_HELP               0
#define DEF_FORCE              0
#define DEF_VERBOSE            0
#define DEF_IR                 1
#define DEF_THREADS            1
#define DEF_OUT                0
#define DEF_PLOTS              0
#define DEF_MIN_CTX            11
#define DEF_MAX_CTX            15
#define BGUARD                 32
#define ALPHABET_SIZE          4

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

