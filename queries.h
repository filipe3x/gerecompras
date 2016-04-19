#ifndef _QUERIES_H
#define _QUERIES_H

int query02_catalogoClientes( CATALOGO_CLIENTES );
int query05_nrProdutosCompradosPeloCliente(MODULO_GESTAO_FILIAIS modulo, CATALOGO_CLIENTES catalogo);
int query09_listaProdutosClienteMaisComprou(MODULO_GESTAO_FILIAIS, CATALOGO_CLIENTES);
int query11_topProdutosClienteGastouDinheiro(MODULO_GESTAO_FILIAIS modulo, CATALOGO_CLIENTES catalogo);


#endif /* _QUERIES_H */
