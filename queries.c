/*
 *  *  * Filipe Marques
 *  *  * Laboratórios Informática III, Universidade do Minho, 2016
 *  *  */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "venda.h"
#include "navegacao.h"
#include "catalogo_clientes.h"
#include "catalogo_produtos.h"
#include "gestao_filiais.h"
#include "faturacao.h"
#include "leitura_ficheiros.h"
#include "menu.h"
#include "queries.h"

static void query02_cli_imprimirCabecalho();
static void query02_cli_imprimirCabecalho_2(char letra, double tempo);
static void query02_pro_imprimirCabecalho();
static void query02_pro_imprimirCabecalho_2(char letra, double tempo);
static void query02_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal);
static void query02_funcaoImpressao(int pos, void *s);
static void query02_imprimirRodape();

static void query05_imprimirCabecalho();
static void query05_imprimirCabecalho_2(String codigo, double tempo);
static void query05_imprimirInfo(int nrFiliais);
static void query05_funcaoImpressao(void** dados, const String mes, int nrFiliais);
static void query05_imprimirRodape(int nrFiliais);

static void query09_imprimirCabecalho();
static void query09_imprimirCabecalho_2(String codigo, String mes, double tempo);
static void query09_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal);
static void query09_funcaoImpressao(int pos, void *produto);
static void query09_imprimirRodape();

static void query11_imprimirCabecalho();
static void query11_imprimirCabecalho_2(String codigo, double tempo);
static void query11_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal);
static void query11_funcaoImpressao(int pos, void *produto);
static void query11_imprimirRodape();

static void query03_imprimirCabecalho();
static void query03_imprimirCabecalho_2(String codigo, double tempo);
static void query03_filial_imprimirInfo(int nrFiliais, String mes);
static void query03_filial_funcaoImpressao(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, Mes_st m, int nrFiliais);
static void query03_filial_imprimirRodape(int nrFiliais);
static void query03_global_imprimirInfo(String mes);
static void query03_global_funcaoImpressao(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, Mes_st m);
static void query03_global_imprimirRodape();

static void query06_imprimirCabecalho();
static void query06_imprimirCabecalho_2(String mes1, String mes2);
static void query06_imprimirInfo();
static void query06_funcaoImpressao(String mes, long quantidade, double faturacao);
static void query06_imprimirRodape(long totaisQuantidade, double totaisFaturacao);

static void query12_funcaoImpressao(long clientes, long produtos, double tempo);

static void query10_global_imprimirCabecalho(double tempo);
static void query10_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal);
static void query10_funcaoImpressao(int pos, GestaoProduto produto);
static void query10_imprimirRodape();
static void query10_filial_imprimirCabecalho();
static void query10_filial_imprimirCabecalho_2(int filial, double tempo);

static void query04_funcaoImpressao(int pos, void *s);
static void query04_filial_imprimirCabecalho();
static void query04_filial_imprimirCabecalho_2(int filial, double tempo);
static void query04_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal);
static void query04_imprimirRodape();
static void query04_global_imprimirCabecalho(double tempo);

static void query07_imprimirCabecalho(double tempo);
static void query07_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal);
static void query07_funcaoImpressao(int pos, void *s);
static void query07_imprimirRodape();

static void query08_imprimirCabecalho();
static void query08_imprimirCabecalho_2(String codigo);
static void query08_filial_imprimirCabecalho_3(String codigo, int filial, double tempo);
static void query08_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal);
static void query08_funcaoImpressao(int pos, void** dados);
static void query08_imprimirRodape();

int query02_catalogoProdutos(CATALOGO_PRODUTOS catalogo){
	int leitura_scanf, estado = QUERY_02;
	char input[INPUT_BUFFER];
	String codigo;
	char opt, c;
	clock_t tini, tfin;
	PAGINA_RESULTADOS pagina;
	int nrResultados;

	query02_pro_imprimirCabecalho();
	printf("│ Inserir primeira letra > ");

	leitura_scanf = scanf("%s",input);
	c = input[0];

	if(leitura_scanf && !isalpha(input[1]) && !isdigit(input[1])){
		switch(c)
		{
			case '1':
					return estado = MENU_CATALOGOS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					estado = QUERY_02;
					break;
		}	
	}

	tini = clock();
	pagina = (PAGINA_RESULTADOS) travessiaProdutosPorLetra(catalogo, c);
	tfin = clock();

	nrResultados = getTamanhoLista(pagina);
	setNrElementosPorPagina(pagina, ELEM_POR_PAG);
	posicoesInit(pagina);

	while(estado == QUERY_02)
	{
		query02_pro_imprimirCabecalho_2(toupper(c), (double) (tfin - tini)/CLOCKS_PER_SEC);

		if(nrResultados == 0){
			printf("│ Não foram encontrados resultados para esta letra.\n");
			printf("│ Opção > ");

			scanf("%s",input);
			opt = input[0];

			switch(opt)
			{
				case '1':
						return estado = MENU_CATALOGOS;
				case '2':
						return estado = MENU_PRINCIPAL;
				case '3':
						return estado = SAIR;
				default:
						return estado = MENU_CATALOGOS;
			}	

		}

		query02_imprimirInfo(getPaginaAtual(pagina), getNrPaginasTotal(pagina) + 1, getPosicaoInicial(pagina), getPosicaoFinal(pagina), getIndiceAtual(pagina));

		codigo = (String) getElementoAtual(pagina);
		while(codigo != NULL){
			query02_funcaoImpressao(getPosicaoAtual(pagina), (void*) codigo);
			codigo = (String) getElementoAtual(pagina);
		}

		query02_imprimirRodape();

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					estado = MENU_CATALOGOS;
					break;
			case '2':
					estado = MENU_PRINCIPAL;
					break;
			case '3':
					estado = SAIR;
					break;
			default:
					estado = QUERY_02;
					break;
		}		

		if(estado == QUERY_02){
			switch(opt)
			{
				case '6':
						virarPagina(pagina);
						break;
				case '5':
						paginaParaTras(pagina);
						break;
				case '7':
						calcularUltimaPag(pagina);
						break;
				case '4':
						calcularPrimeiraPag(pagina);
						break;
				default:
						estado = MENU_CATALOGOS;
						break;
			}
		}
	}

	return estado;

}


int query02_catalogoClientes(CATALOGO_CLIENTES catalogo){
	int leitura_scanf, estado = QUERY_02;
	char input[INPUT_BUFFER];
	String codigo;
	char opt, c;
	clock_t tini, tfin;
	PAGINA_RESULTADOS pagina;
	int nrResultados;

	query02_cli_imprimirCabecalho();
	printf("│ Inserir primeira letra > ");

	leitura_scanf = scanf("%s",input);
	c = input[0];

	if(leitura_scanf && !isalpha(input[1]) && !isdigit(input[1])){
		switch(c)
		{
			case '1':
					return estado = MENU_CATALOGOS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					estado = QUERY_02;
					break;
		}	
	}

	tini = clock();
	pagina = (PAGINA_RESULTADOS) travessiaClientesPorLetra(catalogo, c);
	tfin = clock();

	nrResultados = getTamanhoLista(pagina);
	setNrElementosPorPagina(pagina, ELEM_POR_PAG);
	posicoesInit(pagina);

	while(estado == QUERY_02)
	{
		query02_cli_imprimirCabecalho_2(toupper(c), (double) (tfin - tini)/CLOCKS_PER_SEC);

		if(nrResultados == 0){
			printf("│ Não foram encontrados resultados para esta letra.\n");
			printf("│ Opção > ");

			scanf("%s",input);
			opt = input[0];

			switch(opt)
			{
				case '1':
						return estado = MENU_CATALOGOS;
				case '2':
						return estado = MENU_PRINCIPAL;
				case '3':
						return estado = SAIR;
				default:
						return estado = MENU_CATALOGOS;
			}	

		}

		query02_imprimirInfo(getPaginaAtual(pagina), getNrPaginasTotal(pagina) + 1, getPosicaoInicial(pagina), getPosicaoFinal(pagina), getIndiceAtual(pagina));

		codigo = (String) getElementoAtual(pagina);
		while(codigo != NULL){
			query02_funcaoImpressao(getPosicaoAtual(pagina), (void*) codigo);
			codigo = (String) getElementoAtual(pagina);
		}

		query02_imprimirRodape();

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					estado = MENU_CATALOGOS;
					break;
			case '2':
					estado = MENU_PRINCIPAL;
					break;
			case '3':
					estado = SAIR;
					break;
			default:
					estado = QUERY_02;
					break;
		}		

		if(estado == QUERY_02){
			switch(opt)
			{
				case '6':
						virarPagina(pagina);
						break;
				case '5':
						paginaParaTras(pagina);
						break;
				case '7':
						calcularUltimaPag(pagina);
						break;
				case '4':
						calcularPrimeiraPag(pagina);
						break;
				default:
						estado = MENU_CATALOGOS;
						break;
			}
		}
	}

	return estado;

}


int query05_nrProdutosCompradosPeloCliente(MODULO_GESTAO_FILIAIS modulo, CATALOGO_CLIENTES catalogo){
	int estado = QUERY_05;
	char input[INPUT_BUFFER];
	void** dados;
	int leitura_scanf;
	char opt, c;
	char codigo[INPUT_BUFFER];

	query05_imprimirCabecalho();
	printf("│ Inserir código do cliente > ");
	leitura_scanf = scanf("%s",input);
	c = input[0];

	if(leitura_scanf > 0 && !isalpha(input[1]) && !isdigit(input[1])){
		switch(c)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					estado = QUERY_05;
					break;
		}	
	}

	if(!verificaCodigoCliente( input )  ){
		printf("│ Código inválido.\n");
		printf("│ Opção > ");	

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					return estado = MENU_GESTAO_FILIAIS;
		}
	}

	if(existeClienteCatalogo(catalogo, input) == false){
		printf("│ Cliente não existe.\n");
		printf("│ Opção > ");	

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					return estado = MENU_GESTAO_FILIAIS;
		}
	}

	if(estado == QUERY_05){
		clock_t tini, tfin;
		PAGINA_RESULTADOS pagina;

		tini = clock();
		pagina = (PAGINA_RESULTADOS) quantidadeProdutosClienteCompraPorMes(modulo, input);
		tfin = clock();

		setNrElementosPorPagina(pagina, 12);
		posicoesInit(pagina);

		strncpy(codigo,input,7);
		while(estado == QUERY_05)
		{
			int i;
			query05_imprimirCabecalho_2(codigo, (double) (tfin - tini)/CLOCKS_PER_SEC);
			query05_imprimirInfo(NRFILIAIS);
			/*estadoPaginacao(pagina);*/
			dados = getMultiplosElementos(pagina);

			i = 0;
			while(dados != NULL){
				query05_funcaoImpressao(dados, Mes[i], NRFILIAIS);
				/*estadoPaginacao(pagina);*/
				dados = getMultiplosElementos(pagina);
				i++;
			}

			query05_imprimirRodape(NRFILIAIS);

			scanf("%s",input);
			opt = input[0];

			switch(opt)
			{
				case '1':
						estado = MENU_GESTAO_FILIAIS;
						break;
				case '2':
						estado = MENU_PRINCIPAL;
						break;
				case '3':
						estado = SAIR;
						break;
				default:
						estado = MENU_GESTAO_FILIAIS;
						break;
			}		
		}
	}
	
	return estado;

}


int query09_listaProdutosClienteMaisComprou(MODULO_GESTAO_FILIAIS modulo, CATALOGO_CLIENTES catalogo){
	int estado = QUERY_09;
	char input[INPUT_BUFFER];
	RegistoProduto produto;
	int leitura_scanf, mes, try;
	char opt, c;
	PAGINA_RESULTADOS pagina;
	char codigo[INPUT_BUFFER];

	query09_imprimirCabecalho();
	printf("│ Inserir código do cliente > ");

	
	leitura_scanf = scanf("%s",input);
	c = input[0];

	if(leitura_scanf > 0 && !isalpha(input[1]) && !isdigit(input[1])){
		switch(c)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					estado = QUERY_05;
					break;
		}	
	}

	if(estado == QUERY_09 && !verificaCodigoCliente( input )){
		printf("│ Código inválido.\n");
		printf("│ Opção > ");	

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					return estado = MENU_GESTAO_FILIAIS;
		}
	}

	if(estado == QUERY_09 && existeClienteCatalogo(catalogo, input) == false){
		printf("│ Cliente não existe.\n");
		printf("│ Opção > ");	

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					return estado = MENU_GESTAO_FILIAIS;
		}
	}

	mes = 0, try = 4;
	while(estado == QUERY_09 && !verificaMes(mes) && --try){		
		printf("│ Inserir Mês (número 1-12) > ");
		scanf(" %d",&mes); 
		if(!verificaMes(mes)) printf("│ Mês inválido. ");
	}

	if(try == 0) 
		return estado = MENU_GESTAO_FILIAIS;

	if(estado == QUERY_09){
		clock_t tini, tfin;

		mes--;
		tini = clock();
		pagina = (PAGINA_RESULTADOS) listaProdutosClienteMaisComprou(modulo, input, mes);
		tfin = clock();

		setNrElementosPorPagina(pagina, ELEM_POR_PAG);

		posicoesInit(pagina);

		strncpy(codigo,input,7);
		while(estado == QUERY_09)
		{
			query09_imprimirCabecalho_2(codigo, Mes[mes], (double) (tfin - tini)/CLOCKS_PER_SEC);
			query09_imprimirInfo(getPaginaAtual(pagina), getNrPaginasTotal(pagina) + 1, getPosicaoInicial(pagina), getPosicaoFinal(pagina), getIndiceAtual(pagina));

			produto = (RegistoProduto) getElementoAtual(pagina);
			while(produto != NULL){
				query09_funcaoImpressao(getPosicaoAtual(pagina), (void*) produto);
				produto = (RegistoProduto) getElementoAtual(pagina);
			}

			query09_imprimirRodape();

			scanf("%s",input);
			opt = input[0];

			switch(opt)
			{
				case '1':
						estado = MENU_GESTAO_FILIAIS;
						break;
				case '2':
						estado = MENU_PRINCIPAL;
						break;
				case '3':
						estado = SAIR;
						break;
				default:
						estado = QUERY_09;
						break;
			}		

			if(estado == QUERY_09){
				switch(opt)
				{
					case '6':
							virarPagina(pagina);
							break;
					case '5':
							paginaParaTras(pagina);
							break;
					case '7':
							calcularUltimaPag(pagina);
							break;
					case '4':
							calcularPrimeiraPag(pagina);
							break;
					default:
							estado = MENU_GESTAO_FILIAIS;
							break;
				}
			}
		}
}

	return estado;

}


int query11_topProdutosClienteGastouDinheiro(MODULO_GESTAO_FILIAIS modulo, CATALOGO_CLIENTES catalogo){
	int estado = QUERY_11;
	char input[INPUT_BUFFER];
	RegistoProduto produto;
	PAGINA_RESULTADOS pagina;
	int leitura_scanf;
	char opt, c;
	String codigo[INPUT_BUFFER];

	query11_imprimirCabecalho();
	printf("│ Inserir código do cliente > ");
	leitura_scanf = scanf("%s",input);
	c = input[0];

	if(leitura_scanf > 0 && !isalpha(input[1]) && !isdigit(input[1])){
		switch(c)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					estado = QUERY_11;
					break;
		}	
	}

	if(estado == QUERY_11 && !verificaCodigoCliente( input )){
		printf("│ Código inválido.\n");
		printf("│ Opção > ");	

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					return estado = MENU_GESTAO_FILIAIS;
		}
	}

	if(estado == QUERY_11 && existeClienteCatalogo(catalogo, input) == false){
		printf("│ Cliente não existe.\n");
		printf("│ Opção > ");	

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					return estado = MENU_GESTAO_FILIAIS;
		}
	}


	if(estado == QUERY_11){
		clock_t tini, tfin;
		tini = clock();
		pagina = (PAGINA_RESULTADOS) topProdutosClienteGastouDinheiro(modulo, input);
		tfin = clock();

		setNrElementosPorPagina(pagina, ELEM_POR_PAG);
		posicoesInit(pagina);

		strncpy((String) codigo,input,7);
		while(estado == QUERY_11)
		{
			query11_imprimirCabecalho_2((String) codigo, (double) (tfin - tini)/CLOCKS_PER_SEC);
			query11_imprimirInfo(getPaginaAtual(pagina), getNrPaginasTotal(pagina) + 1, getPosicaoInicial(pagina), getPosicaoFinal(pagina), getIndiceAtual(pagina));

			produto = (RegistoProduto) getElementoAtual(pagina);
			while(produto != NULL){
				query11_funcaoImpressao(getPosicaoAtual(pagina), (void*) produto);
				produto = (RegistoProduto) getElementoAtual(pagina);
			}

			query11_imprimirRodape();

			scanf("%s",input);
			opt = input[0];

			switch(opt)
			{
				case '1':
						estado = MENU_GESTAO_FILIAIS;
						break;
				case '2':
						estado = MENU_PRINCIPAL;
						break;
				case '3':
						estado = SAIR;
						break;
				default:
						estado = QUERY_11;
						break;
			}		

			if(estado == QUERY_11){
				switch(opt)
				{
					case '6':
							virarPagina(pagina);
							break;
					case '5':
							paginaParaTras(pagina);
							break;
					case '7':
							calcularUltimaPag(pagina);
							break;
					case '4':
							calcularPrimeiraPag(pagina);
							break;
					default:
							estado = MENU_GESTAO_FILIAIS;
							break;
				}
			}
		}
	}

	return estado;

}

int query03_quantidadeFaturacaoProduto_filial(MODULO_GESTAO_FILIAIS modulo, CATALOGO_PRODUTOS catalogo){
	int estado = QUERY_03;
	char input[INPUT_BUFFER];
	int leitura_scanf, mes, try;
	char opt, c;
	char codigo[INPUT_BUFFER];

	query03_imprimirCabecalho();
	printf("│ Inserir código do produto > ");

	
	leitura_scanf = scanf("%s",input);
	c = input[0];

	if(leitura_scanf > 0 && !isalpha(input[1]) && !isdigit(input[1])){
		switch(c)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					estado = QUERY_03;
					break;
		}	
	}

	if(estado == QUERY_03 && !verificaCodigoProduto( input )){
		printf("│ Código inválido.\n");
		printf("│ Opção > ");	

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					return estado = MENU_GESTAO_FILIAIS;
		}
	}

	if(estado == QUERY_03 && existeProdutoCatalogo(catalogo, input) == false){
		printf("│ Produto não existe.\n");
		printf("│ Opção > ");	

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					return estado = MENU_GESTAO_FILIAIS;
		}
	}

	mes = 0, try = 4;
	while(estado == QUERY_03 && !verificaMes(mes) && --try){		
		printf("│ Inserir Mês (número 1-12) > ");
		scanf(" %d",&mes); 
		if(!verificaMes(mes)) printf("│ Mês inválido. ");
	}

	if(try == 0) 
		return estado = MENU_GESTAO_FILIAIS;

	if(estado == QUERY_03){
		clock_t tini, tfin;

		mes--;
		tini = clock();
		tfin = clock();

		strncpy(codigo,input,7);
		while(estado == QUERY_03)
		{
			query03_imprimirCabecalho_2(codigo, 0.0001);
			query03_filial_imprimirInfo(NRFILIAIS, Mes[mes]);
			query03_filial_funcaoImpressao(modulo, codigo, mes, NRFILIAIS);
			query03_filial_imprimirRodape(NRFILIAIS);

			scanf("%s",input);
			opt = input[0];

			switch(opt)
			{
				case '1':
						estado = MENU_GESTAO_FILIAIS;
						break;
				case '2':
						estado = MENU_PRINCIPAL;
						break;
				case '3':
						estado = SAIR;
						break;
				default:
						estado = MENU_GESTAO_FILIAIS;
						break;
			}		
		}
}

	return estado;

}

int query03_quantidadeFaturacaoProduto_global(MODULO_GESTAO_FILIAIS modulo, CATALOGO_PRODUTOS catalogo){
	int estado = QUERY_03;
	char input[INPUT_BUFFER];
	int leitura_scanf, mes, try;
	char opt, c;
	char codigo[INPUT_BUFFER];

	query03_imprimirCabecalho();
	printf("│ Inserir código do produto > ");

	
	leitura_scanf = scanf("%s",input);
	c = input[0];

	if(leitura_scanf > 0 && !isalpha(input[1]) && !isdigit(input[1])){
		switch(c)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					estado = QUERY_03;
					break;
		}	
	}

	if(estado == QUERY_03 && !verificaCodigoProduto( input )){
		printf("│ Código inválido.\n");
		printf("│ Opção > ");	

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					return estado = MENU_GESTAO_FILIAIS;
		}
	}

	if(estado == QUERY_03 && existeProdutoCatalogo(catalogo, input) == false){
		printf("│ Produto não existe.\n");
		printf("│ Opção > ");	

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					return estado = MENU_GESTAO_FILIAIS;
		}
	}

	mes = 0, try = 4;
	while(estado == QUERY_03 && !verificaMes(mes) && --try){		
		printf("│ Inserir Mês (número 1-12) > ");
		scanf(" %d",&mes); 
		if(!verificaMes(mes)) printf("│ Mês inválido. ");
	}

	if(try == 0) 
		return estado = MENU_GESTAO_FILIAIS;

	if(estado == QUERY_03){
		clock_t tini, tfin;

		mes--;
		tini = clock();
		tfin = clock();

		strncpy(codigo,input,7);
		while(estado == QUERY_03)
		{
			query03_imprimirCabecalho_2(codigo, 0.0001);
			query03_global_imprimirInfo(Mes[mes]);
			query03_global_funcaoImpressao(modulo, codigo, mes);
			query03_global_imprimirRodape();

			scanf("%s",input);
			opt = input[0];

			switch(opt)
			{
				case '1':
						estado = MENU_GESTAO_FILIAIS;
						break;
				case '2':
						estado = MENU_PRINCIPAL;
						break;
				case '3':
						estado = SAIR;
						break;
				default:
						estado = MENU_GESTAO_FILIAIS;
						break;
			}		
		}
}

	return estado;

}

int query06_faturacaoIntervaloMeses(MODULO_FATURACAO modulo){
	int estado = QUERY_06;
	char input[INPUT_BUFFER];
	int leitura_scanf, mes1, mes2, mes3;
	char opt;

	query06_imprimirCabecalho();
	printf("│ Inserir intervalo meses (no formato xx-yy) > ");
	leitura_scanf = scanf("%d-%d", &mes1, &mes2);

	if(leitura_scanf > 0 && !isdigit(mes1) && !isdigit(mes2) && verificaMes(mes1) && verificaMes(mes2)){
		if(mes1 > mes2){
			mes3 = mes1;
			mes1 = mes2;
			mes2 = mes3;
		}

		mes1--;
		mes2--;

		estado = QUERY_06;

	}else{
		printf("│ Meses inválidos.\n");
		printf("│ Opção > ");
		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					return estado = MENU_FATURACAO;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					return estado = MENU_FATURACAO;
		}		
	}

	if(estado == QUERY_06){

		while(estado == QUERY_06)
		{
			int i;
			query06_imprimirCabecalho_2(Mes[mes1], Mes[mes2]);
			query06_imprimirInfo();

			i = mes1;
			while(i <= mes2){
				query06_funcaoImpressao(Mes[i], consultaQuantidadesMes(modulo,i), consultaFaturacaoMes(modulo, i));
				i++;
			}

			query06_imprimirRodape(consultaQuantidadesIntervaloMeses(modulo,mes1,mes2), consultaFaturacaoIntervaloMeses(modulo,mes1,mes2));

			scanf("%s",input);
			opt = input[0];

			switch(opt)
			{
				case '1':
						estado = MENU_FATURACAO;
						break;
				case '2':
						estado = MENU_PRINCIPAL;
						break;
				case '3':
						estado = SAIR;
						break;
				default:
						estado = MENU_FATURACAO;
						break;
			}		
		}
	}
	
	return estado;

}

int query12_NrClientesSemCompras_NrProdutosNinguemComprou(MODULO_GESTAO_FILIAIS moduloGestao, CATALOGO_CLIENTES catalogoClientes, CATALOGO_PRODUTOS catalogoProdutos){
	int estado = QUERY_12;
	char input[INPUT_BUFFER];
	PAGINA_RESULTADOS pagina_clientes, pagina_produtos;
	char opt;
	long nrClientes, nrProdutos;

	if(estado == QUERY_12){

		clock_t tini, tfin;
		tini = clock();
		pagina_clientes = clientesNaoCompraram_global(moduloGestao, catalogoClientes);
		pagina_produtos = produtosNinguemComprou_global(moduloGestao, catalogoProdutos);
		nrClientes = getIndiceAtual(pagina_clientes);
		nrProdutos =  getIndiceAtual(pagina_produtos);
		tfin = clock();	

		query12_funcaoImpressao(nrClientes , nrProdutos, (double) (tfin - tini)/CLOCKS_PER_SEC);

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					estado = MENU_GESTAO_FILIAIS;
					break;
			case '2':
					estado = MENU_PRINCIPAL;
					break;
			case '3':
					estado = SAIR;
					break;
			default:
					estado = MENU_GESTAO_FILIAIS;
					break;
		}

	}
	return estado;
}

int query10_topProdutosMaisComprados_global(MODULO_GESTAO_FILIAIS modulo, CATALOGO_PRODUTOS catalogo){
	int estado = QUERY_10;
	char input[INPUT_BUFFER];
	GestaoProduto produto;
	PAGINA_RESULTADOS pagina;
	char opt;
	String codigo[INPUT_BUFFER];

	query11_imprimirCabecalho();


	if(estado == QUERY_10){
		clock_t tini, tfin;
		tini = clock();
		pagina = produtosMaisVendidos_global( modulo, calcularTotalProdutos(catalogo));
		tfin = clock();

		setNrElementosPorPagina(pagina, ELEM_POR_PAG);
		posicoesInit(pagina);

		strncpy((String) codigo,input,7);
		while(estado == QUERY_10)
		{
			query10_global_imprimirCabecalho((double) (tfin - tini)/CLOCKS_PER_SEC);
			query10_imprimirInfo(getPaginaAtual(pagina), getNrPaginasTotal(pagina) + 1, getPosicaoInicial(pagina), getPosicaoFinal(pagina), getIndiceAtual(pagina));

			produto = (GestaoProduto) getElementoAtual(pagina);
			while(produto != NULL){
				query10_funcaoImpressao(getPosicaoAtual(pagina), produto);
				produto = (GestaoProduto) getElementoAtual(pagina);
			}

			query10_imprimirRodape();

			scanf("%s",input);
			opt = input[0];

			switch(opt)
			{
				case '1':
						estado = MENU_GESTAO_FILIAIS;
						break;
				case '2':
						estado = MENU_PRINCIPAL;
						break;
				case '3':
						estado = SAIR;
						break;
				default:
						estado = QUERY_10;
						break;
			}		

			if(estado == QUERY_10){
				switch(opt)
				{
					case '6':
							virarPagina(pagina);
							break;
					case '5':
							paginaParaTras(pagina);
							break;
					case '7':
							calcularUltimaPag(pagina);
							break;
					case '4':
							calcularPrimeiraPag(pagina);
							break;
					default:
							estado = MENU_GESTAO_FILIAIS;
							break;
				}
			}
		}
	}

	return estado;

}

int query10_topProdutosMaisComprados_filial(MODULO_GESTAO_FILIAIS modulo, CATALOGO_PRODUTOS catalogo){
	int f, leitura_scanf, estado = QUERY_10;
	char input[INPUT_BUFFER];
	GestaoProduto produto;
	PAGINA_RESULTADOS pagina;
	char opt;
	String codigo[INPUT_BUFFER];

	query10_filial_imprimirCabecalho();
	printf("│ Inserir nº filial (1-%d) > ",NRFILIAIS);

	leitura_scanf = scanf(" %d",&f);

	if(estado == QUERY_10 && !isdigit(f) && !verificaFilial(f)){
		printf("│ Filial inválida.\n");
		printf("│ Opção > ");	

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					return estado = MENU_GESTAO_FILIAIS;
		}
	}

	if(estado == QUERY_10){
		clock_t tini, tfin;
		tini = clock();
		pagina = produtosMaisVendidos_filial( modulo, f);
		tfin = clock();

		setNrElementosPorPagina(pagina, ELEM_POR_PAG);
		posicoesInit(pagina);

		strncpy((String) codigo,input,7);
		while(estado == QUERY_10)
		{
			query10_filial_imprimirCabecalho_2(f, (double) (tfin - tini)/CLOCKS_PER_SEC);
			query10_imprimirInfo(getPaginaAtual(pagina), getNrPaginasTotal(pagina) + 1, getPosicaoInicial(pagina), getPosicaoFinal(pagina), getIndiceAtual(pagina));

			produto = (GestaoProduto) getElementoAtual(pagina);
			while(produto != NULL){
				query10_funcaoImpressao(getPosicaoAtual(pagina), produto);
				produto = (GestaoProduto) getElementoAtual(pagina);
			}

			query10_imprimirRodape();

			scanf("%s",input);
			opt = input[0];

			switch(opt)
			{
				case '1':
						estado = MENU_GESTAO_FILIAIS;
						break;
				case '2':
						estado = MENU_PRINCIPAL;
						break;
				case '3':
						estado = SAIR;
						break;
				default:
						estado = QUERY_10;
						break;
			}		

			if(estado == QUERY_10){
				switch(opt)
				{
					case '6':
							virarPagina(pagina);
							break;
					case '5':
							paginaParaTras(pagina);
							break;
					case '7':
							calcularUltimaPag(pagina);
							break;
					case '4':
							calcularPrimeiraPag(pagina);
							break;
					default:
							estado = MENU_GESTAO_FILIAIS;
							break;
				}
			}
		}
	}

	return estado;

}

int query04_produtosNinguemComprou_filial(MODULO_GESTAO_FILIAIS modulo, CATALOGO_PRODUTOS catalogo){
	int f, leitura_scanf, estado = QUERY_04;
	char input[INPUT_BUFFER];
	String codigo;
	char opt;
	clock_t tini, tfin;
	PAGINA_RESULTADOS pagina;

	query04_filial_imprimirCabecalho();
	printf("│ Inserir nº filial (1-%d) > ",NRFILIAIS);

	leitura_scanf = scanf(" %d",&f);

	if(estado == QUERY_04 && !isdigit(f) && !verificaFilial(f)){
		printf("│ Filial inválida.\n");
		printf("│ Opção > ");	

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					return estado = MENU_GESTAO_FILIAIS;
		}
	}

	tini = clock();
	pagina = produtosNinguemComprou_filial(modulo, catalogo, f);
	tfin = clock();

	setNrElementosPorPagina(pagina, ELEM_POR_PAG);
	posicoesInit(pagina);

	while(estado == QUERY_04)
	{
		query04_filial_imprimirCabecalho_2(f, (double) (tfin - tini)/CLOCKS_PER_SEC);

		query04_imprimirInfo(getPaginaAtual(pagina), getNrPaginasTotal(pagina) + 1, getPosicaoInicial(pagina), getPosicaoFinal(pagina), getIndiceAtual(pagina));

		codigo = (String) getElementoAtual(pagina);
		while(codigo != NULL){
			query04_funcaoImpressao(getPosicaoAtual(pagina), (void*) codigo);
			codigo = (String) getElementoAtual(pagina);
		}

		query04_imprimirRodape();

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					estado = MENU_GESTAO_FILIAIS;;
					break;
			case '2':
					estado = MENU_PRINCIPAL;
					break;
			case '3':
					estado = SAIR;
					break;
			default:
					estado = QUERY_04;
					break;
		}		

		if(estado == QUERY_04){
			switch(opt)
			{
				case '6':
						virarPagina(pagina);
						break;
				case '5':
						paginaParaTras(pagina);
						break;
				case '7':
						calcularUltimaPag(pagina);
						break;
				case '4':
						calcularPrimeiraPag(pagina);
						break;
				default:
						estado = MENU_GESTAO_FILIAIS;;
						break;
			}
		}
	}

	return estado;

}

int query04_produtosNinguemComprou_global(MODULO_GESTAO_FILIAIS modulo, CATALOGO_PRODUTOS catalogo){
	int estado = QUERY_04;
	char input[INPUT_BUFFER];
	String codigo;
	char opt;
	clock_t tini, tfin;
	PAGINA_RESULTADOS pagina;


	tini = clock();
	pagina = produtosNinguemComprou_global(modulo, catalogo);
	tfin = clock();

	setNrElementosPorPagina(pagina, ELEM_POR_PAG);
	posicoesInit(pagina);

	while(estado == QUERY_04)
	{
		query04_global_imprimirCabecalho((double) (tfin - tini)/CLOCKS_PER_SEC);

		query04_imprimirInfo(getPaginaAtual(pagina), getNrPaginasTotal(pagina) + 1, getPosicaoInicial(pagina), getPosicaoFinal(pagina), getIndiceAtual(pagina));

		codigo = (String) getElementoAtual(pagina);
		while(codigo != NULL){
			query04_funcaoImpressao(getPosicaoAtual(pagina), (void*) codigo);
			codigo = (String) getElementoAtual(pagina);
		}

		query04_imprimirRodape();

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					estado = MENU_GESTAO_FILIAIS;;
					break;
			case '2':
					estado = MENU_PRINCIPAL;
					break;
			case '3':
					estado = SAIR;
					break;
			default:
					estado = QUERY_04;
					break;
		}		

		if(estado == QUERY_04){
			switch(opt)
			{
				case '6':
						virarPagina(pagina);
						break;
				case '5':
						paginaParaTras(pagina);
						break;
				case '7':
						calcularUltimaPag(pagina);
						break;
				case '4':
						calcularPrimeiraPag(pagina);
						break;
				default:
						estado = MENU_GESTAO_FILIAIS;;
						break;
			}
		}
	}

	return estado;

}

int query07_clientesCompraramTodasAsFiliais(MODULO_GESTAO_FILIAIS modulo){
	int estado = QUERY_07;
	char input[INPUT_BUFFER];
	String codigo;
	char opt;
	clock_t tini, tfin;
	PAGINA_RESULTADOS pagina;


	tini = clock();
	pagina = clientesCompraramTodasFiliais(modulo);
	tfin = clock();

	setNrElementosPorPagina(pagina, ELEM_POR_PAG);
	posicoesInit(pagina);

	while(estado == QUERY_07)
	{
		query07_imprimirCabecalho((double) (tfin - tini)/CLOCKS_PER_SEC);

		query07_imprimirInfo(getPaginaAtual(pagina), getNrPaginasTotal(pagina) + 1, getPosicaoInicial(pagina), getPosicaoFinal(pagina), getIndiceAtual(pagina));

		codigo = (String) getElementoAtual(pagina);
		while(codigo != NULL){
			query07_funcaoImpressao(getPosicaoAtual(pagina), (void*) codigo);
			codigo = (String) getElementoAtual(pagina);
		}

		query07_imprimirRodape();

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					estado = MENU_GESTAO_FILIAIS;;
					break;
			case '2':
					estado = MENU_PRINCIPAL;
					break;
			case '3':
					estado = SAIR;
					break;
			default:
					estado = QUERY_07;
					break;
		}		

		if(estado == QUERY_07){
			switch(opt)
			{
				case '6':
						virarPagina(pagina);
						break;
				case '5':
						paginaParaTras(pagina);
						break;
				case '7':
						calcularUltimaPag(pagina);
						break;
				case '4':
						calcularPrimeiraPag(pagina);
						break;
				default:
						estado = MENU_GESTAO_FILIAIS;;
						break;
			}
		}
	}

	return estado;

}

int query08_listaClientesCompraramProduto_filial(MODULO_GESTAO_FILIAIS modulo, CATALOGO_PRODUTOS catalogo){
	int f, leitura_scanf, estado = QUERY_08;
	char input[INPUT_BUFFER];
	PAGINA_RESULTADOS pagina;
	char opt,c;
	String codigo[INPUT_BUFFER];
	void** dados;


	query08_imprimirCabecalho();

	printf("│ Inserir código do produto > ");

	leitura_scanf = scanf("%s",input);
	c = input[0];

	if(leitura_scanf > 0 && !isalpha(input[1]) && !isdigit(input[1])){
		switch(c)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					estado = QUERY_08;
					break;
		}	
	}

	if(estado == QUERY_08 && !verificaCodigoProduto( input )){
		printf("│ Código inválido.\n");
		printf("│ Opção > ");	

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					return estado = MENU_GESTAO_FILIAIS;
		}
	}

	if(estado == QUERY_08 && existeProdutoCatalogo(catalogo, input) == false){
		printf("│ Produto não existe.\n");
		printf("│ Opção > ");	

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					return estado = MENU_GESTAO_FILIAIS;
		}
	}

	strncpy((String) codigo,input,7);

	query08_imprimirCabecalho_2((String) codigo);

	printf("│ Inserir nº filial (1-%d) > ",NRFILIAIS);

	leitura_scanf = scanf(" %d",&f);

	if(estado == QUERY_08 && !isdigit(f) && !verificaFilial(f)){
		printf("│ Filial inválida.\n");
		printf("│ Opção > ");	

		scanf("%s",input);
		opt = input[0];

		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
			case '2':
					return estado = MENU_PRINCIPAL;
			case '3':
					return estado = SAIR;
			default:
					return estado = MENU_GESTAO_FILIAIS;
		}
	}

	if(estado == QUERY_08){
		clock_t tini, tfin;
		tini = clock();
		pagina = listaClientesCompraramProduto(modulo, (String) codigo, f);
		tfin = clock();

		setNrElementosPorPagina(pagina, ELEM_POR_PAG);
		posicoesInit(pagina);

		while(estado == QUERY_08)
		{
			query08_filial_imprimirCabecalho_3((String) codigo, f, (double) (tfin - tini)/CLOCKS_PER_SEC);

			query08_imprimirInfo(getPaginaAtual(pagina), getNrPaginasTotal(pagina) + 1, getPosicaoInicial(pagina), getPosicaoFinal(pagina), getIndiceAtual(pagina) / getNrCampos(pagina));

			/*estadoPaginacao(pagina);*/
			dados = getMultiplosElementos(pagina);
			while(dados != NULL){
				query08_funcaoImpressao(getPosicaoAtual(pagina) / getNrCampos(pagina), dados);
				/*estadoPaginacao(pagina);*/
				dados = getMultiplosElementos(pagina);
			}

			query08_imprimirRodape();

			scanf("%s",input);
			opt = input[0];

			switch(opt)
			{
				case '1':
						estado = MENU_GESTAO_FILIAIS;
						break;
				case '2':
						estado = MENU_PRINCIPAL;
						break;
				case '3':
						estado = SAIR;
						break;
				default:
						estado = QUERY_08;
						break;
			}		

			if(estado == QUERY_08){
				switch(opt)
				{
					case '6':
							virarPagina(pagina);
							break;
					case '5':
							paginaParaTras(pagina);
							break;
					case '7':
							calcularUltimaPag(pagina);
							break;
					case '4':
							calcularPrimeiraPag(pagina);
							break;
					default:
							estado = MENU_GESTAO_FILIAIS;
							break;
				}
			}
		}
	}

	return estado;

}

/* QUERY 2 */

static void query02_funcaoImpressao(int pos, void *s){
	printf("│ │ %5d │ %8s │                                   │\n", pos, (char *) s);
}

static void query02_pro_imprimirCabecalho(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> CATÁLOGOS >> QUERY 2 │░░░░░░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Listar produtos começados pela letra...                │\n");
	printf("│ ╭───────────────────────────────────────────────╮      │\n");
	printf("│ │ [1] CATÁLOGOS | [2] Menu Principal | [3] Sair │      │\n");
	printf("│ ╰───────────────────────────────────────────────╯      │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query02_pro_imprimirCabecalho_2(char letra, double tempo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> CATÁLOGOS >> QUERY 2 │░░░░░░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Mostrando produtos começados por %c (em %.5f seg.)   │\n",letra,tempo);
	printf("│ ╭───────────────────────────────────────────────╮      │\n");
	printf("│ │ [1] CATÁLOGOS | [2] Menu Principal | [3] Sair │      │\n");
	printf("│ ╰───────────────────────────────────────────────╯      │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query02_cli_imprimirCabecalho(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> CATÁLOGOS >> QUERY 2 │░░░░░░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Listar clientes começados pela letra...                │\n");
	printf("│ ╭───────────────────────────────────────────────╮      │\n");
	printf("│ │ [1] CATÁLOGOS | [2] Menu Principal | [3] Sair │      │\n");
	printf("│ ╰───────────────────────────────────────────────╯      │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query02_cli_imprimirCabecalho_2(char letra, double tempo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> CATÁLOGOS >> QUERY 2 │░░░░░░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Mostrando clientes começados por %c (em %.5f seg.)   │\n",letra,tempo);
	printf("│ ╭───────────────────────────────────────────────╮      │\n");
	printf("│ │ [1] CATÁLOGOS | [2] Menu Principal | [3] Sair │      │\n");
	printf("│ ╰───────────────────────────────────────────────╯      │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}



static void query02_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal){
	printf("│ Pág. %05d/%05d # %06d-%06d de %06d resultados  │\n", paginaAtual+1, paginasTotais-1, posIni+1, posFin+1, nrResultadosTotal);
	printf("│ ┌───────┬──────────┐                                   │\n");
	printf("│ │     # │  Código  │                                   │\n");
}


static void query02_imprimirRodape(){
	printf("│ ╰───────┴──────────╯                                   │\n");
	printf("│<<[4]   <[5]  < # >  [6]>   [7]>> || Opção > ");
}

/* QUERY 5 */

static void query05_imprimirCabecalho(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 5 │░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Nº produtos comprados pelo cliente...                  │\n");
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query05_imprimirCabecalho_2(String codigo, double tempo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 5 │░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Nº produtos comprados pelo cliente %7s (%.4f seg)│\n",codigo,tempo);
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query05_imprimirInfo(int nrFiliais){
	int f;
	printf("│    MÊS    │");

	f = 1;
	while(f <= nrFiliais){
		printf(" Filial %d │",f);
		f++;
	}

	printf("           │\n");
}

static void query05_funcaoImpressao(void** dados, const String mes, int nrFiliais){
	int f;
	printf("│ %9s │", mes);

	f = 0;
	while(f < nrFiliais){
		printf(" %8ld │",*(long*)dados[f]);
		f++;
	}

	printf("           │\n");
}

static void query05_imprimirRodape(int nrFiliais){
	int f;
	printf("│───────────╯");

	f = 1;
	while(f <= nrFiliais){
		printf("──────────╯");
		f++;
	}

	printf("           │\n");
	printf("│ Opção > ");
}

/* QUERY 9 */

static void query09_imprimirCabecalho(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 9 │░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Produtos mais comprados pelo cliente...                │\n");
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query09_imprimirCabecalho_2(String codigo, String mes, double tempo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 9 │░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Mais comprados pelo cliente %5s em %9s (%.4f)│\n",codigo,mes,tempo);
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query09_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal){
	printf("│ Pág. %05d/%05d # %06d-%06d de %06d resultados  │\n", paginaAtual+1, paginasTotais-1, posIni+1, posFin+1, nrResultadosTotal);
	printf("│ ┌───────┬──────────┬──────────┐                        │\n");
	printf("│ │     # │  Código  │Quantidade│                        │\n");
}

static void query09_funcaoImpressao(int pos, void *produto){
	printf("│ │ %5d │  %6s  │ %9d│                        │\n", pos, getCodProduto_registoProduto(produto), getQuantidade_registoProduto(produto));
}

static void query09_imprimirRodape(){
	printf("│ ╰───────┴──────────┴──────────╯                        │\n");
	printf("│<<[4]   <[5]  < # >  [6]>   [7]>> || Opção > ");
}

/* QUERY 11 */

static void query11_imprimirCabecalho(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 11 │░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Produtos com mais gastos pelo cliente...               │\n");
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query11_imprimirCabecalho_2(String codigo, double tempo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 11 │░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Ranking produtos onde %6s mais gastou (%.4f s)    │\n",codigo,tempo);
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query11_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal){
	printf("│ Pág. %05d/%05d # %06d-%06d de %06d resultados  │\n", paginaAtual+1, paginasTotais-1, posIni+1, posFin+1, nrResultadosTotal);
	printf("│ ┌───────┬──────────┬───────────────┐                   │\n");
	printf("│ │     # │  Código  │    Gastos     │                   │\n");
}

static void query11_funcaoImpressao(int pos, void *produto){
	printf("│ │ %5d │  %6s  │ %12.2f  │                   │\n", pos, getCodProduto_registoProduto(produto), getPreco_registoProduto(produto));
}

static void query11_imprimirRodape(){
	printf("│ ╰───────┴──────────┴───────────────╯                   │\n");
	printf("│<<[4]   <[5]  < # >  [6]>   [7]>> || Opção > ");
}

/* QUERY 03 */

static void query03_imprimirCabecalho(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 3 │░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Nº vendas e total faturado pelo produto num dado mês...│\n");
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query03_imprimirCabecalho_2(String codigo, double tempo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 3 │░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Nº vendas e total faturado de %7s (%.4f seg)     │\n",codigo,tempo);
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query03_filial_imprimirInfo(int nrFiliais, String mes){
	int f;
	printf("│ No mês de %9s │",mes);

	f = 1;
	while(f <= nrFiliais){
		printf(" Filial %d │",f);
		f++;
	}

	printf(" │\n");
}

static void query03_filial_funcaoImpressao(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, Mes_st m, int nrFiliais){
	int f;
	printf("│ Quantidade │ Normal:│");
	f = 1;
	while(f <= nrFiliais){
		printf("%10d│",calculaQuantidadeVendidaProduto_mes(modulo,codigo,f,VENDA_NORMAL,m)); 
		f++;
	}
	printf(" │\n");

	printf("│  Vendida   │ Promo: │");
	f = 1;
	while(f <= nrFiliais){
		printf("%10d│",calculaQuantidadeVendidaProduto_mes(modulo,codigo,f,VENDA_PROMOCIONAL,m)); 
		f++;
	}
	printf(" │\n");

	printf("├────────────┼────────");
	f = 1;
	while(f <= nrFiliais){
		printf("┼──────────");
		f++;
	}
	printf("┤");
	printf(" │\n");

	printf("│    Total   │ Normal:│");
	f = 1;
	while(f <= nrFiliais){
		printf("%10.2f│",calculaFaturacaoProduto_mes(modulo,codigo,f,VENDA_NORMAL,m)); 
		f++;
	}
	printf(" │\n");

	printf("│  Faturado  │ Promo: │");
	f = 1;
	while(f <= nrFiliais){
		printf("%10.2f│",calculaFaturacaoProduto_mes(modulo,codigo,f,VENDA_PROMOCIONAL,m)); 
		f++;
	}
	printf(" │\n");
}

static void query03_filial_imprimirRodape(int nrFiliais){
	int f;
	printf("│────────────╯────────╯");

	f = 1;
	while(f <= nrFiliais){
		printf("──────────╯");
		f++;
	}

	printf(" │\n");
	printf("│ Opção > ");
}

static void query03_global_imprimirInfo(String mes){
	printf("│ No mês de %9s │ Resultados Globais │             │\n",mes);
}

static void query03_global_funcaoImpressao(MODULO_GESTAO_FILIAIS modulo, CodigoProduto_st codigo, Mes_st m){
	printf("│ Quantidade │ Normal:│ %18d │             │\n",calculaQuantidadeVendidaProduto_global_mes(modulo,codigo,VENDA_NORMAL,m)); 

	printf("│  Vendida   │ Promo: │ %18d │             │\n",calculaQuantidadeVendidaProduto_global_mes(modulo,codigo,VENDA_PROMOCIONAL,m));

	printf("├────────────┼────────┼────────────────────┤             │\n");

	printf("│    Total   │ Normal:│ %18.2f │             │\n",calculaFaturacaoProduto_global_mes(modulo,codigo,VENDA_NORMAL,m));

	printf("│  Faturado  │ Promo: │ %18.2f │             │\n",calculaFaturacaoProduto_global_mes(modulo,codigo,VENDA_PROMOCIONAL,m));

}

static void query03_global_imprimirRodape(){
	printf("│────────────╯────────╯────────────────────╯             │\n");
	printf("│ Opção > ");
}

/* QUERY 06 */

static void query06_imprimirCabecalho(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> FATURAÇÃO >> QUERY 6 │░░░░░░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Quantidades vendidas e faturação num dado intervalo... │\n");
	printf("│╭────────────────────────────────────────────────╮      │\n");
	printf("││ [1] FATURAÇÃO | [2] Menu Principal | [3] Sair  │      │\n");
	printf("│╰────────────────────────────────────────────────╯      │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query06_imprimirCabecalho_2(String mes1, String mes2){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> FATURAÇÃO >> QUERY 6 │░░░░░░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Nº vendas e total faturado entre %9s e %9s │\n",mes1,mes2);
	printf("│ ╭───────────────────────────────────────────────╮      │\n");
	printf("│ │ [1] FATURAÇÃO | [2] Menu Principal | [3] Sair │      │\n");
	printf("│ ╰───────────────────────────────────────────────╯      │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query06_imprimirInfo(){
	printf("│    MÊS    │   Quantidade   │    Faturação   │          │\n");
}

static void query06_funcaoImpressao(String mes, long quantidade, double faturacao){
	printf("│ %9s │ %14ld │ %14.2f │          │\n", mes, quantidade, faturacao);
}

static void query06_imprimirRodape(long totaisQuantidade, double totaisFaturacao){
	printf("│    TOTAIS │ %14ld │ %14.2f │          │\n", totaisQuantidade, totaisFaturacao);
	printf("│───────────╯────────────────╯────────────────╯          │\n");
	printf("│ Opção > ");
}

/* QUERY 12 */

static void query12_funcaoImpressao(long clientes, long produtos, double tempo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 12 │░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Nº clientes e produtos sem qualquer registo de vendas  │\n");
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│╭──────────────────────────────────────────┬───────────╮│\n");
	printf("││  Número clientes registados sem compras  │ %9ld ││\n",clientes);
	printf("│├──────────────────────────────────────────┼───────────┤│\n");
	printf("││  Número produtos que ninguém comprou     │ %9ld ││\n",produtos);
	printf("│╰──────────────────────────────────────────┴───────────╯│\n");
	printf("│ (em %6.4f segundos)                                   │\n",tempo);
	printf("│                                                        │\n");
	printf("│  Opção > ");
}

/* QUERY 10 */

static void query10_global_imprimirCabecalho(double tempo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 10 │░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Os produtos mais vendidos em todo o ano (%.4f seg.)  │\n",tempo);
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query10_filial_imprimirCabecalho(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 10 │░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Os produtos mais vendidos em todo o ano na filial...   │\n");
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query10_filial_imprimirCabecalho_2(int filial, double tempo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 10 │░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Os produtos mais vendidos na filial %d (%.4f seg.)    │\n",filial, tempo);
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query10_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal){
	printf("│ Pág. %05d/%05d # %06d-%06d de %06d resultados  │\n", paginaAtual+1, paginasTotais-1, posIni+1, posFin+1, nrResultadosTotal);
	printf("│ ┌────────┬──────────┬────────────┬───────────┐         │\n");
	printf("│ │      # │  Código  │ Quantidade │Nº Clientes│         │\n");
}

static void query10_funcaoImpressao(int pos, GestaoProduto produto){
	printf("│ │ %6d │  %6s  │ %10d │ %9d │         │\n", pos, getCodProduto_gestaoProduto(produto), getQuantidadeTotal_gestaoProduto(produto), getNrClientesTotal_gestaoProduto(produto) );
}

static void query10_imprimirRodape(){
	printf("│ ╰────────┴──────────┴────────────┴───────────╯         │\n");
	printf("│<<[4]   <[5]  < # >  [6]>   [7]>> || Opção > ");
}

/* QUERY 04 */

static void query04_filial_imprimirCabecalho(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 4 │░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Lista de produtos que ninguém comprou na filial...     │\n");
	printf("│ ╭───────────────────────────────────────────────────╮  │\n");
	printf("│ │[1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │  │\n");
	printf("│ ╰───────────────────────────────────────────────────╯  │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query04_filial_imprimirCabecalho_2(int filial, double tempo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 4 │░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Produtos que ninguém comprou na filial %d (em %.4f s) │\n",filial,tempo);
	printf("│ ╭───────────────────────────────────────────────────╮  │\n");
	printf("│ │[1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │  │\n");
	printf("│ ╰───────────────────────────────────────────────────╯  │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query04_global_imprimirCabecalho(double tempo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 4 │░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Lista de produtos que ninguém comprou (em %.4f seg)  │\n",tempo);
	printf("│ ╭───────────────────────────────────────────────────╮  │\n");
	printf("│ │[1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │  │\n");
	printf("│ ╰───────────────────────────────────────────────────╯  │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query04_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal){
	printf("│ Pág. %05d/%05d # %06d-%06d de %06d resultados  │\n", paginaAtual+1, paginasTotais-1, posIni+1, posFin+1, nrResultadosTotal);
	printf("│ ┌────────┬──────────┐                                  │\n");
	printf("│ │      # │  Código  │                                  │\n");
}

static void query04_funcaoImpressao(int pos, void *s){
	printf("│ │ %6d │ %8s │                                  │\n", pos, (char *) s);
}

static void query04_imprimirRodape(){
	printf("│ ╰────────┴──────────╯                                  │\n");
	printf("│<<[4]   <[5]  < # >  [6]>   [7]>> || Opção > ");
}

/* QUERY 07 */

static void query07_imprimirCabecalho(double tempo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 7 │░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Clientes que compraram em todas as filiais (%.4f s)  │\n",tempo);
	printf("│ ╭───────────────────────────────────────────────────╮  │\n");
	printf("│ │[1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │  │\n");
	printf("│ ╰───────────────────────────────────────────────────╯  │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query07_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal){
	printf("│ Pág. %05d/%05d # %06d-%06d de %06d resultados  │\n", paginaAtual+1, paginasTotais-1, posIni+1, posFin+1, nrResultadosTotal);
	printf("│ ┌────────┬──────────┐                                  │\n");
	printf("│ │      # │  Código  │                                  │\n");
}

static void query07_funcaoImpressao(int pos, void *s){
	printf("│ │ %6d │ %8s │                                  │\n", pos, (char *) s);
}

static void query07_imprimirRodape(){
	printf("│ ╰────────┴──────────╯                                  │\n");
	printf("│<<[4]   <[5]  < # >  [6]>   [7]>> || Opção > ");
}

/* QUERY 08 */

static void query08_imprimirCabecalho(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 8 │░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Lista de clientes que compraram um dado produto...     │\n");
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query08_imprimirCabecalho_2(String codigo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 8 │░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Clientes que compraram %7s na filial...            │\n",codigo);
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query08_filial_imprimirCabecalho_3(String codigo, int filial, double tempo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 8 │░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Lista clientes compraram %7s na filial %d (%.4f s)│\n",codigo,filial, tempo);
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

static void query08_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal){
	printf("│ Pág. %05d/%05d # %06d-%06d de %06d resultados  │\n", paginaAtual+1, paginasTotais-1, posIni+1, posFin+1, nrResultadosTotal);
	printf("│ ┌────────┬──────────┬────────────┐                     │\n");
	printf("│ │      # │  Código  │ Tipo Venda │                     │\n");
}

static void query08_funcaoImpressao(int pos, void** dados){
	printf("│ │ %6d │  %6s  │      %s     │                     │\n", pos, (char*)dados[0], (char*)dados[1] );
}

static void query08_imprimirRodape(){
	printf("│ ╰────────┴──────────┴────────────╯                     │\n");
	printf("│<<[4]   <[5]  < # >  [6]>   [7]>> || Opção > ");
}


