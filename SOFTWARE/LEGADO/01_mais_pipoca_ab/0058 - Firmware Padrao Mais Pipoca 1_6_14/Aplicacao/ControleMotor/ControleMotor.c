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
|       Arquivo            :  ControleMotor.c
|       Descrição          :  Biblioteca para controle da velocidade do
|                             motor universal
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  14/02/2015
|                            
|       Revisões           :  1.0.0.0
|                             1.1(01/05/2015) alterado para controle proporcional da velocidade do motor
|                             1.2(02/05/2015) alterado para rampa de temperatura no controle 
|                                             da resistência.
|
|
| __________________________________________________________________________________
*/

/***********************************************************************************
*       Includes
***********************************************************************************/
#include <nxp\iolpc1768.h>
#include "controleMotor.h"
#include "..\..\includes.h"
#include "PIDtemperatura.h"




/***********************************************************************************
*       Variaveis locais
***********************************************************************************/
unsigned short int MU_ciclos=0;
unsigned short int MU_frequencia;
unsigned char MU_ligaSSR=0;
unsigned char contadorSSR=0;
unsigned int periodoRotacaoMotor=0xFFFF;
unsigned int presetRotacao=0;
unsigned char ligarResistencia;
unsigned char ciclosResistencia=0;
unsigned char MU_reloadCiclosResistencia=1;
unsigned int bufferRPM[TAMANHO_BUFFER_RPM]={0};
unsigned int CM_periodoCapturadoMotor;
unsigned short int CM_timeOutCounter=RELOAD_TIMEOUT_RRPM;
unsigned long int CM_temp;
unsigned int CM_presetRPM=0;
unsigned char CM_ligaMotor=0;
unsigned short int CM_fatorAtrasoResistencia=0;
unsigned short int CM_bufferRotacao[TAM_BUF_VELOCIDADE]={0};
unsigned char CM_cntCtrlMotor=CICLOS_INICIO_MOTOR;
unsigned char CM_cicloCounter=0;
unsigned short int CM_rampaAceleracao=0;
unsigned short int presetRPMnovo;
unsigned char novoPresetMotor;

/***********************************************************************************
*       Funções locais
***********************************************************************************/
void MU_controleTemperatura(void);

/***********************************************************************************
*       Implementação das funções
***********************************************************************************/

/***********************************************************************************
*       Descrição       :       Inicialização da biblioteca de controle
*                               do motor universal
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MU_ini(void){
  
  INI_PINS;

  //--------------------------------------------------------------------------//
  //                        Habilita a interrupção                            //
  //                        no NVIC                                           //
  //--------------------------------------------------------------------------//
  
  PCONP_bit.PCTIM1 = 1;  
  IP0_bit.PRI_2 = 0;  
  SETENA0_bit.SETENA2 = 1; // Habilitação da interrupção do timer 1
  
  T1TCR_bit.CE = 1;   // Contador Habilitado
  T1TCR_bit.CR = 1;   // Realiza o reset
  T1TCR_bit.CR = 0;   // do contador
  
  T1CTCR_bit.CTM = 0x00;
  T1PR = PR_TIMER;
   
  PINSEL3_bit.P1_19 = 3;
  
  T1CCR_bit.CAP1RE = 1; // Borda de subida
  T1CCR_bit.CAP1FE = 0; // Borda de descida  
  T1CCR_bit.CAP1I = 1;  // Interrupção por evento no canal  
  //-------------------------------------------------------------------------//
  //                    Inicialização do timer 2                             //
  //-------------------------------------------------------------------------//
  PCONP_bit.PCTIM2 = 1; // Energiza o periférico
  PCLKSEL1_bit.PCLK_TIMER2 = 1; // Seleção do clock
  
  IP0_bit.PRI_3 = 0;  
  SETENA0_bit.SETENA3 = 1; // Habilitação da interrupção do timer 1
  
  T2TCR_bit.CE = 1;   // Contador Habilitado
  T2TCR_bit.CR = 1;   // Realiza o reset
  T2TCR_bit.CR = 0;   // do contador
  
  T2CTCR_bit.CTM = 0x00;
  T2PR = PR_TIMER;  
  
  //-------------------------------------------------------------------------//
  //                   Match 0 para o dimmer do motor                        //
  //-------------------------------------------------------------------------//  
  T2MCR_bit.MR0I = 0;
  T2MCR_bit.MR0R = 0;
  T2EMR_bit.EM0  = 0; // Não altera nenhum pino    
  T2EMR_bit.EMC0 = 0;
  
  T2MR0 = T2TC+1000;
  //-------------------------------------------------------------------------//
  //               Math 1 para o dimmer da resistência                       //
  //-------------------------------------------------------------------------//
  T2MCR_bit.MR1I = 0;
  T2MCR_bit.MR1R = 0;
  T2EMR_bit.EM1  = 0; // Não altera nenhum pino    
  T2EMR_bit.EMC1 = 0;
  
  T2MR1 = T2TC+1000;    
  //-------------------------------------------------------------------------//
  //              Fim da inicialização do match                              //
  //-------------------------------------------------------------------------//
  
  // --------------------------
  // Interrupção externa
  // --------------------------
  PINSEL4_bit.P2_11 = 1;
  EXTMODE_bit.EXTMODE1 = 1;
  EXTPOLAR_bit.EXTPOLAR1 = 1;    
  SETENA0_bit.SETENA19 = 1;   
}
/***********************************************************************************
*       Descrição       :       Interrupção do timer 2
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MU_mathTimer2(void){
  static unsigned char pino=0;
  static unsigned char resistencia=0;
  
  //-----------------------------------------------//
  //               Controle do motor               //
  //-----------------------------------------------//
  if(T2IR_bit.MR0INT){
    if(!pino){
      SET_SSR(1);
      pino = 1;
      SET_ATRASO(10);//+ ou - 50uS
    }
    else{
      SET_SSR(0);
      pino = 0;
      T2MCR_bit.MR0I = 0;
    }  
    T2IR_bit.MR0INT = 1;
    CLRPEND0_bit.CLRPEND3 = 1;
  }
  
  //-----------------------------------------------//
  //             Controle da resistência           //
  //-----------------------------------------------//
  if(T2IR_bit.MR1INT){
    if(!resistencia){
      SET_RESISTENCIA(1);
      resistencia = 1;
      SET_ATRASO_RES(100);//+ ou - 50uS
    }
    else{
      SET_RESISTENCIA(0);
      resistencia = 0;
      T2MCR_bit.MR1I = 0;
    }  
    T2IR_bit.MR1INT = 1;
    CLRPEND0_bit.CLRPEND3 = 1;
  }  
}
/***********************************************************************************
*       Descrição       :       Função que faz o controle da velocidade do 
*                               motor dentro do timer
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
#pragma inline
void MU_controleVelocidade(void){
  static unsigned int iTerm=2500;
  unsigned int rotacao;
  int erro=0;   
  unsigned char dec=1;
  
  if(CM_ligaMotor){
    SET_ATRASO(CM_presetRPM);          
  else
    SET_SSR(0);  
}
/***********************************************************************************
*       Descrição       :       Interrupção do EINT1
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MU_eintInterrupt(void){  
  
  MU_controleVelocidade();
  
  MU_ciclos++;  
  PID_temp();

  EXTINT_bit.EINT1 = 1;
  CLRPEND0_bit.CLRPEND19 = 1;
}
/***********************************************************************************
*       Descrição       :       Controle da temperatura
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MU_controleTemperatura(void){
  /*
  static float temperatura;  
  static float ultimaTemperatura=0;
  static unsigned short int fator=500;
  unsigned char ciclos=1;
  int erro;
  
  if(ligarResistencia){
    
    temperatura = AA_calculaTemperatura();
    
    if(temperatura<(float)ligarResistencia){
      
      // Calcula o novo
      // fator a cada 60 ciclos
      if(!--ciclos){        
        ciclos = 60;
        erro = (int)(temperatura - ultimaTemperatura);        
        //erro = 1 - erro;
        fator = (unsigned short int)((CM_fatorAtrasoResistencia+500)-((erro-1)*150));        
        fator+= erro;
                        
        if(fator>2400)
          fator = 2400;
        if(fator<500)
          fator = 500;                
      }
      
      // A cada semi-ciclo dispara
      // o triac com o fator calculado dentro da 
      // janela de um segundo      
      SET_ATRASO_RES(fator);
     }
    else{
      fator = CM_fatorAtrasoResistencia;
      SET_RESISTENCIA(0);    
    }
    
    ultimaTemperatura = temperatura;
  }
  else
    SET_RESISTENCIA(0);  
  **/
}
/***********************************************************************************
*       Descrição       :       Tick de timer para a biblioteca de controle
*                               da velocidade do motor
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MU_timerTick(void){  
  static unsigned short int contador=1;
  
  //CM_cicloCounter++;  
  
  if(!--contador){
    MU_frequencia = MU_ciclos;
    MU_ciclos = 0;
    contador = 500;
  }    
  
  // Timeout na captura
  // da rotação do motor
  if(CM_timeOutCounter){
    if(CM_timeOutCounter==1){
      for(unsigned char i=0;i<TAM_BUF_VELOCIDADE;i++)
        CM_bufferRotacao[i] = 0;
    }
    CM_timeOutCounter--;
  }    
  
}
/***********************************************************************************
*       Descrição       :       Getter para a frequência da rede elétrica
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
unsigned short int MU_getFrequenciaRede(void){
  
  return MU_frequencia;
}
/***********************************************************************************
*       Descrição       :       Interrupção do capture
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MU_captureInterrupt(void){
  static unsigned int ultimaLeitura=0;
  unsigned int larguraAtual;
  unsigned int tmp;
  static unsigned int ponteiroLeitura=0;
  
  larguraAtual = tmp = T1CR1;
  
  if(larguraAtual>ultimaLeitura)         
    larguraAtual-=ultimaLeitura;    
  else
    larguraAtual = (0xFFFFFFFF - ultimaLeitura) + larguraAtual;
    
  ultimaLeitura = tmp;
  
  T1IR_bit.CR1INT = 1;
  CLRPEND0_bit.CLRPEND2 = 1;
  
  CM_periodoCapturadoMotor = larguraAtual;
  CM_timeOutCounter=RELOAD_TIMEOUT_RRPM;
  
  // Calcula a rotação a cada captura
  // em RPM
  CM_temp = CM_periodoCapturadoMotor;
  CM_temp *= 3;
  CM_temp = 60000000/CM_temp;
    
  CM_bufferRotacao[ponteiroLeitura] = CM_temp;
  ponteiroLeitura= (ponteiroLeitura+1) %TAM_BUF_VELOCIDADE;   
}
/***********************************************************************************
*       Descrição       :       Setter para a rotação do motor
*       Parametros      :       (unsigned int) rpm do motor
*       Retorno         :       nenhum
***********************************************************************************/
void MU_setRPM(unsigned int rpm){
  double tmp;
  
  if(!rpm){
    CM_ligaMotor=0;
    SET_GERAL(0);
  }
  else{
    SET_GERAL(1);    
    presetRPMnovo = rpm;    
    CM_presetRPM = rpm;
    CM_ligaMotor = 1;
    CM_cntCtrlMotor=1;
  }
}
/***********************************************************************************
*       Descrição       :       Setter para o controle da resistencia
*       Parametros      :       (unsigned int) temperatura
*       Retorno         :       nenhum
***********************************************************************************/
void MU_setTemperatura(unsigned int temperatura,unsigned short int ciclos){
  
  ligarResistencia = temperatura;  
}
/***********************************************************************************
*       Descrição       :       Lê os RPMs do motor
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) RPM atual do motor
***********************************************************************************/
unsigned int MU_getRPMmedido(void){  
  unsigned long int media=0;
  
  if(!CM_periodoCapturadoMotor)
    return 0;
  
  for(unsigned char i=0;i<TAM_BUF_VELOCIDADE;i++)
    media += CM_bufferRotacao[i];
  
  return (unsigned int)(media>>DIV_MEDIA_VEL);//CM_temp;
}
/***********************************************************************************
*       Descrição       :       Setter para o relé do geral do neutro
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MU_setterGeralNeutro(unsigned char estado){
  
 SET_GERAL(estado);
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
