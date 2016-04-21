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

	// query02_catalogoClientes(catalogoClientes);
	// query02_catalogoProdutos(catalogoClientes);
	// query05_nrProdutosCompradosPeloCliente(moduloGestao, catalogoClientes);
	// query09_listaProdutosClienteMaisComprou(moduloGestao, catalogoClientes);
	// query11_topProdutosClienteGastouDinheiro(moduloGestao, catalogoClientes);
	// query03_quantidadeFaturacaoProduto_filial(moduloGestao, catalogoProdutos);
	// query03_quantidadeFaturacaoProduto_global(moduloGestao, catalogoProdutos);

	if(STATUS!=EXIT_FAILURE)menu_principal();

/*	printf("faturacao %f\n", calculaFaturacaoProduto_global(moduloGestao, "NR1091", VENDA_NORMAL) + 
		calculaFaturacaoProduto_global(moduloGestao, "NR1091", VENDA_PROMOCIONAL));
	*/
	// PAGINA_RESULTADOS pagina = listaClientesCompraramProduto(moduloGestao, "AF1184", 1);

	// PAGINA_RESULTADOS pagina = produtosNinguemComprou_global( moduloGestao,  catalogoProdutos);
	// printf("nr produtos ninguem comprou: %d\n", getIndiceAtual(pagina));

	// PAGINA_RESULTADOS pagina = clientesNaoCompraram_global( moduloGestao,  catalogoClientes);
	// PAGINA_RESULTADOS pagina = clientesCompraramTodasFiliais(moduloGestao);

	// printf("%s\n", (char*) getnElemento(pagina, 0));
	// printf("%s\n", (char*) getnElemento(pagina, 1));
	// printf("%s\n", (char*) getnElemento(pagina, 2));
	// printf("%s\n", (char*) getnElemento(pagina, 3));
	// printf("%s\n", (char*) getnElemento(pagina, 4));
	// printf("%s\n", (char*) getnElemento(pagina, getIndiceAtual(pagina)-1) );

	// PAGINA_RESULTADOS pagina = produtosMaisVendidos_global( moduloGestao, calcularTotalProdutos(catalogoProdutos));
	// printf("%s %d\n", (char*) getCodProduto_gestaoProduto(getnElemento(pagina, 0)), getNrClientesTotal_gestaoProduto(getnElemento(pagina, 0)) );
	// printf("%s %d\n", (char*) getCodProduto_gestaoProduto(getnElemento(pagina, 1)), getNrClientesTotal_gestaoProduto(getnElemento(pagina, 1)) );
	// printf("%s %d\n", (char*) getCodProduto_gestaoProduto(getnElemento(pagina, 2)), getNrClientesTotal_gestaoProduto(getnElemento(pagina, 2)) );
	// printf("%s %d\n", (char*) getCodProduto_gestaoProduto(getnElemento(pagina, 3)), getNrClientesTotal_gestaoProduto(getnElemento(pagina, 3)) );
	// printf("%s %d\n", (char*) getCodProduto_gestaoProduto(getnElemento(pagina, getIndiceAtual(pagina)-1)), getNrClientesTotal_gestaoProduto(getnElemento(pagina, getIndiceAtual(pagina)-1)) );

	// printf("%f\n",consultaFaturacaoIntervaloMeses(moduloFaturacao,0,11));
	// printf("%ld\n",(long) consultaQuantidadesIntervaloMeses(moduloFaturacao,0,11));

	freeModuloGestaoFiliais(moduloGestao);
	freeModuloFaturacao(moduloFaturacao);
	freeCatalogoClientes(catalogoClientes);
	freeCatalogoProdutos(catalogoProdutos);
	return(EXIT_SUCCESS);
}
