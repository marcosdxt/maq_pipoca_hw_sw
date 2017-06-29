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
|       Arquivo            :  ControleAmostraGratis.c
|       Descrição          :  Gerenciador de grátis
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  11/03/2015
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
#include "controleAmostraGratis.h"

/***********************************************************************************
*       Definições locais
***********************************************************************************/
#define LED_GRATIS_PIN                  (0x01)<<4
#define LED_GRATIS_INI_PIN()            FIO1DIR|=LED_GRATIS_PIN
#define SET_LED_GRATIS(X)               X?(FIO1SET|=LED_GRATIS_PIN):(FIO1CLR|=LED_GRATIS_PIN)

/***********************************************************************************
*       Variaveis locais
***********************************************************************************/
unsigned int CA_tempoAmostras=0;

/***********************************************************************************
*       Funções locais
***********************************************************************************/


/***********************************************************************************
*       Implementação das funções
***********************************************************************************/

/***********************************************************************************
*       Descrição       :       Inicialização do módulo grátis
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void CA_ini(void){
  
  LED_GRATIS_INI_PIN();  
}
/***********************************************************************************
*       Descrição       :     Getter para o contador de validade do 
*                               período de amostra grátis
*       Parametros      :     (nenhum)
*       Retorno         :     (unsigned int) contador de tempo das amostras
***********************************************************************************/
unsigned int CA_getTempo(void){
  
  return CA_tempoAmostras;
}
/***********************************************************************************
*       Descrição       :       Setter para o contador de validade das amostras
*                               gratis  
*       Parametros      :       (unsigned int) tempo em horas
*       Retorno         :       nenhum
***********************************************************************************/
void CA_setTempoAmostras(unsigned int horas){
  
  CA_tempoAmostras = horas*3600;  
}
/***********************************************************************************
*       Descrição       :       Tick de timer para o módulo grátis
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void CA_tickTimer(void){
  static unsigned char contador=1;
  static unsigned char toggle=0;
  
  if(CA_tempoAmostras){
    if(contador)
      contador--;
    else{
      toggle = ~toggle;
      contador=250;        
      SET_LED_GRATIS(toggle);      
    }  
  }
}
/***********************************************************************************
*       Descrição       :       Setter para o led gratis
*       Parametros      :       (unsigned char) flag
*       Retorno         :       nenhum
***********************************************************************************/
void CA_setterLedGratis(unsigned char flag){
  
  SET_LED_GRATIS(flag);
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/

