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
|       Arquivo            :  DosadorMilho.c
|       Descrição          :  Biblioteca para controle da dosador de
|                             milho
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  14/02/2015
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
#include "..\..\includes.h"
#include "DosadorMilho.h"

/***********************************************************************************
*       Definições locais
***********************************************************************************/
#define SAIDA_MOTOR                     (0x01)<<0
#define SET_SAIDA_MOTOR(X)              X?(FIO1SET|=SAIDA_MOTOR):(FIO1CLR|=SAIDA_MOTOR)
#define INI_SAIDA_MOTOR                 FIO1DIR|=SAIDA_MOTOR

#define ENTRADA_DOSE                    (0x01)<<15
#define GET_ENTRADA_DOSE()              (FIO1PIN&ENTRADA_DOSE)?0:1

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
*       Descrição       :       Inicialização da biblioteca
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void DM_ini(void){
   
  INI_SAIDA_MOTOR;  
}
/***********************************************************************************
*       Descrição       :       Ativa a saída do dosador de milho
*       Parametros      :       nenhum
*       Retorno         :       (unsigned char) maior do que zero se conseguir
*                                               realizar a operação, falha
*                                               caso não consiga detectar
*                                               o reed de porção
***********************************************************************************/
unsigned char DM_aplicaDosagem(void){
  unsigned short int timeout;
  
  timeout = 15000;
  SET_SAIDA_MOTOR(1);
  vTaskDelay(1500);
  
  do{  
    if(GET_ENTRADA_DOSE()){
      vTaskDelay(10);
      if(GET_ENTRADA_DOSE()){
        SET_SAIDA_MOTOR(0);      
        return 255;
      }       
    }
    
    vTaskDelay(1);
  }
  while(timeout--);
               
  SET_SAIDA_MOTOR(0);          
  return 0;
}
/***********************************************************************************
*       Descrição       :       Getter para a entrada de dose
*       Parametros      :       nenhum
*       Retorno         :       (unsigned char) maior do que zero 
*                                               quando detectar o sensor
*                                               de dose
***********************************************************************************/
unsigned char DM_getSensor(void){
  
    if(GET_ENTRADA_DOSE()){
      vTaskDelay(50);
      if(GET_ENTRADA_DOSE()){
        SET_SAIDA_MOTOR(0);      
        return 255;
      }       
    }
    
    return 0;
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/

