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
|       Arquivo            :  MenuConfiguracaoSistema.c
|       Descrição          :  Telas para o usuário
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  11/03/2015
|
|       Revisões           :  1.0.0.0
|                             1.1 - (23/06/216) menus reagrupados, por Marcos
|
|
| __________________________________________________________________________________
*/
/************************************************************************************
*       Includes
************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\..\includes.h"
#include "menuConfiguracaoSistema.h"
#include "interfacAjusteRelogio.h"

/************************************************************************************
*       Definições
************************************************************************************/
#define TAM_MENU_PRINCIPAL                              9
#define TAM_MENU_PREPARACAO                             8
#define TAM_MENU_EXIBICAO                               10
#define TAM_MENU_OPERACAO                               6
#define TAMANHO_OPCOES_DESUMI                           4
#define TAM_LISTA_ARRECADACAO                           8
#define TAM_MENU_PERIFERICOS                            4
#define TAM_LISTA_HARDWARE                              19
#define TAM_LISTA_WIFI                                  7
#define TAM_LISTA_AVANCADO                              10
#define TAM_LISTA_CONTADORES                            5
/************************************************************************************
*       Constantes locais
************************************************************************************/
#ifdef PORTUGUES
///----- Idioma Portugues
const char *MCS_opcoesMenuPrincipal[TAM_MENU_PRINCIPAL]={
  "(1) Preparacao  ",  
  "(2) Exibicao    ",
  "(3) Operacao    ",
  "(4) Faturamento ",
  "(5) Perifericos ",
  "(6) Teste hard  ",
  "(7) Wifi        ",
  "(8) Avancado    ",
  "    Voltar      "
};

const char *MCS_opcoesMenuPreparacao[TAM_MENU_PREPARACAO]={
  "(1)Temp. inicial",    
  "(2)Temp. Final  ",
  "(3)Rampa aquec. ",
  "(4)Veloc. prep. ",
  "(5)Atraso emb.  ",
  "(6)Teste prep.  ",
  "(7)Conf. fabrica",
  " Voltar         "
};

const char *MCS_opcoesMenuExibicao[TAM_MENU_EXIBICAO]={
  "(1)Fitaled prop.",
  "(2)Fitaled prep.",
  "(3)Tempo Propag ",
  "(4)Volume musica",
  "(5)Volume voz   ",
  "(6)Mensagens 01 ",
  "(7)Mensagens 02 ",
  "(8)Mensagens 03 ",
  "(9)Mensagens 04 ",
  "  Voltar        "  
};

const char *MCS_opcoesOperacao[TAM_MENU_OPERACAO]={
  "(1) Valor Pipoca",
  "(2) Tipos cedula",
  "(3) Amostras    ",
  "(4) Locacao     ",
  "(5) Desumid.    ",  
  " Voltar         "
};

const char *MCS_opcoesConfiguraCedulas[]={
  " [ ] R$ 2,00    ",
  " [ ] R$ 5,00    ",
  " [ ] R$ 10,00   ",
  " [ ] R$ 20,00   ",
  " [ ] R$ 50,00   ",
  " [ ] R$ 100,00  ",
  " Voltar         ",
};

const char *MCS_opcoesDesumidificacao[TAMANHO_OPCOES_DESUMI]={
  "(1) Habilitar   ",
  "(2) Velocidade  ",
  "(3) Temperatura ",
  "    Voltar      "  
};

const char *MCS_opcoesArrecadacao[TAM_LISTA_ARRECADACAO]={  
  "(1)Zerar parcial",
  "(2) Unid.parcial",
  "(3) Unid.total  ",
  "(4) Moeda parc. ", 
  "(5) Moeda total ",
  "(6) Comissao    ",
  "(7) Cartao Cred.",
  "    Voltar      "
};

const char *MCS_ocpoesPerifericos[TAM_MENU_PERIFERICOS]={
  "(1)Mod. impres. ",  
  "(2)Mod. noteiro ",
  "(3)Mod. moedeiro",
  "  Voltar        "
};

const char *MCS_testeHardware[TAM_LISTA_HARDWARE]={  
  "(1) Sen. temp.  ",
  "(2) Sen. embal. ",
  "(3) Sen. dose   ",
  "(4) Sen. rotacao",
  "(5) Sinal 60Hz  ",
  "(6) Motor emb.  ",
  "(7) Moedeiro    ",
  "(8) Noteiro     ",
  "(9) Musica      ",
  "(10) Voz        ",
  "(11) LEDs       ",
  "(12) Chave porta",
  "(13) Fim papel  ",
  "(14) Telemetria ",
  "(15) Led gratis ",
  "(16) Resistencia",
  "(17) data/hora  ",
  "(18) Saida Aux. ",
  "     voltar     "  
};

const char *MCS_mensagemMenuWifi[TAM_LISTA_WIFI]={
  "(1)Exibir SSID  ",
  "(2)Alterar SSID ",
  "(3)Excluir SSID ",
  "(4)Exibir senha ",
  "(5)Alterar senha",
  "(6)Excluir senha",
  "   Voltar       "  
};

const char *MCS_mensagemAvancado[TAM_LISTA_AVANCADO]={
  "(1) Hora/Data   ",
  "(2) Aj. Contador",
  "(3) Num. serie  ",
  "(4) Test. prep. ",
  "(5) Dur. proces.",
  "(6) Vel. final 1",
  "(7) Vel. final 2",
  "(8) Vel. limpeza",
  "(9) Zeramento   ",
  "    Voltar      "
};

const char *MCS_mensagemMenuAjusteContadores[TAM_LISTA_CONTADORES]={
  "(1) Unid. Parc. ",
  "(2) Unid. Total ",
  "(3) Arrec. Parc.",
  "(4) Arrec. Total",
  "    Voltar      "
};
#endif
///----- Idioma espanhol
#ifdef ESPANHOL
const char *MCS_opcoesMenuPrincipal[TAM_MENU_PRINCIPAL]={
  "(1)Preparacion  ", 
  "(2)Visualizacion",
  "(3)Operacion    ",
  "(4)Faturacion   ",
  "(5)Perifericos  ",
  "(6)Prueba hard  ",
  "(7)WIFI         ",
  "(8)Avanzado     ",
  "   Volver       "
};

const char *MCS_opcoesMenuPreparacao[TAM_MENU_PREPARACAO]={
  "(1)Temp. inicial",    
  "(2)Temp. Final  ",
  "(3)Rampa Calef. ",
  "(4)Veloc. prep. ",
  "(5)Retardo emb. ",
  "(6)Prueba prep. ",
  "(7)Ajuste de fab",
  "   Volver       "
};

const char *MCS_opcoesMenuExibicao[TAM_MENU_EXIBICAO]={
  "(1)Fitaled prop.",
  "(2)Fitaled prep.",
  "(3)Tempo Propag ",
  "(4)Volume musica",
  "(5)Volume voz   ",
  "(6)Mensagens 01 ",
  "(7)Mensagens 02 ",
  "(8)Mensagens 03 ",
  "(9)Mensagens 04 ",
  "  Volver        "  
};

const char *MCS_opcoesOperacao[TAM_MENU_OPERACAO]={
  "(1) Valor Pop   ",
  "(2) Tipos cedula",
  "(3) Muestras    ",
  "(4) Ubication   ",
  "(5) Deshumid.   ",  
  " Volver         "
};

const char *MCS_opcoesConfiguraCedulas[]={
  " [ ] $U 20,00   ",
  " [ ] $U 50,00   ",
  " [ ] $U 100,00  ",
  " [ ] $$ 200,00  ",
  " [ ] $U 500,00  ",
  " [ ] $U 1000,00 ",
  " Volver         ",
};

const char *MCS_opcoesDesumidificacao[TAMANHO_OPCOES_DESUMI]={
  "(1) Permitir    ",
  "(2) Velocidade  ",
  "(3) Calefacion  ",
  "    Voltar      "  
};

const char *MCS_opcoesArrecadacao[TAM_LISTA_ARRECADACAO]={  
  "(1)Zerar parcial",
  "(2) Unid.parcial",
  "(3) Unid.total  ",
  "(4) Moeda parc. ", 
  "(5) Moeda total ",
  "(6) Comissao    ",
  "(7) Cartao Cred.",
  "    Voltar      "
};

const char *MCS_ocpoesPerifericos[TAM_MENU_PERIFERICOS]={
  "(1)Mod. impres. ",  
  "(2)Mod. noteiro ",
  "(3)Mod. moedeiro",
  "  Voltar        "
};

const char *MCS_testeHardware[TAM_LISTA_HARDWARE]={  
  "(1) Sen. temp.  ",
  "(2) Sen. embal. ",
  "(3) Sen. dose   ",
  "(4) Sen. rotacao",
  "(5) Sinal 60Hz  ",
  "(6) Motor emb.  ",
  "(7) Moedeiro    ",
  "(8) Noteiro     ",
  "(9) Musica      ",
  "(10) Voz        ",
  "(11) LEDs       ",
  "(12) Chave porta",
  "(13) Fim papel  ",
  "(14) Telemetria ",
  "(15) Led gratis ",
  "(16) Resistencia",
  "(17) data/hora  ",
  "(18) Saida Aux. ",
  "     voltar     "  
};

const char *MCS_mensagemMenuWifi[TAM_LISTA_WIFI]={
  "(1)Exibir SSID  ",
  "(2)Alterar SSID ",
  "(3)Excluir SSID ",
  "(4)Exibir senha ",
  "(5)Alterar senha",
  "(6)Excluir senha",
  "   Voltar       "  
};

const char *MCS_mensagemAvancado[TAM_LISTA_AVANCADO]={
  "(1) Hora/Data   ",
  "(2) Aj. Contador",
  "(3) Num. serie  ",
  "(4) Test. prep. ",
  "(5) Dur. proces.",
  "(6) Vel. final 1",
  "(7) Vel. final 2",
  "(8) Vel. limpeza",
  "(9) Zeramento   ",
  "    Voltar      "
};

const char *MCS_mensagemMenuAjusteContadores[TAM_LISTA_CONTADORES]={
  "(1) Unid. Parc. ",
  "(2) Unid. Total ",
  "(3) Arrec. Parc.",
  "(4) Arrec. Total",
  "    Voltar      "
};
#endif

/************************************************************************************
*       Funções locais
************************************************************************************/
void MCS_desenhaOpcoes(unsigned char indice);
void MCS_menuParametrosPreparacao(void);
void MCS_configTempoPreparoPipocas(void);
void MCS_configTemperaturaPipocas(void);
void MCS_configuraVelocidadePreparacao(void);
void MCS_configuraVelocidadeExpulsao(void);
void MCS_configuraAtrasoPacote(void);
void MCS_configuraValorPipoca(void);
unsigned char MCS_telaSenha(unsigned short int senha);
void MCS_telaConfiguraBeta(void);
void MCS_telaConfiguracaoRT(void);
void MCS_telaConfiguraTermistor(void);
void MCS_desenhaDadosTermistor(unsigned char indice);
void MCS_telaConfiguraTipoCedulas(void);
void MCS_desenhaOpcoesTipoCedulas(unsigned char indice,unsigned char flags);
void MCS_telaPipocaGratis(void);
void MCS_desenhaOpcoesArrecadacao(unsigned char indice);
void MCS_visualizaArrecadacao(void);
void MCS_telaZeraContadorArrecadacao(void);
void MCS_telaExibeTotalPermanente(void);
void MCS_telaConfiguraIntervaloPropaganda(void);
void MCS_telaConfiguraFitaLed(void);
void MCS_telaPreProcesso(void);
void MCS_menuConfiguraVelocidadeExpulsaoPipocas(void);
void MCS_menuConfiguraVelocidadeFinalExpulsao(void);
void MCS_volumeSom(void);
void MCS_ajusteFatorTemperatura(void);
void MCS_telaCadastroVelocidadeLimpeza(void);
void MCS_telaConfiguraVelocidadePreAquecimento(void);
void MCS_menuTesteHardware(void);
void MCS_desenhaOpcoesHardware(unsigned char indice);
void MCS_telaTesteSensorTemperatura(void);
void MCS_telaTesteSensorPapel(void);
void MCS_testeSensorDose(void);
void MCS_testeRotacaoMotor(void);
void MCS_teste60Hz(void);
void MCS_testeMotorEmbalagem(void);
void MCS_testeMoedeiro(void);
void MCS_telaTesteAudio(void);
void MCS_telaTesteFitaLed(void);
void MCS_telaFitaPreparo(void);
void MCS_telaVolumeVoz(void);
void MCS_telaConfiguracaoComissaoPonto(void);
void MCS_telaTesteChaveFimPapel(void);
void MCS_telaTesteChavePorta(void);
void MCS_telaTesteTelemetria(void);
void MCS_telaTesteSaidaLedGratis(void);
void MCS_testaTesteAnguloFase(void);
void MCS_configuracaoFabrica(void);
void MCS_telaTesteVoz(void);
void MCS_telaCapturaConfigFabrica(void);
void MCS_telaOpcaoLocacao(void);
extern void PIPOCATESTE_prepara(void);
void MCS_zeraContadores(void);
void MCS_configuraTemperaturaInicial(void);
void MCS_configuraTempoPreAquecimento(void);
void MCS_configMensagemCliente(unsigned char index);
void MCS_cadastraMensagem_01(void);
void MCS_cadastraMensagem_02(void);
void MCS_cadastraMensagem_03(void);
void MCS_cadastraMensagem_04(void);
void MCS_menuCadastroMensagens(void);
void MCS_desenhaOpcoesMensagens(unsigned char indice);
void MCS_cadastraDesumidificador(void);
void MCS_configuraVelocidadeDesumificador(void);
void MCS_cadastraTemperaturaDesumificacao(void);
void MCS_telaAssinatura(void);
void MCS_visualizacaoHorimetro(void);
void MCS_liberacaoHorimetro(void);
void MCS_desenhaOpcoesHorimetro(unsigned char indice);
void MCS_menuAjusteContadores(void);
void MCS_desenhaOpcoesContadores(unsigned char indice);
void MCS_ajustaContadorUnidadesParcial(void);
void MCS_ajustaContadorUnidadasTotal(void);
void MCS_ajustaArrecadacaoParcial(void);
void MCS_ajustaArrecadacaoTotal(void);
unsigned int MCS_telaInformaValorInt(char*titulo);
unsigned char MCS_checaContraSenha(void);
void MCS_exibeTotalizadorUnidadesParcial(void);
void MCS_exibeTotalizadorUnidadesTotal(void);
void MCS_configuracaoWifiEntry(void);
void MCS_desenhaOpcoesMenuAjusteWifi(unsigned char indice);
void MCS_menuExibeSSID(void);
void MCS_telaAlterarSSID(void);
void MCS_telaSalvaSSID(char *ssid);
void MCS_removeEspacosFinalString(char*string);
void MCS_removeSSID(void);
void MCS_menuVisualizaSenhaWiFi(void);
void MCS_telaSalvaSenhaWiFi(char *senha);
void MCS_telaEditaSenhaWifi(void);
void MCS_removeEspacosFinalString(char*string);
void MCS_apagaSenhaWifi(void);
void MCS_selecionaTipoNoteiro(void);
void MCS_menuConfiguraImpressora(void);
void MCS_telaVendasCartao(void);
void MCS_telaEscolheMoedeiro(void);
void MCS_ajusteRelogio(void);
void MCS_testeRTC(void);
void MCS_cadastraNumeroSerie(void);
void MCS_menuOpcoesExibicao(void);
void MCS_menuOpcoesOperacao(void);
void MCS_menuPerifericos(void);
void MCS_menuParametrosRestritos(void);
void MCS_menuTelemetria(void);
void MCS_menuDesumidificacao(void);
void MCS_menuConfiguraPerifericos(void);
void MCS_desenhaOpcoesPerifericos(unsigned char indice);
void MCS_menuAvancado(void);
void MCS_desenhaOpcoesAvancado(unsigned char indice);
void MCS_testeFram(void);
void MCS_testeAjusteVolumeMusica(void);
void MCS_interfaceConfiguraVolumeTesteVoz(void);
void MCS_configuraTesteTempoEmbalagem(void);
void MCS_menuConfigExibeCartao(void);
void MCS_testeSaidaBloqueioP70(void);

/************************************************************************************
*       Descrição       
************************************************************************************/
void(*const MCS_funcMenuPrincipal[])(void)={
  MCS_menuParametrosPreparacao,
  MCS_menuOpcoesExibicao,
  MCS_menuOpcoesOperacao,
  MCS_menuArrecadacao,
  MCS_menuConfiguraPerifericos,
  MCS_menuTesteHardware,
  MCS_configuracaoWifiEntry,
  MCS_menuAvancado,
  NULL
};

void(*const MCS_funcMenuPreparo[])(void)={  
  MCS_configuraTemperaturaInicial,
  MCS_configTemperaturaPipocas,
  MCS_ajusteFatorTemperatura,
  MCS_configuraVelocidadePreparacao,
  MCS_configuraTesteTempoEmbalagem,
  PIPOCATESTE_prepara,
  MCS_configuracaoFabrica,
  NULL    
};

void(*const MCS_funcMenuExibicao[])(void)={
  MCS_telaConfiguraFitaLed,
  MCS_telaFitaPreparo,
  MCS_telaConfiguraIntervaloPropaganda,
  MCS_testeAjusteVolumeMusica,
  MCS_interfaceConfiguraVolumeTesteVoz,
  MCS_cadastraMensagem_01,
  MCS_cadastraMensagem_02,
  MCS_cadastraMensagem_03,
  MCS_cadastraMensagem_04,
  NULL
};

void(*const MCS_funcOperacao[])(void)={
  MCS_configuraValorPipoca,
  MCS_telaConfiguraTipoCedulas,
  MCS_telaPipocaGratis,
  MCS_telaOpcaoLocacao,
  MCS_menuDesumidificacao,
  NULL,
};

void(*const MCS_funcDesumidificador[])(void)={
  MCS_cadastraDesumidificador,
  MCS_configuraVelocidadeDesumificador,
  MCS_cadastraTemperaturaDesumificacao,
  NULL
};

void(*const MCS_funcArrecadacao[])(void)={
  MCS_telaZeraContadorArrecadacao, 
  MCS_exibeTotalizadorUnidadesParcial,
  MCS_exibeTotalizadorUnidadesTotal,
  MCS_visualizaArrecadacao,
  MCS_telaExibeTotalPermanente,
  MCS_telaConfiguracaoComissaoPonto,
  MCS_menuConfigExibeCartao,
  NULL  
};

void(*const MCS_funcPerifericos[])(void)={
  MCS_menuConfiguraImpressora,
  MCS_selecionaTipoNoteiro,
  MCS_telaEscolheMoedeiro,
  NULL
};

void(*const MCS_funcHardware[])(void)={
  MCS_telaTesteSensorTemperatura,
  MCS_telaTesteSensorPapel,
  MCS_testeSensorDose,
  MCS_testeRotacaoMotor,
  MCS_teste60Hz,
  MCS_testeMotorEmbalagem,
  MCS_testeMoedeiro,
  MCS_testeMoedeiro,
  MCS_testeAjusteVolumeMusica,
  MCS_interfaceConfiguraVolumeTesteVoz,
  MCS_telaTesteFitaLed,
  MCS_telaTesteChavePorta,
  MCS_telaTesteChaveFimPapel,
  MCS_telaTesteTelemetria,
  MCS_telaTesteSaidaLedGratis,
  MCS_testaTesteAnguloFase,
  MCS_testeRTC,
  MCS_testeSaidaBloqueioP70,
  NULL    
};

void(*const MCS_funcWifi[])(void)={
  MCS_menuExibeSSID,
  MCS_telaAlterarSSID,
  MCS_removeSSID,
  MCS_menuVisualizaSenhaWiFi,
  MCS_telaEditaSenhaWifi,
  MCS_apagaSenhaWifi,
  NULL  
};

void(*const MCS_funcAvancado[])(void)={
  MCS_ajusteRelogio,
  MCS_menuAjusteContadores,
  MCS_cadastraNumeroSerie,
  PIPOCATESTE_prepara,
  MCS_configTempoPreparoPipocas,
  MCS_menuConfiguraVelocidadeExpulsaoPipocas,
  MCS_menuConfiguraVelocidadeFinalExpulsao,
  MCS_telaCadastroVelocidadeLimpeza,
  MCS_zeraContadores,
  NULL
};

void(*const MCS_funcAjusteContadores[])(void)={
   MCS_ajustaContadorUnidadesParcial,
   MCS_ajustaContadorUnidadasTotal,
   MCS_ajustaArrecadacaoParcial,
   MCS_ajustaArrecadacaoTotal,
   NULL
};
/************************************************************************************
*       Variaveis locais
************************************************************************************/
unsigned int MCS_contadorSemente=0;
extern unsigned int BOARD_tempoPiscadas;

/************************************************************************************
*       Funções locais
************************************************************************************/
void MCS_desenhaOpcoes(unsigned char indice);
void MCS_desenhaOpcoesPreparacao(unsigned char indice);


/************************************************************************************
*       Implementação das funções
************************************************************************************/

/************************************************************************************
*       Descrição       :       Ponto de entrada do menu de configuração
*                               dos parametros da máquina
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_entry(void){
  eTECLA tecla;  
  unsigned char indiceOpcoes=0;  
      
  HD44780_clearText();    
  MCS_desenhaOpcoes(indiceOpcoes);
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ESC:
          HD44780_clearText();             
          return;
     case TECLA_DEC:
          indiceOpcoes?(indiceOpcoes--):(indiceOpcoes=TAM_MENU_PRINCIPAL-1);
          MCS_desenhaOpcoes(indiceOpcoes);
          break;
     case TECLA_INC:
          indiceOpcoes = (indiceOpcoes+1) % TAM_MENU_PRINCIPAL;
          MCS_desenhaOpcoes(indiceOpcoes);
          break;
     case TECLA_ENTER:
          if(MCS_funcMenuPrincipal[indiceOpcoes]!=NULL){
            MCS_funcMenuPrincipal[indiceOpcoes]();
            MCS_desenhaOpcoes(indiceOpcoes);  
          }
          else{
             HD44780_clearText();
            return;
          }
          break;
    }           
    vTaskDelay(50);
    MCS_contadorSemente++;
  }   
  
  HD44780_clearText();      
} 
/************************************************************************************
*       Descrição       :       Desenha as opções do menu de configuração
*                               do sistema
*       Parametros      :       (unsigned char) indice da opção
*       Retorno         :       nenhum
************************************************************************************/
void MCS_desenhaOpcoes(unsigned char indice){
  
  HD44780_posicionaTexto(0,0);
  HD44780_writeString("Menu principal");
  
  HD44780_posicionaTexto(0,1);
  HD44780_writeString((char*)MCS_opcoesMenuPrincipal[indice]);
  HD44780_posicionaTexto(0,1);
  HD44780_writeChar(0x7E);
}
/************************************************************************************
*       Descrição       :       Menu com as opções do menu para
*                               ajuste dos parametros de preparação da pipoca
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_menuParametrosPreparacao(void){
  eTECLA tecla;
  unsigned char indiceOpcoes=0;
  
  HD44780_clearText();
  MCS_desenhaOpcoesPreparacao(indiceOpcoes);
  
  for(;TECLADO_getContadorInatividade();){
  
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:  
           if(MCS_funcMenuPreparo[indiceOpcoes]!=NULL){
             MCS_funcMenuPreparo[indiceOpcoes]();
             MCS_desenhaOpcoesPreparacao(indiceOpcoes);  
           }
           else
             return;        
           break;
      case TECLA_ESC:            
           return;
      case TECLA_DEC:
           indiceOpcoes?(indiceOpcoes--):(indiceOpcoes=TAM_MENU_PREPARACAO-1);
           MCS_desenhaOpcoesPreparacao(indiceOpcoes);           
           break;
      case TECLA_INC:
           indiceOpcoes = (indiceOpcoes+1) % TAM_MENU_PREPARACAO;
           MCS_desenhaOpcoesPreparacao(indiceOpcoes);           
           break;
    }
    
    vTaskDelay(50);
  }
}
/************************************************************************************
*       Descrição       :       Menu com as opções do menu de configuração
*                               da preparação
*       Parametros      :       (unsigned char) indice
*       Retorno         :       nenhum
************************************************************************************/
void MCS_desenhaOpcoesPreparacao(unsigned char indice){  
  HD44780_posicionaTexto(0,0);
  HD44780_writeString("Menu  preparo");
  
  HD44780_posicionaTexto(0,1);
  HD44780_writeString((char*)MCS_opcoesMenuPreparacao[indice]);
  HD44780_posicionaTexto(0,1);
  HD44780_writeChar(0x7E);
}
/************************************************************************************
*       Descrição       :       Tela para o usuário configurar o tempo
*                               de preparação das pipocas
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_configTempoPreparoPipocas(void){
  eTECLA tecla;
  char bufferLinha[17];
  unsigned short int tempoPreparo = PARAMETROS_leParametro(TEMPO_PREPARO);

  HD44780_clearText();
  HD44780_writeString(" Tempo preparo  ");
    
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ENTER:
          PARAMETROS_escreveParametro(TEMPO_PREPARO,tempoPreparo);    
     case TECLA_ESC:
          HD44780_clearText();
          return;
     case TECLA_DEC:
          if(tempoPreparo>30)
            tempoPreparo--;       
          break;
     case TECLA_INC:
          if(tempoPreparo<180)
            tempoPreparo++;
          break;
    }
        
    sprintf(bufferLinha,"%03d s",tempoPreparo);
    HD44780_posicionaTexto(6,1);
    HD44780_writeString(bufferLinha);
    
    vTaskDelay(50);
  }   
}
/************************************************************************************
*       Descrição       :       Tela para o usuário configurar a temperatura
*                               de preparação das pipocas
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_configTemperaturaPipocas(void){
  eTECLA tecla;  
  char bufferLinha[17];
  unsigned short int temperatura = PARAMETROS_leParametro(TEMPERATURA_PREPARO);
  
  HD44780_clearText();
  HD44780_writeString("  Temp. maxima  ");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ENTER:
          PARAMETROS_escreveParametro(TEMPERATURA_PREPARO,temperatura);
     case TECLA_ESC:
          HD44780_clearText();
          return;
     case TECLA_DEC:
          if(temperatura>60)
            temperatura--;       
          break;
     case TECLA_INC:
          if(temperatura<200)
            temperatura++;
          break;
    }
           
    sprintf(bufferLinha,"%03d oC",temperatura);
    HD44780_posicionaTexto(6,1);
    HD44780_writeString(bufferLinha);
    
    vTaskDelay(50);
  }    
}
/************************************************************************************
*       Descrição       :       Tela para o usuário configurar a velocidade
*                               de preparação das pipocas
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_configuraVelocidadePreparacao(void){
  eTECLA tecla;
  char bufferLinha[17];
  unsigned short int velPreparo = PARAMETROS_leParametro(VELOCIDADE_PREPARO);

  HD44780_clearText();
  HD44780_writeString(" Veloc. preparo ");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ENTER:
          PARAMETROS_escreveParametro(VELOCIDADE_PREPARO,velPreparo);
     case TECLA_ESC: 
          HD44780_clearText();
          return;
     case TECLA_DEC:
          if(velPreparo>2000)
            if(BOARD_tempoPiscadas>5000)
              velPreparo--;
            else
              if(BOARD_tempoPiscadas>3000)
                velPreparo-=10;
              else
                velPreparo-=100; 
          break;
     case TECLA_INC:
          if(velPreparo<18000)
            if(BOARD_tempoPiscadas>5000)
              velPreparo++;
            else
              if(BOARD_tempoPiscadas>3000)
                velPreparo+=10;
              else
                velPreparo+=100;
          break;
     default:
          BOARD_tempoPiscadas = 5000;
          break;
    }
        
    sprintf(bufferLinha,"%05d RPM",velPreparo);
    HD44780_posicionaTexto(3,1);
    HD44780_writeString(bufferLinha);              
    
    vTaskDelay(50);
  }      
}
/***********************************************************************************
*       Descrição       :       Tela para o usuário configurar a velocidade
*                               de explusão das pipocas
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_configuraVelocidadeExpulsao(void){
  eTECLA tecla;
  char bufferLinha[17];
  unsigned short int velExpulsao = PARAMETROS_leParametro(VELOCIDADE_EXPULSAO);
  
  HD44780_clearText();
  HD44780_writeString(" Veloc. Final   ");  
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ENTER:
          PARAMETROS_escreveParametro(VELOCIDADE_EXPULSAO,velExpulsao);
     case TECLA_ESC: 
          HD44780_clearText();
          return;
     case TECLA_DEC:
          if(velExpulsao>50)
            velExpulsao--;
          break;
     case TECLA_INC:
          if(velExpulsao<100)
            velExpulsao++;
          break;
    }    
    
    sprintf(bufferLinha,"%03d %",velExpulsao);
    HD44780_posicionaTexto(6,1);
    HD44780_writeString(bufferLinha);      
    
    vTaskDelay(50); 
  }  
}
/************************************************************************************
*       Descrição       :       Tela para o usuário configurar o atraso
*                               entre iniciar a preparar a pipoca e 
*                               a liberação do pacote
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_configuraAtrasoPacote(void){
  eTECLA tecla;
  char bufferLinha[17];
  unsigned short int atrasoPacote = PARAMETROS_leParametro(ATRASO_PACOTE);
    
  HD44780_clearText();
  HD44780_writeString(" Atraso pacote  ");    
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ENTER:
          PARAMETROS_escreveParametro(ATRASO_PACOTE,atrasoPacote);
     case TECLA_ESC: 
          HD44780_clearText();
          return;
     case TECLA_DEC:
          if(atrasoPacote>1)
            atrasoPacote--;
          break;
     case TECLA_INC:
          if(atrasoPacote<200)
            atrasoPacote++;
          break;
    }    
    
    bufferLinha[0] = ((atrasoPacote/100)%10) + '0';
    bufferLinha[1] = ((atrasoPacote/10)%10) + '0';
    bufferLinha[2] = ',';
    bufferLinha[3] = (atrasoPacote%10) + '0';
    bufferLinha[4] = '\0';        
    
    HD44780_posicionaTexto(6,1);
    HD44780_writeString(bufferLinha);     
    vTaskDelay(50);
  }    
}
/************************************************************************************
*       Descrição       :       Configura o valor da pipoca
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_configuraValorPipoca(void){
  eTECLA tecla;
  char bufferLinha[17];
  unsigned short int valorPipoca =  PARAMETROS_leParametro(VALOR_PIPOCA);
  
  HD44780_clearText();
  HD44780_writeString(" Valor pipoca   ");   
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ENTER:
          PARAMETROS_escreveParametro(VALOR_PIPOCA,valorPipoca);
     case TECLA_ESC: 
          HD44780_clearText();
          return;
     case TECLA_DEC:
          if(valorPipoca>1)
            valorPipoca--;
          break;
     case TECLA_INC:
          if(valorPipoca<999)
            valorPipoca++;
          break;
    }    
    
    sprintf(bufferLinha,"R$ %03d.00 ",valorPipoca);
    HD44780_posicionaTexto(3,1);
    HD44780_writeString(bufferLinha);     
    vTaskDelay(50);
  }        
}
/************************************************************************************
*       Descrição       :       Tela para o usuário informar uma senha
*       Parametros      :       (unsigned int) senha de acesso
*       Retorno         :       nenhum
************************************************************************************/
unsigned char MCS_telaSenha(unsigned short int senha){
  eTECLA tecla;
  unsigned char indiceDigito=0;
  char digitoSenha[5]={"0000"};
  unsigned short int temp;  
  unsigned char toggle=0;
  unsigned char ciclos=1;
  char bufferLinha[5];
  
  HD44780_clearText();
  HD44780_writeString(" Informe a senha");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           indiceDigito++;
           if(indiceDigito>3){
             
             temp = ((digitoSenha[0]-'0')*1000) + ((digitoSenha[1]-'0')*100) + ((digitoSenha[2]-'0')*10) + (digitoSenha[3]-'0');
             if(temp==senha)
               return 255;
             else{
               
               HD44780_clearText();
               HD44780_writeString("Senha");
               HD44780_posicionaTexto(0,1);
               HD44780_writeString("incorreta!");
               
               vTaskDelay(3000);
               
               HD44780_clearText();
             }             
             return 0;
           }
           break;
      case TECLA_ESC:
           return 0;
      case TECLA_INC:
           digitoSenha[indiceDigito]++;
           if(digitoSenha[indiceDigito]>'9')
             digitoSenha[indiceDigito] = '0';
           break;
      case TECLA_DEC:
           digitoSenha[indiceDigito]--;
           if(digitoSenha[indiceDigito]<'0')
             digitoSenha[indiceDigito] = '9';
           break;
    }
    
    if(!--ciclos){
      
      ciclos = 4;
      toggle = ~toggle;
      memcpy(bufferLinha,digitoSenha,4);
      bufferLinha[4] = 0x00;
      if(toggle)
        bufferLinha[indiceDigito] = '_';
      
      HD44780_posicionaTexto(6,1);
      HD44780_writeString(bufferLinha);
    }        
    
    vTaskDelay(50);
  }      
  
  return 0;
}
/************************************************************************************
*       Descrição       :       Tela para configuração do beta do termistor
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaConfiguraBeta(void){
  eTECLA tecla;
  char bufferLinha[7];
  char bufferTemp[7];
  unsigned int beta = PARAMETROS_carregaBetaTermistor();
  unsigned char indice=0;
  unsigned char toogle=0;
  unsigned char ciclos=1;
  
  bufferLinha[0] = ((beta/100000)%10) + '0';
  bufferLinha[1] = ((beta/10000)%10) + '0';
  bufferLinha[2] = ((beta/1000)%10) + '0';
  bufferLinha[3] = ((beta/100)%10) + '0';
  bufferLinha[4] = ((beta/10)%10) + '0';
  bufferLinha[5] = ((beta)%10) + '0';
  bufferLinha[6] = '\0';
  
  HD44780_clearText();
  HD44780_writeString(" Beta termistor ");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           if(indice>4){
             beta = ((bufferLinha[0]-'0')*100000) + ((bufferLinha[1]-'0')*10000) + ((bufferLinha[2]-'0')*1000) + ((bufferLinha[3]-'0')*100) + ((bufferLinha[4]-'0')*10) + (bufferLinha[5]-'0');
             PARAMETROS_salvaBetaTermistor(beta);
           }
           else{
             indice++;
             break;
           }
      case TECLA_ESC:
           return;
      case TECLA_INC:
           bufferLinha[indice]++;
           if(bufferLinha[indice]>'9')
             bufferLinha[indice] = '0';             
           break;
      case TECLA_DEC:
           bufferLinha[indice]--;
           if(bufferLinha[indice]<'0')
             bufferLinha[indice] = '9';
           break;
    }
    
    if(!--ciclos){
      ciclos = 4;
      toogle=~toogle;
      memcpy(bufferTemp,bufferLinha,6);
      bufferTemp[6] = 0;
      if(toogle)
        bufferTemp[indice] = '_';
      
      HD44780_posicionaTexto(5,1);
      HD44780_writeString(bufferTemp);      
    }
    
    vTaskDelay(50);
  }//for(.... 
}
/************************************************************************************
*       Descrição       :       Configura o rt do termistor
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaConfiguracaoRT(void){
  eTECLA tecla;
  char bufferLinha[9];
  char bufferTmp[9];  
  unsigned int rt = PARAMETROS_carregaRTtermistor();
  unsigned char indice=0;
  unsigned char toggle=0;
  unsigned char ciclos=1;
  
  bufferLinha[0] = ((rt/10000000)%10) + '0';
  bufferLinha[1] = ((rt/1000000)%10) + '0';
  bufferLinha[2] = ((rt/100000)%10) + '0';
  bufferLinha[3] = ((rt/10000)%10) + '0';
  bufferLinha[4] = ((rt/1000)%10) + '0';
  bufferLinha[5] = ((rt/100)%10) + '0';
  bufferLinha[6] = ((rt/10)%10) + '0';
  bufferLinha[7] = (rt%10) + '0';
  bufferLinha[8] = '\0';  
  
  HD44780_clearText();
  HD44780_writeString("RT");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           if(indice>6){
             rt = ((bufferLinha[0]-'0')*10000000) + 
                  ((bufferLinha[1]-'0')*1000000) +
                  ((bufferLinha[2]-'0')*100000) + 
                  ((bufferLinha[3]-'0')*10000) + 
                  ((bufferLinha[4]-'0')*1000) + 
                  ((bufferLinha[5]-'0')*100) + 
                  ((bufferLinha[6]-'0')*10) + 
                  ((bufferLinha[7]-'0'));
             PARAMETROS_salvaRTtermistor(rt);
           }
           else{
             indice++;
             break;
           }
      case TECLA_ESC:
           return;
      case TECLA_INC:
           bufferLinha[indice]++;
           if(bufferLinha[indice]>'9')
             bufferLinha[indice] = '0';
           break;
      case TECLA_DEC:
           bufferLinha[indice]--;
           if(bufferLinha[indice]<'0')
             bufferLinha[indice] = '9';
           break;
    }    
    
    if(!--ciclos){
      
      ciclos = 4;
      toggle=~toggle;
      memcpy(bufferTmp,bufferLinha,8);
      if(toggle)
        bufferTmp[indice]='_';
      
      bufferTmp[8] = 0;      
      HD44780_posicionaTexto(4,1);
      HD44780_writeString(bufferTmp);      
    }
    
    
    vTaskDelay(50);
  }      
}
/************************************************************************************
*       Descrição       :       Tela para configuração das cédulas que 
*                               serão aceitas pelo noteiro
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaConfiguraTipoCedulas(void){
  eTECLA tecla;
  unsigned char indice=0;
  unsigned char flags[7];
  
  PARAMETROS_carregaTipoCedulas(&flags[0],&flags[1],&flags[2],&flags[3],&flags[4],&flags[5]);  
  HD44780_clearText();
  HD44780_writeString("Tipos de cedulas");
  
  MCS_desenhaOpcoesTipoCedulas(indice,flags[indice]);
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           if(indice==6){
            //salva na opção voltar          
            PARAMETROS_gravaTipoCedulas(flags[0],flags[1],flags[2],flags[3],flags[4],flags[5]); 
            return;
           }
           else{
             flags[indice]=~flags[indice];  
             MCS_desenhaOpcoesTipoCedulas(indice,flags[indice]);                   
           }
           break;
      case TECLA_ESC:
           return;
      case TECLA_INC:
           indice = (indice+1) % 7;
           MCS_desenhaOpcoesTipoCedulas(indice,flags[indice]);         
           break;
      case TECLA_DEC:
           indice?(indice--):(indice=6);
           MCS_desenhaOpcoesTipoCedulas(indice,flags[indice]);         
           break;
    }       
    
    vTaskDelay(50);
  }      
}
/************************************************************************************
*       Descrição       :       Desenha as opções das cédulas
*       Parametros      :       (unsigned char) indice
*                               (unsigned char) flag
*       Retorno         :       nenhum
************************************************************************************/
void MCS_desenhaOpcoesTipoCedulas(unsigned char indice,unsigned char flags){
  
  HD44780_posicionaTexto(0,1);
  HD44780_writeString((char*)MCS_opcoesConfiguraCedulas[indice]);
  if(indice<6 && flags){
    HD44780_posicionaTexto(2,1);
    HD44780_writeChar('X');
  }
}
/************************************************************************************
*       Descrição       :       Tela para o usuário gerar a senha
*                               de pipoca grátis
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaPipocaGratis(void){
  eTECLA tecla;
  char bufferLinha[17];
  char bufferTmp[17];
  unsigned char indice=0;
  unsigned char toggle=0;
  unsigned char ciclos=1;
  unsigned char horas;
  unsigned short int senha;
  
  srand(MCS_contadorSemente);
  unsigned short int contraSenha = rand()%10000;
    
  HD44780_clearText();
  sprintf(bufferLinha,"Gerada:%04d",contraSenha);
  HD44780_writeString(bufferLinha);
  
  HD44780_posicionaTexto(0,1);
  HD44780_writeString("Contra:");
  
  sprintf(bufferLinha,"000000");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           if(indice>4){
             
             unsigned int recebido = ((bufferLinha[0]-'0')*100000) + 
                                     ((bufferLinha[1]-'0')*10000) + 
                                     ((bufferLinha[2]-'0')*1000) + 
                                     ((bufferLinha[3]-'0')*100) + 
                                     ((bufferLinha[4]-'0')*10) + 
                                     ((bufferLinha[5]-'0'));
             
             recebido &= 0x3FFFF;             
             horas = recebido>>14;
             
             senha = ((recebido&0x3FFF)^3004);
               HD44780_clearText();             
             if(senha==contraSenha){
               CA_setTempoAmostras(horas);                      
               HD44780_writeString("Amostras");
               HD44780_posicionaTexto(0,1);
               HD44780_writeString("Liberadas");
             }
             else{
               HD44780_writeString("Senha");
               HD44780_posicionaTexto(0,1);
               HD44780_writeString("Invalida");               
             }
             vTaskDelay(3000);
             return;
           }
           else{
             indice++;
             break;
           }
      case TECLA_ESC:
           return;
      case TECLA_INC:
           bufferLinha[indice]++;
           if(bufferLinha[indice]>'9')
             bufferLinha[indice] = '0';
           break;
      case TECLA_DEC:    
           bufferLinha[indice]--;
           if(bufferLinha[indice]<'0')
             bufferLinha[indice] = '9';
           break;
    }
    
    if(!--ciclos){
      ciclos = 4;
      toggle = ~toggle;
      memcpy(bufferTmp,bufferLinha,17);
      bufferTmp[7] = 0x00;
      if(toggle)
        bufferTmp[indice] = '_';
      
      HD44780_posicionaTexto(7,1);
      HD44780_writeString(bufferTmp);
    }       
    
    vTaskDelay(50);
  }      
}
/************************************************************************************
*       Descrição       :       Ponto de entrada do menu de visualização
*                               da arrecadacao
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_menuArrecadacao(void){
  eTECLA tecla;
  unsigned char indice=0;
  
  if(!MCS_telaSenha(4790))
    return;
  
  HD44780_clearText();  
  MCS_desenhaOpcoesArrecadacao(indice);
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           if(MCS_funcArrecadacao[indice]!=NULL){
             MCS_funcArrecadacao[indice]();
             MCS_desenhaOpcoesArrecadacao(indice);           
             break;
           }
      case TECLA_ESC:
           HD44780_clearText();
           return;
      case TECLA_INC:
           indice = (indice+1) % TAM_LISTA_ARRECADACAO;
           MCS_desenhaOpcoesArrecadacao(indice);
           break;
      case TECLA_DEC:      
           indice?(indice--):(indice=(TAM_LISTA_ARRECADACAO-1));
           MCS_desenhaOpcoesArrecadacao(indice);
           break;
    }
                   
    vTaskDelay(100);
  }  
}
/************************************************************************************
*       Descrição       :       Desenha os textos do menu de arrecadação
*       Parametros      :       (unsigned char) indice
*       Retorno         :       nenhum
************************************************************************************/
void MCS_desenhaOpcoesArrecadacao(unsigned char indice){
  
  HD44780_posicionaTexto(0,0);
  HD44780_writeString("Faturamento     ");
  
  HD44780_posicionaTexto(0,1);
  HD44780_writeString((char*)MCS_opcoesArrecadacao[indice]);
  HD44780_posicionaTexto(0,1);
  HD44780_writeChar(0x7E);   
}
/************************************************************************************
*       Descrição       :       Tela para o usuário realizar a visualização
*                               do totalizador 
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_visualizaArrecadacao(void){
  eTECLA tecla;
  unsigned int arrecadacao = PARAMETROS_leContadorArrecadacao();
  char bufferLinha[17];
  
  sprintf(bufferLinha,"R$ %d.00",arrecadacao);
  HD44780_clearText();
  //                   12345678901232456
  HD44780_writeString("Contador parcial ");
  HD44780_posicionaTexto(0,1);
  HD44780_writeString(bufferLinha);    
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
      case TECLA_ESC: 
           return;
    }
            
    vTaskDelay(100);
  }      
}
/************************************************************************************
*       Descrição       :       Tela para o usuário realizar o zeramento
*                               do contador de faturamento
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaZeraContadorArrecadacao(void){
  eTECLA tecla;
    
  HD44780_clearText();
  HD44780_writeString("  Press. enter  ");
  HD44780_posicionaTexto(0,1);
  HD44780_writeString("  p/ imprimir   ");
  
  for(;;){
    tecla = TECLADO_getch();
    if(tecla==TECLA_ENTER)
      RV_relatorioPonto();
    if(tecla==TECLA_ESC)
      break;    
  }
  
  
  if(MCS_telaSenha(1530)){
   
    HD44780_clearText();
    HD44780_writeString("Apagando");
    HD44780_posicionaTexto(0,1);
    HD44780_writeString("Totalizador");
    
    PARAMETROS_salvaContadorArrecadacao(0);
    PARAMETROS_gravaContadorVendasParcial(0);
    
    PARAMETROS_salvaFaturamentoParcialCartao(0);
    
    vTaskDelay(5000);
  }  
}
/************************************************************************************
*       Descrição       :       Tela para exibição do total permanente
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaExibeTotalPermanente(void){
  eTECLA tecla;
  unsigned int total = PARAMETROS_leTotalizadorPermanente();
  char bufferLinha[17];
  
  sprintf(bufferLinha,"R$ %d.00",total);
  HD44780_clearText();
  HD44780_writeString("Contador perman. ");
  HD44780_posicionaTexto(0,1);
  HD44780_writeString(bufferLinha);    
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
      case TECLA_ESC: 
           return;
    }
            
    vTaskDelay(100);
  }      
}
/************************************************************************************
*       Descrição       :       Tela para o usuário configurar o intervalo
*                               da propaganda falada
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaConfiguraIntervaloPropaganda(void){
  eTECLA tecla;
  unsigned short int intervalo = PARAMETROS_leIntervaloPropaganda();
  char bufferLinha[17];
  
  HD44780_clearText();
  HD44780_writeString("Tempo propaganda");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           PARAMETROS_gravaIntervaloPropaganda(intervalo);
      case TECLA_ESC:
           HD44780_clearText();
           return;
      case TECLA_INC:
           intervalo+=10;
           if(intervalo>120)
             intervalo = 0;
           break;
      case TECLA_DEC:
           intervalo?(intervalo-=10):(intervalo=120);
           break;
    }
        
    sprintf(bufferLinha,"%03d min",intervalo);
    HD44780_posicionaTexto(6,1);
    HD44780_writeString(bufferLinha);
    
    vTaskDelay(100);    
  }    
}
/************************************************************************************
*       Descrição       :       Tela para o usuário configurar se a fita
*                               led pisca ou não enquanto a máquina está em 
*                               stand by
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaConfiguraFitaLed(void){
  eTECLA tecla;
  unsigned short int flag = PARAMETROS_leFlagPiscadaPropaganda();
  
  HD44780_clearText();
  HD44780_writeString("Fita led standby");
    
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           PARAMETROS_gravaFlagPiscadaPropaganda(flag);
      case TECLA_ESC: 
           HD44780_clearText();
           return;
      case TECLA_INC:            
      case TECLA_DEC:
           flag = ~flag;
           break;
    }
    
    HD44780_posicionaTexto(6,1);
    if(flag)
      HD44780_writeString("SIM");
    else
      HD44780_writeString("NAO");
    
    vTaskDelay(100);
  }    
}
/************************************************************************************
*       Descrição       :       Interface para o usuário configurar a temperatura
*                               de pré-processo
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaPreProcesso(void){
  eTECLA tecla;
  unsigned short int temperatura = PARAMETROS_leTemperaturaPreProcesso();  
  char bufferLinha[17];
  
  HD44780_clearText();
  HD44780_writeString(" Temp. pre-proc.");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           PARAMETROS_salvaTemperaturaPreProcesso(temperatura);
      case TECLA_ESC:
           HD44780_clearText();
           return;
      case TECLA_INC:
           if(temperatura<150)
             temperatura++;
           break;
      case TECLA_DEC:
           if(temperatura>30)
             temperatura--;
           break;
    }
    
    sprintf(bufferLinha,"%03d oC",temperatura);
    HD44780_posicionaTexto(5,1);
    HD44780_writeString(bufferLinha);
    
    vTaskDelay(100);  
  }    
}
/************************************************************************************
*       Descrição       :       Interface para a visualização dos totalizadores
*                               de dinheiro
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_menuLeiturista(void){
  
  
  if(MCS_telaSenha(2530)){
    MCS_menuArrecadacao();        
  }
  HD44780_clearText();  
}
/************************************************************************************
*       Descrição       :       Configura a velocidade inicial
*                               da expulsão das pipocas
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_menuConfiguraVelocidadeExpulsaoPipocas(void){
  eTECLA tecla;
  char bufferLinha[17];
  unsigned short int velPreparo = PARAMETROS_leRotacaoInicialExpulsao();

  HD44780_clearText();
  HD44780_writeString(" Vel. ini. exp. ");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ENTER:
          PARAMETROS_salvaRotacaoInicialExpulsao(velPreparo);
     case TECLA_ESC: 
          HD44780_clearText();
          return;
     case TECLA_DEC:
          if(velPreparo>2000)
            if(BOARD_tempoPiscadas>1000)
              velPreparo--;
            else
              if(BOARD_tempoPiscadas>500)
                velPreparo-=10;
              else
                velPreparo-=100; 
          break;
     case TECLA_INC:
          if(velPreparo<18000)
            if(BOARD_tempoPiscadas>1000)
              velPreparo++;
            else
              if(BOARD_tempoPiscadas>500)
                velPreparo+=10;
              else
                velPreparo+=100;
          break;
     default:
          BOARD_tempoPiscadas = 3000;
          break;
    }
        
    sprintf(bufferLinha,"%05d RPM",velPreparo);
    HD44780_posicionaTexto(3,1);
    HD44780_writeString(bufferLinha);              
    
    vTaskDelay(50);
  }        
}
/************************************************************************************
*       Descrição       :       Configura a velocidade final da expulsão
*                               das pipocas
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_menuConfiguraVelocidadeFinalExpulsao(void){
  eTECLA tecla;
  char bufferLinha[17];
  unsigned short int velPreparo = PARAMETROS_leVelocidadeFinalExpulsao();

  HD44780_clearText();
  HD44780_writeString(" Vel. fin. exp. ");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ENTER:
          PARAMETROS_salvaVelocidadeFinalExpulsao(velPreparo);
     case TECLA_ESC: 
          HD44780_clearText();
          return;
     case TECLA_DEC:
          if(velPreparo>2000)
            if(BOARD_tempoPiscadas>1000)
              velPreparo--;
            else
              if(BOARD_tempoPiscadas>500)
                velPreparo-=10;
              else
                velPreparo-=100; 
          break;
     case TECLA_INC:
          if(velPreparo<18000)
            if(BOARD_tempoPiscadas>1000)
              velPreparo++;
            else
              if(BOARD_tempoPiscadas>500)
                velPreparo+=10;
              else
                velPreparo+=100;
          break;
     default:
          BOARD_tempoPiscadas = 3000;
          break;
    }
        
    sprintf(bufferLinha,"%05d RPM",velPreparo);
    HD44780_posicionaTexto(3,1);
    HD44780_writeString(bufferLinha);              
    
    vTaskDelay(50);
  }  
}
/************************************************************************************
*       Descrição       :       Tela para configurar o volume do som
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_volumeSom(void){
  eTECLA tecla;
  char bufferLinha[17];
  unsigned short int volume = PARAMETROS_leVolumeSom();

  HD44780_clearText();
  HD44780_writeString(" Volume musica  ");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ENTER:
          PARAMETROS_gravaVolumeSom(volume);
     case TECLA_ESC: 
          HD44780_clearText();
          return;
     case TECLA_DEC:
          if(volume)
            volume--;
          break;
     case TECLA_INC:
          if(volume<100)
            volume++;
          break;
    }
        
    sprintf(bufferLinha,"%03d ",volume);
    HD44780_posicionaTexto(6,1);
    HD44780_writeString(bufferLinha);              
    
    vTaskDelay(50);
  }  
}
/************************************************************************************
*       Descrição       :       Tela para o usuário 
*                               cadastrar o fator de operação da temperatura
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_ajusteFatorTemperatura(void){
  eTECLA tecla;
  char bufferLinha[17];
  unsigned short int volume = PARAMETROS_leFatorTrabalho();

  HD44780_clearText();
  HD44780_writeString(" Tempo de subida");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ENTER:
          PARAMETROS_gravaFatorTrabalho(volume);
     case TECLA_ESC: 
          HD44780_clearText();
          return;
     case TECLA_DEC:
          if(volume>1)
            volume--;
          break;
     case TECLA_INC:
          if(volume<120)
            volume++;
          break;
    }
        
    sprintf(bufferLinha,"%02d oC/s",volume);
    HD44780_posicionaTexto(6,1);
    HD44780_writeString(bufferLinha);              
    
    vTaskDelay(50);
  } 
}
/************************************************************************************
*       Descrição       :       Tela para cadastrar a velocidade da limpeza
*       Parametros      :       nenhun
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaCadastroVelocidadeLimpeza(void){
  eTECLA tecla;
  char bufferLinha[17];
  unsigned short int velPreparo = PARAMETROS_leVelocidadeLimpeza();

  HD44780_clearText();
  HD44780_writeString(" Veloc. Limpeza ");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ENTER:
          PARAMETROS_gravaVelocidadeLimpeza(velPreparo);
     case TECLA_ESC: 
          HD44780_clearText();
          return;
     case TECLA_DEC:
          if(velPreparo>5000)
            if(BOARD_tempoPiscadas>5000)
              velPreparo--;
            else
              if(BOARD_tempoPiscadas>5000)
                velPreparo-=10;
              else
                velPreparo-=100; 
          break;
     case TECLA_INC:
          if(velPreparo<18000)
            if(BOARD_tempoPiscadas>1000)
              velPreparo++;
            else
              if(BOARD_tempoPiscadas>500)
                velPreparo+=10;
              else
                velPreparo+=100;
          break;
     default:
          BOARD_tempoPiscadas = 3000;
          break;
    }
        
    sprintf(bufferLinha,"%05d RPM",velPreparo);
    HD44780_posicionaTexto(6,1);
    HD44780_writeString(bufferLinha);              
    
    vTaskDelay(50);
  } 
}
/************************************************************************************
*       Descrição       :       Tela para o usuário configurar a velocidade
*                               de aquecimento
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaConfiguraVelocidadePreAquecimento(void){
  eTECLA tecla;
  char bufferLinha[17];
  unsigned short int velPreparo = PARAMETROS_leVelocidadeAquecimento();

  HD44780_clearText();
  HD44780_writeString(" Veloc. resfria.");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ENTER:
          PARAMETROS_gravaVelocidadeAquecimento(velPreparo);
     case TECLA_ESC: 
          HD44780_clearText();
          return;
     case TECLA_DEC:
          if(velPreparo>500)
            if(BOARD_tempoPiscadas>500)
              velPreparo--;
            else
              if(BOARD_tempoPiscadas>500)
                velPreparo-=10;
              else
                velPreparo-=100; 
          break;
     case TECLA_INC:
          if(velPreparo<18000)
            if(BOARD_tempoPiscadas>1000)
              velPreparo++;
            else
              if(BOARD_tempoPiscadas>500)
                velPreparo+=10;
              else
                velPreparo+=100;
          break;
     default:
          BOARD_tempoPiscadas = 3000;
          break;
    }
        
    sprintf(bufferLinha,"%05d RPM",velPreparo);
    HD44780_posicionaTexto(6,1);
    HD44780_writeString(bufferLinha);              
    
    vTaskDelay(50);
  }   
}
/************************************************************************************
*       Descrição       :       Menu para teste do hardware
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_menuTesteHardware(void){
  eTECLA tecla;
  unsigned char indiceOpcoes=0;
  
  MCS_desenhaOpcoesHardware(indiceOpcoes);
  
  for(;TECLADO_getContadorInatividade();){
  
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:  
           if(MCS_funcHardware[indiceOpcoes]!=NULL){
             MCS_funcHardware[indiceOpcoes]();
             MCS_desenhaOpcoesHardware(indiceOpcoes);  
           }
           else
             return;        
           break;
      case TECLA_ESC:            
           return;
      case TECLA_DEC:
           indiceOpcoes?(indiceOpcoes--):(indiceOpcoes=TAM_LISTA_HARDWARE-1);
           MCS_desenhaOpcoesHardware(indiceOpcoes);           
           break;
      case TECLA_INC:
           indiceOpcoes = (indiceOpcoes+1) % TAM_LISTA_HARDWARE;
           MCS_desenhaOpcoesHardware(indiceOpcoes);           
           break;
    }
    
    vTaskDelay(50);
  }
}
/************************************************************************************
*       Descrição       :       Desenha o menu para o teste do hardware
*       Parametros      :       (unsigned char) indice
*       Retorno         :       nenhum
************************************************************************************/
void MCS_desenhaOpcoesHardware(unsigned char indice){
  
  HD44780_posicionaTexto(0,0);
  HD44780_writeString("Teste Hardware  ");
  HD44780_posicionaTexto(0,1);
  HD44780_writeString((char*)MCS_testeHardware[indice]);    
  HD44780_posicionaTexto(0,1);
  HD44780_writeChar(0x7e);
}
/************************************************************************************
*       Descrição       :       Tela para exibição da leitura do sensor
*                               de temperatura
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaTesteSensorTemperatura(void){
  eTECLA tecla;
  
  HD44780_clearText();
  HD44780_writeString("Sensor temp.");
  
  for(;;){
    
    tecla = TECLADO_getch();
    if(tecla==TECLA_ESC)
       return;
    
    float temperatura = AA_calculaTemperatura();
    char tmp[16];
    unsigned int tempInt = (unsigned int)temperatura;
    
    sprintf(tmp,"%3d oC",tempInt);
    HD44780_posicionaTexto(0,1);
    HD44780_writeString(tmp);          
    
    vTaskDelay(100);
  }    
}
/************************************************************************************
*       Descrição       :       Tela para exibição da leitura do sensor de papel
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaTesteSensorPapel(void){
  eTECLA tecla;
  unsigned int atrasoPacote;
  
  HD44780_clearText();
  HD44780_writeString("Sensor papel");
  atrasoPacote = PARAMETROS_leParametro(ATRASO_PACOTE);                                      

  for(;;){
    
    tecla = TECLADO_getch();
    if(tecla==TECLA_ESC)
      return;
    if(tecla==TECLA_ENTER){
      HD44780_posicionaTexto(0,1);
      HD44780_writeString("Lib. emb.:--");
      HD44780_posicionaTexto(0,1);           
      if(CE_empurraEmbalagem(atrasoPacote))
        HD44780_writeString("Lib. emb.:OK" );             
      else
       HD44780_writeString("Lib. emb.:NOK");             
      vTaskDelay(3000);      
    }
    
    unsigned short int leitura = AA_mediaMovelPapel();
    char buffer[17];
    sprintf(buffer,"[%05d]",leitura);
    HD44780_posicionaTexto(1,1);
    HD44780_writeString(buffer);
    
    HD44780_posicionaTexto(8,1);
    if(CE_sensorEmbalagem())
      HD44780_writeString("->OK ");
    else
      HD44780_writeString("->NOK");
      
    
    vTaskDelay(100);
  }    
}
/************************************************************************************
*       Descrição       :       Tela para o teste do sensor de dose
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_testeSensorDose(void){
  eTECLA tecla;

  HD44780_clearText();
  HD44780_writeString("Teste dose");
  
  
  for(;;){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:           
           HD44780_posicionaTexto(0,1);
           HD44780_writeString("dosando...");
           DM_aplicaDosagem();           
           HD44780_posicionaTexto(0,1);
           HD44780_writeString("          ");
           break;
      case TECLA_ESC:
           return;
    }
    
    unsigned char temp = DM_getSensor();
    
    HD44780_posicionaTexto(5,1);
    if(temp)
      HD44780_writeString("[1]");
    else
      HD44780_writeString("[0]");        
    
    vTaskDelay(100);
  }    
}
/************************************************************************************
*       Descrição       :       Tela para teste do sensor de rotação do motor
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_testeRotacaoMotor(void){
  eTECLA tecla;
  char bufferLinha[17];
  unsigned char flag=0;
  
  HD44780_clearText();
  HD44780_writeString("Sensor RPM");
  
  for(;;){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
                      flag=~flag;
                      if(flag)
                        MU_setRPM(2500);
                      else
                        MU_setRPM(0);
                      break;
      case TECLA_ESC:
                      return;
    }
    
    sprintf(bufferLinha,"%05d",MU_getRPMmedido());
    HD44780_posicionaTexto(0,1);
    HD44780_writeString(bufferLinha);        
    
    vTaskDelay(100);
  }    
}
/************************************************************************************
*       Descrição       :       Tela para verificar a frequência da rede elétrica
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_teste60Hz(void){
  eTECLA tecla;
  char bufferLinha[17];
  
  HD44780_clearText();
  HD44780_writeString(" Frequencia Rede");
  MU_setterGeralNeutro(255);
  
  for(;;){
    
    tecla = TECLADO_getch();
    if(tecla==TECLA_ESC){
      MU_setterGeralNeutro(0);
      return;
    }
    
    sprintf(bufferLinha,"%02d Hz",MU_getFrequenciaRede());
    HD44780_posicionaTexto(5,1);
    HD44780_writeString(bufferLinha);            
    
    vTaskDelay(100);
  }   
}
/************************************************************************************
*       Descrição       :       Teste do motor da embalagem
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_testeMotorEmbalagem(void){
  eTECLA tecla;
  unsigned char toggle=0;
  unsigned short int atrasoPacote;
  
  atrasoPacote = PARAMETROS_leParametro(ATRASO_PACOTE);                                    
  
  HD44780_clearText();
  HD44780_writeString(" Motor Embalagem");  
  
  for(;;){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           toggle=~toggle;
           if(toggle)
             CE_setterMotor(1);
           else
             CE_setterMotor(0);
           break;
      case TECLA_INC:
      case TECLA_DEC:
           HD44780_posicionaTexto(0,1);
           HD44780_writeString("Lib. emb.:--");
           HD44780_posicionaTexto(0,1);           
           if(CE_empurraEmbalagem(atrasoPacote))
             HD44780_writeString("Lib. emb.:OK" );             
           else
             HD44780_writeString("Lib. emb.:NOK");             
           vTaskDelay(3000);
           break;
      case TECLA_ESC: 
           return;
    }
    
    HD44780_posicionaTexto(0,1);
    if(toggle)
      HD44780_writeString("Ligado     ");
    else
      HD44780_writeString("Desligado  ");
    
    vTaskDelay(100);
  }       
}
/************************************************************************************
*       Descrição       :       Tela para teste do moedeiro
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_testeMoedeiro(void){
  eTECLA tecla;
  unsigned char flag=0;
  float totalMoedas;
  
  MP_enviaSinal(BV20_INICIALIZACAO);  
  
  HD44780_clearText();
  HD44780_writeString("Teste moedeiro");
  
  for(;;){
   
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_INC:
      case TECLA_DEC:
           MP_zeraTotalizadoresMoeda();        
           break;
      case TECLA_ENTER:
           flag =~flag;
           MP_setterInhibitMoedeiro(flag);
           AL_setterLeds(0,0,0);
           break;
      case TECLA_ESC:      
           MP_zeraTotalizadoresMoeda();
           return;
    }
    
    totalMoedas = MP_getTotalDinheiro();
    char bufferLinha[17];
    
    sprintf(bufferLinha,"R$ %02.2f",totalMoedas);
    HD44780_posicionaTexto(0,1);
    HD44780_writeString(bufferLinha);
    
    HD44780_posicionaTexto(8,1);
    if(flag)
      HD44780_writeString("[BLOQU.]");
    else
      HD44780_writeString("[NORMAL]");
    
    vTaskDelay(100);
  }  
}
/************************************************************************************
*       Descrição       :       Tela para teste da saída de audio
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaTesteAudio(void){
  eTECLA tecla;

  HD44780_clearText();
  HD44780_writeString("   Teste Som");
  
  for(;;){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           PLAYERWAVE_iniciaMusica(1,0);
           break;
      case TECLA_ESC:        
            PLAYER_interrompeMusica();
           return;
    }
    
    HD44780_posicionaTexto(5,1);
    if(PLAYERWAVE_verificaToque())
      HD44780_writeString("Executando");
    else
      HD44780_writeString("  Parado  ");
    
    
    vTaskDelay(100);
  }      
}
/************************************************************************************
*       Descrição       :       Tela de teste para as saídas de led
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaTesteFitaLed(void){
  eTECLA tecla;
  unsigned char ciclos=5;
  unsigned char led=1;
  
  HD44780_clearText();
  HD44780_writeString("   Teste leds");
  
  for(;;){
    
    tecla = TECLADO_getch();
    if(tecla==TECLA_ESC)
      return;
    
    if(!--ciclos){
      ciclos = 5;
      AL_setterLeds(1,0,0);
      AL_setterLeds(2,0,0);
      AL_setterLeds(3,0,0);
      AL_setterLeds(4,0,0);
      AL_setterLeds(5,0,0);
      AL_setterLeds(6,0,0);
      AL_setterLeds(7,0,1);
      
      AL_setterLeds(led,1,1);
      led++;
      if(led>7)
        led = 1;
    }
    
    vTaskDelay(100);
  }    
}
/************************************************************************************
*       Descrição       :       Menu para configuração do parâmetro
*                               volume da voz
*       Parametros      :       nenhmn
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaVolumeVoz(void){
  eTECLA tecla;
  char bufferLinha[17];
  unsigned short int volume = PARAMETROS_leVolumeVoz();

  HD44780_clearText();
  HD44780_writeString(" Volume voz     ");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ENTER:
          PARAMETROS_gravaVolumeVoz(volume);
     case TECLA_ESC: 
          HD44780_clearText();
          return;
     case TECLA_DEC:
          if(volume>10)
            volume--;
          break;
     case TECLA_INC:
          if(volume<100)
            volume++;
          break;
    }
        
    sprintf(bufferLinha,"%03d ",volume);
    HD44780_posicionaTexto(6,1);
    HD44780_writeString(bufferLinha);              
    
    vTaskDelay(50);
  }  
}
/************************************************************************************
*       Descrição       :       Menu para configuração da fita de led
*                               durante o preparo da pipoca
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaFitaPreparo(void){
  eTECLA tecla;  
  unsigned short int flag = PARAMETROS_leFitaPreparo();
  
  if(flag)
    flag = 0xFFFF;
  
  HD44780_clearText();
  HD44780_writeString("Fita preparacao ");
    
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           PARAMETROS_gravaFitaPreparo(flag);
      case TECLA_ESC: 
           HD44780_clearText();
           return;
      case TECLA_INC:            
      case TECLA_DEC:
           flag = ~flag;
           break;
    }
    
    HD44780_posicionaTexto(6,1);
    if(flag)
      HD44780_writeString("SIM");
    else
      HD44780_writeString("NAO");
    
    vTaskDelay(100);
  }    
}
/************************************************************************************
*       Descrição       :       Tela para configuração da comissão do ponto
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaConfiguracaoComissaoPonto(void){
  eTECLA tecla;
  unsigned short int comissao=0;
  char digitosComissao[8];
  
  HD44780_clearText();
  HD44780_writeString("Comissao ponto");
  
  comissao = PARAMETROS_leComissaoPonto();
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           PARAMETROS_gravaComissaoPonto(comissao);
      case TECLA_ESC:
           return;
      case TECLA_INC:
           comissao++;
           break;
      case TECLA_DEC:
           if(comissao>1)
             comissao--;
           break;
    }
  
    digitosComissao[0] = ((comissao/1000)%10) + '0';
    digitosComissao[1] = ((comissao/100)%10) + '0';
    digitosComissao[2] = ((comissao/10)%10) + '0';
    digitosComissao[3] = '.';
    digitosComissao[4] = ( comissao % 10) + '0';
    digitosComissao[5] = ' ';
    digitosComissao[6] = '%';
    digitosComissao[7] = '\0';
    
    HD44780_posicionaTexto(6,1);
    HD44780_writeString(digitosComissao);    

    vTaskDelay(100);    
  }    
}
/************************************************************************************
*       Descrição       :       Tela para teste do sinal da chave de fim do
*                               papel
*       Parametros      :       (nenhum)
*       Retorno         :       (nenhum)
************************************************************************************/
void MCS_telaTesteChaveFimPapel(void){
  eTECLA tecla;
  
  HD44780_clearText();   
  HD44780_writeString("Chave Fim Papel");
  
  for(;;){
    
    tecla = TECLADO_getch();
    if(tecla==TECLA_ESC)
      return;
    
    HD44780_posicionaTexto(3,1);
    if(TECLADO_getChaveReservaPapel())
      HD44780_writeString("[Ativado]");
    else
      HD44780_writeString("[-------]");
    
    vTaskDelay(100);
  }    
}
/************************************************************************************
*       Descrição       :       Tela para o teste do sinal de porta aberta
*       Parametros      :       (nenhum)
*       Retorno         :       (nenhum)
************************************************************************************/
void MCS_telaTesteChavePorta(void){
  eTECLA tecla;
  
  HD44780_clearText();   
  HD44780_writeString("  Chave Porta");
  
  for(;;){
    
    tecla = TECLADO_getch();
    if(tecla==TECLA_ESC)
      return;
    
    HD44780_posicionaTexto(3,1);
    if(TECLADO_getChavePorta())
      HD44780_writeString("[Ativado]");
    else
      HD44780_writeString("[-------]");
    
    vTaskDelay(100);
  }   
}
/************************************************************************************
*       Descrição       :       Tela para teste da comunicação com o módulo
*                               de telemetria
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaTesteTelemetria(void){
  eTECLA tecla;

  HD44780_clearText();
  HD44780_writeString(" Comm mod telet");
  
  for(;;){
    
    tecla = TECLADO_getch();
    if(tecla==TECLA_ESC)
      return;
    
    HD44780_posicionaTexto(3,1);
    if(TELET_escreveBlocoOperacao(1,0,0,0,0,0))
      HD44780_writeString("[ONLINE ]");
    else
      HD44780_writeString("[OFFLINE]");
        
    vTaskDelay(100);
  }    
}
/************************************************************************************
*       Descrição       :      Tela para teste do led de grátis
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaTesteSaidaLedGratis(void){
  eTECLA tecla;
  unsigned char toggle=0;
  
  HD44780_clearText();
  HD44780_writeString("Teste saida aux.");      
  
  for(;;){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           toggle^=0xFF;
           break;
      case TECLA_ESC:
           CA_setterLedGratis(0x00);
           return;
    }
    
    CA_setterLedGratis(toggle);
    
    HD44780_posicionaTexto(3,1);
    if(toggle)
      HD44780_writeString("[ATIVO]");
    else
      HD44780_writeString("[-----]");
    
    vTaskDelay(100);
  }    
}
/************************************************************************************
*       Descrição       :       Tela para teste do controle de angulo de fase
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_testaTesteAnguloFase(void){
  eTECLA tecla; 
  unsigned short int ciclo=119;
  char bufferLinha[17];
  
  HD44780_clearText();  
  HD44780_writeString(" Teste CTRL RES ");
  MU_setRPM(PARAMETROS_leParametro(VELOCIDADE_PREPARO));
    
  MU_setTemperatura(115,100);
  
  for(;;){
   
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           ciclo = 0;
           break;
      case TECLA_ESC:
           MU_setRPM(0);
           MU_setTemperatura(0,0);
           return;
      case TECLA_INC:
           ciclo++;
           MU_setTemperatura(ciclo,100);
           break;
      case TECLA_DEC:
           if(ciclo)
             ciclo--;           
           MU_setTemperatura(ciclo,100);
           break;
    }
    
    HD44780_posicionaTexto(10,1);
    bufferLinha[0] = '[';
    bufferLinha[1] = ((ciclo/1000)%10) + '0';
    bufferLinha[2] = ((ciclo/100)%10) + '0';
    bufferLinha[3] = ((ciclo/10)%10) + '0';
    bufferLinha[4] = (ciclo%10) + '0';
    bufferLinha[5] = ']';
    bufferLinha[6] = 0;
    HD44780_writeString(bufferLinha);
    
    IU_escreveTemperaturaResistenciaFree(0);
                   
    vTaskDelay(100); 
  }  
}
/************************************************************************************
*       Descrição       :       Tela para o usuário configurar a máquina com
*                               o padrão de fábrica
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_configuracaoFabrica(void){ 
  
   if(!MCS_telaSenha(3214)){
    HD44780_clearText();
    return;
  }
  
  PARAMETROS_escreveParametro(TEMPO_PREPARO,80);
  PARAMETROS_escreveParametro(TEMPERATURA_PREPARO,100);
  PARAMETROS_escreveParametro(VELOCIDADE_PREPARO,5900);
  PARAMETROS_salvaTemperaturaPreProcesso(65);
  PARAMETROS_escreveParametro(ATRASO_PACOTE,20);
  PARAMETROS_salvaRotacaoInicialExpulsao (14000);
  PARAMETROS_salvaVelocidadeFinalExpulsao(16000);
  PARAMETROS_gravaFatorTrabalho(2);
  PARAMETROS_gravaVelocidadeLimpeza(18000);
  PARAMETROS_gravaVelocidadeAquecimento(5450);
  PARAMETROS_gravaVolumeSom(5);
  PARAMETROS_gravaVolumeVoz(20);
  HD44780_clearText();
  PARAMETROS_gravaTemperaturaInicio(65);
  PARAMETROS_gravaTipoCedulas(1,1,0,0,0,0);
}
/************************************************************************************
*       Descrição       :       Tela para teste da reprodução das vozes
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaTesteVoz(void){
  eTECLA tecla;
  unsigned char indice=0;
  
  HD44780_clearText();
  HD44780_writeString("   Teste Voz");
  
  for(;;){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           PLAYERWAVE_iniciaMusica(indice,0);
           indice = (indice+1) % 4;
           break;
      case TECLA_ESC:        
            PLAYER_interrompeMusica();            
           return;
    }
    
    HD44780_posicionaTexto(5,1);
    if(PLAYERWAVE_verificaToque())
      HD44780_writeString("Executando");
    else
      HD44780_writeString("  Parado  ");
        
    vTaskDelay(100);
  }          
}
/************************************************************************************
*       Descrição       :       Tela para capturar as configurações de fábrica
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaCapturaConfigFabrica(void){

  if(!MCS_telaSenha(3215)){
    HD44780_clearText();
    return;
  }    
         
  PARAMETROS_gravaConfigFabrica(PARAMETROS_leParametro(TEMPO_PREPARO),
                                PARAMETROS_leParametro(TEMPERATURA_PREPARO),
                                PARAMETROS_leParametro(VELOCIDADE_PREPARO),
                                PARAMETROS_leTemperaturaPreProcesso(),
                                PARAMETROS_leParametro(ATRASO_PACOTE),
                                PARAMETROS_leRotacaoInicialExpulsao(),
                                PARAMETROS_leVelocidadeFinalExpulsao(),
                                PARAMETROS_leFatorTrabalho(),
                                PARAMETROS_leVelocidadeLimpeza(),
                                PARAMETROS_leVelocidadeAquecimento(),
                                PARAMETROS_leVolumeSom(),                                              
                                PARAMETROS_leVolumeVoz()
                                );  
  HD44780_clearText();
  HD44780_writeString("Parametros");
  HD44780_posicionaTexto(0,1);
  HD44780_writeString("copiados");
  vTaskDelay(3000);    
  HD44780_clearText();
}
/************************************************************************************
*       Descrição       :       Interface para o usuário cadastrar a opção
*                               de locação da máquina
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaOpcaoLocacao(void){
  eTECLA tecla;
  unsigned char flag;
  
  if(!MCS_telaSenha(4991)){
    HD44780_clearText();
    return;
  }    

  HD44780_clearText();
  HD44780_writeString("  Modo locacao");
    
  flag = PARAMETROS_leFlagLocacao();
 
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:   
           PARAMETROS_gravaFlagLocacao(flag);
      case TECLA_ESC:
           return;
      case TECLA_INC:
      case TECLA_DEC:
           flag ^= 255;   
           break;
    }    
    
    HD44780_posicionaTexto(5,1);
    if(flag)
      HD44780_writeString("[SIM]");
    else
      HD44780_writeString("[NAO]");    
    
    vTaskDelay(50);            
  }      
}
/************************************************************************************
*       Descrição       :       Interface para fazer o zeramento dos
*                               totalizadores
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_zeraContadores(void){
  eTECLA tecla;
  char bufferLinha[17];
  char bufferTmp[17];
  unsigned char indice=0;
  unsigned char toggle=0;
  unsigned char ciclos=1;
  unsigned char horas=0;
  unsigned short int senha;
  
  srand(MCS_contadorSemente);
  unsigned short int contraSenha = rand()%10000;
    
  HD44780_clearText();
  sprintf(bufferLinha,"Gerada:%04d",contraSenha);
  HD44780_writeString(bufferLinha);
  
  HD44780_posicionaTexto(0,1);
  HD44780_writeString("Contra:");
  
  sprintf(bufferLinha,"000000");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           if(indice>4){
             
             unsigned int recebido = ((bufferLinha[0]-'0')*100000) + 
                                     ((bufferLinha[1]-'0')*10000) + 
                                     ((bufferLinha[2]-'0')*1000) + 
                                     ((bufferLinha[3]-'0')*100) + 
                                     ((bufferLinha[4]-'0')*10) + 
                                     ((bufferLinha[5]-'0'));
             
             recebido &= 0x3FFFF;             
             horas = recebido>>14;
             
             senha = ((recebido&0x3FFF)^3004);
               HD44780_clearText();             
             if(senha==contraSenha){               
               HD44780_writeString("Realizando");
               HD44780_posicionaTexto(0,1);
               HD44780_writeString("Reset");
               
               // Totalizador de unidades de pipoca vendidas
               PARAMETROS_salvaContadorVendas(0);
               PARAMETROS_salvaContadorArrecadacao(0);
               PARAMETROS_gravaTotalizadorPermanente(0);      
               PARAMETROS_gravaContadorVendasParcial(0);    
               PARAMETROS_salvaContadorOperacoesCartao(0);
               PARAMETROS_salvaFaturamentoParcialCartao(0);
               PARAMETROS_salvaFaturamentoTotalCartao(0);
               
               vTaskDelay(5000);                                             
             }
             else{
               HD44780_writeString("Senha");
               HD44780_posicionaTexto(0,1);
               HD44780_writeString("Invalida");               
             }
             vTaskDelay(3000);
             return;
           }
           else{
             indice++;
             break;
           }
      case TECLA_ESC:
           return;
      case TECLA_INC:
           bufferLinha[indice]++;
           if(bufferLinha[indice]>'9')
             bufferLinha[indice] = '0';
           break;
      case TECLA_DEC:    
           bufferLinha[indice]--;
           if(bufferLinha[indice]<'0')
             bufferLinha[indice] = '9';
           break;
    }
    
    if(!--ciclos){
      ciclos = 4;
      toggle = ~toggle;
      memcpy(bufferTmp,bufferLinha,17);
      bufferTmp[7] = 0x00;
      if(toggle)
        bufferTmp[indice] = '_';
      
      HD44780_posicionaTexto(7,1);
      HD44780_writeString(bufferTmp);
    }       
    
    vTaskDelay(50);
  }      
}
/************************************************************************************
*       Descrição       :       Tela para configuração do parâmetro de temperatura
*                               inicial de preparo
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_configuraTemperaturaInicial(void){
  eTECLA tecla;  
  char bufferLinha[17];
  unsigned short int temperatura = PARAMETROS_leTemperaturaInicio();
  
  HD44780_clearText();
  HD44780_writeString(" Temp. inicio   ");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ENTER:
          PARAMETROS_gravaTemperaturaInicio(temperatura);
     case TECLA_ESC:
          HD44780_clearText();
          return;
     case TECLA_DEC:
          if(temperatura>35)
            temperatura--;       
          break;
     case TECLA_INC:
          if(temperatura<200)
            temperatura++;
          break;
    }
           
    sprintf(bufferLinha,"%03d oC",temperatura);
    HD44780_posicionaTexto(6,1);
    HD44780_writeString(bufferLinha);
    
    vTaskDelay(50);
  }    
}
/************************************************************************************
*       Descrição       :       Interface para o usuário cadastrar
*                               o tempo de pré-aquecimento
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_configuraTempoPreAquecimento(void){
  eTECLA tecla;  
  char bufferLinha[17];
  unsigned short int tempo = PARAMETROS_leTempoPreaquecimento();
  
  HD44780_clearText();
  HD44780_writeString("Duracao preaquec");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ENTER:
          PARAMETROS_gravaTempoPreAquecimento(tempo);
     case TECLA_ESC:
          HD44780_clearText();
          return;
     case TECLA_DEC:
          if(tempo>0)
            tempo--;       
          break;
     case TECLA_INC:
          if(tempo<100)
            tempo++;
          break;
    }
           
    sprintf(bufferLinha,"%03d (s)",tempo);
    HD44780_posicionaTexto(6,1);
    HD44780_writeString(bufferLinha);
    
    vTaskDelay(50);
  }    
}
/************************************************************************************
*       Descrição       :       Menu para cadastrar a mensagem de cliente
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_configMensagemCliente(unsigned char index){
  const char msg[4][17]={"   Mensagem 01  ",
                         "   Mensagem 02  ",
                         "   Mensagem 03  ",
                         "   Mensagem 04  "};  
  eTECLA tecla;
  char bufferMsg[17];
  char bufferCopia[17];
  unsigned char indice=0;
  unsigned char toggle=0;
  unsigned char ciclos=1;
         
  HD44780_clearText();
  HD44780_posicionaTexto(0,0);
  HD44780_writeString((char*)msg[index]);
  HD44780_ligaCursor();
  
  PARAMETROS_leMensagemCliente(index,bufferMsg);
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           indice++;
           if(indice>15){
             PARAMETROS_gravaMensagemCliente(index,bufferMsg);
             return;
           }
           break;
      case TECLA_ESC:
           HD44780_desligaCursor();
           return;
      case TECLA_DEC:
           // Decrementa o caracter atual
           if(bufferMsg[indice]==' ')
             bufferMsg[indice] = 'z';
           else
             bufferMsg[indice]--;
           break;
      case TECLA_INC:  
           // Incrementa o caracter atual
           if(bufferMsg[indice]++)
             if(bufferMsg[indice]>'z')
               bufferMsg[indice] = ' ';
           break;           
    }            
    
    // Ciclos 
    // refresh da mensagem no display
    if(!--ciclos){
      ciclos = 10;
      toggle =~toggle;
      
      memcpy(bufferCopia,bufferMsg,17);
      
      HD44780_posicionaTexto(0,1);
      if(toggle)
        bufferCopia[indice] = '_';        
         
      HD44780_writeString(bufferCopia);      
    }
    
    vTaskDelay(50);
  }  
}
/************************************************************************************
*       Descrição       :       Cadastra a primeira mensagem de cliente
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_cadastraMensagem_01(void){
  
  MCS_configMensagemCliente(0);  
}
/************************************************************************************
*       Descrição       :       Cadastra a segunda mensagem de cliente
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_cadastraMensagem_02(void){
  
  MCS_configMensagemCliente(1);    
}
/************************************************************************************
*       Descrição       :       Cadastra a terceira mensagem de cliente
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_cadastraMensagem_03(void){
  
  MCS_configMensagemCliente(2);      
}
/************************************************************************************
*       Descrição       :       Cadastra a quarta mensagem de cliente
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_cadastraMensagem_04(void){
  
  MCS_configMensagemCliente(3);          
}
/************************************************************************************
*       Descrição       :       Interface para o usuário cadastrar
*                               o parametro desumificador
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_cadastraDesumidificador(void){
  eTECLA tecla;
  unsigned short int flag = PARAMETROS_leCicloDesumidificacao();
  
  if(flag)
    flag=0xFFFF;
  
  HD44780_clearText();
  HD44780_writeString("Ciclo desumidif.");
    
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           PARAMETROS_gravaCicloDesumificacao(flag);
      case TECLA_ESC: 
           HD44780_clearText();
           return;
      case TECLA_INC:            
      case TECLA_DEC:
           flag ^= 0xFFFF;
           break;
    }
    
    HD44780_posicionaTexto(6,1);
    if(flag)
      HD44780_writeString("SIM");
    else
      HD44780_writeString("NAO");
    
    vTaskDelay(10);
  }    
}
/************************************************************************************
*       Descrição       :       Tela para configurar a velocidade que será
*                               utilizada na autodesumidificação
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_configuraVelocidadeDesumificador(void){
  eTECLA tecla;
  char bufferLinha[17];
  unsigned short int velPreparo = PARAMETROS_leVelocidadeDesumidificacao();

  HD44780_clearText();
  HD44780_writeString(" Veloc. preparo ");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ENTER:
          PARAMETROS_salvaVelocidadeDesumidificacao(velPreparo);
     case TECLA_ESC: 
          HD44780_clearText();
          return;
     case TECLA_DEC:
          if(velPreparo>2000)
            if(BOARD_tempoPiscadas>8000)
              velPreparo--;
            else
              if(BOARD_tempoPiscadas>5000)
                velPreparo-=10;
              else
                velPreparo-=100; 
          break;
     case TECLA_INC:
          if(velPreparo<18000)
            if(BOARD_tempoPiscadas>8000)
              velPreparo++;
            else
              if(BOARD_tempoPiscadas>5000)
                velPreparo+=10;
              else
                velPreparo+=100;
          break;
     default:
          BOARD_tempoPiscadas = 15000;
          break;
    }
        
    sprintf(bufferLinha,"%05d RPM",velPreparo);
    HD44780_posicionaTexto(3,1);
    HD44780_writeString(bufferLinha);              
    
    vTaskDelay(50);
  }   
}
/************************************************************************************
*       Descrição       :       Tela para configurar a temperatura de desumificador
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_cadastraTemperaturaDesumificacao(void){
  eTECLA tecla;  
  char bufferLinha[17];
  unsigned short int temperatura = PARAMETROS_leTemperaturaDesumidificacao();
  
  HD44780_clearText();
  HD44780_writeString(" Temp. desumid. ");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
     case TECLA_ENTER:
          PARAMETROS_salvaTemperaturaDesumidificacao(temperatura);
     case TECLA_ESC:
          HD44780_clearText();
          return;
     case TECLA_DEC:
          if(temperatura>60)
            temperatura--;       
          break;
     case TECLA_INC:
          if(temperatura<200)
            temperatura++;
          break;
    }
           
    sprintf(bufferLinha,"%03d oC",temperatura);
    HD44780_posicionaTexto(6,1);
    HD44780_writeString(bufferLinha);
    
    vTaskDelay(50);
  }    
}
/************************************************************************************
*       Descrição       :       Faz a assinatura do sistema
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
extern unsigned short int ASSINATURA_contraSenhaLocal;
void MCS_telaAssinatura(void){
  eTECLA tecla;
  char bufferLinha[65]={""};
  char bufferTmp[65];
  const char digitos[17]={"0123456789ABCDEF"};
  unsigned char indice=0;
  unsigned char letra=0;
  unsigned char inverter=1;
  unsigned char toggle=0;
    
  srand(MCS_contadorSemente);
  unsigned short int contraSenha = rand()%10000;
  ASSINATURA_contraSenhaLocal = contraSenha;
    
  HD44780_clearText();
  sprintf(bufferLinha,"Gerada:%04d [1/4]",contraSenha);
  HD44780_writeString(bufferLinha);   
  
  for(unsigned char i=0;i<64;i++)
    bufferLinha[i] = ' ';
  bufferLinha[64] = 0;  
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           letra = 0;
           indice++;
           
           if(indice>63){
             unsigned short int senha;
             unsigned  int endereco;
             unsigned char presetValores;
             unsigned int contadorParcial;
             unsigned int contadorTotal;
             unsigned int arrecadacaoParcial;
             unsigned int arrecadacaoTotal;
                                       
             //sprintf(bufferLinha,"731646737316439270EA425C2B93425BFD93425EEA15425D85C29E07F55E4B0F");
             ASSINATURA_decodifica(bufferLinha,&senha,&endereco,&presetValores,&contadorParcial,&contadorTotal,&arrecadacaoParcial,&arrecadacaoTotal);                                                                   
             
             if(senha==contraSenha){                              
               ASSINATURA_gravaAssinatura(endereco,presetValores,contadorParcial,contadorTotal,arrecadacaoParcial,arrecadacaoTotal);
               HD44780_clearText();
               HD44780_writeString("assinado");
               vTaskDelay(3000);
               return;
             }
             else{
               HD44780_clearText();
               HD44780_writeString("assinatura");
               HD44780_posicionaTexto(0,1);
               HD44780_writeString("incorreta");
               vTaskDelay(3000);               
             }
           }
           break;
      case TECLA_ESC:
           return;
      case TECLA_INC:
           letra = (letra+1) % 16;
           bufferLinha[indice] = digitos[letra];
           break;
      case TECLA_DEC:      
           letra?(letra--):(letra=15);
           bufferLinha[indice] = digitos[letra];           
           break;
    }
        
    if(!--inverter){      
      
      sprintf(bufferTmp,"[1/%d",(indice/16)+1);
      HD44780_posicionaTexto(12,0);
      HD44780_writeString(bufferTmp);
      
      unsigned char pagina;
      
      if(indice<16)
        pagina = 0;
      else
        if(indice<32)
          pagina = 16;
        else
          if(indice<48)
            pagina = 32;
          else
           pagina = 48;
      
      memcpy(bufferTmp,&bufferLinha[pagina],64);
      bufferTmp[16] = 0;
      toggle = ~toggle;
      if(toggle)
        bufferTmp[indice%16] = '_';
      
      HD44780_posicionaTexto(0,1);
      HD44780_writeString(bufferTmp);      
      
      inverter = 4;
    }
    
    vTaskDelay(50);
  }  
}
/************************************************************************************
*       Descrição       :       Interface para visualização da contagem
*                               do horímetro
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_visualizacaoHorimetro(void){
  eTECLA tecla;
  unsigned char ticks=20;
  unsigned int contador;
  char bufferLinha[17];
  
  HD44780_clearText();
  HD44780_posicionaTexto(0,0);
  HD44780_writeString("    Horimetro   ");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
      case TECLA_ESC:
           return;
    }
    
    // Ticks para refresh do contador
    if(!--ticks){
      ticks = 20;
      contador = HORIMETRO_carrega();
      sprintf(bufferLinha,"%05d",contador);
      HD44780_posicionaTexto(6,1);
      HD44780_writeString(bufferLinha);
    }
    
    vTaskDelay(50);
  }  
}
/************************************************************************************
*       Descrição       :       Interface para o usuário inserir a senha
*                               de liberação do horímetro
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_liberacaoHorimetro(void){
  eTECLA tecla;
  char bufferLinha[17];  
  char bufferSenha[8]={"    "};
  char espelhoSenha[9];
  const char digitosS[17]={"0123456789ABCDEF"};
  unsigned char letra=0;
  unsigned char indice=0;
  unsigned char refresh=1;
  unsigned char toggle=0;
  unsigned char senha[14];
  unsigned short int resSenha,resHoras;
  
  srand(MCS_contadorSemente);
  unsigned short int contraSenha = rand()%4096;  
  
  HORIMETRO_geraSenha(contraSenha,PARAMETROS_leTotalizadorPermanente(),senha);
  for(unsigned char i=0;i<13;i++)
    senha[i]=digitosS[senha[i]];//'0';
  senha[13] = 0;
  
  HD44780_clearText();
  sprintf(bufferLinha,"CD:%s",senha);
  HD44780_writeString(bufferLinha);  
  bufferSenha[indice] = digitosS[letra];
  
  for(;;){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           indice++;
           if(indice>6){
             
             if(HORIMETRO_decodificaSenha(bufferSenha,&resSenha,&resHoras)){
                                             
               if(resSenha == contraSenha){
                if(resHoras>2000)
                  resHoras = 2000;
                                                
                HORIMETRO_grava(resHoras*60); 
                
                HD44780_clearText();
                HD44780_writeString("Renovado com");
                HD44780_posicionaTexto(0,1);
                HD44780_writeString("sucesso!");
                vTaskDelay(3000);
                
                return;
               }                 
               else{
                 HD44780_clearText();
                 HD44780_writeString("Senha");
                 HD44780_posicionaTexto(0,1);
                 HD44780_writeString("Incorreta");
                 vTaskDelay(3000);
                 return;                 
               }
             }
                                     
             else{
               HD44780_clearText();
               HD44780_writeString("Senha");
               HD44780_posicionaTexto(0,1);
               HD44780_writeString("Incorreta");
               vTaskDelay(3000);
               return;
             }
           }
           letra=0;
           bufferSenha[indice] = digitosS[letra];
           break;
      case TECLA_ESC:
           return;
      case TECLA_INC:
           letra = (letra+1)%16;
           bufferSenha[indice] = digitosS[letra];           
           break;
      case TECLA_DEC:
           letra?(letra--):(letra=15);
           bufferSenha[indice] = digitosS[letra];
           break;
    }    
    
    if(!--refresh){
      refresh = 5;
      toggle = ~toggle;
      memcpy(espelhoSenha,bufferSenha,8);
      espelhoSenha[8] = 0;
      HD44780_posicionaTexto(4,1);
      if(toggle)
        espelhoSenha[indice] = '_';
      HD44780_writeString(espelhoSenha);      
    }
    
    vTaskDelay(50); 
  }  
}
/************************************************************************************
*       Descrição       :       Menu para realizar o acerto nos totalizadores
*                               do sistema
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_menuAjusteContadores(void){
  eTECLA tecla;
  unsigned char indiceOpcoes=0;
  
  if(!MCS_checaContraSenha())
    return;
  
  MCS_desenhaOpcoesContadores(indiceOpcoes);
  
  for(;;){
  
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:  
           if(MCS_funcAjusteContadores[indiceOpcoes]!=NULL){
             MCS_funcAjusteContadores[indiceOpcoes]();
             MCS_desenhaOpcoesContadores(indiceOpcoes);  
           }
           else
             return;        
           break;
      case TECLA_ESC:            
           return;
      case TECLA_DEC:
           indiceOpcoes?(indiceOpcoes--):(indiceOpcoes=TAM_LISTA_CONTADORES-1);
           MCS_desenhaOpcoesContadores(indiceOpcoes);           
           break;
      case TECLA_INC:
           indiceOpcoes = (indiceOpcoes+1) % TAM_LISTA_CONTADORES;
           MCS_desenhaOpcoesContadores(indiceOpcoes);           
           break;
    }
    
    vTaskDelay(50);
  }
}
/************************************************************************************
*       Descrição       :       Desenha as opções do menu de ajuste
*                               dos contadores
*       Parametros      :       (unsigned char) indice
*       Retorno         :       nenhum
************************************************************************************/
void MCS_desenhaOpcoesContadores(unsigned char indice){
  
  HD44780_posicionaTexto(0,0);
  HD44780_writeString(" Ajuste contador");
  HD44780_posicionaTexto(0,1);
  HD44780_writeString((char*)MCS_mensagemMenuAjusteContadores[indice]);        
}
/************************************************************************************
*       Descrição       :       Tela para fazer o ajuste do contador de unidades
*                               vendidas parcial
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_ajustaContadorUnidadesParcial(void){
  unsigned int parcial;
  
  parcial = MCS_telaInformaValorInt(" Unid. Parcial  ");
  PARAMETROS_gravaContadorVendasParcial(parcial);                      
}
/************************************************************************************
*       Descrição       :       Tela para fazer o ajuste do contador de 
*                               unidades vendidas total
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_ajustaContadorUnidadasTotal(void){
  unsigned int total;
  
  total = MCS_telaInformaValorInt(" Unid. Total    ");
  PARAMETROS_salvaContadorVendas(total);  
}
/************************************************************************************
*       Descrição       :       Tela para fazer o ajuste do contador
*                               de arrecadação parcial
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_ajustaArrecadacaoParcial(void){
  unsigned int arrecadacaoParcial;

  arrecadacaoParcial =  MCS_telaInformaValorInt(" Moeda Parcial  "); 
  PARAMETROS_salvaContadorArrecadacao(arrecadacaoParcial);  
}
/************************************************************************************
*       Descrição       :       Tela para fazer o ajuste do contador 
*                               de arrecadacao total 
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_ajustaArrecadacaoTotal(void){
  unsigned int arrecadacaoTotal;
  
  arrecadacaoTotal = MCS_telaInformaValorInt(" Moeda Total    "); 
  PARAMETROS_gravaTotalizadorPermanente(arrecadacaoTotal);  
}
/************************************************************************************
*       Descrição       :       Tela genérica para o usuário digitar
*                               um valor de 8 digitos
*       Parametros      :       (char*) titulo
*       Retorno         :       (unsigned int) valor digitado
************************************************************************************/
unsigned int MCS_telaInformaValorInt(char*titulo){
  char bufDigitos[9]={"00000000"};
  char bufTemp[9];
  eTECLA tecla;
  unsigned char ponteiroDigito=0;
  unsigned char tempoAtualizacao=1;
  unsigned char inverte=0;
  
  HD44780_clearText();
  HD44780_posicionaTexto(0,0);
  HD44780_writeString(titulo);
  
  for(;;){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_INC:
           (bufDigitos[ponteiroDigito]>='9')?(bufDigitos[ponteiroDigito]='0'):(bufDigitos[ponteiroDigito]++);
           break;
      case TECLA_DEC:
           (bufDigitos[ponteiroDigito]<='0')?(bufDigitos[ponteiroDigito]='9'):(bufDigitos[ponteiroDigito]--);
           break;
      case TECLA_ENTER:
           if(ponteiroDigito>6){
             unsigned int valor;
             
             valor = ((bufDigitos[0]-'0')*10000000) +
                     ((bufDigitos[1]-'0')*1000000) +
                     ((bufDigitos[2]-'0')*100000) +
                     ((bufDigitos[3]-'0')*10000) +
                     ((bufDigitos[4]-'0')*1000) + 
                     ((bufDigitos[5]-'0')*100) + 
                     ((bufDigitos[6]-'0')*10) +
                     (bufDigitos[7]-'0');
             
             return valor;
           }
           else
             ponteiroDigito++;
           break;
      case TECLA_ESC:
           return 0;
    }
    
    //  Atualiza os dados no LCD
    if(!--tempoAtualizacao){
      tempoAtualizacao = 10;
      memcpy(bufTemp,bufDigitos,9);
      inverte=~inverte;
      if(inverte)
        bufTemp[ponteiroDigito]='_';
      HD44780_posicionaTexto(4,1);
      HD44780_writeString(bufTemp);
    }        
        
    vTaskDelay(50);    
  }    
}
/************************************************************************************
*       Descrição       :       Faz a verificação da contrasenha
*                               de segurança
*       Parametros      :       nenhum
*       Retorno         :       (unsigned char) maior do que zero
*                               se estiver correto
************************************************************************************/
unsigned char MCS_checaContraSenha(void){
 eTECLA tecla;
  char bufferLinha[17];
  char bufferTmp[17];
  unsigned char indice=0;
  unsigned char toggle=0;
  unsigned char ciclos=1;
  unsigned char horas=0;
  unsigned short int senha;
  
  srand(MCS_contadorSemente);
  unsigned short int contraSenha = rand()%10000;
    
  HD44780_clearText();
  sprintf(bufferLinha,"Gerada:%04d",contraSenha);
  HD44780_writeString(bufferLinha);
  
  HD44780_posicionaTexto(0,1);
  HD44780_writeString("Contra:");
  
  sprintf(bufferLinha,"000000");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           if(indice>4){
             
             unsigned int recebido = ((bufferLinha[0]-'0')*100000) + 
                                     ((bufferLinha[1]-'0')*10000) + 
                                     ((bufferLinha[2]-'0')*1000) + 
                                     ((bufferLinha[3]-'0')*100) + 
                                     ((bufferLinha[4]-'0')*10) + 
                                     ((bufferLinha[5]-'0'));
             
             recebido &= 0x3FFFF;             
             horas = recebido>>14;
             
             senha = ((recebido&0x3FFF)^3004);             
             if(senha==contraSenha)
              return 255;                                           
             
             return 0;
           }
           else{
             indice++;
             break;
           }
      case TECLA_ESC:
           return 0;
      case TECLA_INC:
           bufferLinha[indice]++;
           if(bufferLinha[indice]>'9')
             bufferLinha[indice] = '0';
           break;
      case TECLA_DEC:    
           bufferLinha[indice]--;
           if(bufferLinha[indice]<'0')
             bufferLinha[indice] = '9';
           break;
    }
    
    if(!--ciclos){
      ciclos = 4;
      toggle = ~toggle;
      memcpy(bufferTmp,bufferLinha,17);
      bufferTmp[7] = 0x00;
      if(toggle)
        bufferTmp[indice] = '_';
      
      HD44780_posicionaTexto(7,1);
      HD44780_writeString(bufferTmp);
    }       
    
    vTaskDelay(50);
  }       
  
  return 0;
}
/************************************************************************************
*       Descrição       :       Tela para exibir a quantidade parcial
*                               de pipocas vendidas
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_exibeTotalizadorUnidadesParcial(void){
  eTECLA tecla;
  unsigned int arrecadacao = PARAMETROS_leContadorVendasParcial();
  char bufferLinha[17];
  
  sprintf(bufferLinha,"[%d]",arrecadacao);
  HD44780_clearText();
  //                   12345678901232456
  HD44780_writeString("Contador parcial ");
  HD44780_posicionaTexto(0,1);
  HD44780_writeString(bufferLinha);    
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
      case TECLA_ESC: 
           return;
    }
            
    vTaskDelay(100);
  }     
}
/************************************************************************************
*       Descrição       :       Tela para exibir a quantidade total de 
*                               pipocas vendidas
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_exibeTotalizadorUnidadesTotal(void){
  eTECLA tecla;
  unsigned int arrecadacao = PARAMETROS_leContadorVendas();
  char bufferLinha[17];
  
  sprintf(bufferLinha,"[%d]",arrecadacao);
  HD44780_clearText();
  //                   12345678901232456
  HD44780_writeString("Contador parcial ");
  HD44780_posicionaTexto(0,1);
  HD44780_writeString(bufferLinha);    
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
      case TECLA_ESC: 
           return;
    }
            
    vTaskDelay(100);
  }     
}
/************************************************************************************
*       Descrição       :       Ponto de entrada do menu de configuração
*                               da rede wifi
*       Parametros      :       nenhum
*       Retorno         ;       nenhum
************************************************************************************/
void MCS_configuracaoWifiEntry(void){
  eTECLA tecla;
  unsigned char indiceOpcoes=0;
     
  MCS_desenhaOpcoesMenuAjusteWifi(indiceOpcoes);
  
  for(;TECLADO_getContadorInatividade();){
  
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:  
           if(MCS_funcWifi[indiceOpcoes]!=NULL){
             MCS_funcWifi[indiceOpcoes]();
             MCS_desenhaOpcoesMenuAjusteWifi(indiceOpcoes);  
           }
           else
             return;        
           break;
      case TECLA_ESC:            
           return;
      case TECLA_DEC:
           indiceOpcoes?(indiceOpcoes--):(indiceOpcoes=TAM_LISTA_WIFI-1);
           MCS_desenhaOpcoesMenuAjusteWifi(indiceOpcoes);           
           break;
      case TECLA_INC:
           indiceOpcoes = (indiceOpcoes+1) % TAM_LISTA_WIFI;
           MCS_desenhaOpcoesMenuAjusteWifi(indiceOpcoes);           
           break;
    }
    
    vTaskDelay(50);
  }
}
/************************************************************************************
*       Descrição       :       Desenha as opções do menu de configuração
*                               da rede wifi
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_desenhaOpcoesMenuAjusteWifi(unsigned char indice){
  
  HD44780_posicionaTexto(0,0);
  HD44780_writeString(" Config Wifi    ");
  HD44780_posicionaTexto(0,1);
  HD44780_writeString((char*)MCS_mensagemMenuWifi[indice]);  
}
/************************************************************************************
*       Descrição       :       Exibe o SSID configurado para a rede wifi
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_menuExibeSSID(void){
  eTECLA tecla;
  char bufferSSID[64];
  unsigned char tentativas=10;
  unsigned char flag;
  
    HD44780_posicionaTexto(0,0);
    HD44780_writeString(" Lendo SSID     ");
    HD44780_posicionaTexto(0,1);
    HD44780_writeString(" no mod. WiFi   ");      
  
  do flag = TELET_leSSID(bufferSSID);
  while(!flag && tentativas--);
  
  if(!flag){
    HD44780_posicionaTexto(0,0);
    HD44780_writeString(" Falha ao ler   ");
    HD44780_posicionaTexto(0,1);
    HD44780_writeString(" o mod WiFi     ");    
    vTaskDelay(3000);
    return;
  }
  
  HD44780_clearText();
  HD44780_posicionaTexto(0,0);  
  HD44780_writeString("SSID");
  HD44780_posicionaTexto(0,1);
  bufferSSID[16] = 0;
  HD44780_writeString(bufferSSID);
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
      case TECLA_ESC:
      case TECLA_INC:
      case TECLA_DEC:
           return;
    }       
    vTaskDelay(50);
  }      
}
/************************************************************************************
*       Descrição       :       Tela para o usuário alterar o SSID
*                               da rede
*       Parametros      :       nenhum
*       Retorno         :       nenhm
************************************************************************************/
void MCS_telaAlterarSSID(void){
  eTECLA tecla;
  char bufferSSID[64];
  char bufferTmp[17];
  unsigned char tentativas=10;
  unsigned char flag;
  unsigned char indice=0;
  unsigned char toogle=0;
  unsigned char ciclos=1;
  
    HD44780_posicionaTexto(0,0);
    HD44780_writeString(" Lendo SSID     ");
    HD44780_posicionaTexto(0,1);
    HD44780_writeString(" no mod. WiFi   ");      
  
  do flag = TELET_leSSID(bufferSSID);
  while(!flag && tentativas--);
  
  if(!flag){
    HD44780_posicionaTexto(0,0);
    HD44780_writeString(" Falha ao ler   ");
    HD44780_posicionaTexto(0,1);
    HD44780_writeString(" o mod WiFi     ");    
    vTaskDelay(3000);
    return;
  }
  
  HD44780_clearText();
  HD44780_posicionaTexto(0,0);  
  HD44780_writeString("SSID");
  HD44780_posicionaTexto(0,1);
  bufferSSID[16] = 0;
  HD44780_writeString(bufferSSID);  
  
  for(unsigned char i=0;i<48;i++)
    bufferSSID[16+i] = 0;
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
             if(!bufferSSID[indice])
               bufferSSID[indice] = ' ';
            
             indice++;
             
             if(indice>15){
               MCS_telaSalvaSSID(bufferSSID);
               return;
             }
           break;
      case TECLA_ESC:
           return;
      case TECLA_INC:
           if(bufferSSID[indice]>'~')
             bufferSSID[indice]='~';
           if(bufferSSID[indice]<' ')
             bufferSSID[indice] = ' ';
        
           bufferSSID[indice]++;
           if(bufferSSID[indice]>'~')
             bufferSSID[indice] = ' ';
           break;
      case TECLA_DEC:
           if(bufferSSID[indice]>'~')
             bufferSSID[indice]='~';
           if(bufferSSID[indice]<' ')
             bufferSSID[indice] = ' ';
           
           bufferSSID[indice]--;
           if(bufferSSID[indice]<' ')
             bufferSSID[indice] = '~';
           break;
    }
    
    if(!--ciclos){
      ciclos = 5;
      toogle = ~toogle;
      memcpy(bufferTmp,bufferSSID,16);
      bufferTmp[16] = 0;
      if(toogle)
        bufferTmp[indice] = '_';
      HD44780_posicionaTexto(0,1);
      HD44780_writeString(bufferTmp);      
    }
    
    vTaskDelay(50);
  }      
}
/************************************************************************************
*       Descrição       :       Salva os dados do SSID na memória do módulo
*                               Wifi
*       Parametros      :       (char*) buffer com o SSID
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaSalvaSSID(char *ssid){
  unsigned char flag;
  unsigned char tentativas=10;
  
  HD44780_posicionaTexto(0,0);
  HD44780_writeString(" Escrevendo SSID");
  HD44780_posicionaTexto(0,1);
  HD44780_writeString(" no mod. WiFi   ");    
  
  MCS_removeEspacosFinalString(ssid);
  do flag = TELET_escreveSSID(ssid);
  while(!flag && tentativas--);
  
  if(!flag){
    HD44780_posicionaTexto(0,0);
    HD44780_writeString("Falha  escrever ");
    HD44780_posicionaTexto(0,1);
    HD44780_writeString(" no mod WiFi    ");    
    vTaskDelay(3000);
  }
}
/************************************************************************************
*       Descrição       :       Remove os espaços após o final da string
*       Parametros      :       (char*) string
*       Retorno         :       nenhum
************************************************************************************/
void MCS_removeEspacosFinalString(char*string){  
  char *ptr;
  
  ptr = &string[16];
  
  while(!((*ptr)>='!' && (*ptr)<='~')){
    *ptr = 0;
    ptr--;
  }    
}
/************************************************************************************
*       Descrição       :       Tela para excluir o SSID configurado
*                               para a máquina
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_removeSSID(void){
  eTECLA tecla;
  char bufferSSID[65]={0};
  unsigned char tentativas=5;
  unsigned char flag;
  
  HD44780_posicionaTexto(0,0);
  HD44780_writeString("Conf. exclusao? ");
  HD44780_posicionaTexto(0,1);
  HD44780_writeString("[enter] p/ SIM  ");      

  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:           
           HD44780_posicionaTexto(0,0);
           HD44780_writeString(" Excluindo SSID ");
           HD44780_posicionaTexto(0,1);
           HD44780_writeString(" no mod. WiFi   ");      
          do flag = TELET_escreveSSID(bufferSSID);
          while(!flag && tentativas--);
  
          if(!flag){
             HD44780_posicionaTexto(0,0);
             HD44780_writeString("Falha  escrever ");
             HD44780_posicionaTexto(0,1);
             HD44780_writeString(" no mod WiFi    ");    
             vTaskDelay(3000);
           }
           return;
      case TECLA_ESC:
      case TECLA_INC:
      case TECLA_DEC:
           return;
    }        
    
    vTaskDelay(50);
  }    
}
/************************************************************************************
*       Descrição       :       Tela para visualizar a senha da rede wifi
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_menuVisualizaSenhaWiFi(void){
  eTECLA tecla;
  char bufferSenha[64];
  unsigned char tentativas=10;
  unsigned char flag;
  
    HD44780_posicionaTexto(0,0);
    HD44780_writeString(" Lendo Senha    ");
    HD44780_posicionaTexto(0,1);
    HD44780_writeString(" no mod. WiFi   ");      
  
  do flag = TELET_leSenhaWifi(bufferSenha);
  while(!flag && tentativas--);
  
  if(!flag){
    HD44780_posicionaTexto(0,0);
    HD44780_writeString(" Falha ao ler   ");
    HD44780_posicionaTexto(0,1);
    HD44780_writeString(" o mod WiFi     ");    
    vTaskDelay(3000);
    return;
  }
  
  HD44780_clearText();
  HD44780_posicionaTexto(0,0);  
  HD44780_writeString("Senha");
  HD44780_posicionaTexto(0,1);
  bufferSenha[16] = 0;
  HD44780_writeString(bufferSenha);
 
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
      case TECLA_ESC:
      case TECLA_INC:
      case TECLA_DEC:
           return;
    }    
        
    vTaskDelay(50);
  }      
}
/************************************************************************************
*       Descrição       :       Tela para realizar a edição da senha da
*                               rede wifi
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaEditaSenhaWifi(void){
  eTECLA tecla;
  char bufferSSID[64];
  char bufferTmp[17];
  unsigned char tentativas=10;
  unsigned char flag;
  unsigned char indice=0;
  unsigned char toogle=0;
  unsigned char ciclos=1;
  
    HD44780_posicionaTexto(0,0);
    HD44780_writeString(" Lendo senha    ");
    HD44780_posicionaTexto(0,1);
    HD44780_writeString(" no mod. WiFi   ");      
  
  do flag = TELET_leSenhaWifi(bufferSSID);
  while(!flag && tentativas--);
  
  if(!flag){
    HD44780_posicionaTexto(0,0);
    HD44780_writeString(" Falha ao ler   ");
    HD44780_posicionaTexto(0,1);
    HD44780_writeString(" o mod WiFi     ");    
    vTaskDelay(3000);
    return;
  }
  
  HD44780_clearText();
  HD44780_posicionaTexto(0,0);  
  HD44780_writeString("Senha");
  HD44780_posicionaTexto(0,1);
  bufferSSID[16] = 0;
  HD44780_writeString(bufferSSID);  
  
  for(unsigned char i=0;i<48;i++)
    bufferSSID[16+i] = 0;
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
             if(!bufferSSID[indice])
               bufferSSID[indice] = ' ';
            
             indice++;
             
             if(indice>15){
               MCS_telaSalvaSenhaWiFi(bufferSSID);
               return;
             }
           break;
      case TECLA_ESC:
           return;
      case TECLA_INC:
           if(bufferSSID[indice]>'~')
             bufferSSID[indice]='~';
           if(bufferSSID[indice]<' ')
             bufferSSID[indice] = ' ';
           
           bufferSSID[indice]++;
           if(bufferSSID[indice]>'~')
             bufferSSID[indice] = ' ';
           break;
      case TECLA_DEC:
           if(bufferSSID[indice]>'~')
             bufferSSID[indice]='~';
           if(bufferSSID[indice]<' ')
             bufferSSID[indice] = ' ';
           
           bufferSSID[indice]--;
           if(bufferSSID[indice]<' ')
             bufferSSID[indice] = '~';
           break;
    }
    
    if(!--ciclos){
      ciclos = 5;
      toogle = ~toogle;
      memcpy(bufferTmp,bufferSSID,16);
      bufferTmp[16] = 0;
      if(toogle)
        bufferTmp[indice] = '_';
      HD44780_posicionaTexto(0,1);
      HD44780_writeString(bufferTmp);      
    }
    
    vTaskDelay(50);
  }      
}
/************************************************************************************
*       Descrição       :       Tela para salvar a senha wifi no módulo
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaSalvaSenhaWiFi(char *senha){
  unsigned char flag;
  unsigned char tentativas=10;
  
  HD44780_posicionaTexto(0,0);
  HD44780_writeString(" Escrevendo SSID");
  HD44780_posicionaTexto(0,1);
  HD44780_writeString(" no mod. WiFi   ");    
  
  MCS_removeEspacosFinalString(senha);
  do flag = TELET_escreveSenhaWifi(senha);
  while(!flag && tentativas--);
  
  if(!flag){
    HD44780_posicionaTexto(0,0);
    HD44780_writeString("Falha  escrever ");
    HD44780_posicionaTexto(0,1);
    HD44780_writeString(" no mod WiFi    ");    
    vTaskDelay(3000);
  }
}
/************************************************************************************
*       Descrição       :       Apaga a senha da rede wifi
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_apagaSenhaWifi(void){
  eTECLA tecla;
  char bufferSSID[65]={0};
  unsigned char tentativas=5;
  unsigned char flag;
  
  HD44780_posicionaTexto(0,0);
  HD44780_writeString("Conf. exclusao? ");
  HD44780_posicionaTexto(0,1);
  HD44780_writeString("[enter] p/ SIM  ");      

  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:           
           HD44780_posicionaTexto(0,0);
           HD44780_writeString(" Excluindo senha");
           HD44780_posicionaTexto(0,1);
           HD44780_writeString(" no mod. WiFi   ");      
          do flag = TELET_escreveSenhaWifi(bufferSSID);
          while(!flag && tentativas--);
  
          if(!flag){
             HD44780_posicionaTexto(0,0);
             HD44780_writeString("Falha  escrever ");
             HD44780_posicionaTexto(0,1);
             HD44780_writeString(" no mod WiFi    ");    
             vTaskDelay(3000);
           }
           return;
      case TECLA_ESC:
      case TECLA_INC:
      case TECLA_DEC:
           return;
    }        
    
    vTaskDelay(50);
  }    
}
/************************************************************************************
*       Descrição       :       Tela para o usuário configurar qual noteiro
*                               será utilizado pela máquina
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_selecionaTipoNoteiro(void){
  eTECLA tecla;
  const char tipos[3][2][17]={
    {"[ ] BV20        ","[X] BV20        "},
    {"[ ] P70         ","[X] P70         "},
    {" Voltar         "," Voltar         "}
  };
  eTIPO_NOTEIRO tipo = PARAMETROS_leTipoNoteiro();
  unsigned char indice=0;
  
  HD44780_clearText();
  HD44780_writeString("  Tipo Noteiro  ");  
  HD44780_posicionaTexto(0,1);
  HD44780_writeString((char*)tipos[indice][tipo==indice?1:0]);             
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           if(indice==2)
             return;
           tipo = (eTIPO_NOTEIRO) indice;
           PARAMETROS_salvaTipoNoteiro(tipo);
           HD44780_posicionaTexto(0,1);
           HD44780_writeString((char*)tipos[indice][tipo==indice?1:0]);           
           break;
      case TECLA_ESC:
           return;
      case TECLA_INC:
           indice = (indice+1) % 3;
           HD44780_posicionaTexto(0,1);
           HD44780_writeString((char*)tipos[indice][tipo==indice?1:0]);           
           break;
      case TECLA_DEC:
           if(indice)
             indice--;
           else
             indice = 2;
           HD44780_posicionaTexto(0,1);
           HD44780_writeString((char*)tipos[indice][tipo==indice?1:0]);           
           break;
    }    
    
    vTaskDelay(50);
  }    
}
/************************************************************************************
*       Descrição       :       Configura o modelo da impressora utilizada
*                               para a leitura da arrecadação da máquina
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_menuConfiguraImpressora(void){
  eTECLA tecla;
  const char tipos[3][2][17]={
    {"[ ] ATPRINTER   ","[X] ATPRINTER   "},
    {"[ ] EXTECH      ","[X] EXTECH      "},
    {" Voltar         "," Voltar         "}
  };
  eTIPO_IMPRESSORA tipo = PARAMETROS_carregaTipoImpressora();
  unsigned char indice=0;
  
  HD44780_clearText();
  HD44780_writeString(" Tipo Impressora");  
  HD44780_posicionaTexto(0,1);
  HD44780_writeString((char*)tipos[indice][tipo==indice?1:0]);             
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           if(indice==2)
             return;
           tipo = (eTIPO_IMPRESSORA) indice;
           PARAMETROS_salvaTipoImpressora(tipo);
           HD44780_posicionaTexto(0,1);
           HD44780_writeString((char*)tipos[indice][tipo==indice?1:0]);           
           break;
      case TECLA_ESC:
           return;
      case TECLA_INC:
           indice = (indice+1) % 3;
           HD44780_posicionaTexto(0,1);
           HD44780_writeString((char*)tipos[indice][tipo==indice?1:0]);           
           break;
      case TECLA_DEC:
           if(indice)
             indice--;
           else
             indice = 2;
           HD44780_posicionaTexto(0,1);
           HD44780_writeString((char*)tipos[indice][tipo==indice?1:0]);           
           break;
    }    
    
    vTaskDelay(50);
  }    
}
/************************************************************************************
*       Descrição       :       Tela para visualização das 
*                               vendas realizadas com o cartão de crédito
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaVendasCartao(void){
  eTECLA tecla;
  char linha[17];
  
  HD44780_clearText();
  HD44780_writeString(" Vendas Cartao  ");
  
  unsigned int vendas = PARAMETROS_carregaOperacoesCartao();
  sprintf(linha,"Total:R$ %07d,00",vendas);
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER: return;
    }
 
    HD44780_posicionaTexto(0,1);
    HD44780_writeString(linha);
    
    vTaskDelay(50);
  }    
}
/************************************************************************************
*       Descrição       :       Tela para o usuário configurar o tipo
*                               do moedeiro que será utilizado
*                               na máquina
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_telaEscolheMoedeiro(void){
  eTECLA tecla;
  const char listaOpcoesMoedeiros[4][2][17]={
    "[ ] RM5 Evol.  ","[X] RM5 Evol.  ",
    "[ ] ICT UCA-1  ","[X] ICT UCA-1  ",
    "[ ] SOMYC      ","[X] SOMYC      ",
    " Voltar        "," Voltar        "
  };
  unsigned char indice=0;
  unsigned char opcao=PARAMETROS_carregaTipoMoedeiro();
  
  HD44780_clearText();
  HD44780_writeString("Tipo moedeiro");
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           if(indice==3)
             return;
           else{
             opcao = indice;
             PARAMETROS_salvaTipoMoedeiro((eTIPO_MOEDEIRO)opcao);
           }
           break;
      case TECLA_ESC:
           return;
      case TECLA_INC: 
           indice = (indice+1)%4;
           break;
      case TECLA_DEC:
           if(indice)
             indice--;
           else
             indice = 3;
           break;
    }
   
    HD44780_posicionaTexto(0,1);
    HD44780_writeString((char*)listaOpcoesMoedeiros[indice][(opcao==indice)?1:0]);
    
    vTaskDelay(50);
  }  
}
/***********************************************************************************
*       Descrição       :       Escreve os valores do rtc no displays
*       Parametros      :       (unsigned char) hora
*                               (unsigned char) minuto
*                               (unsigned char) segundo
*                               (unsigned char) dia
*                               (unsigned char) mes
*                               (unsigned int) ano
*                               (unsigned char) blink
*                               (unsigned char) indice                                       
*       Retorno         :       nenhum
***********************************************************************************/
void MCS_exibeValores(unsigned char hora,unsigned char minuto,unsigned char segundo,
                      unsigned char dia,unsigned char mes,unsigned int ano,
                      unsigned char indice){
  char buffer[17];    
                      
  
  HD44780_posicionaTexto(4,0);
  sprintf(buffer,"%02d:%02d:%02d",hora,minuto,segundo);
  HD44780_writeString(buffer);
    
  HD44780_posicionaTexto(3,1);
  sprintf(buffer,"%02d/%02d/%04d",dia,mes,ano);
  HD44780_writeString(buffer);                        
}
/***********************************************************************************
*       Descrição       :       Interface para ajuste do relógio
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MCS_ajusteRelogio(void){
    
  IAR_entry();
}
/************************************************************************************
*       Descrição       :       Interface para teste do RTC
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_testeRTC(void){
  
  IAR_testeRTC();
}
/************************************************************************************
*       Descrição       :       Interface para o usuário cadastrar o numero
*                               de série da máquina
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_cadastraNumeroSerie(void){
  eTECLA tecla;
  unsigned int serial = PARAMETROS_carregaNumeroSerie();
  char numSerie[17]={0x20};
  unsigned char tamanho,ultimoTamanho=0;
  const char *msgInc[]={"+1","+10","+100","+1000"};
  const short int incrementos[]={1,10,100,1000};
  unsigned char incremento=0;
  
  HD44780_clearText();
  HD44780_writeString("Cad. Num. Serie ");    
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           incremento = (incremento + 1) % 4;
           break;
      case TECLA_ESC:
           PARAMETROS_setNumeroSerie(serial);
           return;
      case TECLA_INC:
           serial += incrementos[incremento];
           break;
      case TECLA_DEC:
           if(serial>=incrementos[incremento])
             serial-=incrementos[incremento];
           break;
    }
    
    sprintf(numSerie,"%d(%s)",serial,msgInc[incremento]);
    tamanho = strlen(numSerie);
    if(tamanho!=ultimoTamanho){
      HD44780_posicionaTexto(0,1);
      HD44780_writeString("                ");
    }    
    ultimoTamanho = tamanho;
    HD44780_posicionaTexto((16-tamanho)>>1,1);
    HD44780_writeString(numSerie);   
  }  
}
/************************************************************************************
*       Descrição       :       Desenha as opções do menu de exibição
*       Parametros      :       (unsigned char) indice
*       Retorno         :       nenhum
************************************************************************************/
void MCS_desenhaOpcoesExibicao(unsigned char indice){
  
  HD44780_posicionaTexto(0,0);
  HD44780_writeString("   Exibicao   ");
  
  HD44780_posicionaTexto(1,1);
  HD44780_writeString((char*)MCS_opcoesMenuExibicao[indice]);
  HD44780_posicionaTexto(0,1);
  HD44780_writeChar(0x7E);
}
/************************************************************************************
*       Descrição       :       Menu com as opções de exibição 
*                               da máquina
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_menuOpcoesExibicao(void){
  eTECLA tecla;
  unsigned char indice=0;  
  
  HD44780_clearText();
  MCS_desenhaOpcoesExibicao(indice);
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           if(MCS_funcMenuExibicao[indice]!=NULL)
             MCS_funcMenuExibicao[indice]();
           else
             return;
           break;
      case TECLA_ESC:
           return;
      case TECLA_INC:
           indice = (indice+1) % TAM_MENU_EXIBICAO;
           break;
      case TECLA_DEC:
           if(indice)
             indice--;
           else
             indice = TAM_MENU_EXIBICAO - 1;
           break;
    }
    
    MCS_desenhaOpcoesExibicao(indice);
    
    vTaskDelay(50);
  }    
}
/************************************************************************************
*       Descrição       :       Desenha as opções do menu de operação
*       Parametros      :       (unsigned char) indice
*       Retorno         :       nenhum
************************************************************************************/
void MCS_desenhaOpcoesOperacao(unsigned char indice){
  
  HD44780_posicionaTexto(0,0);
  HD44780_writeString("    Operacao    ");
  
  HD44780_posicionaTexto(1,1);
  HD44780_writeString((char*)MCS_opcoesOperacao[indice]);
  HD44780_posicionaTexto(0,1);
  HD44780_writeChar(0x7E);
}
/************************************************************************************
*       Descrição       :       Menu com as opções de operação
*                               da máquina
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_menuOpcoesOperacao(void){
  eTECLA tecla;
  unsigned char indice=0;  
  
  HD44780_clearText();  
  MCS_desenhaOpcoesOperacao(indice);
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           if(MCS_funcOperacao[indice]!=NULL)
             MCS_funcOperacao[indice]();
           else
             return;
           break;
      case TECLA_ESC:
           return;
      case TECLA_INC:
           indice = (indice+1) % TAM_MENU_OPERACAO;
           break;
      case TECLA_DEC:
           if(indice)
             indice--;
           else
             indice = TAM_MENU_OPERACAO - 1;
           break;
    }
    
    MCS_desenhaOpcoesOperacao(indice);
    
    vTaskDelay(50);
  }    
}
/************************************************************************************
*       Descrição       :       Menu com as opções da telemetria
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_menuTelemetria(void){
  
  
}
/************************************************************************************
*       Descrição       :       Desenha as opções do menu para configuração
*                               do desumidificador
*       Parametros      :       (unsigned char) indice
*       Retorno         :       nenhum
************************************************************************************/
void MCS_desenhaOpcoesDesumidificador(unsigned char indice){
  
  HD44780_posicionaTexto(0,0);
  HD44780_writeString(" Desumidificador");
  
  HD44780_posicionaTexto(1,1);
  HD44780_writeString((char*)MCS_opcoesDesumidificacao[indice]);
  HD44780_posicionaTexto(0,1);
  HD44780_writeChar(0x7E);
}
/************************************************************************************
*       Descrição       :       Menu desumificação
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_menuDesumidificacao(void){
  eTECLA tecla;
  unsigned char indice=0;  
  
  MCS_desenhaOpcoesDesumidificador(indice);
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           if(MCS_funcDesumidificador[indice]!=NULL)
             MCS_funcDesumidificador[indice]();
           else
             return;
           break;
      case TECLA_ESC:
           return;
      case TECLA_INC:
           indice = (indice+1) % TAMANHO_OPCOES_DESUMI;
           break;
      case TECLA_DEC:
           if(indice)
             indice--;
           else
             indice = TAMANHO_OPCOES_DESUMI - 1;
           break;
    }
    
    MCS_desenhaOpcoesDesumidificador(indice);
    
    vTaskDelay(50);
  }       
}
/************************************************************************************
*       Descrição       :       Desenha as opções do menu para configuração
*                               dos periféricos
*       Parametros      :       (unsigned char) indice
*       Retorno         :       nenhum
************************************************************************************/
void MCS_desenhaOpcoesPerifericos(unsigned char indice){
  
  HD44780_posicionaTexto(0,0);
  HD44780_writeString("   Perifericos  ");
  
  HD44780_posicionaTexto(1,1);
  HD44780_writeString((char*)MCS_ocpoesPerifericos[indice]);
  HD44780_posicionaTexto(0,1);
  HD44780_writeChar(0x7E);
}
/************************************************************************************
*       Descrição       :       Menu para configuração dos periféricos da máquina
*       parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_menuConfiguraPerifericos(void){
  eTECLA tecla;
  unsigned char indice=0;  
  
  HD44780_clearText();  
  MCS_desenhaOpcoesPerifericos(indice);
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           if(MCS_funcPerifericos[indice]!=NULL)
             MCS_funcPerifericos[indice]();
           else
             return;
           break;
      case TECLA_ESC:
           return;
      case TECLA_INC:
           indice = (indice+1) % TAM_MENU_PERIFERICOS;
           break;
      case TECLA_DEC:
           if(indice)
             indice--;
           else
             indice = TAM_MENU_PERIFERICOS - 1;
           break;
    }
    
    MCS_desenhaOpcoesPerifericos(indice);
    
    vTaskDelay(50);
  }       
}
/************************************************************************************
*       Descrição       :       Desenha os dados do menu avançado na tela
*       Parametros      :       (unsigned char) indice
*       Retorno         :       nenhum
************************************************************************************/
void MCS_desenhaOpcoesAvancado(unsigned char indice){
  
  HD44780_posicionaTexto(0,0);
  HD44780_writeString("   Perifericos  ");
  
  HD44780_posicionaTexto(1,1);
  HD44780_writeString((char*)MCS_mensagemAvancado[indice]);
  HD44780_posicionaTexto(0,1);
  HD44780_writeChar(0x7E);
}
/************************************************************************************
*       Descrição       :       Menu avançado
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_menuAvancado(void){
  eTECLA tecla;
  unsigned char indice=0;  
  
  if(!MCS_telaSenha(6651)){
    HD44780_clearText();
    return;
  }
  
  MCS_desenhaOpcoesAvancado(indice);
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           if(MCS_funcAvancado[indice]!=NULL)
             MCS_funcAvancado[indice]();
           else
             
             return;
           break;
      case TECLA_ESC:
           return;
      case TECLA_INC:
           indice = (indice+1) % TAM_LISTA_AVANCADO;
           break;
      case TECLA_DEC:
           if(indice)
             indice--;
           else
             indice = TAM_LISTA_AVANCADO - 1;
           break;
    }
    
    MCS_desenhaOpcoesAvancado(indice);
    
    vTaskDelay(50);
  } 
}
/************************************************************************************
*       Descrição       :       Menu para teste/configuração do volume
*                               da música
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_testeAjusteVolumeMusica(void){
  eTECLA tecla;
  unsigned short int volume = PARAMETROS_leVolumeSom();  
  char buffer[17];
  const char *tag[]={"PLAY","STOP"};
    
  HD44780_clearText();
  HD44780_writeString("  Volume Musica ");
  
  for(;;){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           PLAYER_interrompeMusica();        
           vTaskDelay(10);
           
           PARAMETROS_gravaVolumeSom(volume);                        
           vTaskDelay(10);
           
           PLAYERWAVE_iniciaMusica(1,0);
           break;
      case TECLA_ESC:
           PLAYER_interrompeMusica();    
           vTaskDelay(10);
           
           PARAMETROS_gravaVolumeSom(volume);          
           HD44780_clearText();           
           return;
      case TECLA_INC:
           if(volume<100)
             volume++;
           PLAYER_interrompeMusica();           
           break;
      case TECLA_DEC:
           if(volume>1)
             volume--;
           PLAYER_interrompeMusica();                      
           break;
    }
    
    sprintf(buffer,"V:%03d \% (%s)",volume,PLAYERWAVE_verificaToque()?(tag[0]):(tag[1]));
    HD44780_posicionaTexto(0,1);    
    HD44780_writeString(buffer);
    
  }    
}
/************************************************************************************
*       Descrição       :       Interface para o usuário configurar o volume
*                               da voz
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_interfaceConfiguraVolumeTesteVoz(void){
  eTECLA tecla;
  unsigned short int volume = PARAMETROS_leVolumeVoz();  
  char buffer[17];
  const char *tag[]={"PLAY","STOP"};
    
  HD44780_clearText();
  HD44780_writeString("   Volume Voz   ");
  
  for(;;){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           PLAYER_interrompeMusica();
           vTaskDelay(1);
           PARAMETROS_gravaVolumeVoz(volume);  
           vTaskDelay(1);
           PLAYERWAVE_iniciaMusica(0,0);
           break;
      case TECLA_ESC:
           PLAYER_interrompeMusica();     
           PARAMETROS_gravaVolumeVoz(volume);             
           HD44780_clearText();           
           return;
      case TECLA_INC:
           if(volume<100)
             volume++;
           PLAYER_interrompeMusica();           
           break;
      case TECLA_DEC:
           if(volume>1)
             volume--;
           PLAYER_interrompeMusica();                      
           break;
    }
    
    sprintf(buffer,"V:%03d \% (%s)",volume,PLAYERWAVE_verificaToque()?(tag[0]):(tag[1]));
    HD44780_posicionaTexto(0,1);    
    HD44780_writeString(buffer);    
  }     
}
/************************************************************************************
*       Descrição       :       Tela para cadastrar o tempo de desligamento
*                               do motor da embalagem
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_configuraTesteTempoEmbalagem(void){
  eTECLA tecla;
  unsigned int atrasoPacote = PARAMETROS_leParametro(ATRASO_PACOTE);    
  char bufferLinha[17];
  char temp[17];
  
  HD44780_clearText();
  HD44780_writeString(" Tempo embalagem");
    
  for(;;){
  
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           PARAMETROS_escreveParametro(ATRASO_PACOTE,atrasoPacote); 
           HD44780_posicionaTexto(0,1);
           HD44780_writeString("Liber. emb.     ");
           CE_empurraEmbalagem(atrasoPacote);
           HD44780_posicionaTexto(0,1);
           HD44780_writeString("                ");           
           break;
      case TECLA_ESC:
           PARAMETROS_escreveParametro(ATRASO_PACOTE,atrasoPacote);         
           HD44780_clearText();        
           return;
      case TECLA_INC:
           atrasoPacote++;
           break;
      case TECLA_DEC:
           if(atrasoPacote>5)
             atrasoPacote--;
           break;
    }
   
    bufferLinha[0] = ((atrasoPacote/100)%10) + '0';
    bufferLinha[1] = ((atrasoPacote/10)%10) + '0';
    bufferLinha[2] = ',';
    bufferLinha[3] = (atrasoPacote%10) + '0';
    bufferLinha[4] = '\0';          
    sprintf(temp,"Temp:(%s) s",bufferLinha);
    HD44780_posicionaTexto(0,1);
    HD44780_writeString(temp);    
  }    
}
/************************************************************************************
*       Descrição       :       Menu para exibição dos dados de faturamento
*                               de cartão de crédito
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_menuConfigExibeCartao(void){
  eTECLA tecla;
  unsigned int parcial = PARAMETROS_carregaFaturamentoParcialCartao();
  unsigned int total = PARAMETROS_carregaFaturamentoTotalCartao();  
  char buffer[17];
  
  HD44780_clearText();
  
  for(;TECLADO_getContadorInatividade();){
    
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:  
           break;
      case TECLA_ESC:
           return;
      case TECLA_INC:
           break;
      case TECLA_DEC:
           break;
    }
            
    sprintf(buffer,"Par:R$ %06d,00",parcial);
    HD44780_posicionaTexto(0,0);
    HD44780_writeString(buffer);
    
    sprintf(buffer,"Tot:R$ %06d,00",total);
    HD44780_posicionaTexto(0,1);
    HD44780_writeString(buffer);    
    
    vTaskDelay(50);    
  }      
}
/************************************************************************************
*       Descrição       :       Teste das saídas de coletor aberto utilizadas
*                               para bloqueio do P70
*       Parametros      :       nenhum
*       Retorno         :       nenhum
************************************************************************************/
void MCS_testeSaidaBloqueioP70(void){
  eTECLA tecla;
  unsigned char ligar=0;  
  
  HD44780_clearText();
  HD44780_posicionaTexto(0,0);  
  HD44780_writeString("Teste saida aux."); 
  
  for(;TECLADO_getContadorInatividade();){
     
    tecla = TECLADO_getch();
    switch(tecla){
      case TECLA_ENTER:
           ligar ^= 255;
           break;
      case TECLA_ESC:
           return;
      case TECLA_INC:
           ligar = 0;           
           break;
      case TECLA_DEC:
           ligar = 255;
           break;
    }
    
    vTaskDelay(50);       
    AL_bloqueioTrilogy(ligar);
    
    HD44780_posicionaTexto(0,1);
    if(!ligar)
      HD44780_writeString("[Ligado]   ");
    else
      HD44780_writeString("[Desligado]");   
    
    AL_tick();
  }    
}
/************************************************************************************
*       Fim do arquivo
************************************************************************************/ 
