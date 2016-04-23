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
#include "catalogo_produtos.h"
#include "catalogo_clientes.h"
#include "gestao_filiais.h"
#include "menu.h"
#include "faturacao.h"
#include "leitura_ficheiros.h"
#include "queries.h"

extern float TEMPOS_LEITURA;
extern long NRCLIENTES;
extern long NRPRODUTOS;
extern long NRVENDAS;

static int menu_catalogos();
static int menu_faturacao();
static int menu_gestao_filiais();
static int query03_escolha();
static int query04_escolha();
static int query10_escolha();

/* MENU PRINCIPAL */

static void imprimir_menu_principal(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│░░░░░░░░░░░░░░░░░░░░░ │ GEREVENDAS │ ░░░░░░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│   ╭──────────────────────╮                             │\n");
	printf("│   │[1]CATÁLOGOS          │                             │\n");
	printf("│   │[2]FATURAÇÃO          │                             │\n");
	printf("│   │[3]GESTÃO DE FILIAIS  │                             │\n");
	printf("│   │[X]Sair               │                             │\n");
	printf("│   ╰──────────────────────╯                             │\n");
	printf("│                                                        │\n");
	printf("│                               %15ld Clientes │\n",NRCLIENTES);
	printf("│                               %15ld Produtos │\n",NRPRODUTOS);
	printf("│                               %15ld Vendas   │\n",NRVENDAS);
	printf("│   (em %6.3f segundos)                                 │\n",TEMPOS_LEITURA);
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Inserir opção > ");
}

void menu_principal(){
	int estado = MENU_PRINCIPAL;
	char input[INPUT_BUFFER];
	char opt;

	while(estado == MENU_PRINCIPAL){
		imprimir_menu_principal();

		if(scanf("%s", input) > 0){
			opt = input[0];

			switch (toupper(opt)) {
				case '1': estado = menu_catalogos();
					break;
				case '2': estado = menu_faturacao();
					break;
				case '3': estado = menu_gestao_filiais();
					break;
				case 'X': estado = SAIR;
				case 'Q': estado = SAIR;
				case 'S': estado = SAIR;
					break;
				default:
					estado = MENU_PRINCIPAL;
					break;
			}
		}else{
			printf("│ Opções disponíveis: [1] CATÁLOGOS; [2] FATURAÇÃO; [3] GESTÃO DE FILIAIS; [X] Sair.\n");
		}
	}
}

/* CATALOGOS */

static void imprimir_menu_catalogos(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> CATÁLOGOS │░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│╭───────────────────────────────────────────────╮       │\n");
	printf("││ [1] CLIENTES | [2] Menu Principal | [3] Sair  │       │\n");
	printf("│╰───────────────────────────────────────────────╯       │\n");
	printf("│╭─────────────────────────────────────────────────╮     │\n");
	printf("││[1] Catálogo de clientes começados por uma letra │     │\n");
	printf("││[4] Catálogo de produtos começados por uma letra │     │\n");
	printf("│╰─────────────────────────────────────────────────╯     │\n");
	printf("│ Inserir opção > ");
}

static int menu_catalogos(){
	int estado = MENU_CATALOGOS;
	char input[INPUT_BUFFER];
	char opt;

	while(estado == MENU_CATALOGOS){

		imprimir_menu_catalogos();

		if(scanf("%s", input) > 0){
			opt = input[0];

			switch (opt) {
				case '1': estado = query02_catalogoClientes( cat_Clientes );
					break;
				case '2': estado = MENU_PRINCIPAL;
					break;
				case '3': estado = SAIR;
					break;
				case '4': estado = query02_catalogoProdutos( cat_Produtos );
					break;
				default:
					estado = MENU_PRINCIPAL;
					break;
			}
		}
	}
	return estado;
}

/* FATURACAO */

static void imprimir_menu_faturacao(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> FATURAÇÃO │░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│╭────────────────────────────────────────────────╮      │\n");
	printf("││ [1] FATURAÇÃO | [2] Menu Principal | [3] Sair  │      │\n");
	printf("│╰────────────────────────────────────────────────╯      │\n");
	printf("│╭──────────────────────────────────────────────────────╮│\n");
	printf("││[1]Quantidades vendidas e faturação num dado intervalo││\n");
	printf("│╰──────────────────────────────────────────────────────╯│\n");
	printf("│ Inserir opção > ");
}

static int menu_faturacao(){
	int estado = MENU_FATURACAO;
	char input[INPUT_BUFFER];
	char opt;

	while(estado == MENU_FATURACAO){

		imprimir_menu_faturacao();

		if(scanf("%s", input) > 0){
			opt = input[0];

			switch (opt) {
				case '1': estado = query06_faturacaoIntervaloMeses( mod_Faturacao );
					break;
				case '2': estado = MENU_PRINCIPAL;
					break;
				case '3': estado = SAIR;
					break;
				default:
					estado = MENU_PRINCIPAL;
					break;
			}
		}
	}
	return estado;
}

/* GESTAO DE FILIAIS */

static void imprimir_menu_gestao_filiais(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│ GEREVENDAS >> GESTÃO DE FILIAIS │░░░░░░░░░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│╭────────────────────────────────────────────────────╮  │\n");
	printf("││ [1] MAIS VENDIDOS | [2] Menu Principal | [3] Sair  │  │\n");
	printf("│╰────────────────────────────────────────────────────╯  │\n");
	printf("│╭──────────────────────────────────────────────────────╮│\n");
	printf("││[1] Os produtos mais vendidos durante o ano           ││\n");
	printf("││[4] Vendas e faturação de um produto num dado mês     ││\n");
	printf("││[5] Nº produtos comprados por um cliente mês a mês    ││\n");
	printf("││[6] Lista produtos cliente mais comprou num dado mês  ││\n");
	printf("││[7] Top produtos um dado cliente mais gastou dinheiro ││\n");
	printf("││[8] Lista e número de produtos que ninguém comprou    ││\n");
	printf("││[9] Clientes que compraram em todas as filiais        ││\n");
	printf("││[A] Lista de clientes que compraram um dado produto   ││\n");
	printf("││[B] Nº de clientes e produtos sem registo de vendas   ││\n");
	printf("│╰──────────────────────────────────────────────────────╯│\n");
	printf("│ Inserir opção > ");
}

static int menu_gestao_filiais(){
	int estado = MENU_GESTAO_FILIAIS;
	char input[INPUT_BUFFER];
	char opt;

	while(estado == MENU_GESTAO_FILIAIS){

		imprimir_menu_gestao_filiais();

		if(scanf("%s", input) > 0){
			opt = input[0];

			switch (toupper(opt)) {
				case '1': estado = query10_escolha();
					break;
				case '2': estado = MENU_PRINCIPAL;
					break;
				case '3': estado = SAIR;
					break;
				case '4': estado = query03_escolha();
					break;
				case '5': estado = query05_nrProdutosCompradosPeloCliente(mod_Gestao, cat_Clientes );
					break;
				case '6': estado = query09_listaProdutosClienteMaisComprou(mod_Gestao, cat_Clientes);
					break;
				case '7': estado = query11_topProdutosClienteGastouDinheiro(mod_Gestao, cat_Clientes);
					break;
				case '8': estado = query04_escolha();
					break;
				case '9': estado = query07_clientesCompraramTodasAsFiliais(mod_Gestao);
					break;
				case 'A': estado = query08_listaClientesCompraramProduto_filial( mod_Gestao, cat_Produtos);
					break;
				case 'B': estado = query12_NrClientesSemCompras_NrProdutosNinguemComprou(mod_Gestao, cat_Clientes, cat_Produtos);
					break;
				default:
					estado = MENU_PRINCIPAL;
					break;
			}
		}
	}
	return estado;
}

/* QUERY 03 */

static int query03_escolha(){
	int estado = QUERY_03;
	char input[INPUT_BUFFER];
	char opt;

	while(estado == QUERY_03){
		printf("\033[2J\033[1;1H");
		printf("╭────────────────────────────────────────────────────────╮\n");
		printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 3 │░░░░░░░░░░░░░░│\n");
		printf("│════════════════════════════════════════════════════════│\n");
		printf("│ Nº vendas e total faturado pelo produto num dado mês...│\n");
		printf("│ ╭─────────────────────────────────────────────╮        │\n");
		printf("│ │ [1] GLOBAIS | [2] Menu Principal | [3] Sair │        │\n");
		printf("│ ╰─────────────────────────────────────────────╯        │\n");
		printf("│ ╭───────────────────────────────╮                      │\n");
		printf("│ │[1] Resultados Globais         │                      │\n");
		printf("│ │[4] Resultados filial a filial │                      │\n");
		printf("│ ╰───────────────────────────────╯                      │\n");
		printf("│ Inserir opção > ");

		if(scanf("%s", input) > 0){
			opt = input[0];

			switch (opt) {
				case '1': estado = query03_quantidadeFaturacaoProduto_global( mod_Gestao, cat_Produtos );
					break;				
				case '2': estado = MENU_PRINCIPAL;
					break;
				case '3': estado = SAIR;
					break;
				case '4': estado = query03_quantidadeFaturacaoProduto_filial( mod_Gestao, cat_Produtos );
					break;
				default:
					estado = MENU_GESTAO_FILIAIS;
					break;
			}
		}
	}
	return estado;
}

/* QUERY 04 */

static int query04_escolha(){
	int estado = QUERY_04;
	char input[INPUT_BUFFER];
	char opt;

	while(estado == QUERY_04){
		printf("\033[2J\033[1;1H");
		printf("╭────────────────────────────────────────────────────────╮\n");
		printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 4 │░░░░░░░░░░░░░░│\n");
		printf("│════════════════════════════════════════════════════════│\n");
		printf("│ Lista de produtos que ninguém comprou...               │\n");
		printf("│ ╭─────────────────────────────────────────────╮        │\n");
		printf("│ │ [1] GLOBAIS | [2] Menu Principal | [3] Sair │        │\n");
		printf("│ ╰─────────────────────────────────────────────╯        │\n");
		printf("│ ╭───────────────────────────────╮                      │\n");
		printf("│ │[1] Resultados Globais         │                      │\n");
		printf("│ │[4] Resultados filial a filial │                      │\n");
		printf("│ ╰───────────────────────────────╯                      │\n");
		printf("│ Inserir opção > ");

		if(scanf("%s", input) > 0){
			opt = input[0];

			switch (opt) {
				case '1': estado = query04_produtosNinguemComprou_global(mod_Gestao, cat_Produtos);
					break;				
				case '2': estado = MENU_PRINCIPAL;
					break;
				case '3': estado = SAIR;
					break;
				case '4': estado = query04_produtosNinguemComprou_filial(mod_Gestao, cat_Produtos);
					break;
				default:
					estado = MENU_GESTAO_FILIAIS;
					break;
			}
		}
	}
	return estado;
}

/* QUERY 10 */

static int query10_escolha(){
	int estado = QUERY_10;
	char input[INPUT_BUFFER];
	char opt;

	while(estado == QUERY_10){
		printf("\033[2J\033[1;1H");
		printf("╭────────────────────────────────────────────────────────╮\n");
		printf("│ GEREVENDAS >> GESTÃO FILIAIS >> QUERY 10 │░░░░░░░░░░░░░│\n");
		printf("│════════════════════════════════════════════════════════│\n");
		printf("│ Os produtos mais vendidos em todo o ano...             │\n");
		printf("│ ╭─────────────────────────────────────────────╮        │\n");
		printf("│ │ [1] GLOBAIS | [2] Menu Principal | [3] Sair │        │\n");
		printf("│ ╰─────────────────────────────────────────────╯        │\n");
		printf("│ ╭───────────────────────────────╮                      │\n");
		printf("│ │[1] Resultados Globais         │                      │\n");
		printf("│ │[4] Resultados filial a filial │                      │\n");
		printf("│ ╰───────────────────────────────╯                      │\n");
		printf("│ Inserir opção > ");

		if(scanf("%s", input) > 0){
			opt = input[0];

			switch (opt) {
				case '1': estado = query10_topProdutosMaisComprados_global( mod_Gestao , cat_Produtos );
					break;				
				case '2': estado = MENU_PRINCIPAL;
					break;
				case '3': estado = SAIR;
					break;
				case '4': estado = query10_topProdutosMaisComprados_filial( mod_Gestao , cat_Produtos );
					break;
				default:
					estado = MENU_GESTAO_FILIAIS;
					break;
			}
		}
	}
	return estado;
}

/* AJUDA */

void ajuda(){
	fprintf(stderr, "\tComando de execução:\n");
	fprintf(stderr, "./gerevendas [ <ficheiro_clientes.txt> <ficheiro_produtos.txt> <ficheiro_vendas.txt> ]\n");
}
