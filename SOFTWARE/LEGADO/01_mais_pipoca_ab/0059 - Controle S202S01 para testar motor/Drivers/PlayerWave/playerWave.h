/*__________________________________________________________________________________
|	Chave Digital Tecnologia Eletronica Ltda. 
|       
|       Balenário Camboriú - SC
|       www.chavedigital.com.br
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
|       Arquivo            :  PlayerWave.h
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  22/02/2014
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/

#ifndef _PLAYER_WAVE_H_
#define _PLAYER_WAVE_H_


void PLAYERWAVE_ini(void);
void PLAYERWAVE_iniciaMusica(unsigned char indice,unsigned char modo);
void PLAYER_interrompeMusica(void);
unsigned char PLAYERWAVE_verificaToque(void);


/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
#endif//_PLAYER_WAVE_H_