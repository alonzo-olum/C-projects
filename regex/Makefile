CC=gcc
SDIR=src/
ODIR=obj/
IDIR=include/ 
_SRC=main.c matchhere.c
_OBJ=main.o matchhere.o
_DEPS=matchere.h

CFLAGS=-I$(IDIR)

OBJ=$(patsubst %, $(ODIR)%, $(_OBJ))
SRC=$(patsubst %, $(SDIR)%, $(_SRC))
DEPS=$(patsubst %, $(IDIR)%, $(_DEPS))

$(ODIR)%.o : $(SDIR)%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
matcher : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean  : 
	$(RM) $(OBJ) matcher
