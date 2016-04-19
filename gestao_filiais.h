#ifndef GESTAOFILIAIS_H
#define _QUERIES_
typedef struct moduloGestaoFiliais *MODULO_GESTAO_FILIAIS;
typedef struct gestaoFilial *GestaoFilial;

typedef struct gestaoCliente *GestaoCliente;
typedef struct gestaoProduto *GestaoProduto;

typedef struct registoProduto *RegistoProduto;
typedef struct registoCliente *RegistoCliente;

MODULO_GESTAO_FILIAIS moduloGestaoFiliaisInit();
void freeModuloGestaoFiliais(MODULO_GESTAO_FILIAIS modulo);

int compara_clientes_gestaoCliente(const void *a, const void *b, void *param);
int compara_produtos_gestaoProduto(const void *a, const void *b, void *param);
int compara_registo_produtos(const void *a, const void *b, void *param);
int compara_registo_clientes(const void *a, const void *b, void *param);

MODULO_GESTAO_FILIAIS inserirVendaModuloGestaoFiliais(MODULO_GESTAO_FILIAIS modulo, VENDA venda);

int getTotalVendasAdicionadas(MODULO_GESTAO_FILIAIS modulo);

int getQuantidade_registoProduto(RegistoProduto produto);
String getCodProduto_registoProduto(RegistoProduto produto);
double getPreco_registoProduto(RegistoProduto produto);




PAGINA_RESULTADOS quantidadeProdutosClienteCompraPorMes(MODULO_GESTAO_FILIAIS modulo, CodigoCliente_st codigo);

PAGINA_RESULTADOS listaProdutosClienteMaisComprou(MODULO_GESTAO_FILIAIS modulo, CodigoCliente_st codigo, Mes_st mes);

PAGINA_RESULTADOS topProdutosClienteGastouDinheiro(MODULO_GESTAO_FILIAIS modulo, CodigoCliente_st codigo);
#endif /* GESTAOFILIAIS_H */
