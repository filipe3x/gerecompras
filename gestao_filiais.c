/*
 *  *  * Filipe Marques
 *  *  * Laboratórios Informática III, Universidade do Minho, 2016
 *  *  */

#include "venda.h"
#include "avl.h"

int compara_vendas_avl(const void *avl_a, const void *avl_b, void *avl_param);
int compara_clientes_gestaoCliente(const void *avl_a, const void *avl_b, void *avl_param);
int compara_produtos_gestaoProduto(const void *avl_a, const void *avl_b, void *avl_param);
int compara_registo_produtos(const void *avl_a, const void *avl_b, void *avl_param);
int compara_registo_clientes(const void *avl_a, const void *avl_b, void *avl_param);

typedef struct vendas { // estrutura temporaria de teste
    Modulo filial[NRFILIAIS + 1];
} *Vendas;

//*******************************

typedef struct gestaoFilial {
	int totalClientes;
	int totalProdutos;
	Modulo clientes;
	Modulo produtos;
} *GestaoFilial;

typedef struct moduloGestaoFiliais {
	int totalVendas;
    GestaoFilial filial[NRFILIAIS + 1];
} *MODULO_GESTAO_FILIAIS;

typedef struct gestaoCliente
{
	CodigoCliente_st codigoCliente;
	Quantidade_st quantidadeTotal;
	Registos mes[12][2]; //Registo produtos por cliente para cada mes e Tipo compra
} *GestaoCliente;

typedef struct gestaoProduto
{
	CodigoProduto_st codigoProduto;
	Quantidade_st quantidadeTotal;
	Registos mes[12][2]; //Registo clientes por produto para cada mes e Tipo compra
} *GestaoProduto;

typedef struct registoProduto
{
	CodigoProduto_st codigoProduto;
	Preco_st preco;
	Quantidade_st quantidade;
} *RegistoProduto;

typedef struct registoCliente
{
	CodigoCliente_st codigoCliente;
	Preco_st preco;
	Quantidade_st quantidade;
} *RegistoCliente;

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
	modulo->totalVendas = 0;

	int f;
	for(f = 0; f <= NRFILIAIS; f++)
		modulo->filial[f] = gestaoFilialInit();

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

// void freeRegistoProdutos(void *registos, void *param){
// 	Registos registoProdutos = (Registos) registos;
// 	avl_destroy(registoProdutos, freeRegistoProduto);
// 	free((RegistoProduto) registos);
// }

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
	novoCliente->quantidadeTotal = 0;

	int m;
	for(m = 0; m < 12; m++){
		novoCliente->mes[m][VENDA_NORMAL] = NULL; //apontadores para avl
		novoCliente->mes[m][VENDA_PROMOCIONAL] = NULL; //apontadores para avl
	}
	return novoCliente;
}

GestaoProduto gestaoProdutoInit(){
	GestaoProduto novoProduto = (GestaoProduto) malloc(sizeof (struct gestaoProduto));
	novoProduto->quantidadeTotal = 0;

	int m;
	for(m = 0; m < 12; m++){
		novoProduto->mes[m][VENDA_NORMAL] = NULL; //apontadores para avl
		novoProduto->mes[m][VENDA_PROMOCIONAL] = NULL; //apontadores para avl
	}
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

int compara_clientes_gestaoCliente(const void *avl_a, const void *avl_b, void *avl_param) {
	GestaoCliente cliente_a = (GestaoCliente) avl_a;
	GestaoCliente cliente_b = (GestaoCliente) avl_b;
    return strcmp((char *) cliente_a->codigoCliente, (char *) cliente_b->codigoCliente);
}

int compara_produtos_gestaoProduto(const void *avl_a, const void *avl_b, void *avl_param) {
	GestaoProduto produto_a = (GestaoProduto) avl_a;
	GestaoProduto produto_b = (GestaoProduto) avl_b;
    return strcmp((char *) produto_a->codigoProduto, (char *) produto_b->codigoProduto);
}

int compara_registo_produtos(const void *avl_a, const void *avl_b, void *avl_param) {
	RegistoProduto produto_a = (RegistoProduto) avl_a;
	RegistoProduto produto_b = (RegistoProduto) avl_b;
    return strcmp((char *) produto_a->codigoProduto, (char *) produto_b->codigoProduto);
}

int compara_registo_clientes(const void *avl_a, const void *avl_b, void *avl_param) {
	RegistoCliente cliente_a = (RegistoCliente) avl_a;
	RegistoCliente cliente_b = (RegistoCliente) avl_b;
    return strcmp((char *) cliente_a->codigoCliente, (char *) cliente_b->codigoCliente);
}

//funcao de teste
int compara_vendas_avl(const void *avl_a, const void *avl_b, void *avl_param) {
	VENDA venda_a = (VENDA) avl_a;
	VENDA venda_b = (VENDA) avl_b;
    return strcmp((char *) getCodigoCliente(venda_a), (char *) getCodigoCliente(venda_b));
}

//funcao de teste
Vendas filiaisInit(){
    Vendas p = (Vendas) malloc(sizeof (struct vendas)); //inicializar array de 4 apontadores para AVLs (filiais)
    return p;
}

//funcao de teste
Vendas avlFiliaisInit(Vendas res){
    // inicializar 4 AVLs, uma para cada filial
	int i;
    for (i = 0; i <= NRFILIAIS; i++) {
        //a funcao que inicializa a avl precisa de saber qual a funcao de comparacao, respeitando assim o principio de encapsulamento
        res->filial[i] = avl_create(compara_vendas_avl, NULL, NULL); 
    }
    return res;
}

void travessiaTesteVendas(Vendas vendas){
	//travessia
	int f;
	scanf("%d",&f);

    TravessiaModulo trav = avl_trav_alloc();
	avl_t_init(trav, vendas->filial[f]);

	int nr = 0;
	VENDA minhavenda;
	while((minhavenda = avl_t_next(trav)) && nr < 3){
		VENDA clone = linhaVendaInit();
		clonelinhaVenda(minhavenda, clone);
		imprimirLinhaVenda(clone);
		free(clone);
		nr++;
	}
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

RegistoProduto inserirVendaNoRegistoProdutos(GestaoCliente cliente, VENDA venda){
	RegistoProduto registo = registoProdutoInit(); //alocar memoria
	cloneVendaParaRegistoProduto(venda,registo); 

	Mes_st mes = getMes(venda);
	TipoVenda_st tipo = getTipoVenda(venda);
	Registos regGrelha = cliente->mes[mes][tipo];

	// criar estrutura de registos no sitio certo
	RegistoProduto p;
	if(regGrelha != NULL){ //ja existe estrutura criada na grelha
		p = inserirRegistoProduto(registo,regGrelha);
	}else{ //ainda nao existe estrutura
		Registos novoRegGrelha = avl_create(compara_registo_produtos, NULL, NULL);
		cliente->mes[mes][tipo] = novoRegGrelha;
		p = inserirRegistoProduto(registo,novoRegGrelha);
	}


	return p;
}

// Registos alocarRegistosProdutosNoGestaoCliente(GestaoCliente cliente, Mes_st mes, TipoVenda_st tipo){
// 	Registos registos = avl_create(compara_registo_produtos, NULL, NULL);
// 	cliente->mes[mes][tipo] = registos;
// 	return registos;
// }

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
	RegistoCliente registo = registoClienteInit(); //alocar memoria
	cloneVendaParaRegistoCliente(venda,registo); 
	RegistoCliente p = inserirRegistoCliente(registo,registos);
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
		freeRegistosClientesNoGestaoProduto(novoProduto,mes,tipo); //apagar sub-estrutura criada dentro do modulo Gestao Produto
		freeRegistoClientes(cliente,NULL); //apagar cliente
		freeGestaoProduto(novoProduto,NULL); //apagar produto inutilmente criado
		Registos entrada = c->mes[mes][tipo];
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
	//travessia
	int i,j;
	printf("\tProdutos comprados:\n");
	for (i = 0; i < 12; i++)
		for(j = 0; j < 2; j++)
			if(modulo->mes[i][j] != NULL){
				TravessiaModulo trav = avl_trav_alloc();
				avl_t_init(trav, modulo->mes[i][j]);

				int nr = 0;
				RegistoProduto meuproduto;
				while((meuproduto = avl_t_next(trav)) && nr < 10){
					printf("\t%s - Mes: %s - Quantidade: %d - Tipo: %c\n", meuproduto->codigoProduto, Mes[i], meuproduto->quantidade, j ? 'P' : 'N');
					nr++;
				}
			}
}

void travessiaTesteRegistoClientes(GestaoProduto modulo){
	//travessia
	int i,j;
	printf("\tClientes que compraram este produto:\n");
	for (i = 0; i < 12; i++)
		for(j = 0; j < 2; j++)
			if(modulo->mes[i][j] != NULL){
				TravessiaModulo trav = avl_trav_alloc();
				avl_t_init(trav, modulo->mes[i][j]);

				int nr = 0;
				RegistoCliente meucliente;
				while((meucliente = avl_t_next(trav)) && nr < 10){
					printf("\t%s - Mes: %s - Quantidade: %d - Tipo: %c\n", meucliente->codigoCliente, Mes[i], meucliente->quantidade, j ? 'P' : 'N');
					nr++;
				}
			}
}

void travessiaTesteGestaoFilial(GestaoFilial modulo){
	//travessia
    TravessiaModulo trav = avl_trav_alloc();
	avl_t_init(trav, modulo->clientes);

	printf("**MODULO CLIENTES** - total: %d\n",modulo->totalClientes);

	int nr = 0;
	GestaoCliente meucliente;
	while((meucliente = avl_t_next(trav)) && nr < 10){
		printf(" - Cliente %s :: Quantidade total comprada: %d - \n", meucliente->codigoCliente,  meucliente->quantidadeTotal);
		travessiaTesteRegistoProdutos(meucliente);
		nr++;
	}

	printf("**MODULO PRODUTOS** - total: %d\n",modulo->totalProdutos);

    trav = avl_trav_alloc();
	avl_t_init(trav, modulo->produtos);
	nr = 0;
	GestaoProduto meuproduto;
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
	modulo->totalVendas++;
	return modulo;
}
