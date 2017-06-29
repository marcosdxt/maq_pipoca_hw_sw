/*__________________________________________________________________________________
|     	Chave Digital Tecnologia Eletronica Ltda. 
|       
|       Balneário Camboriú - SC
|       www.chavedigital.com.br
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
|       Produto            :  Includes do projeto Apus MP
|                             
|       Arquivo            :  includes.h
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  22/05/2014
|
|       Revisões           :  1.0 - primeira versão
|
|
| __________________________________________________________________________________
*/

#ifndef _INCLUDES_H_
#define _INCLUDES_H_

// Cabeçalhos para o kernel
// do RTOS
#include "kernel\freertosconfig.h"
#include "kernel\freertos.h"
#include "kernel\list.h"
#include "kernel\task.h"
#include "kernel\semphr.h"
#include "kernel\queue.h"

#include "target\mapeamento_gpios.h"
#include "target\board.h"

#include "drivers\wh1602\wh1602.h"
#include "drivers\wh1602_2\wh1602_2.h"
#include "drivers\74595\74595.h"
#include "drivers\sst25vf016b\memoryWrapper.h"
#include "drivers\sst25vf016b\sst25vf016b.h"
#include "drivers\teclado\teclado.h"
#include "drivers\amostragemAnalogica\amostraanalogica.h"
#include "drivers\uarts\uart.h"
#include "drivers\cctalk\cctalk.h"
#include "drivers\protocolo\protocolo.h"
#include "drivers\playerWave\playerWave.h"
#include "drivers\watchdog\watchdog.h"
#include "drivers\rtc\rtc.h"

#include "aplicacao\ControleMotor\controleMotor.h"
#include "aplicacao\ControleMotor\DosadorMilho.h"
#include "aplicacao\ControleMotor\ControleEmbalagem.h"
#include "aplicacao\ControleMotor\PIDtemperatura.h"
#include "aplicacao\parametros\parametros.h"
#include "aplicacao\interfaceusuario\monitorPagamentos.h"
#include "aplicacao\interfaceUsuario\menuConfiguracaoSistema.h"
#include "aplicacao\interfaceUsuario\animacaoLeds.h"
#include "aplicacao\interfaceUsuario\controleAmostraGratis.h"
#include "aplicacao\interfaceUsuario\pipocaTeste.h"
#include "aplicacao\impressora\impressora.h"
#include "aplicacao\relatorioVendas\relatoriovendas.h"
#include "aplicacao\telet\telet.h"
#include "aplicacao\assinatura\assinatura.h"
#include "aplicacao\horimetro\horimetro.h"

#include "mapa_memoria.h"
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
#endif//_INCLUDES_H_