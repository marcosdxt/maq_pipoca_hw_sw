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
|       Arquivo            :  Impressora.c
|       Descrição          :  módulo principal do sistema
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  14/07/2015
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*       Includes
***********************************************************************************/
#include <nxp\iolpc1768.h>
#include <string.h>
#include "..\..\includes.h"

/***********************************************************************************
*       Definições locais
***********************************************************************************/


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
*       Descrição       :       Envia um vetor de strings para a impressora
*       Parametros      :       (char*[][])
*                               (unsigned char*) quantidade de linha
*       Retorno         :       nenhum
***********************************************************************************/
void IMPRESSORA_enviaLinhas(char *linhas[],unsigned char qtd){
  
  if(PARAMETROS_carregaTipoImpressora()==EXTECH)
    UART_init(3,38400,NULL,NULL,NULL);      
  else
    UART_init(3,115200,NULL,NULL,NULL);      
        
  for(unsigned char i=0;i<qtd;i++){    
    PROTOCOLO_enviaDadosDireto((unsigned char*)&linhas[i][0],strlen(&linhas[i][0]));
    while(PROTOCOLO_bytesNoBufferTx());
    vTaskDelay(50);
  }    
  
  UART_init(3,36600,NULL,NULL,NULL);      
}
/***********************************************************************************
*       Descrição       :       Imprimir uma página de teste
*       Parametros      :       nenhu
*       Retorno         :       nenhum
***********************************************************************************/
void IMPRESSORA_paginaTeste(void){      
  char *linhas[3]={"\rALMEIDA TECHNOLOGY INDUSTRIA E COMERCIO\n","\n","CPU Mais Pipoca"};
  
  IMPRESSORA_enviaLinhas(linhas,3);  
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/