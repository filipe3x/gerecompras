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

void query02_imprimirCabecalho();
void query02_imprimirCabecalho_2(char letra, double tempo);
void query02_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal);
void query02_funcaoImpressao(int pos, void *s);
void query02_imprimirRodape();

void query05_imprimirCabecalho();
void query05_imprimirCabecalho_2(String codigo, double tempo);
void query05_imprimirInfo(int nrFiliais);
void query05_funcaoImpressao(void** dados, const String mes, int nrFiliais);
void query05_imprimirRodape(int nrFiliais);

void query09_imprimirCabecalho();
void query09_imprimirCabecalho_2(String codigo, double tempo);
void query09_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal);
void query09_funcaoImpressao(int pos, void *produto);
void query09_imprimirRodape();

void query11_imprimirCabecalho();
void query11_imprimirCabecalho_2(String codigo, double tempo);
void query11_imprimirInfo(int paginaAtual, int paginasTotais, int posIni, int posFin, int nrResultadosTotal);
void query11_funcaoImpressao(int pos, void *produto);
void query11_imprimirRodape();

#endif	/* _MENU_ */
