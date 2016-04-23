#ifndef _MENU_
#define	_MENU_

typedef enum estadoMenu {
    SAIR,
    MENU_PRINCIPAL,
    MENU_CATALOGOS,
    MENU_FATURACAO,
    MENU_GESTAO_FILIAIS,
    QUERY_02, QUERY_03, QUERY_04, QUERY_05, QUERY_06, 
    QUERY_07, QUERY_08, QUERY_09, QUERY_10, QUERY_11, QUERY_12
} EstadoMenu;

void menu_principal();
void ajuda();

#endif	/* _MENU_ */
