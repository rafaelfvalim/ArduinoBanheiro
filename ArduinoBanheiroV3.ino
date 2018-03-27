/*
   -------------------------------------------------------------
    OCTA EIT - Especialista em TI
  Programa para Acionamento de aromatizador de ambiente por
  por detecção de movimento.
  BY::RAFAEL VALIM / CLEBER CASTRO
  VERSAO: 3
  --------------------------------------------------------------
  - Pino trigger = 7
  - Pino echo = 5
  - Distância do raio de ação do sensor 80cm
  - Pino Relay = 6
  - previousMillis  de acionamento = 1800 segundos = 30 minutos
  ---------------------------------------------------------------
   29.01.2018:
   Ajuste para funcionamento do relay normamente fechados
   23.02.2018:
   Aletaração da lógica de funcionamneto do sensor x previousMillis
   27.02.2018:
   Alterado para continuar contagem de previousMillis  idependente do sensor de movimento
   2.03.2018
   Alterado forma de processamento 
*/
#include <NewPing.h>
/*---------------------------------------------------------------
   Valores para ajustar
   --------------------------------------------------------------
*/
#define TRIGGER_PIN  7 // Pino trigger
#define ECHO_PIN     5  // Pino echo 
#define MAX_DISTANCE 80 // Distância de ação do sensor em centimetros
const int pinRele = 6; //Pino do Relay para acionamento do motor
unsigned long ciclo = 15  ; // 1800 seg =  30 min.
unsigned long tempoSpray = 2000; //previousMillis  de acionamento spray em milisegundos
unsigned long standby = 15; // Tempo de standby em segundos
int tentativasSonar = 10; //Numero de leituras do sonar
/*-------------------------------------------------------------
   Valores fixos
  -------------------------------------------------------------
*/
int count = 0;
boolean acionaSpray = false;
boolean debug = true;
long previousMillis  = 0;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);
  pinMode(pinRele, OUTPUT);
  //Relay normalmente aberto
  digitalWrite(pinRele, HIGH);
}
boolean isClearArea( ) {
  int count = 0;
  print("[INCIANDO SCAN]");
  for (int i = 1; i <= tentativasSonar; i++) {
    delay(1000);
    //------------------------------------------
    //Gambiarra para aumentar a precisao do sensor
    //------------------------------------------
    pinMode(ECHO_PIN, OUTPUT);
    digitalWrite(ECHO_PIN, LOW);
    pinMode(ECHO_PIN, INPUT);
    //------------------------------------------
    int ping = sonar.ping_cm();
    if ( ping == 0) {
      count++;
    }
    String ok = "";
    if (ping == 0) {
      ok = "NAO";
    } else {
      ok = "SIM";
    }
    print("SCAN[" + String(i) + "] SONAR[" + String(ping) + "CM] ALVO[" + ok + "]");
  }
  if (count == tentativasSonar) {
    print("[NEHUM MOVIMENTO DETECTADO]");
    return true;
  }
  return false;
}

void loop() {

  long delayseg = ciclo * 1000;
  print("AGUARDANDO [" + String(ciclo) + "] SEGUNDOS");
  delay(delayseg);
  boolean tentativa = true;
  while (tentativa) {
    if (isClearArea( )) {
      acionarSpray();
      tentativa = false;
    } else {
      if (standby > 0) {
        long delaystandby = standby * 1000;
        print("MOVIMENTO DETECTADO");
        print("ENTRANDO EM STANDBY POR [" + String(standby) + "] SEGUNDOS");
        delay(delaystandby);
      }
    }
  }
  delay(1000);
}

void acionarSpray( ) {
  print("============================");
  print(">>>>>>>>>LIGANDO<<<<<<<<<<<<");
  print("============================");
  digitalWrite(pinRele, LOW);
  delay(tempoSpray);
  digitalWrite(pinRele, HIGH);
  print("============================");
  print(">>>>>>>DESLIGANDO<<<<<<<<<<<");
  print("============================");

}

int sonarRead() {
  delay(500);
  return sonar.ping_cm();
}

void print(String msg) {
  if (debug) {
    Serial.print(msg);
    Serial.println();
  }
}

