#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 

#include "settings.h"


int numMails = 0;
int prevMails = 0;
bool alert = false;
 
WiFiClient client ;

void tick() {
  digitalWrite(INDLED, !digitalRead(INDLED));
}

void readAnswer() {
  char c;  
  while (client.available()){
    c = client.read();
    Serial.print(c);
  };
}

void setup() {
  Serial.begin(115200);

  pinMode(INDLED, OUTPUT);
  pinMode(RELAY, OUTPUT);

  digitalWrite(INDLED, HIGH);

  WiFiManager wifiManager;
  //wifiManager.resetSettings();
  wifiManager.autoConnect("MAILALERT");
  
  Serial.println("Wifi connected)");

  digitalWrite(INDLED, LOW);
}

void signal(bool alert) {
  Serial.print("Relay: ") ;
  if (alert) {
    Serial.println("on") ;
  } else {
    Serial.println("off");
  }

  digitalWrite(RELAY, alert) ;
}

void cycle(bool alert) {
   if (alert) {
     for (int i=0; i <= 120; i++) {
       tick();
       delay(500) ;
     }
   } else {
     for (int i=0; i <= 60; i++) {
       tick() ;
       delay(1000);
     }
   }
}

void loop() {
  Serial.begin(115200);

  if (!client.connect(server, port)) {
    Serial.println("connection mail server failed");
  } else {
    Serial.println("connected mail server");

      client.print("USER ");
      client.println(user);
      delay(200);
      readAnswer();

      client.print("PASS ");
      client.println(password);
      delay(500);
      readAnswer();

      client.println("STAT");
      delay(500);
    
      if (client.find("+OK")) {
        numMails = client.parseInt();
        Serial.print(numMails);
        Serial.println(" mails in inbox");
      }

      client.println("QUIT");

      alert = numMails > prevMails ;
      signal(alert) ;
      cycle(alert) ;
  }

}

