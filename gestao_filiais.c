/*
 *  *  * Filipe Marques
 *  *  * Laboratórios Informática III, Universidade do Minho, 2016
 *  *  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#include "venda.h"
#include "avl.h"
#include "navegacao.h"
#include "catalogo_produtos.h"
#include "catalogo_clientes.h"
#include "gestao_filiais.h"

struct moduloGestaoFiliais {
	int totalVendasAdicionadas;
	GestaoFilial filial[NRFILIAIS + 1];
};

struct gestaoFilial {
	int totalClientes;
	int totalProdutos;
	Modulo clientes;
	Modulo produtos;
};

struct gestaoCliente
{
	CodigoCliente_st codigoCliente;
	Quantidade_st quantidadeTotal;
	int nrProdutosDiferentes;
	Registos mes[12][2];
};

struct gestaoProduto
{
	CodigoProduto_st codigoProduto;
	Quantidade_st quantidadeTotal;
	int nrClientesTotal;
	Registos mes[12][2];
};

struct registoProduto
{
	CodigoProduto_st codigoProduto;
	Preco_st preco;
	Quantidade_st quantidade;
};

struct registoCliente
{
	CodigoCliente_st codigoCliente;
	Preco_st preco;
	Quantidade_st quantidade;
};

static GestaoFilial gestaoFilialInit();
static void freeGestaoFilial(GestaoFilial filial);
static GestaoCliente gestaoClienteInit();
static GestaoProduto gestaoProdutoInit();
static void freeGestaoCliente(void* cliente, void* param);
static void freeGestaoProduto(void* produto, void* param);
static void freeImediatoGestaoCliente(void* cliente, void* param);
static void freeImediatoGestaoProduto(void* produto, void* param);
static RegistoCliente registoClienteInit();
static RegistoProduto registoProdutoInit();
static void freeRegistoProduto(void *registo, void *param);
static void freeRegistoCliente(void *registo, void *param);

static int compara_clientes_gestaoCliente(const void *a, const void *b, void *param);
static int compara_produtos_gestaoProduto(const void *a, const void *b, void *param);
static int compara_registo_produtos(const void *a, const void *b, void *param);
static int compara_registo_clientes(const void *a, const void *b, void *param);
static int compara_registo_produtos_por_quantidade_decres(const void *a, const void *b, void *param);
static int compara_registo_produtos_por_custo_decres(const void *a, const void *b, void *param);

static GestaoCliente inserirNoModuloClientes(GestaoFilial modulo, VENDA venda);
static GestaoProduto inserirNoModuloProdutos(GestaoFilial modulo, VENDA venda);
static RegistoCliente inserirVendaNoRegistoClientes(GestaoProduto produto, VENDA venda);
static RegistoProduto inserirVendaNoRegistoProdutos(GestaoCliente cliente, VENDA venda);

/* GET'S E SET'S */

int getTotalVendasAdicionadas(MODULO_GESTAO_FILIAIS modulo){
	return modulo->totalVendasAdicionadas;
}

int getTotalClientesFilial(MODULO_GESTAO_FILIAIS modulo, Filial_st f){
	return modulo->filial[f]->totalClientes;
}

int getTotalProdutosFilial(MODULO_GESTAO_FILIAIS modulo, Filial_st f){
	return modulo->filial[f]->totalProdutos;
}

String getCodCliente_gestaoCliente(GestaoCliente cliente){
	return cliente->codigoCliente;
}

String getCodProduto_gestaoProduto(GestaoProduto produto){
	return produto->codigoProduto;
}

Quantidade_st getQuantidadeTotal_gestaoCliente(GestaoCliente cliente){
	return cliente->quantidadeTotal;
}

Quantidade_st getQuantidadeTotal_gestaoProduto(GestaoProduto produto){
	return produto->quantidadeTotal;
}

int getNrProdutosDiferentes_gestaoCliente(GestaoCliente cliente){
	return cliente->nrProdutosDiferentes;
}

int getNrClientesTotal_gestaoProduto(GestaoProduto produto){
	return produto->nrClientesTotal;
}

String getCodProduto_registoProduto(RegistoProduto produto){
	return produto->codigoProduto;
}

String getCodCliente_registoCliente(RegistoCliente cliente){
	return cliente->codigoCliente;
}

int getQuantidade_registoProduto(RegistoProduto produto){
	return produto->quantidade;
}

int getQuantidade_registoCliente(RegistoCliente cliente){
	return cliente->quantidade;
}

double getPreco_registoProduto(RegistoProduto produto){
	return produto->preco;
}

double getPreco_registoCliente(RegistoCliente cliente){
	return cliente->preco;
}

void calculaTotalFaturado_registoProduto(RegistoProduto produto){
	produto->preco = produto->preco * produto->quantidade;
}

void calculaTotalGasto_registoCliente(RegistoCliente cliente){
	cliente->preco = cliente->preco * cliente->quantidade;
}

static void incTotalVendasAdicionadas(MODULO_GESTAO_FILIAIS modulo){
	modulo->totalVendasAdicionadas++;
}

/* INICIALIZADORES E FREES */

/* Modulo principal */

MODULO_GESTAO_FILIAIS moduloGestaoFiliaisInit(){
	MODULO_GESTAO_FILIAIS modulo = (MODULO_GESTAO_FILIAIS) malloc(sizeof(struct moduloGestaoFiliais));
	int f;

	for(f = 0; f <= NRFILIAIS; f++)
		modulo->filial[f] = gestaoFilialInit();

	modulo->totalVendasAdicionadas = 0;
	return modulo;
}

void freeModuloGestaoFiliais(MODULO_GESTAO_FILIAIS modulo){
	int f;
	for(f = 0; f <= NRFILIAIS; f++)
		freeGestaoFilial(modulo->filial[f]);

	free(modulo);
}

/* Uma filial */

static GestaoFilial gestaoFilialInit(){
	GestaoFilial filial = (GestaoFilial) malloc(sizeof(struct gestaoFilial));

	filial->totalClientes = 0;
	filial->totalProdutos = 0;
	filial->clientes = avl_create(compara_clientes_gestaoCliente, NULL, NULL); 
	filial->produtos = avl_create(compara_produtos_gestaoProduto, NULL, NULL);

	return filial;
}

static void freeGestaoFilial(GestaoFilial filial){
	avl_destroy(filial->clientes,freeGestaoCliente);
	avl_destroy(filial->produtos,freeGestaoProduto);
	free(filial);
}

/* Um cliente */

static GestaoCliente gestaoClienteInit(){
	GestaoCliente novoCliente = (GestaoCliente) malloc(sizeof (struct gestaoCliente));
	int m;

	for(m = 0; m < 12; m++){
		novoCliente->mes[m][VENDA_NORMAL] = NULL; //apontadores para avl
		novoCliente->mes[m][VENDA_PROMOCIONAL] = NULL; //apontadores para avl
	}

	novoCliente->quantidadeTotal = 0;
	novoCliente->nrProdutosDiferentes = 0;
	return novoCliente;
}

GestaoCliente cloneGestaoCliente(GestaoCliente cliente){
	GestaoCliente novo = gestaoClienteInit();
	novo->codigoCliente = cloneCodigo(cliente->codigoCliente);
	novo->quantidadeTotal = cliente->quantidadeTotal;
	novo->nrProdutosDiferentes = cliente->nrProdutosDiferentes;
	return novo;
}

static void freeGestaoCliente(void* cliente, void* param){
	GestaoCliente gestaocliente = (GestaoCliente) cliente;

	int m;
	for(m = 0; m < 12; m++){
		if(gestaocliente->mes[m][VENDA_NORMAL] != NULL)
			avl_destroy(gestaocliente->mes[m][VENDA_NORMAL], freeRegistoProduto);
		if(gestaocliente->mes[m][VENDA_PROMOCIONAL] != NULL)
			avl_destroy(gestaocliente->mes[m][VENDA_PROMOCIONAL], freeRegistoProduto);
	}

	freeCodigo(gestaocliente->codigoCliente);
	free((GestaoCliente) cliente);
}

static void freeImediatoGestaoCliente(void* cliente, void* param){
	GestaoCliente gestaocliente = (GestaoCliente) cliente;
	freeCodigo(gestaocliente->codigoCliente);
	free((GestaoCliente) cliente);
}

/* Um produto */

static GestaoProduto gestaoProdutoInit(){
	GestaoProduto novoProduto = (GestaoProduto) malloc(sizeof (struct gestaoProduto));
	int m;

	for(m = 0; m < 12; m++){
		novoProduto->mes[m][VENDA_NORMAL] = NULL; //apontadores para avl
		novoProduto->mes[m][VENDA_PROMOCIONAL] = NULL; //apontadores para avl
	}

	novoProduto->quantidadeTotal = 0;
	novoProduto->nrClientesTotal = 0;
	return novoProduto;
}

static GestaoProduto cloneGestaoProduto(GestaoProduto produto){
	GestaoProduto novo = gestaoProdutoInit();
	novo->codigoProduto = cloneCodigo(produto->codigoProduto);
	novo->quantidadeTotal = produto->quantidadeTotal;
	novo->nrClientesTotal = produto->nrClientesTotal;
	return novo;
}

static void freeGestaoProduto(void* produto, void* param){
	GestaoProduto gestaoproduto = (GestaoProduto) produto;

	int m;
	for(m = 0; m < 12; m++){
		if(gestaoproduto->mes[m][VENDA_NORMAL] != NULL)
			avl_destroy(gestaoproduto->mes[m][VENDA_NORMAL], freeRegistoCliente);
		if(gestaoproduto->mes[m][VENDA_PROMOCIONAL] != NULL)
			avl_destroy(gestaoproduto->mes[m][VENDA_PROMOCIONAL], freeRegistoCliente);
	}

	freeCodigo(gestaoproduto->codigoProduto);
	free((GestaoProduto) produto);
}

static void freeImediatoGestaoProduto(void* produto, void* param){
	GestaoProduto gestaoproduto = (GestaoProduto) produto;
	freeCodigo(gestaoproduto->codigoProduto);
	free((GestaoProduto) produto);
}

/* Um registo */

static RegistoCliente registoClienteInit(){
	RegistoCliente c = (RegistoCliente) malloc(sizeof (struct registoCliente));
	return c;
}

static RegistoProduto registoProdutoInit(){
	RegistoProduto p = (RegistoProduto) malloc(sizeof (struct registoProduto));
	return p;
}

static void freeRegistoProduto(void *registo, void *param){
	RegistoProduto registoProduto = (RegistoProduto) registo;
	freeCodigo(registoProduto->codigoProduto);
	free(registo);
}

static void freeRegistoCliente(void *registo, void *param){
	RegistoCliente registoCliente = (RegistoCliente) registo;
	freeCodigo(registoCliente->codigoCliente);
	free(registo);
}

/* COMPARADORES DE ESTRUTURAS */

static int compara_clientes_gestaoCliente(const void *a, const void *b, void *param) {
	GestaoCliente cliente_a = (GestaoCliente) a;
	GestaoCliente cliente_b = (GestaoCliente) b;
	return strcmp((char *) cliente_a->codigoCliente, (char *) cliente_b->codigoCliente);
}

static int compara_produtos_gestaoProduto(const void *a, const void *b, void *param) {
	GestaoProduto produto_a = (GestaoProduto) a;
	GestaoProduto produto_b = (GestaoProduto) b;
	return strcmp((char *) produto_a->codigoProduto, (char *) produto_b->codigoProduto);
}

static int compara_registo_produtos(const void *a, const void *b, void *param) {
	RegistoProduto produto_a = (RegistoProduto) a;
	RegistoProduto produto_b = (RegistoProduto) b;
	return strcmp((char *) produto_a->codigoProduto, (char *) produto_b->codigoProduto);
}

static int compara_registo_clientes(const void *a, const void *b, void *param) {
	RegistoCliente cliente_a = (RegistoCliente) a;
	RegistoCliente cliente_b = (RegistoCliente) b;
	return strcmp((char *) cliente_a->codigoCliente, (char *) cliente_b->codigoCliente);
}

/* para o quicksort */
static int compara_registo_produtos_por_quantidade_decres(const void *a, const void *b, void *param) {
	RegistoProduto produto_a = (RegistoProduto) a;
	RegistoProduto produto_b = (RegistoProduto) b;
	return produto_b->quantidade - produto_a->quantidade;
}

static int compara_registo_produtos_por_custo_decres(const void *a, const void *b, void *param) {
	RegistoProduto produto_a = (RegistoProduto) a;
	RegistoProduto produto_b = (RegistoProduto) b;
	return (int) (produto_b->preco - produto_a->preco);
}

static int compara_produtos_gestaoProduto_por_quantidade(const void *a, const void *b, void *param) {
	GestaoProduto produto_a = (GestaoProduto) a;
	GestaoProduto produto_b = (GestaoProduto) b;
	return (int) (produto_b->quantidadeTotal - produto_a->quantidadeTotal);
}

/* INSERCOES */

/* No modulo principal */

MODULO_GESTAO_FILIAIS inserirVendaModuloGestaoFiliais(MODULO_GESTAO_FILIAIS modulo, VENDA venda){
	int f = calculaIndicieFilial(venda);
	inserirNoModuloClientes(modulo->filial[f],venda);
	inserirNoModuloProdutos(modulo->filial[f],venda);
	incTotalVendasAdicionadas(modulo);
	return modulo;
}

/* Gestao de clientes */

static GestaoCliente novoGestaoCliente(CodigoCliente_st codigo){
	GestaoCliente novo = gestaoClienteInit(); //alocar espaco para cliente com grelha tudo a NULL
	novo->codigoCliente = cloneCodigo(codigo); //copia deep do codigo cliente
	return novo;
}

static GestaoCliente inserirNoModuloClientes(GestaoFilial modulo, VENDA venda){
	//inicializar GestaoCliente
	GestaoCliente novoCliente = novoGestaoCliente(getCodigoCliente(venda));
	//vamos tentar inserir
	GestaoCliente c = (GestaoCliente) avl_insert(modulo->clientes, novoCliente); //avl_insert retorna valor do endereco na arvore do item duplicado, retorna NULL caso sucesso

	if(c == NULL) { //cliente ainda nao existia -- pouco eficiente, so em 20 000 / 1M de vezes vai cair aqui
		novoCliente->quantidadeTotal += getQuantidade(venda);
		modulo->totalClientes++;
		inserirVendaNoRegistoProdutos(novoCliente, venda);      
		return novoCliente; //insercao com sucesso
	}else{ //cliente ja existe
		freeImediatoGestaoCliente(novoCliente, NULL); //apagar cliente inutilmente criado -- esta operacao vai ser feita muitas vezes
		c->quantidadeTotal += getQuantidade(venda);
		inserirVendaNoRegistoProdutos(c, venda);
		return c; //nao foi inserido nenhum cliente, so a grelha foi actualizada
	}
}

/* Gestao de produtos */

static GestaoProduto novoGestaoProduto(CodigoProduto_st codigo){
	GestaoProduto novo = gestaoProdutoInit(); //alocar espaco para produto com grelha tudo a NULL
	novo->codigoProduto = cloneCodigo(codigo); //copia deep do codigo produto
	return novo;
}

static GestaoProduto inserirNoModuloProdutos(GestaoFilial modulo, VENDA venda){
	//inicializar GestaoProduto
	GestaoProduto novoProduto = novoGestaoProduto(getCodigoProduto(venda));
	//vamos tentar inserir
	GestaoProduto c = (GestaoProduto) avl_insert(modulo->produtos, novoProduto); //avl_insert retorna valor do endereco na arvore do item duplicado, retorna NULL caso sucesso

	if(c == NULL) { //produto ainda nao existia -- pouco eficiente, so em 20 000 / 1M de vezes vai cair aqui
		novoProduto->quantidadeTotal += getQuantidade(venda);
		modulo->totalProdutos++;
		inserirVendaNoRegistoClientes(novoProduto, venda);      
		return novoProduto; //insercao com sucesso
	}else{ //produto ja existe
		freeImediatoGestaoProduto(novoProduto, NULL); //apagar produto inutilmente criado -- esta operacao vai ser feita muitas vezes
		c->quantidadeTotal += getQuantidade(venda);
		inserirVendaNoRegistoClientes(c, venda);
		return c; //nao foi inserido nenhum produto, so a grelha foi actualizada
	}
}

/* Registo de clientes */

static RegistoCliente inserirRegistoCliente(RegistoCliente cliente, Registos registos){
	RegistoCliente p = (RegistoCliente) avl_insert(registos, cliente);
	if(p != NULL) { //se cliente ja existe
		freeRegistoCliente(cliente, NULL); //nao preciso de inserir cliente
		p->quantidade += cliente->quantidade; //basta actualizar quantidades
	}
	return p;
}

static RegistoCliente cloneVendaParaRegistoCliente(VENDA venda, RegistoCliente registo){
	registo->codigoCliente = cloneCodigo(getCodigoCliente(venda));
	registo->preco = getPreco(venda);
	registo->quantidade = getQuantidade(venda);
	return registo;
}

RegistoCliente cloneRegistoCliente(RegistoCliente registo){
	RegistoCliente p = registoClienteInit();

	p->codigoCliente = cloneCodigo(registo->codigoCliente);
	p->preco = registo->preco;
	p->quantidade = registo->quantidade;
	return p;
}

static RegistoCliente inserirVendaNoRegistoClientes(GestaoProduto produto, VENDA venda){
	RegistoCliente p;
	Mes_st mes = getMes(venda);
	TipoVenda_st tipo = getTipoVenda(venda);
	Registos regGrelha = produto->mes[mes][tipo];

	RegistoCliente registo = registoClienteInit();
	cloneVendaParaRegistoCliente(venda,registo); 

	// criar estrutura de registos no sitio certo
	if(regGrelha != NULL){ //ja existe estrutura criada na grelha
		p = inserirRegistoCliente(registo,regGrelha);
	}else{ //ainda nao existe estrutura
		Registos novoRegGrelha = avl_create(compara_registo_clientes, NULL, NULL);
		produto->mes[mes][tipo] = novoRegGrelha;
		p = inserirRegistoCliente(registo,novoRegGrelha);
	}

	if(p == NULL) produto->nrClientesTotal++;
	return p;
}

/* Registo de produtos */

static RegistoProduto inserirRegistoProduto(RegistoProduto produto, Registos registos){
	RegistoProduto p = (RegistoProduto) avl_insert(registos, produto);
	if(p != NULL) { //se produto ja existe
		freeRegistoProduto(produto, NULL); //nao preciso de inserir produto
		p->quantidade += produto->quantidade; //basta actualizar quantidades
	}
	return p;
}

static RegistoProduto cloneVendaParaRegistoProduto(VENDA venda, RegistoProduto registo){
	registo->codigoProduto = cloneCodigo(getCodigoProduto(venda));
	registo->preco = getPreco(venda);
	registo->quantidade = getQuantidade(venda);
	return registo;
}

static RegistoProduto cloneRegistoProduto(RegistoProduto registo){
	RegistoProduto p = registoProdutoInit();

	p->codigoProduto = cloneCodigo(registo->codigoProduto);
	p->preco = registo->preco;
	p->quantidade = registo->quantidade;
	return p;
}

static RegistoProduto inserirVendaNoRegistoProdutos(GestaoCliente cliente, VENDA venda){
	RegistoProduto p;
	Mes_st mes = getMes(venda);
	TipoVenda_st tipo = getTipoVenda(venda);
	Registos regGrelha = cliente->mes[mes][tipo];

	RegistoProduto registo = registoProdutoInit();
	cloneVendaParaRegistoProduto(venda,registo); 

	// criar estrutura de registos no sitio certo
	if(regGrelha != NULL){ //ja existe estrutura criada na grelha
		p = inserirRegistoProduto(registo,regGrelha);
	}else{ //ainda nao existe estrutura
		Registos novoRegGrelha = avl_create(compara_registo_produtos, NULL, NULL);
		cliente->mes[mes][tipo] = novoRegGrelha;
		p = inserirRegistoProduto(registo,novoRegGrelha);
	}

	if(p == NULL) cliente->nrProdutosDiferentes++;
	return p;
}

/* CONSULTAS */

GestaoCliente clienteFezAlgumaCompra_filial(MODULO_GESTAO_FILIAIS modulo, CodigoCliente_st codigo, Filial_st filial){
	GestaoCliente cliente_aux = novoGestaoCliente(codigo);
	GestaoCliente res = (GestaoCliente) avl_find(modulo->filial[filial]->clientes, cliente_aux);
	freeImediatoGestaoCliente(cliente_aux,NULL);
	return res;
}

GestaoProduto produtoFoiVendido_filial(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, Filial_st filial){
	GestaoProduto produto_aux = novoGestaoProduto(codigo);
	GestaoProduto res = (GestaoProduto) avl_find(modulo->filial[filial]->produtos, produto_aux);
	freeImediatoGestaoProduto(produto_aux,NULL);
	return res;
}

GestaoCliente clienteFezAlgumaCompra_global(MODULO_GESTAO_FILIAIS modulo, CodigoCliente_st codigo){
	int f;
	GestaoCliente cliente = NULL;
	for(f = 0; f <= NRFILIAIS && !cliente; f++)
		cliente = clienteFezAlgumaCompra_filial(modulo, codigo, f);
	return cliente;
}

GestaoProduto produtoFoiVendido_global(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo){
	int f;
	GestaoProduto produto = NULL;
	for(f = 0; f <= NRFILIAIS && !produto; f++)
		produto = produtoFoiVendido_filial(modulo, codigo, f);
	return produto;
}

GestaoCliente encontrarCliente(MODULO_GESTAO_FILIAIS modulo, CodigoCliente_st codigo, Filial_st filial){
	return  clienteFezAlgumaCompra_filial(modulo, codigo, filial);
}

GestaoProduto encontrarProduto(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, Filial_st filial){
	return  produtoFoiVendido_filial(modulo, codigo, filial);
}

Registos verificaClienteFezComprasNumMes(GestaoCliente cliente, Mes_st mes, TipoVenda_st tipo){
	return ( cliente->mes[mes][tipo] );
}

Registos verificaProdutoFoiVendidoNumMes(GestaoProduto produto, Mes_st mes, TipoVenda_st tipo){
	return ( produto->mes[mes][tipo] );
}

Quantidade_st calculaQuantidadeTotalVendida_RegistoProdutos(Registos registos){
	Quantidade_st quantidade_compras = 0;
	RegistoProduto produto;

	TravessiaModulo trav = avl_trav_alloc();
	avl_t_init(trav, registos);
	produto = (RegistoProduto) avl_t_next(trav);

	while(produto){
		quantidade_compras += produto->quantidade;
		produto = avl_t_next(trav);
	}

	avl_trav_free(trav);
	return quantidade_compras;
}

Preco_st faturacaoTotal_RegistoProdutos(Registos registos){
	Preco_st faturacao = 0;
	RegistoProduto produto;

	TravessiaModulo trav = avl_trav_alloc();
	avl_t_init(trav, registos);
	produto = (RegistoProduto) avl_t_next(trav);

	while(produto){
		faturacao += produto->quantidade * produto->preco;
		produto = avl_t_next(trav);
	}

	avl_trav_free(trav);
	return faturacao;
}

Quantidade_st calculaQuantidadeTotalVendida_RegistoClientes(Registos registos){
	Quantidade_st quantidade_compras = 0;
	RegistoCliente cliente;

	TravessiaModulo trav = avl_trav_alloc();
	avl_t_init(trav, registos);
	cliente = (RegistoCliente) avl_t_next(trav);

	while(cliente){
		quantidade_compras += cliente->quantidade;
		cliente = avl_t_next(trav);
	}

	avl_trav_free(trav);
	return quantidade_compras;
}

Preco_st faturacaoTotal_RegistoClientes(Registos registos){
	Preco_st faturacao = 0;
	RegistoCliente cliente;

	TravessiaModulo trav = avl_trav_alloc();
	avl_t_init(trav, registos);
	cliente = (RegistoCliente) avl_t_next(trav);

	while(cliente){
		faturacao += cliente->quantidade * cliente->preco;
		cliente = avl_t_next(trav);
	}

	avl_trav_free(trav);
	return faturacao;
}

Filial_st calcularFilialComMenosClientes(MODULO_GESTAO_FILIAIS modulo){
	int f, f_min=1;
	int min = modulo->filial[1]->totalClientes;

	for(f = 1; f <= NRFILIAIS; f++)
		if(modulo->filial[f]->totalClientes < min){
			min = modulo->filial[f]->totalClientes;
			f_min = f;
		}

	return f_min;
}

/* TRAVESSIAS */

Quantidade_st quantidadeProdutosClienteComprouNumMes(GestaoCliente cliente, Mes_st m){
	Quantidade_st compras_normal=0, compras_promocional=0;
	RegistoProduto produto = NULL;

	if(cliente->mes[m][VENDA_NORMAL] != NULL){
		TravessiaModulo trav = avl_trav_alloc();
		avl_t_init(trav, cliente->mes[m][VENDA_NORMAL]);
		produto = avl_t_next(trav);
		if(produto!=NULL) compras_normal += produto->quantidade;

		while((produto = avl_t_next(trav))){
			compras_normal += produto->quantidade;
		}
		avl_trav_free(trav);
	}

	if(cliente->mes[m][VENDA_PROMOCIONAL] != NULL){ 
		TravessiaModulo trav = avl_trav_alloc();
		avl_t_init(trav, cliente->mes[m][VENDA_PROMOCIONAL]);
		produto = avl_t_next(trav);
		if(produto!=NULL) compras_promocional += produto->quantidade;

		while((produto = avl_t_next(trav))){
			compras_promocional += produto->quantidade;
		}
		avl_trav_free(trav);
	}

	return compras_normal + compras_promocional;
}

PAGINA_RESULTADOS quantidadeProdutosClienteCompraPorMes(MODULO_GESTAO_FILIAIS modulo, CodigoCliente_st codigo){
	int f, m;
	GestaoCliente cliente_aux = NULL;
	PAGINA_RESULTADOS pagina = paginaResultadosInit(NRFILIAIS,12);

	for(f = 1; f <= NRFILIAIS; f++){
		cliente_aux = clienteFezAlgumaCompra_filial(modulo, codigo, f);
		if(cliente_aux != NULL){
			for(m = 0; m < 12; m++){
				long* numero = malloc(sizeof(long));
				*numero = quantidadeProdutosClienteComprouNumMes(cliente_aux, m);
				inserirResultadoLista(pagina, numero);
			}
		}else{
			for(m = 0; m < 12; m++){
				long* numero = malloc(sizeof(long));
				*numero = 0;
				inserirResultadoLista(pagina, numero);
			}       
		}
	}

	transporResultados(pagina);
	return pagina;
}


Registos listaProdutosClienteComprouNumMes(GestaoCliente cliente, Mes_st mes, Registos produtos){
	Registos compras = verificaClienteFezComprasNumMes(cliente, mes, VENDA_NORMAL);
	if(compras != NULL){
		RegistoProduto produto;
		TravessiaModulo trav = avl_trav_alloc();
		avl_t_init(trav, compras);
		produto = (RegistoProduto) avl_t_next(trav);

		while(produto){
			RegistoProduto novo_produto = cloneRegistoProduto(produto);
			inserirRegistoProduto(novo_produto, produtos);
			produto = avl_t_next(trav);
		}

		avl_trav_free(trav);
	}

	compras = verificaClienteFezComprasNumMes(cliente, mes, VENDA_PROMOCIONAL);
	if(compras != NULL){
		RegistoProduto produto;
		TravessiaModulo trav = avl_trav_alloc();
		avl_t_init(trav, compras);
		produto = (RegistoProduto) avl_t_next(trav);

		while(produto){
			RegistoProduto novo_produto = cloneRegistoProduto(produto);
			inserirRegistoProduto(novo_produto, produtos);
			produto = avl_t_next(trav);
		}

		avl_trav_free(trav);
	}

	return produtos;
}

Registos listaProdutosClienteComprouNumMesEmTodasAsFiliais(MODULO_GESTAO_FILIAIS modulo, Registos produtos, CodigoCliente_st codigo, Mes_st mes){
	int f;
	for(f = 1; f <= NRFILIAIS; f++){
		GestaoCliente cliente = clienteFezAlgumaCompra_filial(modulo, codigo, f);
		if(cliente != NULL){
			listaProdutosClienteComprouNumMes(cliente, mes, produtos);
		}
	}
	return produtos;
}

PAGINA_RESULTADOS listaProdutosClienteMaisComprou(MODULO_GESTAO_FILIAIS modulo, CodigoCliente_st codigo, Mes_st mes){
	int tamanho;
	TravessiaModulo trav;
	PAGINA_RESULTADOS pagina;
	RegistoProduto produto;
	Registos produtos = avl_create(compara_registo_produtos, NULL, NULL);
	produtos = listaProdutosClienteComprouNumMesEmTodasAsFiliais(modulo, produtos, codigo, mes);
	tamanho = avl_node_count(produtos);

	pagina = (PAGINA_RESULTADOS) paginaResultadosInit(tamanho, 1);

	trav = (TravessiaModulo) avl_trav_alloc();
	avl_t_init(trav, produtos);
	produto = avl_t_next(trav);

	while(produto){
		inserirResultadoLista(pagina, produto);
		produto = avl_t_next(trav);
	}

	ordenarResultadosLista(pagina, compara_registo_produtos_por_quantidade_decres);
	avl_trav_free(trav);

	return pagina;
}

PAGINA_RESULTADOS topProdutosClienteGastouDinheiro(MODULO_GESTAO_FILIAIS modulo, CodigoCliente_st codigo){
	PAGINA_RESULTADOS pagina;
	RegistoProduto produto;
	TravessiaModulo trav;
	Registos produtos = avl_create(compara_registo_produtos, NULL, NULL);
	
	int mes, tamanho;
	for(mes = 0; mes < 12; mes++){
		produtos = listaProdutosClienteComprouNumMesEmTodasAsFiliais(modulo, produtos, codigo, mes);
	}

	tamanho = avl_node_count(produtos);

	pagina = (PAGINA_RESULTADOS) paginaResultadosInit(tamanho, 1);

	trav = (TravessiaModulo) avl_trav_alloc();
	avl_t_init(trav, produtos);
	produto = (RegistoProduto) avl_t_next(trav);

	while(produto){
		calculaTotalFaturado_registoProduto(produto);
		inserirResultadoLista(pagina, produto);
		produto = avl_t_next(trav);
	}

	ordenarResultadosLista(pagina, compara_registo_produtos_por_custo_decres);
	avl_trav_free(trav);

	return pagina;
}

// query 03
Quantidade_st calculaQuantidadeVendidaProduto(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, Filial_st f, TipoVenda_st t){
	GestaoProduto produto = encontrarProduto(modulo, codigo, f);
	Quantidade_st quantidade = 0;
	Registos registos;
	int m;

	if(produto == NULL) return 0;
	else{
		for(m = 0; m < 12; m++){
			registos = (Registos) verificaProdutoFoiVendidoNumMes(produto, m, t);
			if(registos != NULL){
				quantidade += calculaQuantidadeTotalVendida_RegistoClientes(registos);
			}
		}
	}
	return quantidade;
}

Quantidade_st calculaQuantidadeVendidaProduto_global(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, TipoVenda_st t){
	int f, quantidade = 0;
	for(f = 1; f <= NRFILIAIS; f++){
		quantidade += calculaQuantidadeVendidaProduto(modulo, codigo, f, t);
	}
	return quantidade;
}

Preco_st calculaFaturacaoProduto(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, Filial_st f, TipoVenda_st t){
	GestaoProduto produto = encontrarProduto(modulo, codigo, f);
	Preco_st faturacao = 0;
	Registos registos;
	int m;

	if(produto == NULL) return 0;
	else{
		for(m = 0; m < 12; m++){
			registos = (Registos) verificaProdutoFoiVendidoNumMes(produto, m, t);
			if(registos != NULL){
				faturacao += faturacaoTotal_RegistoClientes(registos);
			}
		}
	}
	return faturacao;
}

Preco_st calculaFaturacaoProduto_global(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, TipoVenda_st t){
	int f;
	Preco_st faturacao = 0;
	for(f = 1; f <= NRFILIAIS; f++){
		faturacao += calculaFaturacaoProduto(modulo, codigo, f, t);
	}
	return faturacao;
}

// query 08
PAGINA_RESULTADOS listaClientesCompraramProduto(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, Filial_st f){
	GestaoProduto produto = produtoFoiVendido_filial(modulo, codigo, f);
	int tamanho = produto->nrClientesTotal;
	RegistoCliente cliente;
	TravessiaModulo trav;
	CodigoCliente_st codigoCliente;

	PAGINA_RESULTADOS pagina = (PAGINA_RESULTADOS) paginaResultadosInit(tamanho, 2);

	int m;
	for(m = 0; m < 12; m++){
		Registos clientes = verificaProdutoFoiVendidoNumMes(produto, m, VENDA_NORMAL);
		if(clientes !=NULL){
			trav = (TravessiaModulo) avl_trav_alloc();
			avl_t_init(trav, clientes);
			cliente = (RegistoCliente) avl_t_next(trav);

			while(cliente){
			char* tipo_N = (char*) malloc(2);
			strncpy(tipo_N, "N", 2);
				codigoCliente = getCodCliente_registoCliente(cliente);
				inserirResultadoLista(pagina, codigoCliente);
				inserirResultadoLista(pagina, tipo_N);
				cliente = avl_t_next(trav);
			}
			avl_trav_free(trav);

		}

		clientes = verificaProdutoFoiVendidoNumMes(produto, m, VENDA_PROMOCIONAL);
		if(clientes !=NULL){
			trav = (TravessiaModulo) avl_trav_alloc();
			avl_t_init(trav, clientes);
			cliente = (RegistoCliente) avl_t_next(trav);
	
			while(cliente){
			char* tipo_P = (char*) malloc(2);
			strncpy(tipo_P, "P", 2);
				codigoCliente = getCodCliente_registoCliente(cliente);
				inserirResultadoLista(pagina, codigoCliente);
				inserirResultadoLista(pagina, tipo_P);
				cliente = avl_t_next(trav);
			}
			avl_trav_free(trav);
		}
	}
	return pagina;
}

//query 04 (e 12)
PAGINA_RESULTADOS produtosNinguemComprou_filial(MODULO_GESTAO_FILIAIS modulo, CATALOGO_PRODUTOS catalogo, Filial_st f){
	int ind; 
	TravessiaModulo trav;
	CodigoProduto_st produto;
	int totalProdutos = calcularTotalProdutos( catalogo );
	PAGINA_RESULTADOS pagina = (PAGINA_RESULTADOS) paginaResultadosInit(totalProdutos,1);

	for(ind = 0; ind < 27; ind++){
		trav = avl_trav_alloc();
		avl_t_init(trav, getCatalogoProdutosPorIndice(catalogo,ind));
		while( (produto = avl_t_next(trav)) ){
			if( !produtoFoiVendido_filial(modulo,produto,f) )
				inserirResultadoLista(pagina, produto);
		}
		freeTravessiaCatalogoProdutos(trav);
	}
	return pagina;
}

PAGINA_RESULTADOS produtosNinguemComprou_global(MODULO_GESTAO_FILIAIS modulo, CATALOGO_PRODUTOS catalogo){
	int ind; 
	TravessiaModulo trav;
	CodigoProduto_st produto;
	int totalProdutos = calcularTotalProdutos( catalogo );
	PAGINA_RESULTADOS pagina = (PAGINA_RESULTADOS) paginaResultadosInit(totalProdutos,1);

	for(ind = 0; ind < 27; ind++){
		trav = avl_trav_alloc();
		avl_t_init(trav, getCatalogoProdutosPorIndice(catalogo,ind));
		while( (produto = avl_t_next(trav)) ){
			if( !produtoFoiVendido_global(modulo,produto) ){
				inserirResultadoLista(pagina, produto);
			}
		}
		freeTravessiaCatalogoProdutos(trav);
	}
	return pagina;
}

//query 12
PAGINA_RESULTADOS clientesNaoCompraram_filial(MODULO_GESTAO_FILIAIS modulo, CATALOGO_CLIENTES catalogo, Filial_st f){
	int ind; 
	TravessiaModulo trav;
	CodigoCliente_st cliente;
	int totalClientes = calcularTotalClientes( catalogo );
	PAGINA_RESULTADOS pagina = (PAGINA_RESULTADOS) paginaResultadosInit(totalClientes,1);

	for(ind = 0; ind < 27; ind++){
		trav = avl_trav_alloc();
		avl_t_init(trav, getCatalogoClientesPorIndice(catalogo,ind));
		while( (cliente = avl_t_next(trav)) ){
			if( !clienteFezAlgumaCompra_filial(modulo,cliente,f) ){
				inserirResultadoLista(pagina, cliente);
			}

		}
		freeTravessiaCatalogoClientes(trav);
	}
	return pagina;
}

PAGINA_RESULTADOS clientesNaoCompraram_global(MODULO_GESTAO_FILIAIS modulo, CATALOGO_CLIENTES catalogo){
	int ind; 
	TravessiaModulo trav;
	CodigoCliente_st cliente;
	int totalClientes = calcularTotalClientes( catalogo );
	PAGINA_RESULTADOS pagina = (PAGINA_RESULTADOS) paginaResultadosInit(totalClientes,1);

	for(ind = 0; ind < 27; ind++){
		trav = avl_trav_alloc();
		avl_t_init(trav, getCatalogoClientesPorIndice(catalogo,ind));
		while( (cliente = avl_t_next(trav)) ){
			if( !clienteFezAlgumaCompra_global(modulo,cliente) ){
				inserirResultadoLista(pagina, cliente);
			}
		}
		freeTravessiaCatalogoClientes(trav);
	}
	return pagina;
}



//query 07
int verificaClienteComprouTodasFiliais(MODULO_GESTAO_FILIAIS modulo, CodigoCliente_st codigo, Filial_st excepto_nesta ){
	int f, resultado = 1;

	for(f = 1; f <= NRFILIAIS; f++){
		if(f == excepto_nesta) 
			continue;
		else if( !clienteFezAlgumaCompra_filial(modulo,codigo,f) )
			resultado = 0;
	}

	return resultado;
}

PAGINA_RESULTADOS clientesCompraramTodasFiliais(MODULO_GESTAO_FILIAIS modulo){
	Filial_st filial_menor = calcularFilialComMenosClientes(modulo);
	PAGINA_RESULTADOS pagina = paginaResultadosInit(getTotalClientesFilial(modulo,filial_menor),1);

	TravessiaModulo trav;
	GestaoCliente cliente;
	CodigoCliente_st codigo;

	trav = avl_trav_alloc();
	avl_t_init(trav, modulo->filial[filial_menor]->clientes);
	while( (cliente = avl_t_next(trav)) ){
		codigo = cliente->codigoCliente;
		if( verificaClienteComprouTodasFiliais(modulo, codigo, filial_menor) ){
			inserirResultadoLista(pagina, codigo);
		}
	}
	avl_trav_free(trav);


	return pagina;
}

//query 10
PAGINA_RESULTADOS produtosMaisVendidos_filial(MODULO_GESTAO_FILIAIS modulo, Filial_st f){
	int tamanho = getTotalProdutosFilial(modulo,f);
	PAGINA_RESULTADOS pagina = paginaResultadosInit(tamanho,1);

	TravessiaModulo trav;
	GestaoProduto produto;

	trav = avl_trav_alloc();
	avl_t_init(trav, modulo->filial[f]->produtos);

	while( (produto = avl_t_next(trav)) ){
		inserirResultadoLista(pagina, produto);
	}

	ordenarResultadosLista(pagina, compara_produtos_gestaoProduto_por_quantidade);
	avl_trav_free(trav);


	return pagina;
}

PAGINA_RESULTADOS produtosMaisVendidos_global(MODULO_GESTAO_FILIAIS modulo, int n){
	// int tamanho = calcularTotalProdutos(catalogo);
	PAGINA_RESULTADOS pagina = paginaResultadosInit(n,1);
	Modulo produtos_global = avl_create(compara_produtos_gestaoProduto, NULL, NULL);

	TravessiaModulo trav;
	GestaoProduto produto, produto_rep, novo;
	int f;

	for(f = 1; f <= NRFILIAIS; f++){
		trav = avl_trav_alloc();
		avl_t_init(trav, modulo->filial[f]->produtos);
		while( (produto = avl_t_next(trav)) ){
			novo = (GestaoProduto) cloneGestaoProduto(produto);
			produto_rep = (GestaoProduto) avl_insert(produtos_global, novo);
			if(produto_rep != NULL){ /* produto ja existe */
				freeImediatoGestaoProduto(novo, NULL);
				produto_rep->quantidadeTotal += getQuantidadeTotal_gestaoProduto(produto);
				produto_rep->nrClientesTotal += getNrClientesTotal_gestaoProduto(produto);
			}
		}
		avl_trav_free(trav);
	}

	trav = avl_trav_alloc();
	avl_t_init(trav, produtos_global);
	while( (produto = avl_t_next(trav)) ){
		produto = (GestaoProduto) cloneGestaoProduto(produto);
		inserirResultadoLista(pagina, produto);
	}
	avl_trav_free(trav);
	avl_destroy(produtos_global,freeGestaoProduto);
	ordenarResultadosLista(pagina, compara_produtos_gestaoProduto_por_quantidade);

	return pagina;
}

/*
void travessiaTesteRegistoProdutos(GestaoCliente modulo){
	int i,j,nr;
	printf("\tProdutos comprados:\n");
	for (i = 0; i < 12; i++)
		for(j = 0; j < 2; j++)
			if(modulo->mes[i][j] != NULL){
				RegistoProduto meuproduto;
				TravessiaModulo trav = avl_trav_alloc();
				avl_t_init(trav, modulo->mes[i][j]);

				nr = 0;
				while((meuproduto = avl_t_next(trav)) && nr < 10){
					printf("\t%s - Mes: %s - Quantidade: %d - Tipo: %c\n", meuproduto->codigoProduto, Mes[i], meuproduto->quantidade, j ? 'P' : 'N');
					nr++;
				}
			}
}


void travessiaTesteRegistoClientes(GestaoProduto modulo){
	int i,j,nr;
	printf("\tClientes que compraram este produto:\n");
	for (i = 0; i < 12; i++)
		for(j = 0; j < 2; j++)
			if(modulo->mes[i][j] != NULL){
				RegistoCliente meucliente;
				TravessiaModulo trav = avl_trav_alloc();
				avl_t_init(trav, modulo->mes[i][j]);

				nr = 0;
				while((meucliente = avl_t_next(trav)) && nr < 10){
					printf("\t%s - Mes: %s - Quantidade: %d - Tipo: %c\n", meucliente->codigoCliente, Mes[i], meucliente->quantidade, j ? 'P' : 'N');
					nr++;
				}
			}
}

void travessiaTesteGestaoFilial(GestaoFilial modulo){
	int nr;
	GestaoCliente meucliente;
	GestaoProduto meuproduto;
	TravessiaModulo trav = avl_trav_alloc();
	avl_t_init(trav, modulo->clientes);

	printf("**MODULO CLIENTES** - total: %d\n",modulo->totalClientes);

	nr = 0;
	while((meucliente = avl_t_next(trav)) && nr < 10){
		printf(" - Cliente %s :: Quantidade total comprada: %d - \n", meucliente->codigoCliente,  meucliente->quantidadeTotal);
		travessiaTesteRegistoProdutos(meucliente);
		nr++;
	}

	printf("**MODULO PRODUTOS** - total: %d\n",modulo->totalProdutos);

	trav = avl_trav_alloc();
	avl_t_init(trav, modulo->produtos);
	nr = 0;
	while((meuproduto = avl_t_next(trav)) && nr < 10){
		printf(" - Produto %s :: Quantidade total vendida: %d - \n", meuproduto->codigoProduto, meuproduto->quantidadeTotal);
		travessiaTesteRegistoClientes(meuproduto);
		nr++;
	}
}
*/

