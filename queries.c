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

#include "avl.h"
#include "venda.h"
#include "navegacao.h"
#include "gestao_filiais.h"
#include "catalogo_clientes.h"
#include "queries.h"
#include "menu.h"

typedef struct resultado_query02_catalogoClientes{
	int indice;
	CodigoCliente_st codigo;
} *Resultado_query02_catalogoClientes;

int query02_catalogoClientes(CATALOGO_CLIENTES catalogo){
	int estado = QUERY_02;
	char input[50];
	String codigo;
	char opt, c;
	clock_t tini, tfin;
	PAGINA_RESULTADOS pagina;
	int nrResultados;

	query02_imprimirCabecalho();
	printf("│ Inserir primeira letra > ");
	scanf("%s",input);
	c = input[0];

	tini = clock();
	pagina = (PAGINA_RESULTADOS) travessiaClientesPorLetra(catalogo, c);
	tfin = clock();

	nrResultados = getTamanhoLista(pagina);
	setNrElementosPorPagina(pagina, ELEM_POR_PAG);
	posicoesInit(pagina);

	while(estado == QUERY_02)
	{
		query02_imprimirCabecalho_2(toupper(c), (double) (tfin - tini)/CLOCKS_PER_SEC);

		if(nrResultados == 0){
			printf("│ Não foram encontrados resultados para esta letra.\n");
			printf("│ Opção > ");

			scanf(" %c",&opt);

			switch(opt)
			{
				case '1':
						return estado = MENU_CATALOGOS;
						break;
				case '2':
						return estado = MENU_PRINCIPAL;
						break;
				case '3':
						return estado = SAIR;
						break;
				default:
						//Nenhuma das opcoes anteriores...
						return estado = MENU_CATALOGOS;
						break;
			}	

		}

		query02_imprimirInfo(getPaginaAtual(pagina), getNrPaginasTotal(pagina) + 1, getPosicaoInicial(pagina), getPosicaoFinal(pagina), getIndiceAtual(pagina));

		codigo = (String) getElementoAtual(pagina);
		while(codigo != NULL){
			query02_funcaoImpressao(getPosicaoAtual(pagina), (void*) codigo);
			codigo = (String) getElementoAtual(pagina);
		}

		query02_imprimirRodape();

		scanf(" %c",&opt);

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
					//Nenhuma das opcoes anteriores...
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
						//Nenhuma das opcoes anteriores...
						estado = MENU_CATALOGOS;
						break;
			}
		}
	}

	return estado;

}


int query05_nrProdutosCompradosPeloCliente(MODULO_GESTAO_FILIAIS modulo, CATALOGO_CLIENTES catalogo){
	int estado = QUERY_05;
	char input[50];
	void** dados;
	int leitura_scanf;
	char opt, c;

	query05_imprimirCabecalho();
	printf("│ Inserir código do cliente > ");
	leitura_scanf = scanf("%s",input);
	c = input[0];

	if(leitura_scanf > 0){
		switch(c)
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
					//Nenhuma das opcoes anteriores...
					estado = QUERY_05;
					break;
		}	
	}

	if(!verificaCodigoCliente( input )){
		printf("│ Código inválido.\n");
		printf("│ Opção > ");	

		scanf(" %c",&opt);
		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
					break;
			case '2':
					return estado = MENU_PRINCIPAL;
					break;
			case '3':
					return estado = SAIR;
					break;
			default:
					//Nenhuma das opcoes anteriores...
					return estado = MENU_GESTAO_FILIAIS;
					break;
		}
	}

	if(existeClienteCatalogo(catalogo, input) == false){
		printf("│ Cliente não existe.\n");
		printf("│ Opção > ");	

		scanf(" %c",&opt);
		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
					break;
			case '2':
					return estado = MENU_PRINCIPAL;
					break;
			case '3':
					return estado = SAIR;
					break;
			default:
					//Nenhuma das opcoes anteriores...
					return estado = MENU_GESTAO_FILIAIS;
					break;
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

		while(estado == QUERY_05)
		{
			int i;
			query05_imprimirCabecalho_2(input, (double) (tfin - tini)/CLOCKS_PER_SEC);
			query05_imprimirInfo(NRFILIAIS);

			dados = getMultiplosElementos(pagina);

			i = 0;
			while(dados != NULL){
				query05_funcaoImpressao(dados, Mes[i], NRFILIAIS);
				dados = getMultiplosElementos(pagina);
				i++;
			}

			query05_imprimirRodape(NRFILIAIS);
			scanf(" %c",&opt);

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
						//Nenhuma das opcoes anteriores...
						estado = MENU_GESTAO_FILIAIS;
						break;
			}		
		}
	}
	
	return estado;

}


int query09_listaProdutosClienteMaisComprou(MODULO_GESTAO_FILIAIS modulo, CATALOGO_CLIENTES catalogo){
	int estado = QUERY_09;
	char input[50];
	RegistoProduto produto;
	int leitura_scanf, mes, try;
	char opt, c;
	PAGINA_RESULTADOS pagina;

	query09_imprimirCabecalho();
	printf("│ Inserir código do cliente > ");
	leitura_scanf = scanf("%s",input);
	c = input[0];

	if(leitura_scanf > 0){
		switch(c)
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
					//Nenhuma das opcoes anteriores...
					estado = QUERY_09;
					break;
		}	
	}

	if(estado == QUERY_09 && !verificaCodigoCliente( input )){
		printf("│ Código inválido.\n");
		printf("│ Opção > ");	

		scanf(" %c",&opt);
		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
					break;
			case '2':
					return estado = MENU_PRINCIPAL;
					break;
			case '3':
					return estado = SAIR;
					break;
			default:
					//Nenhuma das opcoes anteriores...
					return estado = MENU_GESTAO_FILIAIS;
					break;
		}
	}

	if(estado == QUERY_09 && existeClienteCatalogo(catalogo, input) == false){
		printf("│ Cliente não existe.\n");
		printf("│ Opção > ");	

		scanf(" %c",&opt);
		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
					break;
			case '2':
					return estado = MENU_PRINCIPAL;
					break;
			case '3':
					return estado = SAIR;
					break;
			default:
					//Nenhuma das opcoes anteriores...
					return estado = MENU_GESTAO_FILIAIS;
					break;
		}
	}

	mes = 0, try = 3;
	while(estado == QUERY_09 && !verificaMes(mes) && try--){		
		printf("│ Inserir Mês (número 1-12) > ");
		scanf(" %d",&mes); 
		if(!verificaMes(mes)) printf("│ Mês inválido. ");
		printf("\n");
	}

	if(try == 0) return estado = SAIR;
	mes--;

	if(estado == QUERY_09){
		clock_t tini, tfin;

		tini = clock();
		pagina = (PAGINA_RESULTADOS) listaProdutosClienteMaisComprou(modulo, input, mes);
		tfin = clock();

		setNrElementosPorPagina(pagina, ELEM_POR_PAG);
		posicoesInit(pagina);

		while(estado == QUERY_09)
		{
			query09_imprimirCabecalho_2(input, (double) (tfin - tini)/CLOCKS_PER_SEC);
			query09_imprimirInfo(getPaginaAtual(pagina), getNrPaginasTotal(pagina) + 1, getPosicaoInicial(pagina), getPosicaoFinal(pagina), getIndiceAtual(pagina));

			produto = (RegistoProduto) getElementoAtual(pagina);
			while(produto != NULL){
				query09_funcaoImpressao(getPosicaoAtual(pagina), (void*) produto);
				produto = (RegistoProduto) getElementoAtual(pagina);
			}

			query09_imprimirRodape();
			scanf(" %c",&opt);

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
						//Nenhuma das opcoes anteriores...
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
							//Nenhuma das opcoes anteriores...
							estado = SAIR;
							break;
				}
			}
		}
}

	return estado;

}


int query11_topProdutosClienteGastouDinheiro(MODULO_GESTAO_FILIAIS modulo, CATALOGO_CLIENTES catalogo){
	int estado = QUERY_11;
	char input[50];
	RegistoProduto produto;
	PAGINA_RESULTADOS pagina;
	int leitura_scanf;
	char opt, c;

	query11_imprimirCabecalho();
	printf("│ Inserir código do cliente > ");
	leitura_scanf = scanf("%s",input);
	c = input[0];

	if(leitura_scanf > 0){
		switch(c)
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
					//Nenhuma das opcoes anteriores...
					estado = QUERY_11;
					break;
		}	
	}

	if(estado == QUERY_11 && !verificaCodigoCliente( input )){
		printf("│ Código inválido.\n");
		printf("│ Opção > ");	

		scanf(" %c",&opt);
		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
					break;
			case '2':
					return estado = MENU_PRINCIPAL;
					break;
			case '3':
					return estado = SAIR;
					break;
			default:
					//Nenhuma das opcoes anteriores...
					return estado = MENU_GESTAO_FILIAIS;
					break;
		}
	}

	if(estado == QUERY_11 && existeClienteCatalogo(catalogo, input) == false){
		printf("│ Cliente não existe.\n");
		printf("│ Opção > ");	

		scanf(" %c",&opt);
		switch(opt)
		{
			case '1':
					return estado = MENU_GESTAO_FILIAIS;
					break;
			case '2':
					return estado = MENU_PRINCIPAL;
					break;
			case '3':
					return estado = SAIR;
					break;
			default:
					//Nenhuma das opcoes anteriores...
					return estado = MENU_GESTAO_FILIAIS;
					break;
		}
	}


	if(estado == QUERY_11){
		clock_t tini, tfin;
		tini = clock();
		pagina = (PAGINA_RESULTADOS) topProdutosClienteGastouDinheiro(modulo, input);
		tfin = clock();

		setNrElementosPorPagina(pagina, ELEM_POR_PAG);
		posicoesInit(pagina);

		while(estado == QUERY_11)
		{
			query11_imprimirCabecalho_2(input, (double) (tfin - tini)/CLOCKS_PER_SEC);
			query11_imprimirInfo(getPaginaAtual(pagina), getNrPaginasTotal(pagina) + 1, getPosicaoInicial(pagina), getPosicaoFinal(pagina), getIndiceAtual(pagina));

			produto = (RegistoProduto) getElementoAtual(pagina);
			while(produto != NULL){
				query11_funcaoImpressao(getPosicaoAtual(pagina), (void*) produto);
				produto = (RegistoProduto) getElementoAtual(pagina);
			}

			query11_imprimirRodape();
			scanf(" %c",&opt);

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
						//Nenhuma das opcoes anteriores...
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
							//Nenhuma das opcoes anteriores...
							estado = SAIR;
							break;
				}
			}
		}
	}

	return estado;

}


