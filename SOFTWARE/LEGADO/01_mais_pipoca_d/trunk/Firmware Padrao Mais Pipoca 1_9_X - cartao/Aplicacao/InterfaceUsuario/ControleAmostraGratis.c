/*__________________________________________________________________________________
|	Quark Tecnologia Eletr�nica Embarcada
|       
|       Itapema - SC
|       www.quarktee.com.br
| __________________________________________________________________________________
|
|       This source code was developed by Quark  and cannot be copied, in part 
|       or in whole, or used, except when legally licensed by Quark 
|       or its distributors.
|
|       Este c�digo � propriedade da Quark  e n�o pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Quark  ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo            :  ControleAmostraGratis.c
|       Descri��o          :  Gerenciador de gr�tis
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  11/03/2015
|
|       Revis�es           :  1.0.0.0
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
*       Defini��es locais
***********************************************************************************/
#define LED_GRATIS_PIN                  (0x01)<<4
#define LED_GRATIS_INI_PIN()            FIO1DIR|=LED_GRATIS_PIN
#define SET_LED_GRATIS(X)               X?(FIO1SET|=LED_GRATIS_PIN):(FIO1CLR|=LED_GRATIS_PIN)

/***********************************************************************************
*       Variaveis locais
***********************************************************************************/
unsigned int CA_tempoAmostras=0;

/***********************************************************************************
*       Fun��es locais
***********************************************************************************/


/***********************************************************************************
*       Implementa��o das fun��es
***********************************************************************************/

/***********************************************************************************
*       Descri��o       :       Inicializa��o do m�dulo gr�tis
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void CA_ini(void){
  
  LED_GRATIS_INI_PIN();  
}
/***********************************************************************************
*       Descri��o       :     Getter para o contador de validade do 
*                               per�odo de amostra gr�tis
*       Parametros      :     (nenhum)
*       Retorno         :     (unsigned int) contador de tempo das amostras
***********************************************************************************/
unsigned int CA_getTempo(void){
  
  return CA_tempoAmostras;
}
/***********************************************************************************
*       Descri��o       :       Setter para o contador de validade das amostras
*                               gratis  
*       Parametros      :       (unsigned int) tempo em horas
*       Retorno         :       nenhum
***********************************************************************************/
void CA_setTempoAmostras(unsigned int horas){
  
  CA_tempoAmostras = horas*3600;  
}
/***********************************************************************************
*       Descri��o       :       Tick de timer para o m�dulo gr�tis
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
*       Descri��o       :       Setter para o led gratis
*       Parametros      :       (unsigned char) flag
*       Retorno         :       nenhum
***********************************************************************************/
void CA_setterLedGratis(unsigned char flag){
  
  SET_LED_GRATIS(flag);
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/

