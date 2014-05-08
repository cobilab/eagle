#============================================================================#
#                  EAGLE 2014, IEETA/DETI, UNIVERSITY OF AVEIRO              #
#============================================================================#
BIN    = .
CC     = gcc
CPLP   = -fstrict-aliasing -ffast-math -msse2 #-w #-g
#-----------------------------------------------------------------------------
CFLAGS = -O3 -Wall $(CPLP) -DPROGRESS -DPOSITIONS #-DSTREAM
#-----------------------------------------------------------------------------
LIBS   = -lm
DEPS   = defs.h
PROGS  = $(BIN)/eagle
OBJS   = mem.o common.o context.o paint.o
#-----------------------------------------------------------------------------
all:
	$(MAKE) progs
progs: $(PROGS)
$(BIN)/eagle: eagle.c $(DEPS) $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN)/eagle eagle.c $(OBJS) $(LIBS)
mem.o: mem.c mem.h $(DEPS)
	$(CC) -c $(CFLAGS) mem.c
common.o: common.c common.h $(DEPS)
	$(CC) -c $(CFLAGS) common.c
context.o: context.c context.h $(DEPS)
	$(CC) -c $(CFLAGS) context.c
paint.o: paint.c paint.h $(DEPS)
	$(CC) -c $(CFLAGS) paint.c
clean:
	/bin/rm -f *.o
cleanall:
	/bin/rm -f *.o $(PROGS)
#-----------------------------------------------------------------------------
