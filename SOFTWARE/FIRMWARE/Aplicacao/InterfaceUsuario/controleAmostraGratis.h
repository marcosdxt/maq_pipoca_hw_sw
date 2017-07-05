/*__________________________________________________________________________________
|	Marcos Aquino Projetos Exclusivos
|       
|       Balne�rio Cambori� - SC
|       www.aquino.eng.br
| __________________________________________________________________________________
|
|       This source code was developed by Chave Digital and cannot be copied, in part 
|       or in whole, or used, except when legally licensed by Chave Digital
|       or its distributors.
|
|       Este c�digo � propriedade da Chave Digital e n�o pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Chave Digital ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo            :  ControleAmostraGratis.h
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
#ifndef _CONTROLE_AMOSTRA_GRATIS_H_
#define _CONTROLE_AMOSTRA_GRATIS_H_

void CA_ini(void);
unsigned int CA_getTempo(void);
void CA_setTempoAmostras(unsigned int horas);
void CA_tickTimer(void);
void CA_setterLedGratis(unsigned char flag);
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
#endif//_CONTROLE_AMOSTRA_GRATIS_H_