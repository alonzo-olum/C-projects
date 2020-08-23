CC=gcc
_OBJ=hash_func.o main.o
ODIR=obj/
_SRC=hash_func.c main.c
SDIR=src/
_DEPS=hash.h
IDIR=include/

CFLAGS=-I$(IDIR)

OBJ=$(patsubst %, $(ODIR)%, $(_OBJ))
SRC=$(patsubst %, $(SDIR)%, $(_SRC))
DEPS=$(patsubst %, $(IDIR)%, $(_DEPS))

$(ODIR)%.o : $(SDIR)%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hashmake : $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean :
	rm -f $(OBJ) *.exe
