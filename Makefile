CC = gcc
CFLAGS = -ansi -pedantic -Wall -g -O2
VALFLAGS = --tool=memcheck --leak-check=full

objects = queries.o venda.o avl.o navegacao.o menu.o faturacao.o gestao_filiais.o catalogo_clientes.o catalogo_produtos.o leitura_ficheiros.o main.o

gerevendas: $(objects)
	$(CC) $(CFLAGS) -o gerevendas $(objects)

run: gerevendas
	./gerevendas

recompile:
	make clean
	make

valgrind:
	make
	valgrind $(VALFLAGS) ./gerevendas

entrega: recompile
	rm -f LI3_Grupo3.zip
	zip LI3_Grupo3.zip Makefile *.c *.h

.PHONY: clean
clean :
	rm -f gerevendas
	rm -f $(objects)
