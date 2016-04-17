#ifndef _NAVEGACAO_
#define _NAVEGACAO_
typedef void** Listagem;
typedef struct paginaResultados *PAGINA_RESULTADOS;

#define ELEM_POR_PAG 10

PAGINA_RESULTADOS paginaResultadosInit(int tamanhoL, int tamanhoElemento);

void* inserirResultadoLista(PAGINA_RESULTADOS pagina, void* item);

void ordenarResultadosLista(PAGINA_RESULTADOS pagina, int (*funcaoComparacao)() );

void imprimirPagina(PAGINA_RESULTADOS pagina, void (*funcaoImpressao)() );

int virarPagina(PAGINA_RESULTADOS pagina);

int paginaParaTras(PAGINA_RESULTADOS pagina);

void percorrerPaginaResultados(PAGINA_RESULTADOS pagina, int nrPag, int elemPorPag, void (*funcaoImpressao)() );

void setNrElementosPorPagina(PAGINA_RESULTADOS pagina, int n);

void posicoesInit(PAGINA_RESULTADOS pagina);

int getPaginaAtual(PAGINA_RESULTADOS pagina);

int getNrPaginasTotal(PAGINA_RESULTADOS pagina);

int getPosicaoInicial(PAGINA_RESULTADOS pagina);

int getPosicaoFinal(PAGINA_RESULTADOS pagina);

int getIndiceAtual(PAGINA_RESULTADOS pagina);

void calcularUltimaPag(PAGINA_RESULTADOS pagina);

void calcularPrimeiraPag(PAGINA_RESULTADOS pagina);

void* getElementoAtual(PAGINA_RESULTADOS pagina);

int getPosicaoAtual(PAGINA_RESULTADOS pagina);

int getTamanhoLista(PAGINA_RESULTADOS pagina);

#endif /* _NAVEGACAO_ */
