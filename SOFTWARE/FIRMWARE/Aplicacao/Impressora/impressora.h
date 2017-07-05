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
|       Arquivo            :  Impressora.h
|       Descrição          :  módulo principal do sistema
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  14/07/2015
|
|       Revisões           :  1.0.0.0
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