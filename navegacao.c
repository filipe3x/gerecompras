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

struct paginaResultados{
	Listagem resultados;
	int tamanhoLista;
	int tamanhoCadaElemento; //EM BYTES
	int nrCampos;
	int indiceAtual; //so para insercoes
	
	int nrPaginaAtual;
	int nrPaginasTotal;
	int nrElementosPorPagina;
	int posicaoInicial;
	int posicaoFinal;
	int posicaoAtual;
};

int getTamanhoLista(PAGINA_RESULTADOS pagina){
	return pagina->tamanhoLista;
}

int getNrCampos(PAGINA_RESULTADOS pagina){
	return pagina->nrCampos;
}

int getTamanhoCadaElemento(PAGINA_RESULTADOS pagina){
	return pagina->tamanhoCadaElemento;
}

void** getListagem(PAGINA_RESULTADOS pagina){
	return pagina->resultados;
}

void* getnElemento(PAGINA_RESULTADOS pagina, int n){
	return (void *) (pagina->resultados[n]); //da me o apontador da estrutura no indice n
}

void** getnElementos(PAGINA_RESULTADOS pagina, int n){
	return (void **) ( ( pagina->resultados ) + n); //da me o endereco de Listagem no indice n
}

int getIndiceAtual(PAGINA_RESULTADOS pagina){
	return pagina->indiceAtual;
}

int getPaginaAtual(PAGINA_RESULTADOS pagina){
	return pagina->nrPaginaAtual;
}

int getNrPaginasTotal(PAGINA_RESULTADOS pagina){
	return pagina->nrPaginasTotal;
}

int getPosicaoInicial(PAGINA_RESULTADOS pagina){
	return pagina->posicaoInicial;
}

int getPosicaoFinal(PAGINA_RESULTADOS pagina){
	return pagina->posicaoFinal;
}

int getPosicaoAtual(PAGINA_RESULTADOS pagina){
	return pagina->posicaoAtual;
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

void setPosicaoAtual(PAGINA_RESULTADOS pagina, int pos){
	pagina->posicaoAtual = pos;
}

void setPaginaAtual(PAGINA_RESULTADOS pagina, int pag){
	pagina->nrPaginaAtual = pag;
}

void setNrPaginasTotal(PAGINA_RESULTADOS pagina, int pag){
	pagina->nrPaginasTotal = pag;
}

void setNrCampos(PAGINA_RESULTADOS pagina, int nrCampos){
	pagina->nrCampos = nrCampos;
}

int incIndiceAtual(PAGINA_RESULTADOS pagina){
	return pagina->indiceAtual++;
}

int incPosicaoAtual(PAGINA_RESULTADOS pagina){
	return pagina->posicaoAtual++;
}

int incPosicaoAtualNvezes(PAGINA_RESULTADOS pagina, int nrVezes){
	while(nrVezes > 0){
		pagina->posicaoAtual++;
		nrVezes--;
	}
	return getPosicaoAtual(pagina);
}

int incPaginaAtual(PAGINA_RESULTADOS pagina){
	return pagina->nrPaginaAtual++;
}

int decPaginaAtual(PAGINA_RESULTADOS pagina){
	return pagina->nrPaginaAtual--;
}

int calcularIndiceInsercao(PAGINA_RESULTADOS pagina){
	return getIndiceAtual(pagina) * getTamanhoCadaElemento(pagina);
}

PAGINA_RESULTADOS paginaResultadosInit(int tamanhoL, int nrCampos){
	PAGINA_RESULTADOS pagina = (PAGINA_RESULTADOS) malloc(sizeof(struct paginaResultados));
	pagina->resultados = (Listagem) malloc(tamanhoL*nrCampos*sizeof( void* )); //array de apontadores para estruturas

	pagina->tamanhoLista = tamanhoL;
	pagina->nrCampos = nrCampos;

	pagina->indiceAtual = 0;	
	pagina->nrPaginaAtual = 0;
	pagina->nrElementosPorPagina = 0;
	pagina->posicaoInicial = 0;
	pagina->posicaoFinal = 0;
	pagina->posicaoAtual = 0;

	return pagina;
}

void freePaginaResultados(PAGINA_RESULTADOS pagina, void (*funcaofreeItem)() ){
	int i, tamanho = getTamanhoLista(pagina);
	for(i = 0; i < tamanho; i++){
		funcaofreeItem(getnElemento(pagina,i));
	}

	free(getListagem(pagina));
	free(pagina);
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

int calcularNrPaginasInteiras(int nrCamposTotaldeTodosOsElementos, int nrCamposCadaElemento, int nrElementosPorPagina){
	return nrCamposTotaldeTodosOsElementos / (nrElementosPorPagina * nrCamposCadaElemento);
}

int calcularNrElementosUltimaPagina(int nrCamposTotaldeTodosOsElementos, int nrCamposCadaElemento, int nrElementosPorPagina){
	return nrCamposTotaldeTodosOsElementos % (nrElementosPorPagina * nrCamposCadaElemento);
}

void calcularUltimaPag(PAGINA_RESULTADOS pagina){
	int nrPagina = calcularNrPaginasInteiras(getIndiceAtual(pagina), getNrCampos(pagina), getNrElementosPorPagina(pagina));
	int nrElementosUltimaPag = calcularNrElementosUltimaPagina(getIndiceAtual(pagina), getNrCampos(pagina), getNrElementosPorPagina(pagina));

	if(nrElementosUltimaPag == 0){ 
		nrPagina--;
		setPaginaAtual(pagina, nrPagina);
		setPosicaoInicial(pagina, nrPagina * getNrElementosPorPagina(pagina) * getNrCampos(pagina));
		setPosicaoFinal(pagina, getPosicaoInicial(pagina) +  getNrElementosPorPagina(pagina) * getNrCampos(pagina) - 1);
		setPosicaoAtual(pagina, nrPagina * getNrElementosPorPagina(pagina) * getNrCampos(pagina));
	}else{
		setPaginaAtual(pagina, nrPagina);
		setPosicaoInicial(pagina, nrPagina * getNrElementosPorPagina(pagina) * getNrCampos(pagina));
		setPosicaoFinal(pagina, getPosicaoInicial(pagina) +  nrElementosUltimaPag * getNrCampos(pagina) - 1);
		setPosicaoAtual(pagina, nrPagina * getNrElementosPorPagina(pagina) * getNrCampos(pagina));
	}
}

void calcularPrimeiraPag(PAGINA_RESULTADOS pagina){
	setPaginaAtual(pagina, 0);
	setPosicaoInicial(pagina, 0);
	setPosicaoFinal(pagina, getNrElementosPorPagina(pagina)-1);
	setPosicaoAtual(pagina, 0);
}

int calcularNrPaginasTotal(PAGINA_RESULTADOS pagina){
	int nrPaginasInteiras = calcularNrPaginasInteiras(getIndiceAtual(pagina), getNrCampos(pagina), getNrElementosPorPagina(pagina));
	int nrElementosUltimaPag = calcularNrElementosUltimaPagina(getIndiceAtual(pagina), getNrCampos(pagina), getNrElementosPorPagina(pagina));
	return nrElementosUltimaPag > 0 ? nrPaginasInteiras + 1 : nrPaginasInteiras;
}

int virarPagina(PAGINA_RESULTADOS pagina){
	int max = getIndiceAtual(pagina);
	int nrElemPorPag = getNrElementosPorPagina(pagina);
	int nrCampos = getNrCampos(pagina);
	int posicaoInic = getPosicaoInicial(pagina);
	int posicaoFin = getPosicaoFinal(pagina);

	if(getPaginaAtual(pagina) >= getNrPaginasTotal(pagina)-1){
		setPosicaoAtual(pagina, posicaoInic);
		return 0;
	}

	if (posicaoFin + nrElemPorPag * nrCampos >= max){ //chegamos ao fim
		int nrElementosUltimaPag = calcularNrElementosUltimaPagina(getIndiceAtual(pagina), nrCampos, getNrElementosPorPagina(pagina));
		if(nrElementosUltimaPag > 0) calcularUltimaPag(pagina);
		return 0;
	}else{
		incPaginaAtual(pagina);
		setPosicaoInicial(pagina, posicaoFin + 1 * nrCampos);
		setPosicaoFinal(pagina, posicaoFin + nrElemPorPag * nrCampos);
		setPosicaoAtual(pagina, posicaoFin + 1 * nrCampos);
		return getPosicaoInicial(pagina);
	}
}

int paginaParaTras(PAGINA_RESULTADOS pagina){
	int nrElemPorPag = getNrElementosPorPagina(pagina);
	int nrCampos = getNrCampos(pagina);
	int posicaoInic = getPosicaoInicial(pagina);
	if(getPaginaAtual(pagina) == 0){
			setPosicaoAtual(pagina, 0);
			return 0; //ja estamos na primeira pagina
	}else{
		decPaginaAtual(pagina);
		setPosicaoInicial(pagina, posicaoInic - nrElemPorPag * nrCampos);
		setPosicaoFinal(pagina, posicaoInic - 1 * nrCampos);
		setPosicaoAtual(pagina, posicaoInic - nrElemPorPag * nrCampos);
		return getPosicaoInicial(pagina);
	}
}

void posicoesInit(PAGINA_RESULTADOS pagina){
	int nrpag = getPaginaAtual(pagina);
	int elemPorPag = getNrElementosPorPagina(pagina);
	int nrCampos = getNrCampos(pagina);

	setPosicaoInicial(pagina, nrpag * elemPorPag * nrCampos);
	setPosicaoFinal(pagina, getPosicaoInicial(pagina) + elemPorPag * nrCampos - 1 * nrCampos);
	setPosicaoAtual(pagina, nrpag * elemPorPag * nrCampos);

	setNrPaginasTotal(pagina, calcularNrPaginasTotal(pagina));
}

void imprimirPagina(PAGINA_RESULTADOS pagina, void (*funcaoImpressao)() ){
	int i = getPosicaoInicial(pagina);
	int fim = getPosicaoFinal(pagina);

	while( i  <= fim ){
		funcaoImpressao( getnElemento(pagina, i) ); //mandei o endereco da estrutura para fora
		i++;
	}
}

void* getElementoAtual(PAGINA_RESULTADOS pagina){
	int p = getPosicaoAtual(pagina);
	int fim = getPosicaoFinal(pagina);
	int nrCampos = getNrCampos(pagina);
	// printf("getPosicaoAtual %d - getPosicaoFinal %d - getNrCampos %d\n", p,fim,nrCampos);

	if( p  <= fim ){
		void* elemento = getnElemento(pagina, p);
		// printf("%ld %ld %ld %ld\n", *((long*) elemento) , elemento , ((long*) elemento)[1] , (long*)elemento+1 );
		incPosicaoAtualNvezes(pagina,nrCampos);
		return elemento;
	}else{
		return NULL;
	}
}

void* getMultiplosElementos(PAGINA_RESULTADOS pagina){
	int p = getPosicaoAtual(pagina);
	int fim = getPosicaoFinal(pagina);
	int nrCampos = getNrCampos(pagina);
	// printf("getPosicaoAtual %d - getPosicaoFinal %d - getNrCampos %d\n", p,fim,nrCampos);

	if( p <= fim ){
		void** elementos = getnElementos(pagina, p);
		// printf("%ld %ld %ld %ld\n", *((long*) elemento) , elemento , ((long*) elemento)[1] , (long*)elemento+1 );
		incPosicaoAtualNvezes(pagina,nrCampos);
		return elementos;
	}else{
		return NULL;
	}
}

void percorrerPaginaResultados(PAGINA_RESULTADOS pagina, int nrPag, int elemPorPag, void (*funcaoImpressao)() ){
	setNrElementosPorPagina(pagina, elemPorPag);
	posicoesInit(pagina);

	while(scanf("%d",&nrPag) != 0){
		printf("\tTOTAL PAG: %d\n", calcularNrPaginasTotal(pagina));
		estadoPaginacao(pagina);
		imprimirPagina(pagina, funcaoImpressao);
		virarPagina(pagina);
	}
}

// n = tamanho
static void quicksort(void **resultados, int (*f_comparacao)() , int n, void *param){
	int i, j;
	void *p, *temp;
	
	if (n < 2) return;
	p = resultados[n/2];
	
	for (i = 0, j = n - 1;; i++, j--) {
		
		while(f_comparacao(resultados[i],p, param) < 0) i++;
		while(f_comparacao(p,resultados[j], param) < 0) j--;
		if (i >= j) break;
		
		temp = resultados[i];
		resultados[i] = resultados[j];
		resultados[j] = temp;
	}
	quicksort(resultados,f_comparacao, i, param);
	quicksort(resultados+i,f_comparacao, n - i, param);
}

void ordenarResultadosLista(PAGINA_RESULTADOS pagina, int (*funcaoComparacao)() ){
	quicksort(getListagem(pagina), funcaoComparacao, getIndiceAtual(pagina), NULL);
}

PAGINA_RESULTADOS transporResultados(PAGINA_RESULTADOS pagina) {
	int n;
	int N = getTamanhoLista(pagina) / getNrCampos(pagina); //3
	int M = getNrCampos(pagina); //12
	Listagem dst = (Listagem) malloc( getTamanhoLista(pagina) * sizeof( void* ) );
	Listagem src = pagina->resultados;

	for(n = 0; n<N*M; n++) {
		int i = n/N;
		int j = n%N;
		dst[n] = src[M*j + i];
	}

	free(pagina->resultados);
	pagina->resultados = dst;
	setNrCampos(pagina, N);
	return pagina;
}

void swapResultados(PAGINA_RESULTADOS pagina, Listagem novo){
	pagina->resultados = novo;
}
