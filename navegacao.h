#ifndef _NAVEGACAO_
#define _NAVEGACAO_
typedef void** Listagem;
typedef struct paginaResultados *PAGINA_RESULTADOS;

#define ELEM_POR_PAG 10

PAGINA_RESULTADOS paginaResultadosInit(int tamanhoL, int tamanhoElemento);

void* inserirResultadoLista(PAGINA_RESULTADOS pagina, void* item);

void percorrerPaginaResultados(PAGINA_RESULTADOS pagina, int nrPag, int elemPorPag, void (*funcaoImpressao)() );

#endif /* _NAVEGACAO_ */
