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
|       Arquivo            :  ControleEmbalagem.h
|       Descrição          :  Biblioteca para controle do liberador
|                             de embalagem
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  17/02/2015
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/

#ifndef _CONTROLE_EMBALAGEM_H_
#define _CONTROLE_EMBALAGEM_H_



void CE_ini(void);
unsigned char CE_empurraEmbalagem(unsigned int tempo);
unsigned char CE_sensorEmbalagem(void);
unsigned char CE_mediaSinalPapel(void);
void CE_setterMotor(unsigned char flag);






#endif// _CONTROLE_EMBALAGEM_H_