CC=gcc
CFLAGS=-I -Wall.
DEPS = abrir_ficheiros.h
OBJ = gerecompras.o abrir_ficheiros.o vendas.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

gerecompras: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
