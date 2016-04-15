#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "avl.h"
#include "venda.h"
#include "navegacao.h"
#include "catalogo_clientes.h"

// FALTA FAZER FUNCAO DE ORDENACAO

struct paginaResultados{
    Listagem resultados;
    int tamanhoLista;
    int tamanhoCadaElemento; //EM BYTES
    int nrCampos;
    int indiceAtual; //so para insercoes
    int tempos;

    int* tamanhoCampos;// meter aqui um array com o tamanho de cada um dos campos
    
    int nrPaginaAtual;
    int nrPaginasTotal;
    int nrElementosPorPagina;
    int posicaoInicial;
    int posicaoFinal;
};

int getTamanhoLista(PAGINA_RESULTADOS pagina){
	return pagina->tamanhoLista;
}

int getTamanhoCadaElemento(PAGINA_RESULTADOS pagina){
	return pagina->tamanhoCadaElemento;
}

void* getnElemento(PAGINA_RESULTADOS pagina, int n){
	return (void *) (pagina->resultados[n]); //da me o apontador da estrutura no indice n
}

int getIndiceAtual(PAGINA_RESULTADOS pagina){
	return pagina->indiceAtual;
}

int getPaginaAtual(PAGINA_RESULTADOS pagina){
	return pagina->nrPaginaAtual;
}

int getPosicaoInicial(PAGINA_RESULTADOS pagina){
	return pagina->posicaoInicial;
}

int getPosicaoFinal(PAGINA_RESULTADOS pagina){
	return pagina->posicaoFinal;
}

int getNrElementosPorPagina(PAGINA_RESULTADOS pagina){
	return pagina->nrElementosPorPagina;
}

void setNrElementosPorPagina(PAGINA_RESULTADOS pagina, int n){
	int max = getIndiceAtual(pagina);

	if(n > max) pagina->nrElementosPorPagina = max;
	else pagina->nrElementosPorPagina = n;
}

void setPosicaoInicial(PAGINA_RESULTADOS pagina, int pos){
	pagina->posicaoInicial = pos;
}

void setPosicaoFinal(PAGINA_RESULTADOS pagina, int pos){
	pagina->posicaoFinal = pos;
}

void setPaginaAtual(PAGINA_RESULTADOS pagina, int pag){
	pagina->nrPaginaAtual = pag;
}

int incIndiceAtual(PAGINA_RESULTADOS pagina){
	return pagina->indiceAtual++;
}

int incPaginaAtual(PAGINA_RESULTADOS pagina){
	return pagina->nrPaginaAtual++;
}

int calcularIndiceInsercao(PAGINA_RESULTADOS pagina){
	return getIndiceAtual(pagina) * getTamanhoCadaElemento(pagina);
}

PAGINA_RESULTADOS paginaResultadosInit(int tamanhoL, int tamanhoElemento){
	PAGINA_RESULTADOS pagina = (PAGINA_RESULTADOS) malloc(sizeof(struct paginaResultados));
	pagina->resultados = (Listagem) malloc(tamanhoL*sizeof( void* )); //array de apontadores para estruturas

	pagina->tamanhoLista = tamanhoL;
	pagina->tamanhoCadaElemento = tamanhoElemento;

	pagina->indiceAtual = 0;	
	pagina->nrPaginaAtual = 0;
	pagina->nrElementosPorPagina = 0;
	pagina->posicaoInicial = 0;
	pagina->posicaoFinal = 0;

	return pagina;
}

void estadoPaginacao(PAGINA_RESULTADOS pagina){
    int tamanhoLista = getTamanhoLista(pagina);
    int tamanhoCadaElemento = getTamanhoCadaElemento(pagina);
    int indiceAtual = getIndiceAtual(pagina);

    int nrPaginaAtual = getPaginaAtual(pagina);
    int nrElementosPorPagina = getNrElementosPorPagina(pagina);
    int posicaoInicial = getPosicaoInicial(pagina);
    int posicaoFinal = getPosicaoFinal(pagina);

	printf("- tamanhoLista %d - tamanhoCadaElemento %d - indiceAtual %d -\n", tamanhoLista, tamanhoCadaElemento, indiceAtual);
	printf("nrPaginaAtual %d\n", nrPaginaAtual);
	printf("nrElementosPorPagina %d\n", nrElementosPorPagina);
	printf("posicaoInicial %d\n", posicaoInicial);
	printf("posicaoFinal %d\n", posicaoFinal);
}

void* inserirResultadoLista(PAGINA_RESULTADOS pagina, void* item){ //FAZER DEEP COPY - E FEITO FORA DESTA FUNCAO
	pagina->resultados[getIndiceAtual(pagina)] = item;
	incIndiceAtual(pagina);
	return item;
}

bool testeUltimaPagina(PAGINA_RESULTADOS pagina){
	return getPosicaoFinal(pagina) == getIndiceAtual(pagina);
}

int calcularNrPaginasInteiras(int nrElementosTotal, int nrElementosPorPagina){
	return nrElementosTotal / nrElementosPorPagina;
}

int calcularNrElementosUltimaPagina(int nrElementosTotal, int nrElementosPorPagina){
	return nrElementosTotal % nrElementosPorPagina;
}

void calcularUltimaPag(PAGINA_RESULTADOS pagina){
	int nrPagina = calcularNrPaginasInteiras(getIndiceAtual(pagina), getNrElementosPorPagina(pagina));
	int nrElementosUltimaPag = calcularNrElementosUltimaPagina(getIndiceAtual(pagina), getNrElementosPorPagina(pagina));

	if(nrElementosUltimaPag == 0) //?

	setPaginaAtual(pagina, nrPagina);
	setPosicaoInicial(pagina, nrPagina * getNrElementosPorPagina(pagina));
	setPosicaoFinal(pagina, getPosicaoInicial(pagina) +  nrElementosUltimaPag - 1);
}

int calcularNrPaginasTotal(PAGINA_RESULTADOS pagina){
	int nrPaginasInteiras = calcularNrPaginasInteiras(getIndiceAtual(pagina), getNrElementosPorPagina(pagina));
	int nrElementosUltimaPag = calcularNrElementosUltimaPagina(getIndiceAtual(pagina), getNrElementosPorPagina(pagina));
	return nrElementosUltimaPag > 0 ? nrPaginasInteiras + 1 : nrPaginasInteiras;
}

int virarPagina(PAGINA_RESULTADOS pagina){
	int max = getIndiceAtual(pagina);
	int nrElemPorPag = getNrElementosPorPagina(pagina);
	// int posicaoInic = getPosicaoInicial(pagina);
	int posicaoFin = getPosicaoFinal(pagina);

	if (posicaoFin + nrElemPorPag >= max){ //chegamos ao fim
		int nrElementosUltimaPag = calcularNrElementosUltimaPagina(getIndiceAtual(pagina), getNrElementosPorPagina(pagina));
		if(nrElementosUltimaPag > 0) calcularUltimaPag(pagina);
		return 0;
	}else{
		incPaginaAtual(pagina);
		setPosicaoInicial(pagina, posicaoFin + 1);
		setPosicaoFinal(pagina, posicaoFin + nrElemPorPag);
		return getPosicaoInicial(pagina);
	}
}

void posicoesInit(PAGINA_RESULTADOS pagina){
	int nrpag = getPaginaAtual(pagina);
	int elemPorPag = getNrElementosPorPagina(pagina);

	setPosicaoInicial(pagina, nrpag * elemPorPag);
	setPosicaoFinal(pagina, getPosicaoInicial(pagina) + elemPorPag - 1);
}


void imprimirElementos(PAGINA_RESULTADOS pagina, void (*funcaoImpressao)() ){
	int i = getPosicaoInicial(pagina);
	int fim = getPosicaoFinal(pagina);

	while( i  <= fim ){
		funcaoImpressao( getnElemento(pagina, i) ); //mandei o endereco da estrutura para fora
		i++;
	}
}


void percorrerPaginaResultados(PAGINA_RESULTADOS pagina, int nrPag, int elemPorPag, void (*funcaoImpressao)() ){
	setNrElementosPorPagina(pagina, elemPorPag);
	posicoesInit(pagina);

	while(scanf("%d",&nrPag) != 0){
		printf("\tTOTAL PAG: %d\n", calcularNrPaginasTotal(pagina));
		estadoPaginacao(pagina);
		imprimirElementos(pagina, funcaoImpressao);
		virarPagina(pagina);
	}
}
