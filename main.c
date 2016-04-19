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

#define NRFILIAIS 3

#include "avl.h"
#include "venda.h"
#include "menu.h"
#include "navegacao.h"
#include "catalogo_clientes.h"
#include "faturacao.h"
#include "gestao_filiais.h"
#include "leitura_ficheiros.h"
#include "queries.h"

//nr clientes validos: 16 384 distintos
//nr produtos validos: 171 008 distintos
//nr vendas validos: 891 108

int main(int argc, char const *argv[]){
	clock_t tini, tfin;
	tini = clock();

	CATALOGO_CLIENTES catalogoClientes = catalogoClientesInit();
	abrirFicheiroClientes(catalogoClientes, "Clientes.txt");
	// query02_catalogoClientes(catalogoClientes);

	MODULO_GESTAO_FILIAIS moduloGestao = moduloGestaoFiliaisInit();
	MODULO_FATURACAO moduloFaturacao = moduloFaturacaoInit();
	abrirFicheiroVendas("Vendas_100K.txt", moduloGestao, moduloFaturacao);

	// query05_nrProdutosCompradosPeloCliente(moduloGestao, catalogoClientes);
	// query09_listaProdutosClienteMaisComprou(moduloGestao, catalogoClientes);
	// query11_topProdutosClienteGastouDinheiro(moduloGestao, catalogoClientes);

	freeModuloGestaoFiliais(moduloGestao);
	freeModuloFaturacao(moduloFaturacao);
	freeCatalogoClientes(catalogoClientes);

	tfin = clock();
	// printf("Leitura feita em %.3f segundos.\n ", (double) (tfin - tini)/CLOCKS_PER_SEC);
	return(EXIT_SUCCESS);
}

int vendasMain(){
	MODULO_GESTAO_FILIAIS moduloGestao = moduloGestaoFiliaisInit();
	MODULO_FATURACAO moduloFaturacao = moduloFaturacaoInit();

	abrirFicheiroVendas("Vendas_teste.txt", moduloGestao, moduloFaturacao);

	// travessiaFaturacao(moduloFaturacao);

	freeModuloGestaoFiliais(moduloGestao);
	freeModuloFaturacao(moduloFaturacao);

	return (EXIT_SUCCESS);
}
