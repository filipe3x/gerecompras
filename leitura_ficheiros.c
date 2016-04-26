/*
 *  *  * Filipe Marques
 *  *  * Laboratórios Informática III, Universidade do Minho, 2016
 *  *  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "venda.h"
#include "navegacao.h"
#include "catalogo_clientes.h"
#include "catalogo_produtos.h"
#include "gestao_filiais.h"
#include "faturacao.h"
#include "menu.h"
#include "leitura_ficheiros.h"

CATALOGO_CLIENTES cat_Clientes;
CATALOGO_PRODUTOS cat_Produtos;
MODULO_GESTAO_FILIAIS mod_Gestao;
MODULO_FATURACAO mod_Faturacao;

String FICHEIRO_CLIENTES = "Clientes.txt";
String FICHEIRO_PRODUTOS = "Produtos.txt";
String FICHEIRO_VENDAS = "Vendas_1M.txt";

const char** argumentos;
int nrArg;
int STATUS;
float TEMPOS_LEITURA = 0;
long NRCLIENTES;
long NRPRODUTOS;
long NRVENDAS;

static int abrirFicheiros();
static void abrirFicheiroClientes(String nomeFicheiro);
static void abrirFicheiroProdutos(String nomeFicheiro);
static void abrirFicheiroVendas(String nomeFicheiro);
static int verificaFicheiros(String ficheiro1, String ficheiro2, String ficheiro3);
static int validaVenda(VENDA venda, CATALOGO_PRODUTOS catalogoProdutos, CATALOGO_CLIENTES catalogoClientes);

int abrirFicheirosInit(CATALOGO_CLIENTES catalogoClientes, CATALOGO_PRODUTOS catalogoProdutos, 
	MODULO_GESTAO_FILIAIS moduloGestaoFiliais, MODULO_FATURACAO moduloFaturacao){

	cat_Clientes = catalogoClientes;
	cat_Produtos = catalogoProdutos;
	mod_Gestao = moduloGestaoFiliais;
	mod_Faturacao = moduloFaturacao;

	return STATUS = abrirFicheiros();
}

static int abrirFicheiros(){
	String ficheiroClientes;
	String ficheiroProdutos;
	String ficheiroVendas;

	if(nrArg == 1){
		ficheiroClientes = FICHEIRO_CLIENTES;
		ficheiroProdutos = FICHEIRO_PRODUTOS;
		ficheiroVendas = FICHEIRO_VENDAS;
	}else{
		if(nrArg != 4){
			ajuda();
			return(EXIT_FAILURE);
		}else{
			ficheiroClientes = (String) argumentos[1];
			ficheiroProdutos = (String) argumentos[2];
			ficheiroVendas = (String) argumentos[3];
		}
	}

	if(verificaFicheiros(ficheiroClientes, ficheiroProdutos, ficheiroVendas)){
		abrirFicheiroClientes(ficheiroClientes);
		abrirFicheiroProdutos(ficheiroProdutos);
		abrirFicheiroVendas(ficheiroVendas);
	}else{
		if(nrArg == 1)
			ajuda();
		else
			fprintf(stderr, "\tFicheiro(s) não existe(m) ou não existem permissões suficientes.\n");
		return(EXIT_FAILURE);
	}

	return(EXIT_SUCCESS);
}

static int verificaFicheiros(String ficheiro1, String ficheiro2, String ficheiro3){
	/*fopen won't work on "r" mode if file doesn't exist*/
	int resultado = 1;
	FILE *fp1 = fopen(ficheiro1,"r");
	FILE *fp2 = fopen(ficheiro2,"r");
	FILE *fp3 = fopen(ficheiro3,"r");

	resultado = fp1 && fp2 && fp3;

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);

	return resultado;
}

void argumentosMain(int argc, const char** argv){
	nrArg = argc;
	argumentos = argv;
}

static void abrirFicheiroVendas(String nomeFicheiro){

	CATALOGO_CLIENTES catalogoClientes = cat_Clientes;
	CATALOGO_PRODUTOS catalogoProdutos = cat_Produtos;
	MODULO_GESTAO_FILIAIS moduloGestaoFiliais = mod_Gestao;
	MODULO_FATURACAO moduloFaturacao = mod_Faturacao;

	int i = 0;
	clock_t tini, tfin;
	const char *modoAbertura = "r";
	FILE *ficheiroVendas = fopen(nomeFicheiro, modoAbertura);

	String linhaLida = (String) malloc(128);
	VENDA novaVenda  = linhaVendaInit();
	StringVenda structStrVendas = stringVendaInit();

	printf("===VENDAS===\n");
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
	TEMPOS_LEITURA += (double) (tfin - tini)/CLOCKS_PER_SEC;
	printf("Ficheiro: %s\n", nomeFicheiro);
	printf("Total Linhas Lidas: %d\n", i );
	printf("Total Linhas Válidas: %ld\n", NRVENDAS = getTotalVendasAdicionadas(moduloGestaoFiliais) );
	printf("Leitura feita em %.3f segundos.\n ", (double) (tfin - tini)/CLOCKS_PER_SEC);
}

static void abrirFicheiroClientes(String nomeFicheiro){
	CATALOGO_CLIENTES catalogo = cat_Clientes;

	clock_t tini, tfin;
	int nrlinhasLidas = 0;
	String linhaLida = (String) malloc(64);
	FILE *ficheiroClientes = fopen(nomeFicheiro, "r");

	printf("===CLIENTES===\n");
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
	TEMPOS_LEITURA += (double) (tfin - tini)/CLOCKS_PER_SEC;
	printf("Ficheiro: %s\n", nomeFicheiro);
	printf("Total Linhas Lidas: %d\n", nrlinhasLidas );
	printf("Total Linhas Válidas: %ld\n", NRCLIENTES = calcularTotalClientes(catalogo));
	printf("Leitura feita em %.3f segundos.\n ", (double) (tfin - tini)/CLOCKS_PER_SEC);

}

static void abrirFicheiroProdutos(String nomeFicheiro){
	CATALOGO_PRODUTOS catalogo = cat_Produtos;

	clock_t tini, tfin;
	int nrlinhasLidas = 0;
	String linhaLida = (String) malloc(64);
	FILE *ficheiroProdutos = fopen(nomeFicheiro, "r");

	printf("===PRODUTOS===\n");
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
	TEMPOS_LEITURA += (double) (tfin - tini)/CLOCKS_PER_SEC;
	printf("Ficheiro: %s\n", nomeFicheiro);
	printf("Total Linhas Lidas: %d\n", nrlinhasLidas );
	printf("Total Linhas Válidas: %ld\n", NRPRODUTOS = calcularTotalProdutos(catalogo));
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
