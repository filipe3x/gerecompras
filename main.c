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

#define MAXLINHAS 10000000
#define NRFILIAIS 3
#define MAX_BUFFER_VENDAS 64
#define MAX_BUFFER_CATALOGO 32

#include "leitura_ficheiros.c"
#include "menu.c"

//nr clientes validos: 16 384 distintos
//nr produtos validos: 171 008 distintos
//nr vendas validos: 891 108

int main(int argc, char const *argv[]){
    clock_t tini, tfin;
    tini = clock();

	printf("%s\n","Hello world!");

    // abrirFicheiroClientes("Clientes.txt");

    gestaoFiliaisMain();

    // menu();

    tfin = clock();

    printf("Leitura feita em %.3f segundos.\n ", (double) (tfin - tini)/CLOCKS_PER_SEC);

    return(EXIT_SUCCESS);
}

/*
//TESTAR INPUTS DO UTILIZADOR
static int vendaValida(COMPRA compra) {
    return cat_existe_cliente(catalogo_clientes, get_cod_cliente(compra))
            && cat_existe_produto(catalogo_produtos, get_cod_produto(compra))
            && get_mes(compra) >= 1 && get_mes(compra) <= 12
            && get_preco_unit(compra) >= 0
            && (get_quantidade(compra) > 0)
            && (get_promo(compra) == 'N' || get_promo(compra) == 'P');
}
*/
/*
static int dividirCodigo(char *codigo, int onde, char **prefixo, char **numero){
	*prefixo = codigo;
	int size = asprintf(numero, "%s", codigo + onde); //malloc
	(*prefixo)[onde] = '\0'; //divide aqui

	return (size == -1) ? -1 : 0; //malloc bem sucedido ou nao
}
*/
/*
static char* deepCopiaStr(char *orig){
	//char *dest = (char*) malloc(strlen(orig)*sizeof(char));
//	return strcpy(dest,orig);
	return NULL;
}
*/

/*

static int traduzirProduto(char *produto, ProdutoCod *produtoStruct){
	char *str;
	dividirCodigo(produto, 2, &produtoStruct->prefixo, &str);
	produtoStruct->numero = (int) strtol(str, (char **)NULL, 10);
	return 0;
}


static int traduzirCliente(char *cliente, ClienteCod *clienteStruct){
	return 0;
}

static int traduzirLinhaVenda(char *linha){
	return 0;
}
*/

/*
int main() {

    char pressed_key;
    int ascii_code = 0;

    puts("Ascii code");
    while(ascii_code != 27) {
        puts("\nPress a key / ESC to exit");
        system("/bin/stty raw");
        pressed_key = getchar();
        system("/bin/stty cooked");
        ascii_code = pressed_key;
        printf(" the code is %d\n", ascii_code);
    }
    return 0;
}
*/
