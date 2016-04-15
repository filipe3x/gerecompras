#include <stdio.h>
#include <stdlib.h>

#include "venda.h"
#include "faturacao.h"

struct contas{
	Quantidade_st quantidadeTotal;
	Preco_st totalFaturado;	
};

struct faturacao{
	Quantidade_st quantidadeTotalNormal;
	Quantidade_st quantidadeTotalPromocional;
	Preco_st totalFaturadoNormal;
	Preco_st totalFaturadoPromocional;

	Contas filial[NRFILIAIS + 1][12][2];
};

Quantidade_st getQuantidadeTotalNormal(MODULO_FATURACAO modulo){
	return modulo->quantidadeTotalNormal;
}

Quantidade_st getQuantidadeTotalPromocional(MODULO_FATURACAO modulo){
	return modulo->quantidadeTotalPromocional;
}

Preco_st getTotalFaturadoNormal(MODULO_FATURACAO modulo){
	return modulo->totalFaturadoNormal;
}

Preco_st getTotalFaturadoPromocional(MODULO_FATURACAO modulo){
	return modulo->totalFaturadoPromocional;
}

////

void setQuantidadeTotalNormal(MODULO_FATURACAO modulo, Quantidade_st quantidade){
	modulo->quantidadeTotalNormal = quantidade;
}

void setQuantidadeTotalPromocional(MODULO_FATURACAO modulo, Quantidade_st quantidade){
	modulo->quantidadeTotalPromocional = quantidade;
}

void setTotalFaturadoNormal(MODULO_FATURACAO modulo, Preco_st valor){
	modulo->totalFaturadoNormal = valor;
}

void setTotalFaturadoPromocional(MODULO_FATURACAO modulo, Preco_st valor){
	modulo->totalFaturadoPromocional = valor;
}

////

void incQuantidadeTotalNormal(MODULO_FATURACAO modulo, Quantidade_st quantidade){
	modulo->quantidadeTotalNormal += quantidade;
}

void incQuantidadeTotalPromocional(MODULO_FATURACAO modulo, Quantidade_st quantidade){
	modulo->quantidadeTotalPromocional += quantidade;
}

void incTotalFaturadoNormal(MODULO_FATURACAO modulo, Preco_st valor){
	modulo->totalFaturadoNormal += valor;
}

void incTotalFaturadoPromocional(MODULO_FATURACAO modulo, Preco_st valor){
	modulo->totalFaturadoPromocional += valor;
}

////

Quantidade_st getContas_QuantidadeTotal(MODULO_FATURACAO modulo, Filial_st f, Mes_st mes, TipoVenda_st t){
	return modulo->filial[f][mes][t]->quantidadeTotal;
}

Preco_st getContas_TotalFaturado(MODULO_FATURACAO modulo, Filial_st f, Mes_st mes, TipoVenda_st t){
	return modulo->filial[f][mes][t]->totalFaturado;
}

void setContas_QuantidadeTotal(MODULO_FATURACAO modulo, Filial_st f, Mes_st mes, TipoVenda_st t, Quantidade_st quantidade){
	modulo->filial[f][mes][t]->quantidadeTotal = quantidade;
}

void setContas_TotalFaturado(MODULO_FATURACAO modulo, Filial_st f, Mes_st mes, TipoVenda_st t, Preco_st valor){
	modulo->filial[f][mes][t]->totalFaturado = valor;
}

void incContas_QuantidadeTotal(MODULO_FATURACAO modulo, Filial_st f, Mes_st mes, TipoVenda_st t, Quantidade_st quantidade){
	modulo->filial[f][mes][t]->quantidadeTotal += quantidade;
}

void incContas_TotalFaturado(MODULO_FATURACAO modulo, Filial_st f, Mes_st mes, TipoVenda_st t, Preco_st valor){
	modulo->filial[f][mes][t]->totalFaturado += valor;
}

////

Contas contasInit(){
	return (Contas) malloc(sizeof(struct contas));
}

void freeContas(Contas contas){
	free(contas);
}

Contas alocarContasNaGrelhaFaturacao(MODULO_FATURACAO modulo, Filial_st f, Mes_st mes, TipoVenda_st t){
	return modulo->filial[f][mes][t] = contasInit();
}

void freeContasNaGrelhaFaturacao(MODULO_FATURACAO modulo, Filial_st f, Mes_st mes, TipoVenda_st t){
	freeContas(modulo->filial[f][mes][t]);
}

MODULO_FATURACAO moduloFaturacaoInit(){
	MODULO_FATURACAO modulo = (MODULO_FATURACAO) malloc(sizeof(struct faturacao));

	setQuantidadeTotalNormal(modulo, 0);
	setQuantidadeTotalPromocional(modulo, 0);
	setTotalFaturadoNormal(modulo, 0);
	setTotalFaturadoPromocional(modulo, 0);

	int f, mes;
	for (f = 0; f <= NRFILIAIS; f++)
		for (mes = 0; mes < 12; mes++){
			alocarContasNaGrelhaFaturacao(modulo,f,mes,VENDA_NORMAL);
			setContas_QuantidadeTotal(modulo,f,mes,VENDA_NORMAL, 0);
			setContas_TotalFaturado(modulo,f,mes,VENDA_NORMAL, 0);

			alocarContasNaGrelhaFaturacao(modulo,f,mes,VENDA_PROMOCIONAL);
			setContas_QuantidadeTotal(modulo,f,mes,VENDA_PROMOCIONAL, 0);
			setContas_TotalFaturado(modulo,f,mes,VENDA_PROMOCIONAL, 0);
		}

	return modulo;
}

void freeModuloFaturacao(MODULO_FATURACAO modulo){
	int f, mes;

	for (f = 0; f <= NRFILIAIS; f++)
		for (mes = 0; mes < 12; mes++){
			freeContasNaGrelhaFaturacao(modulo,f,mes,VENDA_NORMAL);
			freeContasNaGrelhaFaturacao(modulo,f,mes,VENDA_PROMOCIONAL);
		}

	free(modulo);
}

void inserirVendaModuloFacturacao(MODULO_FATURACAO modulo, VENDA venda){
	Filial_st f = getFilial(venda);
	Mes_st mes = getMes(venda);
	TipoVenda_st t = getTipoVenda(venda);
	Preco_st p = getPreco(venda);
	Quantidade_st q = getQuantidade(venda);

	incContas_QuantidadeTotal(modulo,f,mes,t, q);
	incContas_TotalFaturado(modulo,f,mes,t, q * p);

 	t == VENDA_NORMAL ? incQuantidadeTotalNormal(modulo, q) : incQuantidadeTotalPromocional(modulo, q);
 	t == VENDA_NORMAL ? incTotalFaturadoNormal(modulo, p * q) : incTotalFaturadoPromocional(modulo, p * q);
}

Resultados consultaFaturacaoMes(MODULO_FATURACAO modulo, Mes_st mes){
	Resultados res_normal, res_promocional;
	Filial_st f;

	for(f = 0, res_normal = res_promocional = 0; f <= NRFILIAIS; f++){
		res_normal += getContas_TotalFaturado(modulo,f,mes,VENDA_NORMAL);
		res_promocional += getContas_TotalFaturado(modulo,f,mes,VENDA_PROMOCIONAL);
	}

	return res_normal + res_promocional;
}

Resultados consultaFaturacaoFilial(MODULO_FATURACAO modulo, Filial_st filial){
	Resultados res_normal, res_promocional;
	Mes_st mes;

	for(mes = 0, res_normal = res_promocional = 0; mes < 12; mes++){
		res_normal += getContas_TotalFaturado(modulo,filial,mes,VENDA_NORMAL);
		res_promocional += getContas_TotalFaturado(modulo,filial,mes,VENDA_PROMOCIONAL);
	}

	return res_normal + res_promocional;
}

Resultados consultaFaturacaoMesFilial(MODULO_FATURACAO modulo, Mes_st mes, Filial_st filial){
	Resultados res_normal = getContas_TotalFaturado(modulo,filial,mes,VENDA_NORMAL);
	Resultados res_promocional = getContas_TotalFaturado(modulo,filial,mes,VENDA_PROMOCIONAL);
	return res_normal + res_promocional;
}

void travessiaFaturacao(MODULO_FATURACAO modulo){
	printf("%d\n", getQuantidadeTotalNormal(modulo));
	printf("%d\n", getQuantidadeTotalPromocional(modulo));
	printf("%f\n", getTotalFaturadoNormal(modulo));
	printf("%f\n", getTotalFaturadoPromocional(modulo));

	int f, mes;
	for (f = 0; f <= NRFILIAIS; f++){
			printf("** Filial %d **\n", f);
			for (mes = 0; mes < 12; mes++){
				printf(" - Mes %s -\n", Mes[mes]);
				printf("\tNormal %f\n", getContas_TotalFaturado(modulo,f,mes,VENDA_NORMAL));
				printf("\tPromocional %f\n", getContas_TotalFaturado(modulo,f,mes,VENDA_PROMOCIONAL));
			}
		}
}
