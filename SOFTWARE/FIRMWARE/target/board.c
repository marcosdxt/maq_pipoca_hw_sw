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
|       Arquivo            :  main.c
|       Descri��o          :  m�dulo principal do sistema
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  09/02/2015
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
#include "board.h"
#include "mapeamento_gpios.h"
#include "..\includes.h"

/***********************************************************************************
*       Variaveis
***********************************************************************************/
extern unsigned int IU_contadorPropaganda;
extern unsigned short int MP_timeoutCounter;
extern unsigned int IU_ciclosPropaganda;
unsigned int BOARD_tempoPiscadas=0;
extern unsigned int CA_tempoAmostras;
extern unsigned short int TELET_silentTime;

/***********************************************************************************
*       Fun��es locais
***********************************************************************************/
void BOARD_umSegundo(void);
extern void MP_tickTimer(void);
/***********************************************************************************
*       Implementa��o das fun��es
***********************************************************************************/

/***********************************************************************************
*       Descri��o       :       Inicializa��o do m�dulo
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void BOARD_ini(void){
  
  INI_LED_STATUS_DIR();  
  LIB74595_ini();
  MEMORYWRAPPER_init();
  RTC_init();
  AA_ini();
  MU_ini();
  DM_ini();
  CE_ini();
  PLAYERWAVE_ini();
}
/***********************************************************************************
*       Descri��o       :       Timerhook 
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void BOARD_timerHook(void){
  static unsigned short int umSegundo=1;  
 
  if(!--umSegundo){
    umSegundo = 1000;
    BOARD_umSegundo();
    if(CA_tempoAmostras)
      CA_tempoAmostras--;
  }
    
  TECLADO_timerTick();
  ES_tickTimer();
  CCTALK_tick();
  PROTOCOLO_timerTick();
  MP_tickTimer();
  
  if(IU_contadorPropaganda)
    IU_contadorPropaganda--;
  
  if(MP_timeoutCounter)
    MP_timeoutCounter--;
  
  if(BOARD_tempoPiscadas)
    BOARD_tempoPiscadas--;
  
  if(TELET_silentTime)
    TELET_silentTime--;
  
  CA_tickTimer();
}
/***********************************************************************************
*       Descri��o       :       Fun��o executada a cada um segundo
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void BOARD_umSegundo(void){
  static unsigned char flag=0;
  static unsigned char umMinuto=60;
  
  SET_LED_STATUS(flag);
  flag = !flag;
  
  // flag de tempo
  // de um minuto
  if(!--umMinuto){
    umMinuto = 60;
    if(IU_ciclosPropaganda)
      IU_ciclosPropaganda--;  
  }
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/


