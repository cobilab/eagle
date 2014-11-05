#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *Cat(char *a, char *b){
  char *base = (char *) malloc(strlen(a) + strlen(b) + 1);
  strcpy(base, a);
  strcat(base, b);
  return base;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

FILE *Fopen(const char *p, const char *m){
  FILE *F = fopen(p, m);
  if(F == NULL){
    fprintf(stderr, "Error opening: %s (mode %s). Does it exist?\n", p, m);
    exit(1);
    }
  return F;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int main(int argc, char *argv[]){
  FILE *IN  = NULL;
  FILE *OUT = NULL;
  unsigned x, init, end;

  if(argc < 2) {
    printf("Usage: projector <FILE>\n");
    return 1;
    }

  printf("Starting projector ...\n");
  IN = Fopen(argv[argc-1], "r");
  OUT = Fopen(Cat(argv[argc-1], ".proj"), "w");
  while(fscanf(IN, "%u:%u\n", &init, &end) == 2){
    for(x = init ; x <= end ; ++x){
      fprintf(OUT, "%u\n", x);
      }
    }

  printf("Done all!\n");
  return 0;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
