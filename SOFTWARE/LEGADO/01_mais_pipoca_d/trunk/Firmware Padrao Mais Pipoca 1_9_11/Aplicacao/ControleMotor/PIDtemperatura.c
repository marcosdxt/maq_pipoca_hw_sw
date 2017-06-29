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
|       Arquivo            :  PItemperatura.c
|       Descri��o          :  Controle da temperatura
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  08/08/2015
|
|       Revis�es           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/

/***********************************************************************************
*       Includes
***********************************************************************************/
#include <math.h>
#include <nxp\iolpc1768.h>
#include "..\..\includes.h"
#include "PIDtemperatura.h"
#include "controleMotor.h"

/***********************************************************************************
*       Defini��es locais
***********************************************************************************/
#define K1      1.0
#define K2      0.5
#define K3      0.25

#define P1      50
#define P2      25      
#define P3      5

/***********************************************************************************
*       Vari�veis locais
***********************************************************************************/
extern unsigned char ligarResistencia;
unsigned short int PIDTEMP_tPre=50;
unsigned short int PIDTEP_reload=30;
static unsigned short int atraso=2400;

unsigned int PID_temperatura;
float PID_temperaturaF;
/***********************************************************************************
*       Implementa��o das fun��es
***********************************************************************************/

/***********************************************************************************
*       Descri��o       :       Faz o controle PID da temperatura
*                               da resist�ncia
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void PID_temp(void){
 
  PID_temperaturaF = AA_calculaTemperatura();
  PID_temperatura = (unsigned int)PID_temperaturaF; 
  
  if(ligarResistencia){    
    if(PID_temperatura<=ligarResistencia){
      SET_RESISTENCIA(1);          
    }
    else{
      SET_RESISTENCIA(0);          
    }             
  }  
  else{
    SET_RESISTENCIA(0);          
  }
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
