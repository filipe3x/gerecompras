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

int getTotalVendasAdicionadas(MODULO_GESTAO_FILIAIS modulo){
	return modulo->totalVendasAdicionadas;
}

int getNrClientesTotal_gestaoProduto(GestaoProduto produto){
	return produto->nrClientesTotal;
}

int getNrProdutosDiferentes_gestaoCliente(GestaoCliente cliente){
	return cliente->nrProdutosDiferentes;
}

int getQuantidade_registoProduto(RegistoProduto produto){
	return produto->quantidade;
}

double getPreco_registoProduto(RegistoProduto produto){
	return produto->preco;
}

void calculaTotalFaturado_registoProduto(RegistoProduto produto){
	produto->preco = produto->preco * produto->quantidade;
}

String getCodProduto_registoProduto(RegistoProduto produto){
	return produto->codigoProduto;
}

// Inicializadores e frees

GestaoFilial gestaoFilialInit(){
	GestaoFilial filial = (GestaoFilial) malloc(sizeof(struct gestaoFilial));

	filial->totalClientes = 0;
	filial->totalProdutos = 0;
	filial->clientes = avl_create(compara_clientes_gestaoCliente, NULL, NULL); 
	filial->produtos = avl_create(compara_produtos_gestaoProduto, NULL, NULL);

	return filial;
}

MODULO_GESTAO_FILIAIS moduloGestaoFiliaisInit(){
	MODULO_GESTAO_FILIAIS modulo = (MODULO_GESTAO_FILIAIS) malloc(sizeof(struct moduloGestaoFiliais));
	int f;

	for(f = 0; f <= NRFILIAIS; f++)
		modulo->filial[f] = gestaoFilialInit();

	modulo->totalVendasAdicionadas = 0;
	return modulo;
}

void freeRegistoProduto(void *registo, void *param){
	RegistoProduto registoProduto = (RegistoProduto) registo;
	freeCodigo(registoProduto->codigoProduto);
	free(registo);
}

void freeRegistoClientes(void *codigo, void *param){
	free((RegistoCliente) codigo);
}

void freeGestaoCliente(void* cliente, void* param){
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

void freeImediatoGestaoCliente(void* cliente, void* param){
	GestaoCliente gestaocliente = (GestaoCliente) cliente;
	freeCodigo(gestaocliente->codigoCliente);
	free((GestaoCliente) cliente);
}

void freeGestaoProduto(void* produto, void* param){
	free((GestaoProduto) produto);
}

void freeGestaoFilial(GestaoFilial filial){
	avl_destroy(filial->clientes,freeGestaoCliente);
	avl_destroy(filial->produtos,freeGestaoProduto);
	free(filial);
}

void freeModuloGestaoFiliais(MODULO_GESTAO_FILIAIS modulo){
	int f;
	for(f = 0; f <= NRFILIAIS; f++)
		freeGestaoFilial(modulo->filial[f]);

	free(modulo);
}

////

GestaoCliente gestaoClienteInit(){
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

GestaoProduto gestaoProdutoInit(){
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

void freeModuloClientes_ModuloProdutos(MODULO_GESTAO_FILIAIS modulo){
	int f;
	for(f = 0; f <= NRFILIAIS; f++){
		avl_destroy(modulo->filial[f]->clientes, freeGestaoCliente);
		avl_destroy(modulo->filial[f]->produtos, freeGestaoProduto);
	}
}

RegistoCliente registoClienteInit(){
	RegistoCliente c = (RegistoCliente) malloc(sizeof (struct registoCliente));
	return c;
}

RegistoProduto registoProdutoInit(){
	RegistoProduto p = (RegistoProduto) malloc(sizeof (struct registoProduto));
	return p;
}

void freeRegistosProdutosNoGestaoCliente(GestaoCliente cliente, Mes_st mes, TipoVenda_st tipo){
	avl_destroy(cliente->mes[mes][tipo], freeRegistoProduto); //CORRIGIR ISTO
	cliente->mes[mes][tipo] = NULL;
}

void freeRegistosClientesNoGestaoProduto(GestaoProduto produto, Mes_st mes, TipoVenda_st tipo){
	avl_destroy(produto->mes[mes][tipo], freeRegistoClientes);
	produto->mes[mes][tipo] = NULL;
}

////

int compara_clientes_gestaoCliente(const void *a, const void *b, void *param) {
	GestaoCliente cliente_a = (GestaoCliente) a;
	GestaoCliente cliente_b = (GestaoCliente) b;
	return strcmp((char *) cliente_a->codigoCliente, (char *) cliente_b->codigoCliente);
}

int compara_produtos_gestaoProduto(const void *a, const void *b, void *param) {
	GestaoProduto produto_a = (GestaoProduto) a;
	GestaoProduto produto_b = (GestaoProduto) b;
	return strcmp((char *) produto_a->codigoProduto, (char *) produto_b->codigoProduto);
}

int compara_registo_produtos(const void *a, const void *b, void *param) {
	RegistoProduto produto_a = (RegistoProduto) a;
	RegistoProduto produto_b = (RegistoProduto) b;
	return strcmp((char *) produto_a->codigoProduto, (char *) produto_b->codigoProduto);
}

int compara_registo_clientes(const void *a, const void *b, void *param) {
	RegistoCliente cliente_a = (RegistoCliente) a;
	RegistoCliente cliente_b = (RegistoCliente) b;
	return strcmp((char *) cliente_a->codigoCliente, (char *) cliente_b->codigoCliente);
}

// Modulo Registo de produtos

RegistoProduto inserirRegistoProduto(RegistoProduto produto, Registos registos){
	RegistoProduto p = (RegistoProduto) avl_insert(registos, produto);
	if(p != NULL) { //se produto ja existe
		freeRegistoProduto(produto, NULL); //nao preciso de inserir produto
		p->quantidade += produto->quantidade; //basta actualizar quantidades
	}
	return p;
}

RegistoProduto cloneVendaParaRegistoProduto(VENDA venda, RegistoProduto registo){
	registo->codigoProduto = cloneCodigo(getCodigoProduto(venda));
	registo->preco = getPreco(venda);
	registo->quantidade = getQuantidade(venda);
	return registo;
}

RegistoProduto cloneRegistoProduto(RegistoProduto registo){
	RegistoProduto p = registoProdutoInit();

	p->codigoProduto = cloneCodigo(registo->codigoProduto);
	p->preco = registo->preco;
	p->quantidade = registo->quantidade;
	return p;
}

RegistoProduto inserirVendaNoRegistoProdutos(GestaoCliente cliente, VENDA venda){
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

	return p;
}

// Modulo Registo de clientes

RegistoCliente inserirRegistoCliente(RegistoCliente cliente, Registos registos){
	RegistoCliente p = (RegistoCliente) avl_insert(registos, cliente);
	if(p != NULL) p->quantidade += cliente->quantidade; //se cliente ja existe, actualizar quantidades
	return p;
}

RegistoCliente cloneVendaParaRegistoCliente(VENDA venda, RegistoCliente registo){
	registo->codigoCliente = cloneCodigo(getCodigoCliente(venda));
	registo->codigoCliente = getCodigoCliente(venda);
	registo->preco = getPreco(venda);
	registo->quantidade = getQuantidade(venda);
	return registo;
}

RegistoCliente inserirVendaNoRegistoClientes(VENDA venda, Registos registos){
	RegistoCliente p;
	RegistoCliente registo = registoClienteInit(); //alocar memoria
	cloneVendaParaRegistoCliente(venda,registo); 
	p = inserirRegistoCliente(registo,registos);
	return p;
}

// Modulo Gestao de clientes

GestaoCliente novoGestaoCliente(CodigoCliente_st codigo){
	GestaoCliente novo = gestaoClienteInit(); //alocar espaco para cliente com grelha tudo a NULL
	novo->codigoCliente = cloneCodigo(codigo); //copia deep do codigo cliente
	return novo;
}

GestaoCliente inserirNoModuloClientes(GestaoFilial modulo, VENDA venda){
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

// Modulo Gestao de produtos

GestaoProduto novoGestaoProduto(CodigoProduto_st codigo){
	GestaoProduto novo = gestaoProdutoInit(); //alocar espaco para produto com grelha tudo a NULL
	novo->codigoProduto = cloneCodigo(codigo); //copia deep do codigo produto
	return novo;
}

Modulo alocarRegistosClientesNoGestaoProduto(GestaoProduto produto, Mes_st mes, TipoVenda_st tipo){
	Modulo modulo = avl_create(compara_registo_clientes, NULL, NULL);
	produto->mes[mes][tipo] = modulo;
	return modulo;
}

GestaoProduto inserirNoModuloProdutos(GestaoFilial modulo, VENDA venda){
	GestaoProduto novoProduto = novoGestaoProduto(getCodigoProduto(venda));
	//vamos assumir que produto ainda nao existe
	int mes = getMes(venda);
	int tipo = getTipoVenda(venda);
	Registos registos = (Registos) alocarRegistosClientesNoGestaoProduto(novoProduto,mes,tipo); //alocar modulo registos na grelha
	RegistoCliente cliente = inserirVendaNoRegistoClientes(venda, registos);
	GestaoProduto c = (GestaoProduto) avl_insert(modulo->produtos, novoProduto); //avl_insert retorna valor do endereco na arvore do item duplicado, retorna NULL caso sucesso

	if(c == NULL) {
		novoProduto->quantidadeTotal += getQuantidade(venda);
		modulo->totalProdutos++;
		return novoProduto; //insercao com sucesso
	}else{ //produto ja existe
		Registos entrada;
		freeRegistosClientesNoGestaoProduto(novoProduto,mes,tipo); //apagar sub-estrutura criada dentro do modulo Gestao Produto
		freeRegistoClientes(cliente,NULL); //apagar cliente
		freeGestaoProduto(novoProduto,NULL); //apagar produto inutilmente criado
		entrada = (Registos) c->mes[mes][tipo];
		if(entrada == NULL){ //e ainda nao ha entradas na grelha
			registos = (Registos) alocarRegistosClientesNoGestaoProduto(c,mes,tipo);
			c->quantidadeTotal += getQuantidade(venda);
			inserirVendaNoRegistoClientes(venda, registos);
		}else{ //ja existe entrada na grelha, por isso basta adicionar registo
			c->quantidadeTotal += getQuantidade(venda);
			inserirVendaNoRegistoClientes(venda, entrada);
		}
		return c;
	}
}

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


MODULO_GESTAO_FILIAIS inserirVendaModuloGestaoFiliais(MODULO_GESTAO_FILIAIS modulo, VENDA venda){
	int f = calculaIndicieFilial(venda);
	/////antes de inserir verificar se produto/cliente existe no catalogo (aprender a usar .h primeiro)
	inserirNoModuloClientes(modulo->filial[f],venda);
	//inserirNoModuloProdutos(modulo->filial[f],venda);
	modulo->totalVendasAdicionadas++;
	return modulo;
}

GestaoCliente clienteFezAlgumaCompra(MODULO_GESTAO_FILIAIS modulo, CodigoCliente_st codigo, Filial_st filial){
	GestaoCliente cliente_aux = novoGestaoCliente(codigo);
	GestaoCliente res = (GestaoCliente) avl_find(modulo->filial[filial]->clientes, cliente_aux);
	freeImediatoGestaoCliente(cliente_aux,NULL);
	return res;
}

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
	PAGINA_RESULTADOS pagina = paginaResultadosInit(12 * NRFILIAIS,12);

	for(f = 1; f <= NRFILIAIS; f++){
		cliente_aux = clienteFezAlgumaCompra(modulo, codigo, f);
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

Registos verificaClienteFezComprasNumMes(GestaoCliente cliente, Mes_st mes, TipoVenda_st tipo){
	return ( cliente->mes[mes][tipo] );
}

void listaProdutosClienteComprouNumMes(GestaoCliente cliente, Mes_st mes, Registos produtos){
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
}

Registos listaProdutosClienteComprouNumMesEmTodasAsFiliais(MODULO_GESTAO_FILIAIS modulo, Registos produtos, CodigoCliente_st codigo, Mes_st mes){
	int f;
	for(f = 1; f <= NRFILIAIS; f++){
		GestaoCliente cliente = clienteFezAlgumaCompra(modulo, codigo, f);
		if(cliente != NULL){
			listaProdutosClienteComprouNumMes(cliente, mes, produtos);
		}
	}
	return produtos;
}

int compara_registo_produtos_por_quantidade_decres(const void *a, const void *b, void *param) {
	RegistoProduto produto_a = (RegistoProduto) a;
	RegistoProduto produto_b = (RegistoProduto) b;
	return produto_b->quantidade - produto_a->quantidade;
}

int compara_registo_produtos_por_custo_decres(const void *a, const void *b, void *param) {
	RegistoProduto produto_a = (RegistoProduto) a;
	RegistoProduto produto_b = (RegistoProduto) b;
	return (int) (produto_b->preco - produto_a->preco);
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
	return pagina;
}
