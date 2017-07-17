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
|       Arquivo            :  InterfaceUsuario.h
|       Descrição          :  Menus de interface com o usuário
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  09/02/2015
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/
#ifndef _INTERFACE_USUARIO_H_
#define _INTERFACE_USUARIO_H_

void IU_entry(void*pPar);
void IU_setterLeds(unsigned char led,unsigned char estado);
void IU_exibeNumeroSerieMaquina(void);
unsigned int IU_getNumeroSerie(void);
void IU_escreveTemperaturaResistenciaFree(unsigned char rpm);
unsigned char IU_getFalhaDosador(void);
unsigned char IU_getFalhaMotor(void);

#endif//_INTERFACE_USUARIO_H_
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/