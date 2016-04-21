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
#include "catalogo_clientes.h"

struct catalogoClientes {
	int totalClientes[27];
	Modulo indice[27];
};

static int calculaIndiceCliente(char l);
static void freeCodigoCliente(void *codigo, void *param);

/* GET'S e SET's */

Modulo getCatalogoClientesPorLetra(CATALOGO_CLIENTES catalogo, char l) {
	return catalogo->indice[calculaIndiceCliente(l)];
}

Modulo getCatalogoClientesPorIndice(CATALOGO_CLIENTES catalogo, int i) {
	return catalogo->indice[i];
}

int getTotalClientes(CATALOGO_CLIENTES catalogo, int ind){
	return catalogo->totalClientes[ind];
}

int calcularTotalClientes(CATALOGO_CLIENTES catalogo){
	int i, somatorio = 0;
	for(i = 0; i <= 26; i++)
		somatorio += getTotalClientes(catalogo, i);
	return somatorio;
}

static void setTotalClientes(CATALOGO_CLIENTES catalogo, int ind, int valor){
	catalogo->totalClientes[ind] = valor;
}

static void incTotalClientes(CATALOGO_CLIENTES catalogo, int ind){
	catalogo->totalClientes[ind]++;
}

static int comparaClienteAlfabeticamente(const void *cliente_a, const void *cliente_b, void *param) {
	return strcmp((char *) cliente_a, (char *) cliente_b);
}

/* INICIALIZACAO E FREE */

CATALOGO_CLIENTES catalogoClientesInit(){
	CATALOGO_CLIENTES catalogo = (CATALOGO_CLIENTES) malloc(sizeof (struct catalogoClientes)); //inicializar array de 27 apontadores para AVLs

	int i;
	for (i = 0; i <= 26; i++){ // inicializar 27 AVLs, uma para cada letra do alfabeto
		catalogo->indice[i] = avl_create(comparaClienteAlfabeticamente, NULL, NULL); //a funcao que inicializa a avl precisa de saber qual a funcao de comparacao, respeitando assim o principio de encapsulamento
		setTotalClientes(catalogo, i, 0);
	}

	return catalogo;
}

void freeCatalogoClientes(CATALOGO_CLIENTES catalogo){
	int i;
	
	if(catalogo != NULL){
		for (i = 0; i <= 26; i++) {
			avl_destroy(catalogo->indice[i], freeCodigoCliente);
		}
	}
	free(catalogo);
}

static void freeCodigoCliente(void *codigo, void *param) {
	free((CodigoCliente_st) codigo);
}

/* INSERCOES */

bool existeClienteCatalogo(CATALOGO_CLIENTES catalogo, CodigoCliente_st codigo) {
	bool resultado = false;
	char letra = codigo[0];
	int i;

	if (codigo != NULL) {
		i = calculaIndiceCliente(letra);

		if (avl_find(catalogo->indice[i], codigo) != NULL) 
			resultado = true;
		else
			resultado = false;
	}

	return resultado;
}

CodigoCliente_st inserirClienteCatalogo(CATALOGO_CLIENTES catalogo, CodigoCliente_st codigo){
	int i = calculaIndiceCliente(codigo[0]);

	CodigoCliente_st novo = cloneCodigo(codigo);
	CodigoCliente_st c =  (CodigoCliente_st) avl_insert(catalogo->indice[i], novo);

	if(c == NULL) incTotalClientes(catalogo, i); //nao encontrou repetido - insersao com sucesso
	else freeCodigo(novo); //caso ja exista fazer free

	return c;
}

static int calculaIndiceCliente(char l) {
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

PAGINA_RESULTADOS travessiaClientesPorLetra(CATALOGO_CLIENTES catalogo, char letra){
	int n;
	CodigoCliente_st cliente;
	PAGINA_RESULTADOS pagina;
	int i = calculaIndiceCliente(toupper(letra));
	int totalResultados = getTotalClientes(catalogo,i);

	TravessiaModulo trav = avl_trav_alloc();
	avl_t_init(trav, getCatalogoClientesPorLetra(catalogo,letra));

	cliente = (CodigoCliente_st) avl_t_next(trav);
	pagina = (PAGINA_RESULTADOS) paginaResultadosInit(totalResultados,1);
	inserirResultadoLista(pagina, cliente); /* a inserir enderecos dos codigos (sem malloc) */

	n = 0;
	while((cliente = avl_t_next(trav)) && n < totalResultados){
		inserirResultadoLista(pagina, cliente);
		n++;
	}

	freeTravessiaCatalogoClientes(trav);

	return pagina;
}

PAGINA_RESULTADOS testarTravessiaClientes(CATALOGO_CLIENTES catalogo, int (*funcaoComparacao)(), void (*funcaoImpressao)() ){
	PAGINA_RESULTADOS pagina;

	char c;
	scanf("%c",&c);

	pagina = (PAGINA_RESULTADOS) travessiaClientesPorLetra(catalogo, c);
	ordenarResultadosLista(pagina, funcaoComparacao);
	percorrerPaginaResultados(pagina, 1, ELEM_POR_PAG, funcaoImpressao);

	printf("Total clientes começados por %c: %d\n", toupper(c), getTotalClientes(catalogo, calculaIndiceCliente(c)));

	return pagina;
}

void freeTravessiaCatalogoClientes(TravessiaModulo travessia){
	if(travessia != NULL)
		avl_trav_free(travessia);
}

