#ifndef LEITURAFICHEIROS
#define LEITURAFICHEIROS

#define MAXLINHAS 10000000
#define MAX_BUFFER_VENDAS 64
#define MAX_BUFFER_CATALOGO 32
#define INPUT_BUFFER 50

extern int STATUS;
extern float TEMPOS_LEITURA;
extern long NRCLIENTES;
extern long NRPRODUTOS;
extern long NRVENDAS;

extern CATALOGO_CLIENTES cat_Clientes;
extern CATALOGO_PRODUTOS cat_Produtos;
extern MODULO_GESTAO_FILIAIS mod_Gestao;
extern MODULO_FATURACAO mod_Faturacao;

void argumentosMain(int, const char **);
int abrirFicheirosInit(CATALOGO_CLIENTES,CATALOGO_PRODUTOS,MODULO_GESTAO_FILIAIS,MODULO_FATURACAO);

#endif /* LEITURAFICHEIROS */
