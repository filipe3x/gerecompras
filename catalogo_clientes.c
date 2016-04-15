#include "avl.h"

typedef struct catalogoClientes {
    int totalClientes[27];
    Modulo indice[27];
} *CatalogoClientes;

int calculaIndiceCliente(char l) {
    char letra = toupper(l);
    int i = 0;

    if (isalpha(letra)) {
        i = letra - 'A';
    } else {
        i = 26;
    }
    return i;
}

int getTotalClientes(CatalogoClientes catalogo, int ind){
    return catalogo->totalClientes[ind];
}

#include "navegacao.c"

int calcularTotalClientes(CatalogoClientes catalogo){
    int i, somatorio = 0;
    for(i = 0; i <= 26; i++)
        somatorio += getTotalClientes(catalogo, i);
    return somatorio;
}

void setTotalClientes(CatalogoClientes catalogo, int ind, int valor){
    catalogo->totalClientes[ind] = valor;
}

void incTotalClientes(CatalogoClientes catalogo, int ind){
	catalogo->totalClientes[ind]++;
}

int comparaCliente(const void *cliente_a, const void *cliente_b, void *param) {
    return strcmp((char *) cliente_a, (char *) cliente_b);
}

CatalogoClientes catalogoClientesInit(){
    CatalogoClientes catalogo = (CatalogoClientes) malloc(sizeof (struct catalogoClientes)); //inicializar array de 27 apontadores para AVLs

    int i;
    for (i = 0; i <= 26; i++){ // inicializar 27 AVLs, uma para cada letra do alfabeto
        catalogo->indice[i] = avl_create(comparaCliente, NULL, NULL); //a funcao que inicializa a avl precisa de saber qual a funcao de comparacao, respeitando assim o principio de encapsulamento
        setTotalClientes(catalogo, i, 0);
    }

    return catalogo;
}

static void freeCodigoCliente(void *codigo, void *param) {
    free((CodigoCliente_st) codigo);
}

void freeCatalogoClientes(CatalogoClientes catalogo){
    int i;
    
    if(catalogo != NULL){
        for (i = 0; i <= 26; i++) {
            avl_destroy(catalogo->indice[i], freeCodigoCliente);
        }
    }
    free(catalogo);
}

bool existeClienteCatalogo(CatalogoClientes catalogo, CodigoCliente_st codigo) {
    bool resultado = false;
    char letra = codigo[0];
    int i;

    if (codigo != NULL) {
        i = calculaIndiceCliente(letra);

        if (avl_find(catalogo->indice[i], codigo) != NULL) 
            resultado = true;
        else
            resultado = false;
    }

    return resultado;
}

CodigoCliente_st inserirClienteCatalogo(CatalogoClientes catalogo, CodigoCliente_st codigo){
	int i = calculaIndiceCliente(codigo[0]);

	CodigoCliente_st novo = cloneCodigo(codigo);
	CodigoCliente_st c =  (CodigoCliente_st) avl_insert(catalogo->indice[i], novo);

	if(c == NULL) incTotalClientes(catalogo, i); //nao encontrou repetido - insersao com sucesso
	else freeCodigo(novo); //caso ja exista fazer free

	return c;
}
