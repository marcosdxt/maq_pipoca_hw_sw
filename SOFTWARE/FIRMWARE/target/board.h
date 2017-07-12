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
|                             (20/05/2017) Alterado para rodar a vesão
|                                          do controlador PID que utiliza
|                                          soma para a integração e diferença
|                                          para o diferenciador
|                                          Removida a interrupção adicionar para
|                                          desligar o gate do TRIAC, agora ele
|                                          é desligado no cruzamento por zero
|
|
| __________________________________________________________________________________
*/
#ifndef _BOARD_H_
#define _BOARD_H_

//----------------------------------------------------------------------------------
// Pais
//----------------------------------------------------------------------------------
#define _BRASIL_
//#define _URUGUAI_
//#define _PARAGUAI_
//#define _PARAGUAI_60HZ_
//#define _URUGUAI_60HZ_
//----------------------------------------------------------------------------------
//      Definições para o brasil
//----------------------------------------------------------------------------------
#ifdef _BRASIL_
  #define PORTUGUES
  #define FQ_REDE_60_HZ
  #define STR_VERSAO_BOARD      "     1.11.2-BR  "
#endif

#ifdef _URUGUAI_
  #define FQ_REDE_50_HZ
  #define ESPANHOL
  #define STR_VERSAO_BOARD      "     1.11.2-UY  "
#endif

#ifdef _PARAGUAI_
  #define FQ_REDE_50_HZ
  #define ESPANHOL
  #define STR_VERSAO_BOARD      "     1.11.2-PY  "
#endif

#ifdef _PARAGUAI_60HZ_
  #define FQ_REDE_60_HZ
  #define ESPANHOL
  #define STR_VERSAO_BOARD      "  1.11.2-PYBR   "
#endif

#ifdef _URUGUAI_60HZ_
  #define FQ_REDE_60_HZ
  #define ESPANHOL
  #define STR_VERSAO_BOARD      "  1.11.2-UYBR   "
#endif
//----------------------------------------------------------------------------------
//      Definições para os demais países da América Latina
//----------------------------------------------------------------------------------


void BOARD_ini(void);
void BOARD_timerHook(void);




#endif// _BOARD_H_
/************************************************************************************
*        Fim do arquivo
************************************************************************************/
