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

typedef struct
  {
  uint32_t ctx;
  uint32_t ir;
  }
ModelPar;

typedef struct
  {
  uint8_t  help;
  uint8_t  verbose;
  ModelPar *model;
  char     *ref;
  char     *output;
  char     **tar;
  uint8_t  nTar;
  uint32_t pt;
  uint32_t ptN;
  int32_t  sub;
  uint64_t *size;
  }
Parameters;

uint32_t garbage;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// SYSTEM VALUES:
#define BUFFER_SIZE            262144      
#define PROGRESS_MIN           1000000
#define DEFAULT_HELP           0
#define DEFAULT_VERBOSE        0
#define DEFAULT_IR             0
#define DEFAULT_CTX            13
#define BGUARD                 32
#define DEFAULT_MAX_COUNT      ((1 << (sizeof(ACCounter) * 8)) - 1)
#define MX_PMODEL              65535
#define ALPHABET_SIZE          4
#define MATCH_SYMBOL           48 //88
#define UNIQUE_SYMBOL          49 //45
#define N_SYMBOL               78 //'N'
#define NO_MATCH_REGION        0             
#define MATCHED_REGION         1               
#define DEFAULT_SUBSAMPLE      -1
#define DEFAULT_SAMPLE_RATIO   50000 // 10000

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

