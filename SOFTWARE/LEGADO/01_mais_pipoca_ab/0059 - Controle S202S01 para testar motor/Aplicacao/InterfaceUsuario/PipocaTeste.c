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
|       Arquivo            :  Pipoca.c
|       Descrição          :  Faz a acumulação do dinheiro recebido
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  04/05/2015
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/

/***********************************************************************************
*       Include
***********************************************************************************/
#include "..\..\includes.h"
#include "PipocaTeste.h"

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
*       Descrição       :       Menu para preparar uma pipoca
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void PIPOCATESTE_prepara(void){
    
  if(!MCS_telaSenha(1234)){
    HD44780_clearText();
    return;
  }      
  
  IU_preparaPipoca();
  unsigned int valorPipoca = PARAMETROS_leParametro(VALOR_PIPOCA);
          
  IU_limpezaPipoqueira(valorPipoca);
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
