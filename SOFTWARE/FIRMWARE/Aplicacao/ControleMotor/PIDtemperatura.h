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
|       Arquivo            :  PItemperatura.h
|       Descrição          :  Controle da temperatura
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  08/08/2015
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/
#ifndef _PID_TEMPERATURA_H_
#define _PID_TEMPERATURA_H_



void PID_temp(void);
void PITEMP_setterPreAquecimento(unsigned short int preAquecimento);
void PITEMP_setterCiclosControle(unsigned short int ciclos);













/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
#endif// _PID_TEMPERATURA_H_