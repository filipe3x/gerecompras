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
#include "faturacao.h"
#include "gestao_filiais.h"
#include "catalogo_clientes.h"
#include "catalogo_produtos.h"
#include "leitura_ficheiros.h"
#include "queries.h"
#include "menu.h"

int main(int argc, char const *argv[]){
	CATALOGO_CLIENTES catalogoClientes = catalogoClientesInit();
	CATALOGO_PRODUTOS catalogoProdutos = catalogoProdutosInit();
	MODULO_GESTAO_FILIAIS moduloGestao = moduloGestaoFiliaisInit();
	MODULO_FATURACAO moduloFaturacao = moduloFaturacaoInit();

	abrirFicheiroClientes("Clientes.txt", catalogoClientes);
	abrirFicheiroProdutos("Produtos.txt", catalogoProdutos);
	abrirFicheiroVendas("Vendas_1M.txt", moduloGestao, moduloFaturacao, catalogoProdutos, catalogoClientes);

	// query02_catalogoClientes(catalogoClientes);
	// query02_catalogoProdutos(catalogoClientes);
	// query05_nrProdutosCompradosPeloCliente(moduloGestao, catalogoClientes);
	// query09_listaProdutosClienteMaisComprou(moduloGestao, catalogoClientes);
	// query11_topProdutosClienteGastouDinheiro(moduloGestao, catalogoClientes);
	// query03_quantidadeFaturacaoProduto_filial(moduloGestao, catalogoProdutos);
	// query03_quantidadeFaturacaoProduto_global(moduloGestao, catalogoProdutos);

	menu_principal();

	printf("faturacao %f\n", calculaFaturacaoProduto_global(moduloGestao, "NR1091", VENDA_NORMAL) + 
		calculaFaturacaoProduto_global(moduloGestao, "NR1091", VENDA_PROMOCIONAL));

	freeModuloGestaoFiliais(moduloGestao);
	freeModuloFaturacao(moduloFaturacao);
	freeCatalogoClientes(catalogoClientes);
	freeCatalogoProdutos(catalogoProdutos);
	return(EXIT_SUCCESS);
}
