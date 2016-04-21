/*
 *  *  * Filipe Marques
 *  *  * Laboratórios Informática III, Universidade do Minho, 2016
 *  *  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#include "avl.h"
#include "venda.h"
#include "navegacao.h"
#include "catalogo_clientes.h"
#include "catalogo_produtos.h"
#include "gestao_filiais.h"
#include "faturacao.h"
#include "leitura_ficheiros.h"

static int validaVenda(VENDA venda, CATALOGO_PRODUTOS catalogoProdutos, CATALOGO_CLIENTES catalogoClientes);

void abrirFicheiroVendas(String nomeFicheiro, MODULO_GESTAO_FILIAIS moduloGestaoFiliais, 
	MODULO_FATURACAO moduloFaturacao, CATALOGO_PRODUTOS catalogoProdutos, CATALOGO_CLIENTES catalogoClientes){

	int i = 0;
	clock_t tini, tfin;
	const char *modoAbertura = "r";
	FILE *ficheiroVendas = fopen(nomeFicheiro, modoAbertura);

	String linhaLida = (String) malloc(128);
	VENDA novaVenda  = linhaVendaInit();
	StringVenda structStrVendas = stringVendaInit();

	tini = clock();
	while(fgets(linhaLida, MAX_BUFFER_VENDAS, ficheiroVendas) && i < MAXLINHAS){
		linhaLida[strcspn( linhaLida, "\r\n")] = 0;
		lerLinhaVenda(linhaLida, structStrVendas);
		inserirLinhaVenda(novaVenda, structStrVendas);
		if(validaVenda(novaVenda, catalogoProdutos, catalogoClientes)){
			inserirVendaModuloGestaoFiliais(moduloGestaoFiliais, novaVenda);
			inserirVendaModuloFacturacao(moduloFaturacao, novaVenda);
		}
		i++;
	}
	tfin = clock();

	fclose(ficheiroVendas);
	freeStringVenda(structStrVendas);
	free(novaVenda);
	free(linhaLida);
	printf("===VENDAS===\n");
	printf("Ficheiro: %s\n", nomeFicheiro);
	printf("Total Linhas Lidas: %d\n", i );
	printf("Total Linhas Válidas: %d\n", getTotalVendasAdicionadas(moduloGestaoFiliais) );
	printf("Leitura feita em %.3f segundos.\n ", (double) (tfin - tini)/CLOCKS_PER_SEC);
}

void abrirFicheiroClientes(String nomeFicheiro, CATALOGO_CLIENTES catalogo){
	clock_t tini, tfin;
	int nrlinhasLidas = 0;
	String linhaLida = (String) malloc(64);
	FILE *ficheiroClientes = fopen(nomeFicheiro, "r");

	tini = clock();
	while(fgets(linhaLida, MAX_BUFFER_CATALOGO, ficheiroClientes) && nrlinhasLidas < MAXLINHAS){
		linhaLida[strcspn( linhaLida, "\r\n")] = 0;
		if(TAMANHO_COD_CLIENTE != 5) inserirClienteCatalogo(catalogo, linhaLida);
		else if(verificaCodigoCliente(linhaLida)) inserirClienteCatalogo(catalogo, linhaLida);
		inserirClienteCatalogo(catalogo, linhaLida);
		nrlinhasLidas++;
	}
	free(linhaLida);
	fclose(ficheiroClientes);
	tfin = clock();

	printf("===CLIENTES===\n");
	printf("Ficheiro: %s\n", nomeFicheiro);
	printf("Total Linhas Lidas: %d\n", nrlinhasLidas );
	printf("Total Linhas Válidas: %d\n", calcularTotalClientes(catalogo));
	printf("Leitura feita em %.3f segundos.\n ", (double) (tfin - tini)/CLOCKS_PER_SEC);

}

void abrirFicheiroProdutos(String nomeFicheiro, CATALOGO_PRODUTOS catalogo){
	clock_t tini, tfin;
	int nrlinhasLidas = 0;
	String linhaLida = (String) malloc(64);
	FILE *ficheiroProdutos = fopen(nomeFicheiro, "r");

	tini = clock();
	while(fgets(linhaLida, MAX_BUFFER_CATALOGO, ficheiroProdutos) && nrlinhasLidas < MAXLINHAS){
		linhaLida[strcspn( linhaLida, "\r\n")] = 0;
		if(TAMANHO_COD_PRODUTO != 6) inserirProdutoCatalogo(catalogo, linhaLida);
		else if(verificaCodigoProduto(linhaLida)) inserirProdutoCatalogo(catalogo, linhaLida);
		nrlinhasLidas++;
	}
	free(linhaLida);
	fclose(ficheiroProdutos);
	tfin = clock();

	printf("===PRODUTOS===\n");
	printf("Ficheiro: %s\n", nomeFicheiro);
	printf("Total Linhas Lidas: %d\n", nrlinhasLidas );
	printf("Total Linhas Válidas: %d\n", calcularTotalProdutos(catalogo));
	printf("Leitura feita em %.3f segundos.\n ", (double) (tfin - tini)/CLOCKS_PER_SEC);
}

static int validaVenda(VENDA venda, CATALOGO_PRODUTOS catalogoProdutos, CATALOGO_CLIENTES catalogoClientes){
	int resultado = 1;
	String produto = getCodigoProduto(venda);
	String cliente = getCodigoCliente(venda);

	if(TAMANHO_COD_PRODUTO == 6 && TAMANHO_COD_CLIENTE == 5){ 
		resultado = verificaCodigoProduto(produto)
		&& verificaCodigoCliente(cliente);
	}
	resultado = resultado
	&& true == existeClienteCatalogo(catalogoClientes, cliente)
	&& true == existeProdutoCatalogo(catalogoProdutos, produto);
	return resultado;
}
