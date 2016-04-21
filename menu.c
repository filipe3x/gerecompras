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
#include "menu.h"
#include "navegacao.h"
#include "catalogo_produtos.h"
#include "catalogo_clientes.h"
#include "gestao_filiais.h"

/* MENU PRINCIPAL */

void menu_principal(){
printf("\033[2J\033[1;1H");
printf("╭────────────────────────────────────────────────────────╮\n");
printf("│░░░░░░░░░░░░░░░░░░░░ │ GERECOMPRAS │ ░░░░░░░░░░░░░░░░░░░│\n");
printf("│════════════════════════════════════════════════════════│\n");
printf("│   ╭──────────────────────╮                             │\n");
printf("│   │[1]CATÁLOGOS          │                             │\n");
printf("│   │[2]FATURAÇÃO          │                             │\n");
printf("│   │[3]GESTÃO DE FILIAIS  │                             │\n");
printf("│   │[X]Sair               │                             │\n");
printf("│   ╰──────────────────────╯                             │\n");
printf("│════════════════════════════════════════════════════════│\n");
printf("│════════════════════════════════════════════════════════│\n");
printf("│ Inserir opção > ");
}

/* QUERY 2 */

void query02_funcaoImpressao(int pos, void *s){
	printf("│ │ %5d │ %8s │                                   │\n", pos, (char *) s);
}

void query02_imprimirCabecalho(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│GERECOMPRAS >> CATÁLOGOS >> QUERY 2 │░░░░░░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Listar clientes começados pela letra...                │\n");
	printf("│ ╭───────────────────────────────────────────────╮      │\n");
	printf("│ │ [1] CATÁLOGOS | [2] Menu Principal | [3] Sair │      │\n");
	printf("│ ╰───────────────────────────────────────────────╯      │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

void query02_imprimirCabecalho_2(char letra, double tempo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│GERECOMPRAS >> CATÁLOGOS >> QUERY 2 │░░░░░░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Mostrando clientes começados por %c (em %.5f seg.)   │\n",letra,tempo);
	printf("│ ╭───────────────────────────────────────────────╮      │\n");
	printf("│ │ [1] CATÁLOGOS | [2] Menu Principal | [3] Sair │      │\n");
	printf("│ ╰───────────────────────────────────────────────╯      │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

void query02_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal){
	printf("│ Pág. %05d/%05d # %06d-%06d de %06d resultados  │\n", paginaAtual+1, paginasTotais-1, posIni+1, posFin+1, nrResultadosTotal);
	printf("│ ┌───────┬──────────┐                                   │\n");
	printf("│ │     # │  Código  │                                   │\n");
}

/* QUERY 5 */

void query02_imprimirRodape(){
	printf("│ ╰───────┴──────────╯                                   │\n");
	printf("│<<[4]   <[5]  < # >  [6]>   [7]>> || Opção > ");
}

void query05_imprimirCabecalho(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│GERECOMPRAS >> GESTÃO FILIAIS >> QUERY 5 │░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Nº produtos comprados pelo cliente...                  │\n");
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

void query05_imprimirCabecalho_2(String codigo, double tempo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│GERECOMPRAS >> GESTÃO FILIAIS >> QUERY 5 │░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Nº produtos comprados pelo cliente %7s (%.4f seg)│\n",codigo,tempo);
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

void query05_imprimirInfo(int nrFiliais){
	int f;
	printf("│    MÊS    │");

	f = 1;
	while(f <= nrFiliais){
		printf(" Filial %d │",f);
		f++;
	}

	printf("           │\n");
}

void query05_funcaoImpressao(void** dados, const String mes, int nrFiliais){
	int f;
	printf("│ %9s │", mes);

	f = 0;
	while(f < nrFiliais){
		printf(" %8ld │",*(long*)dados[f]);
		f++;
	}

	printf("           │\n");
}


void query05_imprimirRodape(int nrFiliais){
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

void query09_imprimirCabecalho(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│GERECOMPRAS >> GESTÃO FILIAIS >> QUERY 9 │░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Produtos mais comprados pelo cliente...                │\n");
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

void query09_imprimirCabecalho_2(String codigo, String mes, double tempo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│GERECOMPRAS >> GESTÃO FILIAIS >> QUERY 9 │░░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Mais comprados pelo cliente %5s em %9s (%.4f)│\n",codigo,mes,tempo);
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

void query09_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal){
	printf("│ Pág. %05d/%05d # %06d-%06d de %06d resultados  │\n", paginaAtual+1, paginasTotais-1, posIni+1, posFin+1, nrResultadosTotal);
	printf("│ ┌───────┬──────────┬──────────┐                        │\n");
	printf("│ │     # │  Código  │Quantidade│                        │\n");
}

void query09_funcaoImpressao(int pos, void *produto){
	printf("│ │ %5d │  %6s  │ %9d│                        │\n", pos, getCodProduto_registoProduto(produto), getQuantidade_registoProduto(produto));
}

void query09_imprimirRodape(){
	printf("│ ╰───────┴──────────┴──────────╯                        │\n");
	printf("│<<[4]   <[5]  < # >  [6]>   [7]>> || Opção > ");
}

/* QUERY 11 */

void query11_imprimirCabecalho(){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│GERECOMPRAS >> GESTÃO FILIAIS >> QUERY 11 │░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Produtos com mais gastos pelo cliente...               │\n");
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

void query11_imprimirCabecalho_2(String codigo, double tempo){
	printf("\033[2J\033[1;1H");
	printf("╭────────────────────────────────────────────────────────╮\n");
	printf("│GERECOMPRAS >> GESTÃO FILIAIS >> QUERY 11 │░░░░░░░░░░░░░│\n");
	printf("│════════════════════════════════════════════════════════│\n");
	printf("│ Ranking produtos onde %6s mais gastou (%.4f s)    │\n",codigo,tempo);
	printf("│ ╭────────────────────────────────────────────────────╮ │\n");
	printf("│ │ [1] GESTÃO FILIAIS | [2] Menu Principal | [3] Sair │ │\n");
	printf("│ ╰────────────────────────────────────────────────────╯ │\n");
	printf("│════════════════════════════════════════════════════════│\n");
}

void query11_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal){
	printf("│ Pág. %05d/%05d # %06d-%06d de %06d resultados  │\n", paginaAtual+1, paginasTotais-1, posIni+1, posFin+1, nrResultadosTotal);
	printf("│ ┌───────┬──────────┬───────────────┐                   │\n");
	printf("│ │     # │  Código  │    Gastos     │                   │\n");
}

void query11_funcaoImpressao(int pos, void *produto){
	printf("│ │ %5d │  %6s  │ %12.2f  │                   │\n", pos, getCodProduto_registoProduto(produto), getPreco_registoProduto(produto));
}

void query11_imprimirRodape(){
	printf("│ ╰───────┴──────────┴───────────────╯                   │\n");
	printf("│<<[4]   <[5]  < # >  [6]>   [7]>> || Opção > ");
}
