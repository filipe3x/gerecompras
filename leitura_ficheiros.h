#ifndef LEITURAFICHEIROS
#define LEITURAFICHEIROS

#define MAXLINHAS 10000000
#define MAX_BUFFER_VENDAS 64
#define MAX_BUFFER_CATALOGO 32

extern int STATUS;

void argumentosMain(int, const char **);
int abrirFicheirosInit(CATALOGO_CLIENTES,CATALOGO_PRODUTOS,MODULO_GESTAO_FILIAIS,MODULO_FATURACAO);

// void abrirFicheiroVendas(String nomeFicheiro, MODULO_GESTAO_FILIAIS moduloGestaoFiliais, 
// 	MODULO_FATURACAO moduloFaturacao, CATALOGO_PRODUTOS catalogoProdutos, CATALOGO_CLIENTES catalogoClientes);
// void abrirFicheiroClientes(String nomeFicheiro, CATALOGO_CLIENTES catalogo);
// void abrirFicheiroProdutos(String nomeFicheiro, CATALOGO_PRODUTOS catalogo);

#endif /* LEITURAFICHEIROS */
