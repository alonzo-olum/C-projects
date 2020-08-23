CC=gcc
SDIR=src/
ODIR=obj/
IDIR=include/
_SRC=main.c swap.c
_OBJ=main.o swap.o
_DEPS=swap.h

CFLAGS=-I$(IDIR)

OBJ=$(patsubst %, $(ODIR)%, $(_OBJ))
SRC=$(patsubst %, $(SDIR)%, $(_SRC))
DEPS=$(patsubst %, $(IDIR)%, $(_DEPS))

$(ODIR)%.o : $(SDIR)%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

swapper : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean : 
	$(RM) $(OBJ) swapper
