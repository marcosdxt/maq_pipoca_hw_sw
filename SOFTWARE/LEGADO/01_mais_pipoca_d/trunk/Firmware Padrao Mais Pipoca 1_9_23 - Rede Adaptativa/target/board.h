/*__________________________________________________________________________________
|	Marcos Aquino Projetos Exclusivos
|       
|       Balneário Camboriú - SC
|       www.aquino.eng.br
| __________________________________________________________________________________
|
|       This source code was developed by Chave Digital and cannot be copied, in part 
|       or in whole, or used, except when legally licensed by Chave Digital
|       or its distributors.
|
|       Este código é propriedade da Chave Digital e não pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Chave Digital ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo            :  board.h
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  09/02/2015
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/
#ifndef _BOARD_H_
#define _BOARD_H_

//----------------------------------------------------------------------------------
//      Definições para o brasil
//----------------------------------------------------------------------------------
#define PORTUGUES
#define FQ_REDE_60_HZ

//----------------------------------------------------------------------------------
//      Definições para os demais países da América Latina
//----------------------------------------------------------------------------------
//#define FQ_REDE_50_HZ
//#define ESPANHOL

void BOARD_ini(void);
void BOARD_timerHook(void);




#endif// _BOARD_H_
/************************************************************************************
*        Fim do arquivo
************************************************************************************/
