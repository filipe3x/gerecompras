#ifndef CAT_CLIENTES
#define CAT_CLIENTES
#include <stdbool.h>
typedef struct catalogoClientes *CATALOGO_CLIENTES;

CATALOGO_CLIENTES catalogoClientesInit( void );
void freeCatalogoClientes( CATALOGO_CLIENTES );

CodigoCliente_st inserirClienteCatalogo( CATALOGO_CLIENTES , CodigoCliente_st );
bool existeClienteCatalogo( CATALOGO_CLIENTES , CodigoCliente_st );

Modulo getCatalogoClientesPorLetra( CATALOGO_CLIENTES , char );
Modulo getCatalogoClientesPorIndice( CATALOGO_CLIENTES , int );

int getTotalClientes( CATALOGO_CLIENTES , int );
int calcularTotalClientes( CATALOGO_CLIENTES );

PAGINA_RESULTADOS travessiaClientesPorLetra( CATALOGO_CLIENTES , char );
PAGINA_RESULTADOS testarTravessiaClientes(CATALOGO_CLIENTES catalogo, int (*funcaoComparacao)(), void (*funcaoImpressao)() );
void freeTravessiaCatalogoClientes(TravessiaModulo travessia);

#endif /* CAT_CLIENTES */
