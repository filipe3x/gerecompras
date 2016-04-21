/*
 *  *  * Filipe Marques
 *  *  * Laboratórios Informática III, Universidade do Minho, 2016
 *  *  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "avl.h"
#include "venda.h"
#include "navegacao.h"
#include "catalogo_produtos.h"

struct catalogoProdutos {
	int totalProdutos[27];
	Modulo indice[27];
};

static int calculaIndiceProduto(char l);
static void freeCodigoProduto(void *codigo, void *param);

/* GET'S e SET's */

Modulo getCatalogoProdutosPorLetra(CATALOGO_PRODUTOS catalogo, char l) {
	return catalogo->indice[calculaIndiceProduto(l)];
}

Modulo getCatalogoProdutosPorIndice(CATALOGO_PRODUTOS catalogo, int i) {
	return catalogo->indice[i];
}

int getTotalProdutos(CATALOGO_PRODUTOS catalogo, int ind){
	return catalogo->totalProdutos[ind];
}

int calcularTotalProdutos(CATALOGO_PRODUTOS catalogo){
	int i, somatorio = 0;
	for(i = 0; i <= 26; i++)
		somatorio += getTotalProdutos(catalogo, i);
	return somatorio;
}

static void setTotalProdutos(CATALOGO_PRODUTOS catalogo, int ind, int valor){
	catalogo->totalProdutos[ind] = valor;
}

static void incTotalProdutos(CATALOGO_PRODUTOS catalogo, int ind){
	catalogo->totalProdutos[ind]++;
}

static int comparaProdutoAlfabeticamente(const void *produto_a, const void *produto_b, void *param) {
	return strcmp((char *) produto_a, (char *) produto_b);
}

/* INICIALIZACAO E FREE */

CATALOGO_PRODUTOS catalogoProdutosInit(){
	CATALOGO_PRODUTOS catalogo = (CATALOGO_PRODUTOS) malloc(sizeof (struct catalogoProdutos)); //inicializar array de 27 apontadores para AVLs

	int i;
	for (i = 0; i <= 26; i++){ // inicializar 27 AVLs, uma para cada letra do alfabeto
		catalogo->indice[i] = avl_create(comparaProdutoAlfabeticamente, NULL, NULL); //a funcao que inicializa a avl precisa de saber qual a funcao de comparacao, respeitando assim o principio de encapsulamento
		setTotalProdutos(catalogo, i, 0);
	}

	return catalogo;
}

void freeCatalogoProdutos(CATALOGO_PRODUTOS catalogo){
	int i;
	
	if(catalogo != NULL){
		for (i = 0; i <= 26; i++) {
			avl_destroy(catalogo->indice[i], freeCodigoProduto);
		}
	}
	free(catalogo);
}

/* INSERCOES */

bool existeProdutoCatalogo(CATALOGO_PRODUTOS catalogo, CodigoProduto_st codigo) {
	bool resultado = false;
	char letra = codigo[0];
	int i;

	if (codigo != NULL) {
		i = calculaIndiceProduto(letra);

		if (avl_find(catalogo->indice[i], codigo) != NULL) 
			resultado = true;
		else
			resultado = false;
	}

	return resultado;
}

CodigoProduto_st inserirProdutoCatalogo(CATALOGO_PRODUTOS catalogo, CodigoProduto_st codigo){
	int i = calculaIndiceProduto(codigo[0]);

	CodigoProduto_st novo = cloneCodigo(codigo);
	CodigoProduto_st c =  (CodigoProduto_st) avl_insert(catalogo->indice[i], novo);

	if(c == NULL) incTotalProdutos(catalogo, i); //nao encontrou repetido - insersao com sucesso
	else freeCodigo(novo); //caso ja exista fazer free

	return c;
}

static void freeCodigoProduto(void *codigo, void *param) {
	free((CodigoProduto_st) codigo);
}

static int calculaIndiceProduto(char l) {
	char letra = toupper(l);
	int i = 0;

	if (isalpha(letra)) {
		i = letra - 'A';
	} else {
		i = 26;
	}
	return i;
}

/* TRAVESSIAS */

PAGINA_RESULTADOS travessiaProdutosPorLetra(CATALOGO_PRODUTOS catalogo, char letra){
	int n;
	CodigoProduto_st produto;
	PAGINA_RESULTADOS pagina;
	int i = calculaIndiceProduto(toupper(letra));
	int totalResultados = getTotalProdutos(catalogo,i);

	TravessiaModulo trav = avl_trav_alloc();
	avl_t_init(trav, getCatalogoProdutosPorLetra(catalogo,letra));

	produto = (CodigoProduto_st) avl_t_next(trav);
	pagina = (PAGINA_RESULTADOS) paginaResultadosInit(totalResultados,1);
	inserirResultadoLista(pagina, produto); /* a inserir enderecos dos codigos (sem malloc) */

	n = 0;
	while((produto = avl_t_next(trav)) && n < totalResultados){
		inserirResultadoLista(pagina, produto);
		n++;
	}

	freeTravessiaCatalogoProdutos(trav);

	return pagina;
}

PAGINA_RESULTADOS testarTravessiaProdutos(CATALOGO_PRODUTOS catalogo, int (*funcaoComparacao)(), void (*funcaoImpressao)() ){
	PAGINA_RESULTADOS pagina;

	char c;
	scanf("%c",&c);

	pagina = (PAGINA_RESULTADOS) travessiaProdutosPorLetra(catalogo, c);
	ordenarResultadosLista(pagina, funcaoComparacao);
	percorrerPaginaResultados(pagina, 1, ELEM_POR_PAG, funcaoImpressao);

	printf("Total produtos começados por %c: %d\n", toupper(c), getTotalProdutos(catalogo, calculaIndiceProduto(c)));

	return pagina;
}

void freeTravessiaCatalogoProdutos(TravessiaModulo travessia){
	if(travessia != NULL)
		avl_trav_free(travessia);
}

