#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "avl.h"
#include "venda.h"
#include "navegacao.h"
#include "catalogo_clientes.h"

struct paginaResultados{
    Listagem resultados;
    int tamanhoLista;
    int tamanhoCadaElemento; //EM BYTES - APAGAR ISTO
    int nrCampos; // APAGAR ISTO
    int indiceAtual; //so para insercoes
    int tempos; //APAGAR ISTO
    
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

int getTamanhoCadaElemento(PAGINA_RESULTADOS pagina){
	return pagina->tamanhoCadaElemento;
}

void** getListagem(PAGINA_RESULTADOS pagina){
	return pagina->resultados;
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

int incIndiceAtual(PAGINA_RESULTADOS pagina){
	return pagina->indiceAtual++;
}

int incPosicaoAtual(PAGINA_RESULTADOS pagina){
	return pagina->posicaoAtual++;
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

int calcularNrPaginasInteiras(int nrElementosTotal, int nrElementosPorPagina){
	return nrElementosTotal / nrElementosPorPagina;
}

int calcularNrElementosUltimaPagina(int nrElementosTotal, int nrElementosPorPagina){
	return nrElementosTotal % nrElementosPorPagina;
}

void calcularUltimaPag(PAGINA_RESULTADOS pagina){
	int nrPagina = calcularNrPaginasInteiras(getIndiceAtual(pagina), getNrElementosPorPagina(pagina));
	int nrElementosUltimaPag = calcularNrElementosUltimaPagina(getIndiceAtual(pagina), getNrElementosPorPagina(pagina));

	if(nrElementosUltimaPag == 0){ 
		nrPagina--;
		setPaginaAtual(pagina, nrPagina);
		setPosicaoInicial(pagina, nrPagina * getNrElementosPorPagina(pagina));
		setPosicaoFinal(pagina, getPosicaoInicial(pagina) +  getNrElementosPorPagina(pagina) - 1);
		setPosicaoAtual(pagina, nrPagina * getNrElementosPorPagina(pagina));
	}else{
		setPaginaAtual(pagina, nrPagina);
		setPosicaoInicial(pagina, nrPagina * getNrElementosPorPagina(pagina));
		setPosicaoFinal(pagina, getPosicaoInicial(pagina) +  nrElementosUltimaPag - 1);
		setPosicaoAtual(pagina, nrPagina * getNrElementosPorPagina(pagina));
	}
}

void calcularPrimeiraPag(PAGINA_RESULTADOS pagina){
	setPaginaAtual(pagina, 0);
	setPosicaoInicial(pagina, 0);
	setPosicaoFinal(pagina, getNrElementosPorPagina(pagina)-1);
	setPosicaoAtual(pagina, 0);
}

int calcularNrPaginasTotal(PAGINA_RESULTADOS pagina){
	int nrPaginasInteiras = calcularNrPaginasInteiras(getIndiceAtual(pagina), getNrElementosPorPagina(pagina));
	int nrElementosUltimaPag = calcularNrElementosUltimaPagina(getIndiceAtual(pagina), getNrElementosPorPagina(pagina));
	return nrElementosUltimaPag > 0 ? nrPaginasInteiras + 1 : nrPaginasInteiras;
}

int virarPagina(PAGINA_RESULTADOS pagina){
	int max = getIndiceAtual(pagina);
	int nrElemPorPag = getNrElementosPorPagina(pagina);
	int posicaoInic = getPosicaoInicial(pagina);
	int posicaoFin = getPosicaoFinal(pagina);

	if(getPaginaAtual(pagina) >= getNrPaginasTotal(pagina)-1){
		setPosicaoAtual(pagina, posicaoInic);
		return 0;
	}

	if (posicaoFin + nrElemPorPag >= max){ //chegamos ao fim
		int nrElementosUltimaPag = calcularNrElementosUltimaPagina(getIndiceAtual(pagina), getNrElementosPorPagina(pagina));
		if(nrElementosUltimaPag > 0) calcularUltimaPag(pagina);
		return 0;
	}else{
		incPaginaAtual(pagina);
		setPosicaoInicial(pagina, posicaoFin + 1);
		setPosicaoFinal(pagina, posicaoFin + nrElemPorPag);
		setPosicaoAtual(pagina, posicaoFin + 1);
		return getPosicaoInicial(pagina);
	}
}

int paginaParaTras(PAGINA_RESULTADOS pagina){
	int nrElemPorPag = getNrElementosPorPagina(pagina);
	int posicaoInic = getPosicaoInicial(pagina);
	if(getPaginaAtual(pagina) == 0){
			setPosicaoAtual(pagina, 0);
			return 0; //ja estamos na primeira pagina
	}else{
		decPaginaAtual(pagina);
		setPosicaoInicial(pagina, posicaoInic - nrElemPorPag);
		setPosicaoFinal(pagina, posicaoInic-1);
		setPosicaoAtual(pagina, posicaoInic - nrElemPorPag);
		return getPosicaoInicial(pagina);
	}
}

void posicoesInit(PAGINA_RESULTADOS pagina){
	int nrpag = getPaginaAtual(pagina);
	int elemPorPag = getNrElementosPorPagina(pagina);

	setPosicaoInicial(pagina, nrpag * elemPorPag);
	setPosicaoFinal(pagina, getPosicaoInicial(pagina) + elemPorPag - 1);
	setPosicaoAtual(pagina, nrpag * elemPorPag);

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

	if( p  <= fim ){
		return getnElemento(pagina, incPosicaoAtual(pagina));
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
