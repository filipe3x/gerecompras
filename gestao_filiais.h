#ifndef GESTAOFILIAIS_H
#define GESTAOFILIAIS_H

typedef struct moduloGestaoFiliais *MODULO_GESTAO_FILIAIS;
typedef struct gestaoFilial *GestaoFilial;

typedef struct gestaoCliente *GestaoCliente;
typedef struct gestaoProduto *GestaoProduto;

typedef struct registoProduto *RegistoProduto;
typedef struct registoCliente *RegistoCliente;

#define NRFILIAIS 3

MODULO_GESTAO_FILIAIS moduloGestaoFiliaisInit();
void freeModuloGestaoFiliais(MODULO_GESTAO_FILIAIS modulo);

MODULO_GESTAO_FILIAIS inserirVendaModuloGestaoFiliais(MODULO_GESTAO_FILIAIS modulo, VENDA venda);

int getTotalVendasAdicionadas(MODULO_GESTAO_FILIAIS modulo);
int getTotalClientesFilial(MODULO_GESTAO_FILIAIS modulo, Filial_st f);
int getTotalProdutosFilial(MODULO_GESTAO_FILIAIS modulo, Filial_st f);
String getCodCliente_gestaoCliente(GestaoCliente cliente);
String getCodProduto_gestaoProduto(GestaoProduto produto);
Quantidade_st getQuantidadeTotal_gestaoCliente(GestaoCliente cliente);
Quantidade_st getQuantidadeTotal_gestaoProduto(GestaoProduto produto);
int getNrProdutosDiferentes_gestaoCliente(GestaoCliente cliente);
int getNrClientesTotal_gestaoProduto(GestaoProduto produto);
String getCodProduto_registoProduto(RegistoProduto produto);
String getCodCliente_registoCliente(RegistoCliente cliente);
int getQuantidade_registoProduto(RegistoProduto produto);
int getQuantidade_registoCliente(RegistoCliente cliente);
double getPreco_registoProduto(RegistoProduto produto);
double getPreco_registoCliente(RegistoCliente cliente);
void calculaTotalFaturado_registoProduto(RegistoProduto produto);
void calculaTotalGasto_registoCliente(RegistoCliente cliente);

GestaoCliente encontrarCliente(MODULO_GESTAO_FILIAIS modulo, CodigoCliente_st codigo, Filial_st filial);
GestaoProduto encontrarProduto(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, Filial_st filial);
GestaoCliente clienteFezAlgumaCompra_filial(MODULO_GESTAO_FILIAIS modulo, CodigoCliente_st codigo, Filial_st filial);
GestaoCliente clienteFezAlgumaCompra_global(MODULO_GESTAO_FILIAIS modulo, CodigoCliente_st codigo);
GestaoProduto produtoFoiVendido_filial(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, Filial_st filial);
GestaoProduto produtoFoiVendido_global(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo);
Registos verificaClienteFezComprasNumMes(GestaoCliente cliente, Mes_st mes, TipoVenda_st tipo);
Registos verificaProdutoFoiVendidoNumMes(GestaoProduto produto, Mes_st mes, TipoVenda_st tipo);

Filial_st calcularFilialComMenosClientes(MODULO_GESTAO_FILIAIS modulo);

Quantidade_st calculaQuantidadeVendidaProduto(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, Filial_st f, TipoVenda_st t);
Quantidade_st calculaQuantidadeVendidaProduto_global(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, TipoVenda_st t);
Preco_st calculaFaturacaoProduto(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, Filial_st f, TipoVenda_st t);
Preco_st calculaFaturacaoProduto_global(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, TipoVenda_st t);

PAGINA_RESULTADOS quantidadeProdutosClienteCompraPorMes(MODULO_GESTAO_FILIAIS modulo, CodigoCliente_st codigo);
PAGINA_RESULTADOS listaProdutosClienteMaisComprou(MODULO_GESTAO_FILIAIS modulo, CodigoCliente_st codigo, Mes_st mes);
PAGINA_RESULTADOS topProdutosClienteGastouDinheiro(MODULO_GESTAO_FILIAIS modulo, CodigoCliente_st codigo);
PAGINA_RESULTADOS listaClientesCompraramProduto(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, Filial_st f);
PAGINA_RESULTADOS produtosNinguemComprou_filial(MODULO_GESTAO_FILIAIS modulo, CATALOGO_PRODUTOS catalogo, Filial_st f);
PAGINA_RESULTADOS produtosNinguemComprou_global(MODULO_GESTAO_FILIAIS modulo, CATALOGO_PRODUTOS catalogo);
PAGINA_RESULTADOS clientesNaoCompraram_filial(MODULO_GESTAO_FILIAIS modulo, CATALOGO_CLIENTES catalogo, Filial_st f);
PAGINA_RESULTADOS clientesNaoCompraram_global(MODULO_GESTAO_FILIAIS modulo, CATALOGO_CLIENTES catalogo);
PAGINA_RESULTADOS clientesCompraramTodasFiliais(MODULO_GESTAO_FILIAIS modulo);
PAGINA_RESULTADOS produtosMaisVendidos_filial(MODULO_GESTAO_FILIAIS modulo, Filial_st f);
PAGINA_RESULTADOS produtosMaisVendidos_global(MODULO_GESTAO_FILIAIS modulo, int n);

#endif /* GESTAOFILIAIS_H */
