#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "seu_id";
const char* password = "sua_senha";
int visual = 2;
int sonoro = 5;
unsigned long last_count = 0;
int controle = 0;
String tp2, tim, Bt1, Bt2 = "0";

void setup() {
  pinMode(visual, OUTPUT);
  pinMode(sonoro, OUTPUT);
  Serial.begin(115200);
  delay(4000);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {  //Check the current connection status

    HTTPClient http;

    //http.begin("https://www.seuservidor.com.br"); //Specify the URL
    String url = "https://www.seuservidor.com.br/11___neighbor/status_acionamento.php";   
    http.begin(url);    
    int httpCode = http.GET();  //Make the request
    //Serial.prinln(millis());
    if (httpCode > 0)  //Check for the returning code
    {
      String payload = http.getString();
      String Bt1 = String(payload[0]);
      String Bt2 = String(payload[1]);
      String tp1 = String(payload[2]);
      tp2 = String(payload[3]);
      tim = String(tp1 + tp2);
      int temp = tim.toInt();
      //Serial.println(temp);
      temp = temp * 1000;
      


      if(controle == 1){
        if(millis() - last_count >= temp){   
          Serial.println("Entrou no millis");
          String url = "https://www.seuservidor.com.br/11___neighbor/acionamentos.php?b=1&a=0";
          digitalWrite(visual, LOW);
          http.begin(url);
          int httpCode = http.GET(); 
          url = "https://www.seuservidor.com.br/11___neighbor/acionamentos.php?b=2&a=0";
          digitalWrite(sonoro, LOW);
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
        digitalWrite(visual, HIGH);
        String url = "https://www.seuservidor.com.br/11___neighbor/acionamentos.php?s=1&a=1";
        http.begin(url);
        int httpCode = http.GET();  
        controle = 1;  
        Serial.println("comecou");    
      }

      if (Bt1 == "0") {
        digitalWrite(visual, LOW);
        String url = "https://www.seuservidor.com.br/11___neighbor/acionamentos.php?s=1&a=0";
        http.begin(url);
        int httpCode = http.GET();
        Serial.println("Bt1=0");
      }

      if (Bt2 == "1") {
        digitalWrite(sonoro, HIGH);
        String url = "https://www.seuservidor.com.br/11___neighbor/acionamentos.php?s=2&a=1";
        http.begin(url);
        int httpCode = http.GET();
        controle = 1;
      }

      if (Bt2 == "0") {
        digitalWrite(sonoro, LOW);
        String url = "https://www.seuservidor.com.br/11___neighbor/acionamentos.php?s=2&a=0";
        http.begin(url);
        int httpCode = http.GET();
        Serial.println("Bt2=0");
      }



    } else {
      Serial.println("Error on HTTP request");
    }
    //___________________________________________________________________
    

    http.end();  //Free the resources
  }

  delay(500);
}
