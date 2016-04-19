#ifndef LEITURAFICHEIROS
#define LEITURAFICHEIROS

#define MAXLINHAS 10000000
#define MAX_BUFFER_VENDAS 64
#define MAX_BUFFER_CATALOGO 32

void abrirFicheiroVendas(String nomeFicheiro, MODULO_GESTAO_FILIAIS moduloGestaoFiliais, MODULO_FATURACAO moduloFaturacao);
int abrirFicheiroClientes(CATALOGO_CLIENTES catalogo, String nomeFicheiro);

#endif /* LEITURAFICHEIROS */
