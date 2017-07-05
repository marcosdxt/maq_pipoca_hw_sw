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
|       Arquivo            :  DosadorMilho.h
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
#ifndef _DOSADOR_MILHO_H_
#define _DOSADOR_MILHO_H_



void DM_ini(void);
unsigned char DM_aplicaDosagem(void);
unsigned char DM_getSensor(void);


/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
#endif// _DOSADOR_MILHO_H_