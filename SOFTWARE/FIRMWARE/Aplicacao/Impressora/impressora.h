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
|       Arquivo            :  Impressora.h
|       Descri��o          :  m�dulo principal do sistema
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  14/07/2015
|
|       Revis�es           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/
#ifndef _IMPRESSORA_H_
#define _IMPRESSORA_H_


void IMPRESSORA_enviaLinhas(char *linhas[],unsigned char qtd);
void IMPRESSORA_dobra_fonte(void);
void IMPRESSORA_fonte_tamanho_normal(void);
void IMPRESSORA_enviaLinha(char *linhas);

#endif//_IMPRESSORA_H_