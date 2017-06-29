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
||
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

#define STRING_EMPRESA                                          "ALMEIDA TECHNOLO"
#define STRING_VERSAO                                           "     1.6.4      "
//#define _DEBUG_                   
#define INTERVALO_MENSAGEM_PRE_BLOQUEIO                         3*60*1000
/***********************************************************************************
*       Constantes
***********************************************************************************/
 const unsigned int numeroSerie=321;

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
void IU_exibeNumeroSerieMaquina(void);
void IU_enviaTelemetria(void);
void IU_stampEstadoConexaoModuloTelemetria(void);
unsigned char IU_interfaceResfriamento(unsigned short int temperatura);
void IU_cicloPosResfriamento(void);
void IU_mensagensAguardaAquecimento(unsigned char indice);
void IU_processoSecagem(void);
void IU_verificaHorimetro(void);

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
  vTaskDelay(3000);
  
  CA_ini();// Controle de amostra grátis
     
  unsigned short int velocidadeLimpeza = PARAMETROS_leVelocidadeLimpeza();
  IU_limpezaPipoqueira(velocidadeLimpeza);
    
  HD44780_clearText();
  HD44780_2_clearText();  
  TELET_ini();  
  
  /*
  unsigned short int enderecoAssinatura;
  
  if(!ASSINATURA_verifica(&enderecoAssinatura)){
    
    HD44780_clearText();
    HD44780_writeString("Placa nao");
    HD44780_posicionaTexto(0,1);
    HD44780_writeString("assinada");
    
    for(;;){
      
      HORIMETRO_tick();
      
      tecla = TECLADO_getch();
      if(tecla==TECLA_ENTER){
        MCS_entry();    
        HD44780_clearText();
        HD44780_writeString("Placa nao");
        HD44780_posicionaTexto(0,1);
        HD44780_writeString("assinada");        
      }      
      vTaskDelay(50);
    }
  }
  else
    numeroSerie = enderecoAssinatura;    
  */
  
  IU_exibeNumeroSerieMaquina();    
  
  for(;;){

    IU_enviaTelemetria();
    IU_processoSecagem();
    
    //HORIMETRO_tick();
    //IU_verificaHorimetro();
      
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           MCS_entry();
           break;
      case TECLA_ESC:
           {
             unsigned short int velocidadeLimpeza = PARAMETROS_leVelocidadeLimpeza();

             HD44780_clearText();
             HD44780_writeString("Limpando...");           
             IU_limpezaPipoqueira(velocidadeLimpeza);
             HD44780_clearText();
             IU_desenhaPrecoPipoca();         
           }
           break;
      case TECLA_INC:
           HD44780_clearText();
           HD44780_writeString("Dosando...");
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
    
      AL_setterEstado(LEDS_NORMAL);      
      
      if(!CA_getTempo()){
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
        HD44780_2_posicionaTexto(0,0);
        HD44780_2_writeString(" Amostra Gratis ");
        HD44780_2_posicionaTexto(0,1);
        HD44780_2_writeString(" Aperte o botao ");
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
        flagLocacao = 1;
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
            case 1: HD44780_2_writeString("Falha dosador   ");
                    HD44780_2_posicionaTexto(0,1);
                    HD44780_2_writeString("Chamar suporte  ");                       
                    IU_dosador = 255;
                    for(;;);                    
                    break;
            case 2: HD44780_2_writeString("Falha embalagem ");                    
                    HD44780_2_posicionaTexto(0,1);
                    HD44780_2_writeString("Chamar suporte  ");                                   
                    for(;;);                    
                    break;       
            case 4: HD44780_2_writeString("Falha ventilador");   
                    IU_falhaMotor = 255;
                    HD44780_2_posicionaTexto(0,1);
                    HD44780_2_writeString("Chamar suporte  ");                                           
                    for(;;);                    
                    break;
            case 5: HD44780_2_writeString("Falha pre-aquec.");                    
                    HD44780_2_posicionaTexto(0,1);
                    HD44780_2_writeString("Chamar suporte  ");                                   
                    for(;;);                    
                    break;
          }    
          
          
          eTECLA tecla;          
          do tecla = TECLADO_getch();
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
     IU_escreveErros(codigo);          
     AL_setterEstado(LEDS_FORA_SERVICO);     
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
  HD44780_2_posicionaTexto(0,1);  
  
  switch(codigo){
      // Não foi detectada embalagem
      case 1: HD44780_writeString("| E01");
              HD44780_2_writeString("Falta embalagem ");
              MP_enviaSinal(BV20_BLOQUEADO);   
              bloqueado = 1;
              break;       
      // Não foi detectado termistor
      case 2: HD44780_writeString("| E02");
              HD44780_2_writeString("Falha termistor ");
              MP_enviaSinal(BV20_BLOQUEADO);          
              bloqueado = 1;
              break;     
      // Não foi detectado noteiro
      case 3: HD44780_writeString("| E03");
              HD44780_2_writeString("Falha noteiro   ");
              MP_enviaSinal(BV20_BLOQUEADO);          
              bloqueado = 1;
              break;               
      case 4: HD44780_writeString("| E04");
              HD44780_2_writeString("Falha vent.     ");
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
  
  HD44780_2_clearText();
  HD44780_2_writeString("Iniciando");
  HD44780_2_posicionaTexto(0,1);
  HD44780_2_writeString("preparo");
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
  /*
  if(rpm){
    sprintf(tmp,"  %05d",MU_getRPMmedido());
    HD44780_writeString(tmp);
  } 
  */
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
  sprintf(bufferLinha,"Valor(R$):%1d.00 ",valor);  
  HD44780_2_writeString(bufferLinha);  
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
    sprintf(bufferLinha,"Pago (R$):%3.2f  ",valor);  
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
  unsigned short int temperatura,tempo,v1,atrasoPacote,inicial,final;
  unsigned short int fator = PARAMETROS_leFatorTrabalho();
  unsigned short int velocidadeAquecimento = PARAMETROS_leParametro(VELOCIDADE_PREPARO);
  unsigned short int animacaoPreparo = PARAMETROS_leFitaPreparo();
  unsigned short int rampa = PARAMETROS_leFatorTrabalho();
  
  temperatura = PARAMETROS_leParametro(TEMPERATURA_PREPARO);
  tempo = PARAMETROS_leParametro(TEMPO_PREPARO);
  v1 = PARAMETROS_leParametro(VELOCIDADE_PREPARO);
  atrasoPacote = PARAMETROS_leParametro(ATRASO_PACOTE);                                    
  inicial = PARAMETROS_leRotacaoInicialExpulsao();
  final = PARAMETROS_leVelocidadeFinalExpulsao();
  unsigned short int tempResf = PARAMETROS_leTemperaturaInicio();
        
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
      IU_escreveTemperaturaResistencia(0);
      
      vTaskDelay(1000);      
            
      // Oscila a fita led durante a etapa de aguardar
      // o início da preparação
      if(animacaoPreparo)
        IU_oscilaFitaPreparacao();        
    }
    while(tempInt>tempResf);
  }
  
  
  HD44780_2_clearText();
  HD44780_2_writeString("    Iniciando   ");
  HD44780_2_posicionaTexto(0,1);
  HD44780_2_writeString("    preparo...  "); 
  
  // Se houver erro no dosador, 
  // marca com erro e encerra
  // antes desligando o motor e a resistência
  if(!DM_aplicaDosagem()){
     MU_setRPM(0); 
     MU_setTemperatura(0,0);
     return 1;
  }
      
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
  
  HD44780_2_clearText(); 
  HD44780_2_writeString("    OBRIGADO    ");    
  IU_servePipoca(inicial,final);
  
  vTaskDelay(5000);
  
  // Toca o mensagem
  // para pegar as pipocas
  PLAYER_interrompeMusica();
  PLAYERWAVE_iniciaMusica(2,0);
  while(PLAYERWAVE_verificaToque());  
  
  IU_inatividadeProcesso = RELOAD_INATIVIDADE_UMIDADE;  
  
  return 0;  
}
/***********************************************************************************
*       Descrição       :       Fica rolando as mensagens no display durante
*                               a fase onde aguarda a resistência atingir
*                               a temperatura de inicio do processo
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
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
      
      PLAYERWAVE_iniciaMusica(3,0);
      while(PLAYERWAVE_verificaToque());      
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
    sprintf(bufferLinha,"Saldo(R$):%3.2f ",valor);  
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
  vTaskDelay(1500);      
  
  MU_setRPM(final);
  vTaskDelay(2500);      
  
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
  
  /*
  const char mensagemCliente[]={"Almeida Technology -- www.almeidatechnology.com -- Mais Pipocas -- Almeida Technology" };
  static unsigned char indice=0;  
  char bufferLinha[17];
  
  if(IU_propagandaAlmeida)
    return;
  
  IU_propagandaAlmeida = 100;
  
  HD44780_2_posicionaTexto(0,1);
  memcpy(bufferLinha,(char*)&mensagemCliente[indice],16);
  bufferLinha[16] = 0;  
  HD44780_2_writeString(bufferLinha);  
  indice++;
  if(indice>56)
    indice=0;
  */
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
*       Descrição       :       Exibe o número de série no display
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_exibeNumeroSerieMaquina(void){
  char linha[17];
      
  HD44780_clearText();
  HD44780_writeString("Num. serial");
  sprintf(linha,"%010d",numeroSerie);
  HD44780_posicionaTexto(0,1);
  HD44780_writeString(linha);      
  
  vTaskDelay(3000);
}
/***********************************************************************************
*       Descrição       :       Getter para o número de série da máquina
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
unsigned int IU_getNumeroSerie(void){
  
  return numeroSerie;  
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
 
  IU_estadoConexaoTelemetria = TELET_escreveBlocoOperacao(numeroSerie,PARAMETROS_leContadorVendas(),PARAMETROS_leContadorArrecadacao(),PARAMETROS_leTotalizadorPermanente(),flags); 
}
/***********************************************************************************
*       Descrição       :       Stamp do estado da conexão com 
*                               o módulo de telemetria
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_stampEstadoConexaoModuloTelemetria(void){
  
  HD44780_posicionaTexto(8,1);
  if(IU_estadoConexaoTelemetria)
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
  
  HD44780_2_clearText();
  HD44780_2_posicionaTexto(0,0);
  HD44780_2_writeString("Desumidificando");
  HD44780_2_posicionaTexto(0,1);

  MU_setTemperatura(PARAMETROS_leTemperaturaDesumidificacao(),0);  
  MU_setRPM(PARAMETROS_leVelocidadeDesumidificacao()); 
  
  for(unsigned char i=0;i<16;i++){
    HD44780_2_posicionaTexto(i,1);
    HD44780_2_writeString(".");
    vTaskDelay(3000);
    //IU_escreveTemperaturaResistencia(1);
  }

  IU_inatividadeProcesso =  RELOAD_INATIVIDADE_UMIDADE;
  
  MU_setTemperatura(0,0);  
  MU_setRPM(0);  
}
/***********************************************************************************
*       Descrição       :       Verifica o horímetro
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void IU_verificaHorimetro(void){
  /*
  switch(HORIMETRO_verificaSaldoHoras()){
    case LIBERADO:
         
         break;
    case RENOVAR:
         if(!IU_tempoInfoHorimetro){
        
           HD44780_posicionaTexto(0,0);
           HD44780_writeString("Renove liberacao");
           HD44780_posicionaTexto(0,1);
           HD44780_writeString("p/ nao bloquear  ");
           vTaskDelay(2000);
           
           IU_tempoInfoHorimetro = INTERVALO_MENSAGEM_PRE_BLOQUEIO;
         }
         break;
    case BLOQUEADO:
         for(;;){
           eTECLA tecla;
           
           tecla = TECLADO_getch();
           
           if(tecla==TECLA_ENTER)
             MCS_entry();              
           
           HD44780_posicionaTexto(0,0);
           HD44780_writeString("Bloqueado      ");
           HD44780_posicionaTexto(0,1);
           HD44780_writeString("pelo horimetro ");                     
           
           HD44780_2_posicionaTexto(0,0);
           HD44780_2_writeString("Bloqueado      ");
           HD44780_2_posicionaTexto(0,1);
           HD44780_2_writeString("pelo horimetro ");                                
         }
         break;
  }  
  */
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/