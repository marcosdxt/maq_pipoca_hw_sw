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
|       Arquivo            :  InterfaceUsuario.h
|       Descri��o          :  Menus de interface com o usu�rio
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  09/02/2015
|
|       Revis�es           :  1.0.0.0
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