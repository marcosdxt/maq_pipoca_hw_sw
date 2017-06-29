/*__________________________________________________________________________________
|	Quark Tecnologia Eletrônica Embarcada
|       
|       Itapema - SC
|       www.quarktee.com.br
| __________________________________________________________________________________
|
|       This source code was developed by Quark  and cannot be copied, in part 
|       or in whole, or used, except when legally licensed by Quark 
|       or its distributors.
|
|       Este código é propriedade da Quark  e não pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Quark  ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo            :  RelatorioVendas.c
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  15/07/2015
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/

/***********************************************************************************
*       Includes
***********************************************************************************/
#include <string.h>
#include <stdio.h>
#include "..\..\includes.h"

/***********************************************************************************
*       Definições locais
***********************************************************************************/
//#define _DEBUG_RELATORIO_
#define UNIDADES_PARCIAL        500
#define UNIDADES_TOTAL          1000
#define ARRECADACAO_PARCIAL     2000
#define ARRECADACAO_TOTAL       4000
/***********************************************************************************
*       Constantes
***********************************************************************************/
const char *cabecalho[]=
{  
  "\n",
  "*****************************************\n",     
  "*              Mais PIPOCA              *\n",    
  "*****************************************\n",       
  "*         Relatorio de Comissao         *\n",  
  "*****************************************\n",    
};

const char *TitulocontadorVendas[]=
{
  "\n\r",
  "[Contador de vendas](unidades)\n"    
};

const char *TitulocontadorDinheiroVendas[]=
{
  "\n\r",
  "[Arrecadao em moeda](R$)\n"    
};

const char *TitulocomissaoPonto[]=
{
  "\n\r",
  "[Comissao do Ponto](R$)\n"    
};

const char *TituloTotalTeste[]=
{
  "\n\r",
  "[Teste de Preparo](un)\n"    
};

const char *TituloIdentificao[]=
{
 "\n\r",
 "[Identificacao]\n"  
};

const char *TituloHoraData[]=
{
 "\n\r",
 "\n\r",
 "[Data e Hora]\n"    
};

const char *Rodape[]=
{
  "\n\r",
  "*****************************************\n",      
  "visite:\n\r",
  "www.maispipoca.com.br\n\r",
  "*****************************************\n\r",    
  "\n\r",
  "\n\r",   
  "\n\r",
  "\n\r",  
  "\n\r"    
};

const char *TituloAssinatura[]=
{
"\n\r",    
"\n\r",    
"\n\r",  
"\n\r",  
"\n\r",  
"\n\r",
"\n\r",
"\n\r",
"_____________________________\n\r",
"  Responsavel pelo ponto\n\r",
"\n\r"
};


/***********************************************************************************
*       Variaveis locais
***********************************************************************************/


/***********************************************************************************
*       Funções locais
***********************************************************************************/


/***********************************************************************************
*       Implementação das funções
***********************************************************************************/

/***********************************************************************************
*       Descrição       :       Gera o relatório de faturamento
*                               do ponto
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void RV_relatorioPonto(void){
  char linha[40];
  char *pLinha[]={linha};
  unsigned int vendasParcial,vendasTotal;
  char bufferTexto[20];
  unsigned char hora,minuto,segundo,dia,mes;
  unsigned int ano;
  
  RTC_getValue(&hora,&minuto,&segundo,&dia,&mes,&ano);
  
  IMPRESSORA_enviaLinhas((char**)cabecalho,6);   
  
  // Titulo contador
  IMPRESSORA_enviaLinhas((char**)TitulocontadorVendas,2);  
  // Contador parcial
#ifndef _DEBUG_RELATORIO_
  vendasParcial =PARAMETROS_leContadorVendasParcial();
#else
  vendasParcial = UNIDADES_PARCIAL;
#endif
  sprintf(linha,"Parcial:%05d\n",vendasParcial);  
  IMPRESSORA_enviaLinhas(pLinha,1);
  // Contador total
#ifndef _DEBUG_RELATORIO_  
  vendasTotal = PARAMETROS_leContadorVendas();
#else
  vendasTotal = UNIDADES_TOTAL;
#endif  
  sprintf(linha,"Total  :%08d\n",vendasTotal);
  IMPRESSORA_enviaLinhas(pLinha,1);
  //Titulo arrecadacao
  IMPRESSORA_enviaLinhas((char**)TitulocontadorDinheiroVendas,2);  
  // Arrecadacao parcial
#ifndef _DEBUG_RELATORIO_    
  vendasParcial = PARAMETROS_leContadorArrecadacao();
#else
  vendasParcial = ARRECADACAO_PARCIAL;
#endif  
  sprintf(linha,"Parcial: %d.00\n",vendasParcial);
  IMPRESSORA_enviaLinhas(pLinha,1);    
  // Contador total
#ifndef _DEBUG_RELATORIO_   
  vendasTotal = PARAMETROS_leTotalizadorPermanente();
#else
  vendasTotal = ARRECADACAO_TOTAL;
#endif
  sprintf(linha,"Total  : %d.00\n",vendasTotal);
  IMPRESSORA_enviaLinhas(pLinha,1);  

  //Titulo arrecadacao
  IMPRESSORA_enviaLinhas((char**)TitulocomissaoPonto,2);    
  unsigned short int taxa = PARAMETROS_leComissaoPonto();
  unsigned int comissao = (vendasParcial*taxa)/100;
  
  bufferTexto[0] = ((taxa/1000)%10) + '0';
  bufferTexto[1] = ((taxa/100)%10) + '0';
  bufferTexto[2] = ((taxa/10)%10) + '0';
  bufferTexto[3] = ',';  
  bufferTexto[4] =  (taxa%10) + '0';
  bufferTexto[5] = '\0';
  
  sprintf(linha,"Percentual(%):%s\n",(char*)bufferTexto);
  IMPRESSORA_enviaLinhas(pLinha,1);    
  bufferTexto[0] = ((comissao/1000)%10) + '0';
  bufferTexto[1] = ((comissao/100)%10) + '0';
  bufferTexto[2] = ((comissao/10)%10) + '0';
  bufferTexto[3] = ',';
  bufferTexto[4] = (comissao%10)+ '0';
  bufferTexto[5] = 0;
  sprintf(linha,"Valor        :%s\n",bufferTexto);
  IMPRESSORA_enviaLinhas(pLinha,1);  

  unsigned int saldo = (vendasParcial*10)-comissao;
  bufferTexto[0] = ((saldo/100000)%10) + '0';  
  bufferTexto[1] = ((saldo/10000)%10) + '0';      
  bufferTexto[2] = ((saldo/1000)%10) + '0';    
  bufferTexto[3] = ((saldo/100)%10) + '0';
  bufferTexto[4] = ((saldo/10)%10) + '0';
  bufferTexto[5] = ',';
  bufferTexto[6] = (saldo%10) + '0';
  bufferTexto[7] = '\0';
  sprintf(linha,"Saldo        :%s\n",bufferTexto);
  IMPRESSORA_enviaLinhas(pLinha,1);  
  
  unsigned short int teste = PARAMETROS_leContadorPipocasTeste();
  bufferTexto[0] = ((teste/1000)%10) + '0';
  bufferTexto[1] = ((teste/100)%10) + '0';
  bufferTexto[2] = ((teste/10)%10) + '0';
  bufferTexto[3] = (teste%10) + '0';
  bufferTexto[4] = '\0';
  IMPRESSORA_enviaLinhas((char**)TituloTotalTeste,2);    
  sprintf(linha,"Preparo teste:%s\n",bufferTexto);
  IMPRESSORA_enviaLinhas(pLinha,1);    

  IMPRESSORA_enviaLinhas((char**)TituloIdentificao,2);
  sprintf(linha,"Serial:%08d",IU_getNumeroSerie());  
  IMPRESSORA_enviaLinhas(pLinha,1);    
    
  IMPRESSORA_enviaLinhas((char**)TituloHoraData,3);
  sprintf(linha,"\n%02d:%02d:%02d",hora,minuto,segundo);
  IMPRESSORA_enviaLinhas(pLinha,1);  
  sprintf(linha,"\n%02d/%02d/%04d",dia,mes,ano);
  IMPRESSORA_enviaLinhas(pLinha,1);
  
  //saldo, e inverter taxa e arrecadacao
  IMPRESSORA_enviaLinhas((char**)TituloAssinatura,11);     
  IMPRESSORA_enviaLinhas((char**)Rodape,10);   
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
