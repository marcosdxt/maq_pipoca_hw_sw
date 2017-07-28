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
|       Arquivo            :  board.h
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  09/02/2015
|
|       Revis�es           :  1.0.0.0
|                             (20/05/2017) Alterado para rodar a ves�o
|                                          do controlador PID que utiliza
|                                          soma para a integra��o e diferen�a
|                                          para o diferenciador
|                                          Removida a interrup��o adicionar para
|                                          desligar o gate do TRIAC, agora ele
|                                          � desligado no cruzamento por zero
|                             (21/07/2017) Removidas as op��es do usu�rio de 
|                                          mudar as configura��o da temperatura                                             
|                                          final e a rampa. Inseridas mudan�as
|                                          para o envio correto dos dados para a
|                                          Telemetria. Refeito o TELET para
|                                          funcionar como uma task separada,
|                                          recebendo informa��es atrav�s de uma 
|                                          fila. Ganhos PID agora s�o configuraveis.
|                             (24/07/2017) Corre��o do envio da informa��o Cart�o
|                                          Total para a Telemetria.
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
//      Defini��es para o brasil
//----------------------------------------------------------------------------------
#ifdef _BRASIL_
  #define PORTUGUES
  #define FQ_REDE_60_HZ
  #define STR_VERSAO_BOARD      "     1.12.3-BR  "
  #define STR_VERSAO_BOARD_TELEMETRIA      "1.12.3-BR"
#endif

#ifdef _URUGUAI_
  #define FQ_REDE_50_HZ
  #define ESPANHOL
  #define STR_VERSAO_BOARD      "     1.12.3-UY  "
  #define STR_VERSAO_BOARD_TELEMETRIA      "1.12.3-UY"
#endif

#ifdef _PARAGUAI_
  #define FQ_REDE_50_HZ
  #define ESPANHOL
  #define STR_VERSAO_BOARD      "     1.12.3-PY  "
  #define STR_VERSAO_BOARD_TELEMETRIA      "1.12.3-PY"
#endif

#ifdef _PARAGUAI_60HZ_
  #define FQ_REDE_60_HZ
  #define ESPANHOL
  #define STR_VERSAO_BOARD      "  1.12.3-PYBR   "
  #define STR_VERSAO_BOARD_TELEMETRIA      "1.12.3-PYBR"
#endif

#ifdef _URUGUAI_60HZ_
  #define FQ_REDE_60_HZ
  #define ESPANHOL
  #define STR_VERSAO_BOARD      "  1.12.3-UYBR   "
  #define STR_VERSAO_BOARD_TELEMETRIA      "1.12.3-UYBR"
#endif
//----------------------------------------------------------------------------------
//      Defini��es para os demais pa�ses da Am�rica Latina
//----------------------------------------------------------------------------------

void BOARD_ini(void);
void BOARD_timerHook(void);


#endif// _BOARD_H_
/************************************************************************************
*        Fim do arquivo
************************************************************************************/
