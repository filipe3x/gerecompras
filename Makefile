CC = gcc
CFLAGS = -ansi -pedantic -Wall -g -O2
#CFLAGS = -pedantic -Wall
VALFLAGS = --tool=memcheck --leak-check=full --show-leak-kinds=all
FICHEIROS = Clientes.txt Produtos.txt Vendas_100K.txt

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
	valgrind $(VALFLAGS) ./gerevendas $(FICHEIROS)

entrega: recompile
	rm -f LI3_Grupo3.zip
	zip LI3_Grupo3.zip Makefile *.c *.h

.PHONY: clean
clean :
	rm -f gerevendas
	rm -f $(objects)

venda.o: venda.h
navegacao.o: navegacao.h
faturacao.o: faturacao.h
catalogo_clientes.o: avl.h venda.h navegacao.h catalogo_clientes.h
catalogo_produtos.o: avl.h venda.h navegacao.h catalogo_produtos.h
gestao_filiais.o: venda.h avl.h navegacao.h catalogo_produtos.h catalogo_clientes.h gestao_filiais.h
main.o: venda.h navegacao.h faturacao.h catalogo_clientes.h catalogo_produtos.h gestao_filiais.h leitura_ficheiros.h menu.h main.c
menu.o: venda.h navegacao.h catalogo_produtos.h catalogo_clientes.h gestao_filiais.h faturacao.h leitura_ficheiros.h queries.h menu.h
queries.o: venda.h navegacao.h catalogo_clientes.h catalogo_produtos.h gestao_filiais.h faturacao.h leitura_ficheiros.h menu.h queries.h
leitura_ficheiros.o: venda.h navegacao.h catalogo_clientes.h catalogo_produtos.h gestao_filiais.h faturacao.h menu.h leitura_ficheiros.h
