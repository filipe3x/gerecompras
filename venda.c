typedef char* Codigo;
typedef char* String;

typedef char* CodigoProduto_st;
typedef double Preco_st;
typedef int Quantidade_st;
typedef int TipoVenda_st;
typedef char* CodigoCliente_st;
typedef int Mes_st;
typedef int Filial_st;

typedef struct stringLinhaVenda
{
	char *codigoProduto;
	char *preco;
	char *quantidade;
	char *tipoVenda;
	char *codigoCliente;
	char *mes;
	char *filial;
} StringVenda;

typedef struct linhaVenda
{
	CodigoProduto_st codigoProduto;
	Preco_st preco;
	Quantidade_st quantidade;
	TipoVenda_st tipoVenda;
	CodigoCliente_st codigoCliente;
	Mes_st mes;
	Filial_st filial;
} *VENDA;

typedef VENDA Venda;

#define VENDA_NORMAL 0
#define VENDA_PROMOCIONAL 1

const char* Mes[] = {"janeiro","fevereiro","marco","abril","maio","junho","julho","agosto","setembro","outubro","novembro","dezembro"};

Codigo cloneCodigo(Codigo codigo){
	int tamanho = strlen(codigo);
	Codigo novoCodigo;

	novoCodigo = (Codigo) malloc(tamanho + 1);
    strncpy(novoCodigo, codigo, tamanho + 1);

    return novoCodigo;
}

void freeCodigo(Codigo codigo){
	free(codigo);
}

VENDA linhaVendaInit(){
	return (VENDA) malloc(sizeof(struct linhaVenda));
}

void freeLinhaVENDA(VENDA venda){
	if(venda != NULL){
		freeCodigo(venda->codigoProduto);
		freeCodigo(venda->codigoCliente);
		free(venda);
	}
}

CodigoProduto_st getCodigoProduto(VENDA venda){
	return venda->codigoProduto;
}

Preco_st getPreco(VENDA venda){
	return venda->preco;
}

Quantidade_st getQuantidade(VENDA venda){
	return venda->quantidade;
}

TipoVenda_st getTipoVenda(VENDA venda){
	return venda->tipoVenda;
}

CodigoCliente_st getCodigoCliente(VENDA venda){
	return venda->codigoCliente;
}

Mes_st getMes(VENDA venda){
	return venda->mes;
}

Filial_st getFilial(VENDA venda){
	return venda->filial;
}

void setCodigoProduto(VENDA venda, CodigoProduto_st codigo){
	venda->codigoProduto = cloneCodigo(codigo);
}

void setPreco(VENDA venda, Preco_st preco){
	venda->preco = preco;
}

void setQuantidade(VENDA venda, Quantidade_st quantidade){
	venda->quantidade = quantidade;
}

void setTipoVenda(VENDA venda, TipoVenda_st tipo){
	venda->tipoVenda = tipo;
}

void setCodigoCliente(VENDA venda, CodigoCliente_st codigo){
	venda->codigoCliente = cloneCodigo(codigo);
}

void setMes(VENDA venda, Mes_st mes){
	venda->mes = mes;
}

void setFilial(VENDA venda, Filial_st filial){
	venda->filial = filial;
}

Filial_st calcula_indice_filial(VENDA venda) {
	return  getFilial(venda);
}

VENDA clonelinhaVenda(VENDA origem, VENDA novaVenda){
	setCodigoProduto(novaVenda, getCodigoProduto(origem)); //clone
	setPreco(novaVenda, getPreco(origem));
	setQuantidade(novaVenda, getQuantidade(origem));
	setTipoVenda(novaVenda, getTipoVenda(origem));
	setCodigoCliente(novaVenda, getCodigoCliente(origem)); //clone
	setMes(novaVenda, getMes(origem));
	setFilial(novaVenda, getFilial(origem));
	return novaVenda;
}

static int lerLinhaVenda(char *linha, StringVenda *strings){
	char *contexto = linha;
	strings->codigoProduto = strtok_r(linha, " ",&contexto);
	strings->preco = strtok_r(NULL, " ",&contexto);
	strings->quantidade = strtok_r(NULL, " ",&contexto);
	strings->tipoVenda = strtok_r(NULL, " ",&contexto);
	strings->codigoCliente = strtok_r(NULL, " ",&contexto);
	strings->mes = strtok_r(NULL, " ",&contexto);
	strings->filial = strtok_r(NULL, " ",&contexto);

	if(strtok_r(NULL, " ",&contexto) == NULL) return 0;
	else return -1;
}

static int inserirLinhaVenda(VENDA structVenda, StringVenda strings){
	structVenda->codigoProduto = (char*) strings.codigoProduto;
	structVenda->preco = (double) strtod(strings.preco, NULL); //guardar como inteiro em centimos
	//structVenda->preco = (double) atof(strings.preco); //guardar como inteiro em centimos
	structVenda->quantidade = (int) strtol(strings.quantidade, (char **)NULL, 10);
	structVenda->tipoVenda = (char) *(strings.tipoVenda) == 'P' ? 1 : 0;
	structVenda->codigoCliente = (char*) strings.codigoCliente;
	structVenda->mes = (int) strtol(strings.mes, (char **)NULL, 10) - 1;
	structVenda->filial = (int) strtol(strings.filial, (char **)NULL, 10);
	return 0;
}

static char imprimirTipoVenda(int t){
	return (t == VENDA_NORMAL ? 'N' : 'P');
}

static void imprimirLinhaVenda(struct linhaVenda structVenda){
	printf("produto: %s\n",structVenda.codigoProduto);
	printf("preco: %.2f\n",structVenda.preco);
	printf("quantidade: %d\n",structVenda.quantidade);
	printf("tipoVenda: %c\n",imprimirTipoVenda(structVenda.tipoVenda));
	printf("cliente: %s\n",structVenda.codigoCliente);
	printf("mes: %s\n",Mes[structVenda.mes]);
	printf("filial: %d\n",structVenda.filial);
}

////

int verificaCodigoProduto(Codigo produto){
	if(strlen(produto) != 6) return 0;
	if(isalpha(produto[0]) && isalpha(produto[1]))
		if(isdigit(produto[2]) && isdigit(produto[3]) && isdigit(produto[4]) && isdigit(produto[5]))
			return 1;
	return 0;
}

int verificaCodigoCliente(Codigo cliente){
	if(strlen(cliente) != 5) return 0;
	if(isalpha(cliente[0]))
		if(isdigit(cliente[1]) && isdigit(cliente[2]) && isdigit(cliente[3]) && isdigit(cliente[4]))
			return 1;
	return 0;
}

long verificaVenda(String venda){
	if(strlen(venda) > 50) return 0;
	long n = strtol(venda,NULL,10);
	return n;
}

int verificaQuantidade(int n){
	return(n >= 1 && n <= 200);
}

int verificaPreco(double n){
	return(n >= 0.0 && n <= 999.99);
}

int verificaFilial(int n){
	return(n >= 1 && n <= NRFILIAIS);
}

int verificaMes(int mes){
	return(mes >= 1 && mes <= 12);
}

int verificaTipoCompra(char c){
	return(c == 'N' || c == 'P');
}
