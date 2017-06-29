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


/***********************************************************************************
*       Constantes
***********************************************************************************/
const char *cabecalho[]=
{  
  "\n",
  "*****************************\n",     
  "*         Mais PIPOCA       *\n",    
  "*****************************\n",       
  "*   Relatorio de Comissao   *\n",  
  "*****************************\n",    
};

const char *TitulocontadorVendas[]=
{
  "\n",
  "[Contador de vendas](unidades)\n"    
};

const char *TitulocontadorDinheiroVendas[]=
{
  "\n",
  "[Arrecadao em moeda](R$)\n"    
};

const char *TitulocomissaoPonto[]=
{
  "\n",
  "[Comissao do Ponto](R$)\n"    
};

const char *TituloIdentificao[]=
{
 "\n",
 "[Identificacao]\n"  
};

const char *Rodape[]=
{
  "\n",
  "*****************************\n",      
  "visite:\n",
  "www.maispipoca.com.br\n",
  "*****************************\n",    
  "\n",
  "\n",   
  "\n",
  "\n",  
  "\n"    
};

const char *TituloAssinatura[]=
{
"\n",    
"\n",    
"\n",  
"\n",  
"_____________________________\n",
"  Responsavel pelo ponto\n",
"\n"
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
  
  IMPRESSORA_enviaLinhas((char**)cabecalho,6);   
  
  // Titulo contador
  IMPRESSORA_enviaLinhas((char**)TitulocontadorVendas,2);  
  // Contador parcial
  vendasParcial = PARAMETROS_leContadorVendasParcial();
  sprintf(linha,"Parcial:%05d\n",vendasParcial);  
  IMPRESSORA_enviaLinhas(pLinha,1);
  // Contador total
  vendasTotal = PARAMETROS_leContadorVendas();
  sprintf(linha,"Total  :%08d\n",vendasTotal);
  IMPRESSORA_enviaLinhas(pLinha,1);
  //Titulo arrecadacao
  IMPRESSORA_enviaLinhas((char**)TitulocontadorDinheiroVendas,2);  
  // Arrecadacao parcial
  vendasParcial = PARAMETROS_leContadorArrecadacao();
  sprintf(linha,"Parcial: %d.00\n",vendasParcial);
  IMPRESSORA_enviaLinhas(pLinha,1);    
  // Contador total
  vendasTotal = PARAMETROS_leTotalizadorPermanente();
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
  
  bufferTexto[0] = ((saldo/1000)%10) + '0';
  bufferTexto[1] = ((saldo/100)%10) + '0';
  bufferTexto[2] = ((saldo/10)%10) + '0';
  bufferTexto[3] = ',';
  bufferTexto[4] = (saldo%10) + '0';
  bufferTexto[5] = '\0';
  sprintf(linha,"Saldo        :%s\n",bufferTexto);
  IMPRESSORA_enviaLinhas(pLinha,1);  

  IMPRESSORA_enviaLinhas((char**)TituloIdentificao,2);
  sprintf(linha,"Serial:%08d",IU_getNumeroSerie());  
  IMPRESSORA_enviaLinhas(pLinha,1);    
  
  //saldo, e inverter taxa e arrecadacao
  IMPRESSORA_enviaLinhas((char**)TituloAssinatura,7);     
  IMPRESSORA_enviaLinhas((char**)Rodape,10);   
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
