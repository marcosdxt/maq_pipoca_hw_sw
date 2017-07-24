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
|       Arquivo            :  InterfaceUsuario.c
|       Descrição          :  Menus de interface com o usuário
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  09/02/2015
|
|       Revisões           :  1.0.0.0
|
|                             (23/08/2016) Alterado para enviar os dados TELET
|                                          quando houver um bloqueio por falha
|                                          Foram inseridas chamadas para envio
|                                          no loop de falha da resistência e no
|                                          de sinalização de falha
|                             (20/05/2017) Alterado a forma como faz a limpeza da
|                                          máquina durante o preparo. Agora utiliza
|                                          a função serve_pipoca.
|                                          Dentro do menu pipoca_teste havia uma 
|                                          chamada errada para a limpeza, onde a velocidade
|                                          estava com o parâmetro do valor da pipoca
|                                          Criado menu para reiniciar a senha mestre,
|                                          para utilizá-lo é necessário usar o gerador
|                                          de contra-senha
| __________________________________________________________________________________
*/

/***********************************************************************************
*       Includes
***********************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "..\..\includes.h"


/***********************************************************************************
*       Definições
***********************************************************************************/
#define INVERTE_CICLO                                           2
#define CICLOS_PIPOCA                                           2
#define CICLO_PRE_AQUECIMENTO                                   10000
#define RELOAD_INATIVIDADE_UMIDADE                              30*60*1000

#ifdef PORTUGUES
  #define STRING_EMPRESA                                          "  GRUPO ALTECH  "
  #define STRING_VERSAO                                           STR_VERSAO_BOARD
#endif

#ifdef ESPANHOL
  #define STRING_EMPRESA                                          "  GRUPO ALTECH  "
  #define STRING_VERSAO                                           STR_VERSAO_BOARD
#endif

                  
#define INTERVALO_MENSAGEM_PRE_BLOQUEIO                         3*60*1000

#define MAX_COMPENSADOR_TEMPERATURA                             10
#define MAX_COMPENSADOR_ROTACAO                                 500
#define TEMPO_COMPENSADOR                                       2*60*1000
#define TEMPO_SAIDA_COMPENSADOR                                 2*60*1000

/***********************************************************************************
*       Constantes
***********************************************************************************/
// const unsigned int numeroSerie=147;

/***********************************************************************************
*       Enumerações
***********************************************************************************/
enum eESTADO_LEDS{
  NORMAL,
  RETIRAR_COPO,
  DOBRAR_COPO,
  INSERIR_COPO,
  FORA_SERVICO  
};

/***********************************************************************************
*       Variaveis
***********************************************************************************/
unsigned char bufferTeste[16],bufferLeitura[16];
unsigned int IU_contadorPropaganda=0;
unsigned int IU_ciclosPropaganda=1;
unsigned char tmp=4;
unsigned char IU_toogleDinheiro=0;
extern unsigned int BOARD_tempoPiscadas;
unsigned short int UI_ciclosFatorTemperatura=0;
unsigned char IU_contadorTemperatura=0;
unsigned int IU_propagandaAlmeida=0;
unsigned char IU_creditoPipoca=0;
unsigned char IU_falhaMotor=0;
unsigned char IU_dosador=0;
unsigned short int IU_tempoRefreshSegundoLCD=1;
unsigned char IU_estadoConexaoTelemetria;
unsigned int IU_contadorResfriamento=CICLO_PRE_AQUECIMENTO;
unsigned int IU_inatividadeProcesso=60000;
unsigned int IU_tempoInfoHorimetro=0;

unsigned int IU_contadorCompensador=0;
unsigned int IU_contadorSaidaCompensador=0;
unsigned char IU_compensadorTemperatura=0;
unsigned short int IU_compensadorRotacao=0;
/***********************************************************************************
*       Funções
***********************************************************************************/
void IU_desenhaPrecoPipoca(void);
void IU_escreveDinheiroInserido(void);
void IU_escreveTemperaturaResistencia(unsigned char rpm);
unsigned char IU_preparaPipoca(void);
void IU_limpezaPipoqueira(unsigned short int velocidade);
void IU_propaganda(void);
void IU_exibeContadorVendas(void);
void IU_exibeTempoUsuario(unsigned short int tempo);
unsigned char IU_autoTesteHardware(void);
void IU_atualizaTotalizadores(unsigned int valorPipoca);
void IU_mensagemIniciandoPreparo(void);
unsigned char IU_verificacaoCiclica(void);
void IU_telaConfiguraIntervaloPropaganda(void);
unsigned char IU_preAquecimento(unsigned short int velocidade);
void IU_escreveDinheiroSaldo(void);
void IU_servePipoca(unsigned int inicial,unsigned int final);
void IU_rampaTemperatura(unsigned short int *setPoint,unsigned short int fator,
                         unsigned short int maximo);
void IU_mensagemClienteStandby(void);
void IU_escreveErros(unsigned char codigo);
void IU_oscilaFitaPreparacao(void);
void IU_enviaTelemetria(void);
void IU_stampEstadoConexaoModuloTelemetria(void);
unsigned char IU_interfaceResfriamento(unsigned short int temperatura);
void IU_cicloPosResfriamento(void);
void IU_mensagensAguardaAquecimento(unsigned char indice);
void IU_processoSecagem(void);
unsigned char IU_verificacaoInicialResistencia(void);
void IU_cicloCompensador(void);
void IU_resfriamentoPanela(void);
void IU_exibeNumeroSerie(void);
unsigned char IU_getFalhaDosador(void);
unsigned char IU_getFalhaMotor(void);

/***********************************************************************************
*       Implementação das funções
***********************************************************************************/

/***********************************************************************************
*       Descrição       :       Função principal da interface com o usuário
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_entry(void*pPar){
  eTECLA tecla;
  unsigned char moedeiroTravado=0;  
    
  HD44780_init(LCD_DISPLAY_8X5 | LCD_2_LINHAS,
               LCD_DISPLAY_LIGADO | LCD_CURSOR_DESLIGADO | LCD_CURSOR_FIXO);  
  
  HD44780_2_init(LCD_DISPLAY_8X5 | LCD_2_LINHAS,
                  LCD_DISPLAY_LIGADO | LCD_CURSOR_DESLIGADO | LCD_CURSOR_FIXO);  
  
  HD44780_clearText();
  HD44780_writeString(STRING_EMPRESA);
  
  HD44780_2_clearText();  
  HD44780_2_posicionaTexto(0,0);
  HD44780_2_writeString(STRING_EMPRESA);
  HD44780_2_posicionaTexto(0,0);  
  HD44780_2_writeString(STRING_EMPRESA);  
  
  HD44780_posicionaTexto(0,1);
  HD44780_2_posicionaTexto(0,1);
  HD44780_writeString  (STRING_VERSAO);
  HD44780_2_writeString(STRING_VERSAO);
  
  // Verifica se deve entrar no 
  // menu avançado
  for(unsigned char i=0;i<10;i++){
    if(TECLADO_getch()==TECLA_MENU_AVANCADO){
      HD44780_clearText();
      HD44780_writeString(" Acesso restrito");
      while(TECLADO_getch()==TECLA_MENU_AVANCADO);
      MCS_menuAvancado();
    }
    
    vTaskDelay(300);
  }   
  
  IU_exibeNumeroSerie();
  vTaskDelay(2000);    
  
  CA_ini();// Controle de amostra grátis
     
  
  unsigned short int velocidadeLimpeza = PARAMETROS_leVelocidadeLimpeza();
  IU_limpezaPipoqueira(velocidadeLimpeza);
  

  HD44780_clearText();
  HD44780_2_clearText();  
  //TELET_ini();  
        
  
  // Faz a verificação inicial da resistência
  // e trava o processo se não localizar    
  HD44780_clearText();
  HD44780_writeString("  Aguardando   ");
  HD44780_posicionaTexto(0,1);
  HD44780_writeString("  parada RPM...");
  
  while(MU_getRPMmedido());
  
  HD44780_clearText();
  HD44780_writeString("  Verificando  ");
  HD44780_posicionaTexto(0,1);
  HD44780_writeString("  resistencia  ");  
  
  if(!IU_verificacaoInicialResistencia()){
    for(;;){
#ifdef PORTUGUES
      HD44780_2_posicionaTexto(0,0);
      HD44780_2_writeString("Falha resistencia");
      HD44780_2_posicionaTexto(0,1);
      HD44780_2_writeString("Chamar suporte  ");       
#endif      
      
#ifdef ESPANHOL
      HD44780_2_posicionaTexto(0,0);
      HD44780_2_writeString("Falla resistencia");
      HD44780_2_posicionaTexto(0,1);
      HD44780_2_writeString("Llamar suporte  ");       
#endif
      
      tecla = TECLADO_getch();
      MP_enviaSinal(BV20_BLOQUEADO);        
      IU_falhaMotor = 255;
      
      TELET_enviaEstado(ESTADO_MONITORACAO);
      
      if(tecla==TECLA_ENTER){
        MCS_entry();
        if(IU_verificacaoInicialResistencia())
          break;
      }
    }  
    MP_enviaSinal(BV20_INICIALIZACAO);  
  }  
  
  
  vTaskDelay(2000);
  MP_zeraTotalizadoresMoeda();
  
  
  for(;;){

    TELET_enviaEstado(ESTADO_MONITORACAO);
    IU_processoSecagem();
         
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           MCS_entry();
           break;
      case TECLA_ESC:
           {
             unsigned short int velocidadeLimpeza = PARAMETROS_leVelocidadeLimpeza();

             HD44780_clearText();
             #ifdef PORTUGUES             
             HD44780_writeString("Limpando...");           
             #endif
             #ifdef ESPANHOL
             HD44780_writeString("Limpieza   ");           
             #endif
             IU_limpezaPipoqueira(velocidadeLimpeza);
             HD44780_clearText();
             IU_desenhaPrecoPipoca();         
           }
           break;
      case TECLA_INC:
           HD44780_clearText();
           #ifdef PORTUGUES
           HD44780_writeString("Dosando...");
           #endif
           #ifdef ESPANHOL
           HD44780_writeString("Dosaficar ");
           #endif
           DM_aplicaDosagem();
           break;
      case TECLA_DEC:           
           MCS_menuArrecadacao();
           HD44780_clearText();
           break;
      case TECLA_GRATIS:           
           if(CA_getTempo()){
             IU_creditoPipoca = 0xFF;
           }           
           break;
    }
    
    unsigned char codigo = IU_verificacaoCiclica();
    
    if(!codigo){
    
      unsigned char locacao = PARAMETROS_leFlagLocacao();
      
	  TELET_enviaEstado(ESTADO_MONITORACAO);
      AL_setterEstado(LEDS_NORMAL);      
      
      if(!CA_getTempo() && !locacao){
        IU_desenhaPrecoPipoca();
        IU_propaganda();        
        IU_escreveDinheiroInserido();   
        // Destrava o moedeiro 
        // na primeira vez que passar por aqui
        if(moedeiroTravado)
          MP_enviaSinal(BV20_INICIALIZACAO);   
      }
      else{
        // Trava o modeiro
        // na primeira vez que passar por aqui
        if(!moedeiroTravado)
          MP_enviaSinal(BV20_BLOQUEADO);
        
        moedeiroTravado = 255;
        if(!locacao){
          #ifdef PORTUGUES
          HD44780_2_posicionaTexto(0,0);
          HD44780_2_writeString(" Amostra Gratis ");
          HD44780_2_posicionaTexto(0,1);
          HD44780_2_writeString(" Aperte o botao ");
          #endif        
          #ifdef ESPANHOL
          HD44780_2_posicionaTexto(0,0);
          HD44780_2_writeString("Muestra gratuita");
          HD44780_2_posicionaTexto(0,1);
          HD44780_2_writeString(" Pulse el buton ");        
          #endif
        }
        else{
          #ifdef PORTUGUES
          HD44780_2_posicionaTexto(0,0);
          HD44780_2_writeString(" Para iniciar   ");
          HD44780_2_posicionaTexto(0,1);
          HD44780_2_writeString(" Aperte o botao ");
          #endif        
          #ifdef ESPANHOL
          HD44780_2_posicionaTexto(0,0);
          HD44780_2_writeString(" Para iniciar   ");
          HD44780_2_posicionaTexto(0,1);
          HD44780_2_writeString(" Pulse el buton ");        
          #endif          
        }
      }
      
      
      HD44780_posicionaTexto(7,1);  // Remove as indicações de erro do 
      HD44780_writeString("     "); // lcd interno
      
      unsigned int valorPipoca = PARAMETROS_leParametro(VALOR_PIPOCA);
      
      ///---------------------------------------------------///
      ///                                                   ///
      /// Faz o modo de funcionamento de locacação          ///
      ///                                                   ///
      ///---------------------------------------------------///
      static unsigned char flagLocacao=0;      
      if(PARAMETROS_leFlagLocacao()){
        IU_propaganda();  // Faz a propaganda              
        flagLocacao = 1; // Esse flag fica setado para indicar que precisar reiniciar o noteiro quando saida da locação
        CA_setterLedGratis(1);
        MP_enviaSinal(BV20_BLOQUEADO);        
      }else{
        if(flagLocacao){
          CA_setterLedGratis(0);  
          MP_enviaSinal(BV20_INICIALIZACAO);              
        }
        flagLocacao = 0;  
      }
      
      if(MP_descontaValor(valorPipoca) || IU_creditoPipoca || (flagLocacao&&TECLADO_getch() == TECLA_GRATIS)){
        
        MP_enviaSinal(BV20_BLOQUEADO);
        IU_mensagemIniciandoPreparo();
        
        unsigned res  = IU_preparaPipoca();
        if(res){
          //Pega os erros durante o preparo          
          HD44780_2_clearText();
          switch(res){                  
            case 1:
                    HD44780_2_posicionaTexto(0,0);              
                    HD44780_2_writeString("Falha dosador   ");
                    HD44780_2_posicionaTexto(0,1);
                    HD44780_2_writeString("Chamar suporte  ");                       
                    IU_dosador = 255;
                    for(;;) TELET_enviaEstado(ESTADO_MONITORACAO);                    
                    break;
            case 2: 
                    HD44780_2_posicionaTexto(0,0);              
                    HD44780_2_writeString("Falha embalagem ");                    
                    HD44780_2_posicionaTexto(0,1);
                    HD44780_2_writeString("Chamar suporte  ");                                   
                    for(;;) TELET_enviaEstado(ESTADO_MONITORACAO);                    
                    break;       
            case 4: 
                    HD44780_2_posicionaTexto(0,0);                             
                    HD44780_2_writeString("Falha ventilador");   
                    IU_falhaMotor = 255;
                    HD44780_2_posicionaTexto(0,1);
                    HD44780_2_writeString("Chamar suporte  ");                                           
                    for(;;) TELET_enviaEstado(ESTADO_MONITORACAO);                    
                    break;
            case 5: 
                    HD44780_2_posicionaTexto(0,0);                            
                    HD44780_2_writeString("Falha pre-aquec.");                    
                    HD44780_2_posicionaTexto(0,1);
                    HD44780_2_writeString("Chamar suporte  ");                                   
                    for(;;) TELET_enviaEstado(ESTADO_MONITORACAO);                    
                    break;
          }    
          
          
          eTECLA tecla;          
          do {
            tecla = TECLADO_getch();
            TELET_enviaEstado(ESTADO_MONITORACAO);

          }
          while(tecla!=TECLA_ESC);          
          
          unsigned int valorPipoca = PARAMETROS_leParametro(VALOR_PIPOCA);
          
          IU_limpezaPipoqueira(valorPipoca); //Limpa 
          MP_devolveArrecadacao(valorPipoca);//Devolve o valor da pipoca para o cliente caso ocorra um erro no preparo...
          // fim da captura dos erros
        } 
        else {
          // Incrementa os contadores de 
          // dinheiro e unidades vendidas        
          if(!IU_creditoPipoca){
            IU_atualizaTotalizadores(valorPipoca);          
            MP_enviaSinal(BV20_INICIALIZACAO);              
          }
        }          
        
        AL_setterLeds(FITA_LED_ESQUERDA,0,0);
        AL_setterLeds(FITA_LED_DIREITA,0,1);
        IU_ciclosPropaganda = 10;           
      }//Fim do inicio da preparação
      
      IU_creditoPipoca = 0;          
   }//Fim da verificação cíclica
   else{          
     // Falha no teste de verificação 
     // funcional     
     for(;;){
       TELET_enviaEstado(ESTADO_MONITORACAO);
       IU_escreveErros(codigo);          
       AL_setterEstado(LEDS_FORA_SERVICO);     
       tecla = TECLADO_getch();
       if(tecla==TECLA_ENTER)
         MCS_entry();
     }       
   }
   
   // Escreve informações no display
   IU_escreveTemperaturaResistencia(0);            
   IU_exibeContadorVendas();
   AL_tick(); 
   
   vTaskDelay(100);
  }  
}
/***********************************************************************************
*       Descrição       :       Desenha os erros nos displays da máquina
*       Parametros      :       (unsigned char) código
*       Retorno         :       nenhum
***********************************************************************************/
void IU_escreveErros(unsigned char codigo){
  static unsigned char bloqueado=0;
  
  HD44780_posicionaTexto(7,1);
  //HD44780_2_posicionaTexto(0,1);  
  
  switch(codigo){
      // Não foi detectada embalagem
      case 1: 
              HD44780_writeString("| E01");
              #ifdef PORTUGUES              
              HD44780_2_posicionaTexto(0,0);              
              HD44780_2_writeString("Fora de servico ");
              HD44780_2_posicionaTexto(0,1);              
              HD44780_2_writeString("Falta embalagem ");
              #endif
              #ifdef ESPANHOL
              HD44780_2_posicionaTexto(0,0);              
              HD44780_2_writeString(" Fuera servico  ");
              HD44780_2_posicionaTexto(0,1);              
              HD44780_2_writeString(" Falta envase   ");              
              #endif
              MP_enviaSinal(BV20_BLOQUEADO);   
              bloqueado = 1;
              break;       
      // Não foi detectado termistor
      case 2: HD44780_writeString("| E02");
              #ifdef PORTUGUES         
              HD44780_2_posicionaTexto(0,0);              
              HD44780_2_writeString("Fora de servico ");
              HD44780_2_posicionaTexto(0,1);              
              HD44780_2_writeString("Falha termistor ");
              #endif
              #ifdef ESPANHOL
              HD44780_2_posicionaTexto(0,0);              
              HD44780_2_writeString(" Fuera servico  ");
              HD44780_2_posicionaTexto(0,1);              
              HD44780_2_writeString("Falla embalagem ");                            
              #endif              
              MP_enviaSinal(BV20_BLOQUEADO);          
              bloqueado = 1;
              break;     
      // Não foi detectado noteiro
      case 3: HD44780_writeString("| E03");
              #ifdef PORTUGUES   
              HD44780_2_posicionaTexto(0,0);                     
              HD44780_2_writeString("Fora de servico ");
              HD44780_2_posicionaTexto(0,1);              
              HD44780_2_writeString("Falha noteiro   ");
              #endif
              #ifdef ESPANHOL
              HD44780_2_posicionaTexto(0,0);              
              HD44780_2_writeString(" Fuera servico  ");
              HD44780_2_posicionaTexto(0,1);              
              HD44780_2_writeString("Falla billetero ");                            
              #endif              
              MP_enviaSinal(BV20_BLOQUEADO);          
              bloqueado = 1;
              break;        
      // Não foi detectada rotação do ventilador
      case 4: HD44780_writeString("| E04");
              #ifdef PORTUGUES         
              HD44780_2_posicionaTexto(0,0);              
              HD44780_2_writeString("Fora de servico ");
              HD44780_2_posicionaTexto(0,1);              
              HD44780_2_writeString("Falha vent.     ");
              #endif
              #ifdef ESPANHOL
              HD44780_2_posicionaTexto(0,0);              
              HD44780_2_writeString(" Fuera servico  ");
              HD44780_2_posicionaTexto(0,1);              
              HD44780_2_writeString(" Falla el motor ");                            
              #endif              
              MP_enviaSinal(BV20_BLOQUEADO);          
              bloqueado = 1;
              break;        
      case 5:       
              if(bloqueado){
                bloqueado = 0;
                MP_enviaSinal(BV20_INICIALIZACAO);                     
              }
              break;
  }  
}
/***********************************************************************************
*       Descrição       :       Escreve a mensagem iniciando preparo
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_mensagemIniciandoPreparo(void){
  
  #ifdef PORTUGUES
  HD44780_2_clearText();
  HD44780_2_writeString("  Iniciando");
  HD44780_2_posicionaTexto(0,1);
  HD44780_2_writeString("  preparo");
  #endif
  
  #ifdef ESPANHOL
  HD44780_2_clearText();
  HD44780_2_writeString("comienzo");
  HD44780_2_posicionaTexto(0,1);
  HD44780_2_writeString("el preparo");
  #endif  
}
/***********************************************************************************
*       Descrição       :       Escreve a temperatura de preparo no lcd interno
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_escreveTemperaturaResistencia(unsigned char rpm){
  float temperatura = AA_calculaTemperatura();
  char tmp[17];
  unsigned int tempInt = (unsigned int)temperatura;
    
  sprintf(tmp,"%3d oC",tempInt);
  HD44780_posicionaTexto(0,1);
  HD44780_writeString(tmp);        
  
  if(rpm){
    sprintf(tmp,"  %05d",MU_getRPMmedido());
    HD44780_writeString(tmp);
  }
  
  IU_stampEstadoConexaoModuloTelemetria();
}
/***********************************************************************************
*       Descrição       :       Getter para o IU_falhaDosador
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
unsigned char IU_getFalhaDosador(void){
  
    return IU_dosador;
}
/***********************************************************************************
*       Descrição       :       Getter para o IU_falhaMotor
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
unsigned char IU_getFalhaMotor(void){

  return IU_falhaMotor;
}
/***********************************************************************************
*       Descrição       :       Escreve a temperatura de preparo no lcd interno
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_escreveTemperaturaResistenciaFree(unsigned char rpm){
  float temperatura = AA_calculaTemperatura();
  char tmp[17];
  unsigned int tempInt = (unsigned int)temperatura;
    
  sprintf(tmp,"%3d oC",tempInt);
  HD44780_posicionaTexto(0,1);
  HD44780_writeString(tmp);        
}
/***********************************************************************************
*       Descrição       :       Escreve o valor da pipoca no display de cliente
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_desenhaPrecoPipoca(void){
  char bufferLinha[17];
  unsigned int valor = PARAMETROS_leParametro(VALOR_PIPOCA);
  
  HD44780_2_posicionaTexto(0,0);
#ifdef PORTUGUES
  sprintf(bufferLinha,"Valor(R$):%1d.00 ",valor);  
#endif
  
#ifdef _URUGUAI_
  sprintf(bufferLinha,"Valor($U):%1d.00 ",valor);  
#endif
  
#ifdef _URUGUAI_60HZ_
  sprintf(bufferLinha,"Valor($U):%1d.00 ",valor);  
#endif
  
#ifdef _PARAGUAI_
  sprintf(bufferLinha,"Valor(G$):%5d  ",valor);  
#endif  
#ifdef _PARAGUAI_60HZ_
  sprintf(bufferLinha,"Valor(G$):%5d  ",valor);  
#endif   
  HD44780_2_posicionaTexto(0,0);
  HD44780_2_posicionaTexto(0,0);  
  HD44780_2_writeString(bufferLinha);  
  HD44780_2_posicionaTexto(15,0);
  HD44780_2_writeChar(' ');
}
/***********************************************************************************
*       Descrição       :       Escreve no display o total de dinheiro inserido
*                               pelo usuário
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_escreveDinheiroInserido(void){
  char bufferLinha[17];
  float valor = MP_getTotalDinheiro();
    
  if(valor){
#ifdef PORTUGUES    
    sprintf(bufferLinha,"Pago (R$):%3.2f  ",valor);  
#endif

#ifdef  _URUGUAI_
    sprintf(bufferLinha,"Pago ($U):%3.2f  ",valor);  
#endif    
    
#ifdef  _URUGUAI_60HZ_
    sprintf(bufferLinha,"Pago ($U):%3.2f  ",valor);  
#endif    
    
#ifdef _PARAGUAI_
    sprintf(bufferLinha,"Pago (G$):%5.0f    ",valor);  
#endif    
    
#ifdef _PARAGUAI_60HZ_
    sprintf(bufferLinha,"Pago (G$):%5.0f    ",valor);  
#endif        
   
    HD44780_2_posicionaTexto(0,1);
    HD44780_2_writeString(bufferLinha);      
  }
  else
    IU_mensagemClienteStandby();  
}
/***********************************************************************************
*       Descrição       :       Inicia o procedimento de preparação da pipoca
*       Parametros      :       nenhum
*       Retorno         :       (unsigned char) código de erro
***********************************************************************************/
unsigned char IU_preparaPipoca(void){
  unsigned char resfriamento=0;
  
  if(AA_calculaTemperaturaInteira()>40)
    resfriamento = 255;    
  
  TELET_enviaEstado(ESTADO_BLOQUEADO);
  
  IU_cicloCompensador();
  
  unsigned short int temperatura,tempo,v1,atrasoPacote,inicial,final;
  unsigned short int fator = 3; //PARAMETROS_leFatorTrabalho();
  unsigned short int velocidadeAquecimento = PARAMETROS_leParametro(VELOCIDADE_PREPARO);
  unsigned short int animacaoPreparo = PARAMETROS_leFitaPreparo();
  unsigned short int rampa = 3; //PARAMETROS_leFatorTrabalho();
      
  temperatura = PARAMETROS_leTemperaturaInicio() + 20; //PARAMETROS_leParametro(TEMPERATURA_PREPARO);
  tempo = PARAMETROS_leParametro(TEMPO_PREPARO);
  if(IU_compensadorRotacao)
    v1 = PARAMETROS_leParametro(VELOCIDADE_PREPARO) + ((IU_compensadorRotacao-1)>>1);
  else
    v1 = PARAMETROS_leParametro(VELOCIDADE_PREPARO);
  atrasoPacote = PARAMETROS_leParametro(ATRASO_PACOTE);                                    
  inicial = PARAMETROS_leRotacaoInicialExpulsao();
  final = PARAMETROS_leVelocidadeFinalExpulsao();
  unsigned short int tempResf;
  if(IU_compensadorTemperatura)//aqui123
    tempResf = PARAMETROS_leTemperaturaInicio() + ((IU_compensadorTemperatura-1)>>1);
  else
    tempResf = PARAMETROS_leTemperaturaInicio();
           
  AL_setterEstado(LEDS_PRE_PROCESSO);
  
  unsigned char res = IU_preAquecimento(tempResf);
  
  // Recodifica os erros para o padrão da
  // função principal, corrigir isso algum dia!
  switch(res){
    case 0 : break;
    case 1 : return 5;
    case 2 : return 4;   
    case 3 : return 6;
    default: return 8;
  } 
  
  // Seta a temperatura de processo na resistência
  MU_setTemperatura(tempResf,fator);  
  MU_setRPM(v1);  
  
  // Aguarda a resistência atingir a temperatura de processo
  // antes de jogar o milho na panela  
  unsigned char iA=0;
  {
    unsigned int tempInt;
    float tempF;
    
    do{
      IU_mensagensAguardaAquecimento(iA++);
      tempF = AA_calculaTemperatura();
      tempInt = (unsigned int)tempF;      
      IU_escreveTemperaturaResistencia(1);
      
      vTaskDelay(1000);      
            
      // Oscila a fita led durante a etapa de aguardar
      // o início da preparação
      if(animacaoPreparo)
        IU_oscilaFitaPreparacao();        
    }
    while(tempInt>tempResf);
  }
  
  
  
  #ifdef PORTUGUES
  HD44780_2_clearText();
  HD44780_2_writeString("Iniciando");
  HD44780_2_posicionaTexto(0,1);
  HD44780_2_writeString("preparo");
  #endif
  
  #ifdef ESPANHOL
  HD44780_2_clearText();
  HD44780_2_writeString("comienzo");
  HD44780_2_posicionaTexto(0,1);
  HD44780_2_writeString("el preparo");
  #endif  
  
  /*
  A DOSAGEM ESTAVA AQUI
  */
  MU_setRPM(0);
  vTaskDelay(160);
  MU_setRPM(3000);//
  //MU_setRPM(v1);    
  vTaskDelay(1000);
  // Se houver erro no dosador, 
  // marca com erro e encerra
  // antes desligando o motor e a resistência
  if(!DM_aplicaDosagem()){
     MU_setRPM(0); 
     MU_setTemperatura(0,0);
     return 1;
  }
  
  MU_setRPM(0);
  vTaskDelay(160);  
  MU_setRPM(v1);      
      
  AL_setterEstado(LEDS_RETIRAR_COPO);   
  CE_empurraEmbalagem(atrasoPacote);  
  PLAYERWAVE_iniciaMusica(0,0);  
  do{  
    AL_tick();
    vTaskDelay(100);
  }
  while(PLAYERWAVE_verificaToque());          
  //--------------------------------------------------------------
  // Falta verificar o erro na temperatura....
  // Fim do local
  // onde precisaremos mudar...
  //--------------------------------------------------------------
  
  PLAYER_interrompeMusica();
  PLAYERWAVE_iniciaMusica(1,0);
  
  unsigned char ciclosCopo=0;
      
  HD44780_2_clearText();    
  unsigned char buc=rampa;
  if(rampa==0)
    rampa=1;
  if(rampa>20)
    rampa=20;
  
  for(unsigned char i=0;i<tempo;i++){
    
    // Ciclos de 
    // atualização da temperatura
    if(!--buc){
      buc = rampa;
      if(temperatura>tempResf){
        tempResf++;
        MU_setTemperatura(tempResf,fator); 
      }
    }// Fim do ciclo de atualização 
    
    if(ciclosCopo<10){
      AL_setterEstado(LEDS_DOBRA_COPO);
      ciclosCopo++;
    }
    else
      AL_setterEstado(LEDS_INSERIR_COPO);
    
    IU_exibeTempoUsuario(tempo-i);               
    IU_escreveTemperaturaResistencia(0);       
    AL_tick();
    vTaskDelay(1000);  
    
    // Motor sem rotação é falha
    // no fusível ou sensor
    if(!MU_getRPMmedido()){
       MU_setRPM(0); 
       MU_setTemperatura(0,1);
       return 4;
    }        
    
    if(animacaoPreparo)
      IU_oscilaFitaPreparacao();
  }  
               
  MU_setTemperatura(0,1);
  
  MU_setRPM(0);
  vTaskDelay(160);    
  
#ifdef PORTUGUES
  HD44780_2_clearText(); 
  HD44780_2_writeString("    OBRIGADO    ");    
#endif
#ifdef ESPANHOL
  HD44780_2_clearText(); 
  HD44780_2_writeString("     GRACIAS    ");    
#endif
  
  IU_servePipoca(inicial,final);
  MU_setRPM(0);
  while(MU_getRPMmedido());
  vTaskDelay(5000);
  
  // Toca o mensagem
  // para pegar as pipocas
  PLAYER_interrompeMusica();
  PLAYERWAVE_iniciaMusica(2,0);
  while(PLAYERWAVE_verificaToque()); 
  PLAYER_interrompeMusica();
  
  IU_inatividadeProcesso = RELOAD_INATIVIDADE_UMIDADE;  
  
  if(resfriamento)
    IU_resfriamentoPanela();
  
  while(MU_getRPMmedido());  
  
  return 0;  
}
/***********************************************************************************
*       Descrição       :       Fica rolando as mensagens no display durante
*                               a fase onde aguarda a resistência atingir
*                               a temperatura de inicio do processo
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
#ifdef PORTUGUES
const char mensagensAguardandoAquecer[10][2][17]={  
  [0]={" Iniciando      "," Processo       "},  
  [1]={" Iniciando      "," Processo.      "},  
  [2]={" Iniciando      "," Processo..     "},  
  [3]={" Iniciando      "," Processo...    "},  
  [4]={" Iniciando      "," Processo....   "},    
  [5]={" Iniciando      "," Processo.....  "},    
  [6]={" Iniciando      "," Processo...... "},    
  [7]={" Iniciando      "," Processo......."}    
};
#endif
#ifdef ESPANHOL
const char mensagensAguardandoAquecer[10][2][17]={  
  [0]={" Inicio del     "," proceso        "},  
  [1]={" Inicio del     "," proceso.       "},  
  [2]={" Inicio del     "," proceso..      "},  
  [3]={" Inicio del     "," proceso...     "},  
  [4]={" Inicio del     "," proceso....    "},    
  [5]={" Inicio del     "," proceso.....   "},    
  [6]={" Inicio del     "," processo.....  "},    
  [7]={" Inicio del     "," processo...... "}    
};
#endif

void IU_mensagensAguardaAquecimento(unsigned char indice){
    
  HD44780_2_posicionaTexto(0,0);
  HD44780_2_writeString((char*)mensagensAguardandoAquecer[indice%8][0]);
  HD44780_2_posicionaTexto(0,1);
  HD44780_2_writeString((char*)mensagensAguardandoAquecer[indice%8][1]);
}
/***********************************************************************************
*       Descrição       :       Limpeza da pipoqueira com ar
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_limpezaPipoqueira(unsigned short int velocidade){
  
  TELET_enviaEstado(ESTADO_BLOQUEADO);
  MU_setRPM(velocidade);
  vTaskDelay(5000);  
  MU_setRPM(0);    
}
/***********************************************************************************
*       Descrição       :       Piscada da fita de leds e exibição da mensagem
*                               de propaganda
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_propaganda(void){
  static unsigned char inverte=0;
  
  // Controle da piscada 
  // da fita led durante o modo
  // standby
  if(!IU_contadorPropaganda){        
    IU_contadorPropaganda = 500;        
    inverte =~inverte;
    AL_setterLeds(FITA_LED_ESQUERDA,0,0);
    AL_setterLeds(FITA_LED_DIREITA,0,1);
    
    unsigned short int flag = PARAMETROS_leFlagPiscadaPropaganda();
    if(flag){
      if(inverte)
        AL_setterLeds(FITA_LED_ESQUERDA,1,1);
      else
        AL_setterLeds(FITA_LED_DIREITA,1,1);
    }                      
  }  
  
  // Ciclos da propaganda
  // com a mensagem de voz
  if(!IU_ciclosPropaganda){    
    unsigned int tempoPropaganda = PARAMETROS_leIntervaloPropaganda();
    
    if(tempoPropaganda){    
      IU_ciclosPropaganda = tempoPropaganda;
      TELET_enviaEstado(ESTADO_BLOQUEADO);
      
      PLAYERWAVE_iniciaMusica(3,0);
      while(PLAYERWAVE_verificaToque());   
      PLAYER_interrompeMusica();
    }
    else
      IU_ciclosPropaganda = 1;  
  }      
}
/***********************************************************************************
*       Descrição       :       Exibe o contador de vendas no display interno
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_exibeContadorVendas(void){       
  unsigned int total = PARAMETROS_leContadorVendas();
  char bufferLinha[17];
  
  sprintf(bufferLinha,"[%09d]",total);
  HD44780_posicionaTexto(0,0);
  HD44780_writeString(bufferLinha);      
  
  sprintf(bufferLinha,"%05d",MU_getRPMmedido());
  HD44780_writeString(bufferLinha);
}
/***********************************************************************************
*       Descrição       :       Tela que exibe no display externo
*                               o tempo que falta durante o preparo da pipoca
*       Parametros      :       (unsigned short int) tempo restante
*       Retorno         :       nenhum
***********************************************************************************/
void IU_exibeTempoUsuario(unsigned short int tempo){
  unsigned char minutos,segundos;
  char bufferLinha[17];
  
  minutos = tempo/60;
  segundos = tempo - (minutos*60);
  
  //sprintf(bufferLinha,"  %01d:%02d",minutos,segundos);
  
  bufferLinha[0] = ' ';
  bufferLinha[1] = ' ';
  bufferLinha[2] = ' ';
  bufferLinha[3] = ' ';
  bufferLinha[4] = ' ';
  bufferLinha[5] = ((minutos/10)%10) + '0';
  bufferLinha[6] =  (minutos%10) + '0';
  bufferLinha[7] = ':';                    
  bufferLinha[8] = ((segundos/10)%10) + '0';
  bufferLinha[9] =  (segundos%10) + '0';
  bufferLinha[10]= ' ';
  bufferLinha[11]= ' ';
  bufferLinha[12]= ' ';
  bufferLinha[13]= ' ';
  bufferLinha[14]= ' ';  
  bufferLinha[15]= ' ';  
  bufferLinha[16]= '\0';
  
  HD44780_2_posicionaTexto(0,0);                   
  HD44780_2_writeString("   PRONTAS EM   ");
  HD44780_2_posicionaTexto(0,1);
  HD44780_2_writeString(bufferLinha);        
}
/***********************************************************************************
*       Descrição       :       Atualiza os totalizadores 
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_atualizaTotalizadores(unsigned int valorPipoca){
  // Totalizador de unidades de pipoca vendidas
  unsigned int total = PARAMETROS_leContadorVendas();
  total++;
  PARAMETROS_salvaContadorVendas(total);
          
  // Totalizador de dinheiro arrecadado
  unsigned int dinheiro = PARAMETROS_leContadorArrecadacao();
  dinheiro+=valorPipoca;
  PARAMETROS_salvaContadorArrecadacao(dinheiro);
          
  // Totalizador de histórico de faturamento
  unsigned int historico = PARAMETROS_leTotalizadorPermanente();
  historico+=valorPipoca;
  PARAMETROS_gravaTotalizadorPermanente(historico);      
  
  unsigned int contadorParcial = PARAMETROS_leContadorVendasParcial();
  contadorParcial++;
  PARAMETROS_gravaContadorVendasParcial(contadorParcial);
  
  if(MP_getterMaquinaCartao()){
    unsigned int totalCartao = PARAMETROS_carregaOperacoesCartao();
    totalCartao+= valorPipoca;
    PARAMETROS_salvaContadorOperacoesCartao(totalCartao);                                                   
    MP_setterMaquinaCartao(0);
    
    total = PARAMETROS_carregaFaturamentoParcialCartao();
    total += valorPipoca;
    PARAMETROS_salvaFaturamentoParcialCartao(total);
    
    total = PARAMETROS_carregaFaturamentoTotalCartao();
    total += valorPipoca;    
    PARAMETROS_salvaFaturamentoTotalCartao(total);    
  }
  TELET_enviaEstado(ESTADO_MONITORACAO);

}
/***********************************************************************************
*       Descrição       :       Verificação ciclica
*       Parametros      :       nenhum
*       Retorno         :       (unsigned char) maior do que zero
*                                               senão houverem falhas
*                                               no hardware
***********************************************************************************/
unsigned char IU_verificacaoCiclica(void){
  
   if(!CE_sensorEmbalagem()){
     AL_setterEstado(LEDS_FORA_SERVICO);
     return 1; 
   }
     
   if(!AA_verificaConexaoTermistor()){
     AL_setterEstado(LEDS_FORA_SERVICO);
     return 2;
   }
   
   if(!MP_timeOutNoteiro()){
     AL_setterEstado(LEDS_FORA_SERVICO);
     return 3;     
   }
   
   return 0;  
}
/***********************************************************************************
*       Descrição       :       Tela com o pré-aquecimento da resistência
*       Parametros      :       nenhum
*       Retorno         :       (unsigned char) maior do que zero
*                                               se conseguir executar
***********************************************************************************/
unsigned char IU_preAquecimento(unsigned short int velocidade){
  float temperatura;
  unsigned int tempInt;
  unsigned short int preSet = velocidade;
  unsigned short int fator = PARAMETROS_leFatorTrabalho();
  unsigned short int tempoPre = PARAMETROS_leTempoPreaquecimento();
  unsigned short int animacaoPreparo = PARAMETROS_leFitaPreparo();
       
  temperatura = AA_calculaTemperatura();
  tempInt = (unsigned int)temperatura;  
      
  unsigned char iA=0;
  
  if(tempInt>=preSet){    
    
    unsigned short int velMotor = PARAMETROS_leVelocidadeAquecimento();
    MU_setRPM(velMotor);    
    MU_setTemperatura(0,500); 
        
    do{      
      //IU_escreveDinheiroSaldo();     
      IU_escreveTemperaturaResistencia(0);      
      temperatura = AA_calculaTemperatura();
      tempInt = (unsigned int)temperatura;  
      
      if(animacaoPreparo)
        IU_oscilaFitaPreparacao();         
      
      IU_mensagensAguardaAquecimento(iA++);
      
      vTaskDelay(1000);
    }        
    while(tempInt>=(preSet));       
    
    MU_setTemperatura(preSet,500);
    velMotor=PARAMETROS_leParametro(VELOCIDADE_PREPARO);
    MU_setRPM(velMotor);    
  }
  else{
    unsigned short int velMotor = PARAMETROS_leParametro(VELOCIDADE_PREPARO);
    MU_setRPM(velMotor);
    MU_setTemperatura(preSet,500);
    
    do{      
      //IU_escreveDinheiroSaldo();     
      IU_escreveTemperaturaResistencia(0);      
      temperatura = AA_calculaTemperatura();
      tempInt = (unsigned int)temperatura;  
      
      if(animacaoPreparo)
        IU_oscilaFitaPreparacao();         
      
      IU_mensagensAguardaAquecimento(iA++);
      
      vTaskDelay(1000);      
    }        
    while(tempInt<preSet);      
  }
    
  if(!MU_getRPMmedido()){
     MU_setRPM(0); 
     MU_setTemperatura(0,500);
     return 2;
  }  
  
  return 0;   
}
/***********************************************************************************
*       Descrição       :       Escreve no display o total de dinheiro inserido
*                               pelo usuário
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_escreveDinheiroSaldo(void){
  char bufferLinha[17];
  float valor = MP_getTotalDinheiro();
  
  
  if(valor){
#ifdef PORTUGUES    
    sprintf(bufferLinha,"Saldo(R$):%3.2f ",valor);  
#endif
    
#ifdef ESPANHOL
    sprintf(bufferLinha,"Saldo($U):%3.2f ",valor);  
#endif        
    HD44780_2_posicionaTexto(0,1);
    HD44780_2_writeString(bufferLinha);  
  }
  else
    IU_mensagemClienteStandby();
}
/***********************************************************************************
*       Descrição       :       Interface para servir a pipoca ao usuário
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_servePipoca(unsigned int inicial,unsigned int final){
  
  MU_setRPM(inicial);
  vTaskDelay(5000);        
  
  MU_setRPM(0);  
}
/***********************************************************************************
*       Descrição       :       Faz a rolagem da mensagem na segunda linha
*                               do display de cliente
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_mensagemClienteStandby(void){
  char bufferMsg[17];
  static unsigned char indice=0;
  
  if(IU_propagandaAlmeida)
    return;
  
  IU_propagandaAlmeida = 3000;
    
  PARAMETROS_leMensagemCliente(indice,bufferMsg);
  bufferMsg[16] = '\0';
  
  HD44780_2_posicionaTexto(0,1);
  HD44780_2_writeString(bufferMsg);
  
  indice = (indice+1) % 4;  
}
/***********************************************************************************
*       Descrição       :       Oscila a fita de leds durante a preparação
*       Parametros      :       nenhum
*       Retorno         :       nwenhum
***********************************************************************************/
void IU_oscilaFitaPreparacao(void){
  static unsigned char toggle=0;
  
  if(!BOARD_tempoPiscadas){
    BOARD_tempoPiscadas = 250;
    toggle = ~toggle;
    AL_setterLeds(FITA_LED_ESQUERDA,0,0);
    AL_setterLeds(FITA_LED_DIREITA,0,0);
    if(toggle)
      AL_setterLeds(FITA_LED_ESQUERDA,1,1);
    else
      AL_setterLeds(FITA_LED_DIREITA,1,1);    
  }  
}
/***********************************************************************************
*       Descrição       :       Envia os dados de telemetria para o módulo
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_enviaTelemetria(void){
  unsigned char flags=0;
  
  flags |= CE_sensorEmbalagem()?0x80:0x00;
  flags |= TECLADO_getChavePorta()?0x40:0x00;
  flags |= TECLADO_getChaveReservaPapel()?0x20:0x00;
  flags |= AA_verificaConexaoTermistor()?0x10:0x20;
  flags |= MP_timeOutNoteiro()?0x08:0x00;
  flags |= IU_falhaMotor?0x04:0x00;
  flags |= IU_dosador?0x02:0x00;
 
  /*IU_estadoConexaoTelemetria = TELET_escreveBlocoOperacao(0,
                                                          PARAMETROS_leContadorVendas(),
                                                          PARAMETROS_leContadorArrecadacao(),
                                                          PARAMETROS_leTotalizadorPermanente(),
                                                          flags,
                                                          PARAMETROS_carregaOperacoesCartao()); */
}
/***********************************************************************************
*       Descrição       :       Stamp do estado da conexão com 
*                               o módulo de telemetria
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_stampEstadoConexaoModuloTelemetria(void){
  
  HD44780_posicionaTexto(8,1);
  if(TELET_getEstadoConexaoTelemetria())
    HD44780_writeString("[TON ]");
  else
    HD44780_writeString("[TOFF]");  
}
/***********************************************************************************
*       Descrição       :       Interface para resfriamento da
*                               resistência
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
unsigned char IU_interfaceResfriamento(unsigned short int temperatura){
  //float temperatura;
  float tmp;
  unsigned int tempInt;
  unsigned short int ciclosMaximo = 600;
  
  MU_setRPM(0);
  
  if(AA_calculaTemperatura()<temperatura)
    return 0;
  else
    MU_setRPM(4500);     
  
  vTaskDelay(5000);
     
  
  HD44780_2_clearText();
  HD44780_2_writeString("Pre-aquecendo ");
    
  do{
    
    if(!MU_getRPMmedido()){
       MU_setRPM(0); 
       MU_setTemperatura(0,500);
       return 2;
    }
        
    IU_escreveDinheiroSaldo();     
    IU_escreveTemperaturaResistencia(0);
    tmp = AA_calculaTemperatura();
    tempInt = (unsigned int)tmp;  
      
    if(tempInt<temperatura){
       HD44780_2_clearText();              
       MU_setRPM(0); 
       return 0;
    }
        
    AL_tick();
    vTaskDelay(200);
  }
  while(ciclosMaximo--);

  MU_setRPM(0);   
  HD44780_2_clearText();
  return 1;
}
/***********************************************************************************
*       Descrição       :       Função que faz o aquecimento da panela 
*                               por fim de realizar a secagem da umidade
*                               interna da máquina
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_processoSecagem(void){
  
  if(IU_inatividadeProcesso)
    return;
   
  if(!PARAMETROS_leCicloDesumidificacao()){
    IU_inatividadeProcesso = 3000;
    return;
  }    
  
  TELET_enviaEstado(ESTADO_BLOQUEADO);

  HD44780_clearText();
  HD44780_posicionaTexto(0,0);
  HD44780_writeString(" Desumidificando");
  HD44780_posicionaTexto(0,1);
  HD44780_2_clearText();
  HD44780_2_posicionaTexto(0,0);
  HD44780_2_writeString(" Desumidificando");
  HD44780_2_posicionaTexto(0,1);

  MU_setTemperatura(50,0); 
  MU_setRPM(6000); 
  
  for(unsigned char i=0;i<16;i++){
    HD44780_posicionaTexto(i,1);
    HD44780_writeString(".");
	HD44780_2_posicionaTexto(i,1);
    HD44780_2_writeString(".");
      
    IU_escreveTemperaturaResistencia(0);
      
    vTaskDelay(3000);
  }

  IU_inatividadeProcesso =  RELOAD_INATIVIDADE_UMIDADE;
  
  MU_setTemperatura(0,0);  
  MU_setRPM(0);    
  HD44780_clearText();
}
/***********************************************************************************
*       Descrição       :       Faz a verificação inicial da resistência
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
unsigned char IU_verificacaoInicialResistencia(void){
  unsigned short int temperatura;
  unsigned short int temperaturaAtual;
  unsigned short int preSet;
  unsigned char timeout=250;
  
  
  MU_setRPM(5000);       
  HD44780_2_clearText();
  HD44780_2_writeString("Verificando");
  HD44780_2_posicionaTexto(0,1);
  HD44780_2_writeString("Resistencia");  
  
  temperaturaAtual = AA_calculaTemperatura();
  preSet = temperaturaAtual+10;
  MU_setTemperatura(preSet,500);
  
  for(;timeout--;){
    
    temperatura = AA_calculaTemperatura();
    if(temperatura>(temperaturaAtual+5)){
      MU_setTemperatura(0,500);    
      MU_setRPM(0);       
      return 1;
    }
               
    
    vTaskDelay(100);
  }
  
  MU_setTemperatura(0,500);    
  MU_setRPM(0);         
  HD44780_2_clearText();
  return 0;
}
/***********************************************************************************
*       Descrição       :       Timer hook para o compensador
*                               da temperatura de processo
*       Parametros      :       nenhum
*       Retorno         :       
***********************************************************************************/
void IU_tickCompensador(void){
  
  if(IU_contadorSaidaCompensador)
    IU_contadorSaidaCompensador--;
  else{
    IU_contadorSaidaCompensador = TEMPO_SAIDA_COMPENSADOR;
    
    if(IU_compensadorTemperatura)
      IU_compensadorTemperatura--;
    if(IU_compensadorRotacao>=50)
      IU_compensadorRotacao-=50;
  }  
  
  if(IU_contadorCompensador)
    IU_contadorCompensador--;
}
/***********************************************************************************
*       Descrição       :       Chamada para o compensador que ocorre
*                               a cada preparação de pipoca
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_cicloCompensador(void){
  
  if(IU_contadorCompensador){
    if(IU_compensadorTemperatura<MAX_COMPENSADOR_TEMPERATURA)
      IU_compensadorTemperatura+= PARAMETROS_le_correcao_erro();//aqui123 trazer parametro
    if(IU_compensadorRotacao<MAX_COMPENSADOR_ROTACAO)
      IU_compensadorRotacao+=50;//aqui123 estava alterando o IU_compensadorTemperatura
    
    IU_contadorCompensador = TEMPO_COMPENSADOR;
    IU_contadorSaidaCompensador = TEMPO_SAIDA_COMPENSADOR;
  }  
}
/***********************************************************************************
*       Descrição       :       Realiza o resfriamento da panela 
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_resfriamentoPanela(void){
      
  MU_setRPM(0);
  vTaskDelay(160);
  while(MU_getRPMmedido());
  vTaskDelay(3000);
  
  MU_setRPM(6000);    
  
  HD44780_2_clearText();
  HD44780_2_posicionaTexto(0,0);
  HD44780_2_posicionaTexto(0,0);  
  HD44780_2_posicionaTexto(0,0);    
  
  HD44780_2_posicionaTexto(0,0);
  HD44780_2_posicionaTexto(0,0);  
  HD44780_2_posicionaTexto(0,0);  
  
  HD44780_2_writeString("  Resfriando");
  HD44780_2_posicionaTexto(0,1);
  
  for(unsigned char i=0;i<16;i++){
    HD44780_2_writeChar('.');
    vTaskDelay(1500);    
  }           
     
  MU_setRPM(0);      
}
/***********************************************************************************
*       Descrição       :       Getter para o número de série da máquina
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) número de série da máquina
***********************************************************************************/
unsigned int IU_getNumeroSerie(void){
  
   return PARAMETROS_carregaNumeroSerie();
}
/***********************************************************************************
*       Descrição       :       Exibe o número de série da placa
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_exibeNumeroSerie(void){
  char bufferLinha[17];
  unsigned int numeroSerie =  IU_getNumeroSerie();
  unsigned char tamanho; 
  
  sprintf(bufferLinha,"%d",numeroSerie);
  
  tamanho = strlen(bufferLinha);
  HD44780_clearText();
  HD44780_posicionaTexto((16-tamanho)>>1,1);
  HD44780_writeString(bufferLinha);
  HD44780_posicionaTexto(0,0);
  HD44780_writeString("   Num. Serie   ");
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
