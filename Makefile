CC = gcc
CFLAGS = -pedantic -Wall -g

objects = main.o venda.o avl.o catalogo_clientes.o faturacao.o navegacao.o

gerecompras: $(objects)
	$(CC) $(CFLAGS) -o gerecompras $(objects)

run: gerecompras
	./gerecompras

recompile:
	make clean
	make

.PHONY: clean
clean :
	rm -f gerecompras
	rm -f $(objects)
