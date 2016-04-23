/*
 *  *  * Filipe Marques
 *  *  * Laboratórios Informática III, Universidade do Minho, 2016
 *  *  */
#include <stdio.h>

#include <stdlib.h>
#include "venda.h"
#include "navegacao.h"
#include "faturacao.h"
#include "catalogo_clientes.h"
#include "catalogo_produtos.h"
#include "gestao_filiais.h"
#include "leitura_ficheiros.h"
#include "queries.h"
#include "menu.h"

int main(int argc, char const *argv[]){
	CATALOGO_CLIENTES catalogoClientes = catalogoClientesInit();
	CATALOGO_PRODUTOS catalogoProdutos = catalogoProdutosInit();
	MODULO_GESTAO_FILIAIS moduloGestao = moduloGestaoFiliaisInit();
	MODULO_FATURACAO moduloFaturacao = moduloFaturacaoInit();

	argumentosMain(argc,argv);
	abrirFicheirosInit(catalogoClientes,catalogoProdutos,moduloGestao,moduloFaturacao);
	if(STATUS!=EXIT_FAILURE)menu_principal();

	freeModuloGestaoFiliais(moduloGestao);
	freeModuloFaturacao(moduloFaturacao);
	freeCatalogoClientes(catalogoClientes);
	freeCatalogoProdutos(catalogoProdutos);
	return(EXIT_SUCCESS);
}
