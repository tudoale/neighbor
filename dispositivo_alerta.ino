#include <WiFi.h>      //ESP32 Core WiFi Library  
#include <HTTPClient.h>
#include <EEPROM.h> // Include The EEPROM.h Library To Read & Write To The FLASH Memory
#define EEPROM_SIZE 2

//needed for library
#include <DNSServer.h> //Local DNS Server used for redirecting all requests to the configuration portal ( https://github.com/zhouhan0126/DNSServer---esp32 )
#include <WebServer.h> //Local WebServer used to serve the configuration portal ( https://github.com/zhouhan0126/WebServer-esp32 )
#include <WiFiManager.h>   // WiFi Configuration Magic ( https://github.com/zhouhan0126/WIFIMANAGER-ESP32 ) >> https://github.com/tzapu/WiFiManager (ORIGINAL)

// Include required libraries
#include "Arduino.h"
//#include "WiFi.h"


//flag para indicar se foi salva uma nova configuração de rede
bool shouldSaveConfig = false;

//pino do botão
const int PIN_AP = 2;
const int PIN_GIROFLEX = 5;
const int sonoro = 25;
const int TROCA = 4;
unsigned long last_count = 0;
int tempo_1 = 0;
int eeprom_address = 0;
int controle = 0;
int temp=0;
int eeprom_address_id = 1;
String tp1, tp2, tim, Bt1, Bt2, cmd = "0";
String id1, id2, id_esp32_c, id_esp32;


void setup() {
  Serial.begin(9600);
  pinMode(PIN_AP, INPUT);
  pinMode(PIN_GIROFLEX, OUTPUT);
  //declaração do objeto wifiManager
  WiFiManager wifiManager;

  //utilizando esse comando, as configurações são apagadas da memória
  //caso tiver salvo alguma rede para conectar automaticamente, ela é apagada.
 wifiManager.resetSettings();

//por padrão as mensagens de Debug vão aparecer no monitor serial, caso queira desabilitá-la
//utilize o comando setDebugOutput(false);
//  wifiManager.setDebugOutput(false);

//caso queira iniciar o Portal para se conectar a uma rede toda vez, sem tentar conectar 
//a uma rede salva anteriormente, use o startConfigPortal em vez do autoConnect
//  wifiManager.startConfigPortal(char const *apName, char const *apPassword = NULL);

  //setar IP fixo para o ESP (deve-se setar antes do autoConnect)
//  setAPStaticIPConfig(ip, gateway, subnet);
//  wifiManager.setAPStaticIPConfig(IPAddress(192,168,16,2), IPAddress(192,168,16,1), IPAddress(255,255,255,0)); //modo AP

//  setSTAStaticIPConfig(ip, gateway, subnet);
//  wifiManager.setSTAStaticIPConfig(IPAddress(192,168,0,99), IPAddress(192,168,0,1), IPAddress(255,255,255,0)); //modo estação

//callback para quando entra em modo de configuração AP
  wifiManager.setAPCallback(configModeCallback); 
//callback para quando se conecta em uma rede, ou seja, quando passa a trabalhar em modo estação
  wifiManager.setSaveConfigCallback(saveConfigCallback); 

  wifiManager.autoConnect("ESP_AP","12345678"); //cria uma rede sem senha
//wifiManager.autoConnect(); //gera automaticamente o SSID com o chip ID do ESP e sem senha

//  wifiManager.setMinimumSignalQuality(10); // % minima para ele mostrar no SCAN

//wifiManager.setRemoveDuplicateAPs(false); //remover redes duplicadas (SSID iguais)

//wifiManager.setConfigPortalTimeout(10); //timeout para o ESP nao ficar esperando para ser configurado para sempre
EEPROM.begin(EEPROM_SIZE);
//EEPROM.write(1, 99);
//EEPROM.write(0, 21);
//EEPROM.commit();

}



void loop() {
  WiFiManager wifiManager;
  //se o botão foi pressionado
   if ( digitalRead(PIN_AP) == HIGH ) {      
      Serial.println("resetar"); //tenta abrir o portal
      if(!wifiManager.startConfigPortal("ESP_AP", "12345678") ){
        Serial.println("Falha ao conectar");
        delay(2000);
        ESP.restart();
        delay(1000);
      }
      Serial.println("Conectou ESP_AP!!!");
   }
  tempo_1 = EEPROM.read(eeprom_address);
  id_esp32 = EEPROM.read(eeprom_address_id);
   
HTTPClient http;
String url = "https://www.gabd.com.br/tudoale/10___neighbor/status_acionamento.php";
http.begin(url);
int httpCode = http.GET();                                        //Make the request
  
    if (httpCode > 0) { //Check for the returning code
  
      String payload = http.getString();
           
      Bt1 = String(payload[0]); // Botao 1
      Bt2 = String(payload[1]); // Botao 2
      cmd = String(payload[2]); // Comando 
      tp1 = String(payload[3]); // valor 1 do comando
      tp2 = String(payload[4]); // valor 2 do comando
      id1 = String(payload[5]); // id para receber o comando
      id2 = String(payload[6]); // id para receber o comando
      
      id_esp32_c = String(id1 + id2);  // junta os dos valores do comando 
      Serial.println(id_esp32_c); 
      Serial.println(id_esp32);     
      tim = String(id_esp32);
      id_esp32 = tim;
      tim = String(tp1 + tp2);  // junta os dos valores do comando
      temp = tim.toInt();       // Converte para valor inteiro      

      if(cmd == "1"){ // Atualiza o time
        Serial.println("Comando recebido:");
        Serial.println(payload);
        EEPROM.write(0, temp);
        EEPROM.commit();
      }

      if(cmd == "9"){ // Modifica o ID do ESP32
        if(EEPROM.read(1) == 99){
          Serial.println("Comando recebido:");
          Serial.println(cmd);
          EEPROM.write(1, temp);
          EEPROM.commit();
        }        
      }

      if(id_esp32_c == id_esp32) {
        if(tp1 == "1") {
          digitalWrite(PIN_GIROFLEX, HIGH);
          Serial.println("Giroflex Ligado");
        } else {digitalWrite(PIN_GIROFLEX, LOW);}
      }
        
          //Serial.println(payload);
        
        //audio.connecttohost(payload);
    }
    String url2 = "https://www.gabd.com.br/tudoale/10___neighbor/acionamentos.php";
    //String junta2 = url2 + temp2 ;
    http.begin(url2);
    http.end(); //Free the resources   
    Serial.println("Lendo a Memoria: ");
    Serial.print("Valor tempo: ");
    Serial.println(tempo_1 );
    Serial.print("ID do ESP: ");
    Serial.println(id_esp32); 

    if(controle == 1){
        if(millis() - last_count >= temp){   
          Serial.println("Entrou no millis");
          String url = "https://gabd.com.br/tudoale/10___neighbor/acionamentos.php?b=1&a=0&id=1";
          digitalWrite(PIN_GIROFLEX, LOW);
          http.begin(url);
          int httpCode = http.GET(); 
          url = "https://gabd.com.br/tudoale/10___neighbor/acionamentos.php?b=2&a=0&id=1";
          digitalWrite(PIN_GIROFLEX, LOW);
          http.begin(url);        
          httpCode = http.GET(); 
          last_count = millis();
          controle = 0;
          Bt1 = "0";
          Bt2 = "0";
          //delay(500);
        }
      }
      
      
      if (Bt1 == "1") {
        digitalWrite(PIN_GIROFLEX, HIGH);
        String url = "https://gabd.com.br/tudoale/10___neighbor/acionamentos.php?s=1&a=1&id=1";
        http.begin(url);
        int httpCode = http.GET();  
        controle = 1;  
        Serial.println("comecou");    
      }

      if (Bt1 == "0") {
        digitalWrite(PIN_GIROFLEX, LOW);
        String url = "https://gabd.com.br/tudoale/10___neighbor/acionamentos.php?s=1&a=0&id=1";
        http.begin(url);
        int httpCode = http.GET();
        Serial.println("Bt1=0");
      }

      if (Bt2 == "1") {
        digitalWrite(sonoro, HIGH);
        String url = "https://gabd.com.br/tudoale/10___neighbor/acionamentos.php?s=2&a=1&id=1";
        http.begin(url);
        int httpCode = http.GET();
        controle = 1;
      }

      if (Bt2 == "0") {
        digitalWrite(sonoro, LOW);
        String url = "https://gabd.com.br/tudoale/10___neighbor/acionamentos.php?s=2&a=0&id=1";
        http.begin(url);
        int httpCode = http.GET();
        Serial.println("Bt2=0");
      }
    delay(200);
}  // Fim VOID LOOP


//callback que indica que o ESP entrou no modo AP
void configModeCallback (WiFiManager *myWiFiManager) {  
//  Serial.println("Entered config mode");
  Serial.println("Entrou no modo de configuração");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
  Serial.println(myWiFiManager->getConfigPortalSSID()); //imprime o SSID criado da rede

}

//callback que indica que salvamos uma nova rede para se conectar (modo estação)
void saveConfigCallback () {
//  Serial.println("Should save config");
  Serial.println("Configuração salva");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
}


