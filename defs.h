#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <stdint.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <unistd.h>

typedef unsigned long long ULL;

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
  char     **tar;
  uint8_t  nTar;
  uint64_t size;
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
#define MAX_CTX                15
#define MIN_CTX                1
#define HASH_TABLE_BEGIN_CTX   15
#define BGUARD                 32
#define DEFAULT_MAX_COUNT      ((1 << (sizeof(ACCounter) * 8)) - 1)
#define MX_PMODEL              65535
#define DEFAULT_HASH_SIZE      39999999
#define ALPHABET_SIZE          4

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

