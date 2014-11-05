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

#define MAX 	100
#define LENGTH 	19000

int main(int argc, char *argv[]){
  FILE *IN  = NULL;
  FILE *OUT = NULL;
  unsigned x, y, strain, pos, order, **matrix;

  if(argc < 2) {
    printf("Usage: rebat <FILE>\n");
    return 1;
    }

  printf("Starting rebat ...\n");
  IN  = Fopen(argv[argc-1], "r");
  OUT = Fopen("out", "w");
  matrix = (unsigned **) malloc(MAX * sizeof(unsigned *));
  for(x = 1 ; x < MAX ; ++x){
    matrix[x] = (unsigned *) malloc(LENGTH * sizeof(unsigned));
    for(y = 1 ; y < LENGTH ; ++y)
      matrix[x][y] = 15;
    }

  while(fscanf(IN, "%u\t%u\t%u\n", &strain, &pos, &order) == 3){
    if(matrix[strain][pos] > order)
      matrix[strain][pos] = order;  
    }

  fprintf(OUT, "# X	Y	Z\n");
  for(x = 1 ; x < MAX ; ++x)
    for(y = 1 ; y < LENGTH ; ++y)
      if(matrix[x][y] != 15)
        fprintf(OUT, "%u\t%u\t%u\n", y, x, matrix[x][y]);

  printf("Done!\n");
  return 0;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
