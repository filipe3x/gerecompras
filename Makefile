CC = gcc
CFLAGS = -pedantic -Wall -std=gnu89 -g
#CFLAGS = -pedantic -ansi -Wall -g
VALFLAGS = --tool=memcheck --leak-check=full

objects = queries.o venda.o avl.o navegacao.o menu.o faturacao.o gestao_filiais.o catalogo_clientes.o leitura_ficheiros.o main.o

gerecompras: $(objects)
	$(CC) $(CFLAGS) -o gerecompras $(objects)

run: gerecompras
	./gerecompras

recompile:
	make clean
	make

valgrind:
	make
	valgrind $(VALFLAGS) ./gerecompras

.PHONY: clean
clean :
	rm -f gerecompras
	rm -f $(objects)
