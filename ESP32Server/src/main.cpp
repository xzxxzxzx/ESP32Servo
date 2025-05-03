#include <Arduino.h>
#include <Wifi.h>
#include <WebServer.h>

#define LED_PIN 2

const char* ssid = "Redmi 9C NFC";
const char* password = "stak_64_64";

WebServer server (80);

void hR();
void switchLED(bool status);

bool state = false; 

void setup() {
Serial.begin(115200);
WiFi.begin(ssid, password);

Serial.print("Connection to WiFi");
while (WiFi.status() !=WL_CONNECTED){
  delay(250);
  Serial.print(".");
}
Serial.println("WiFi Connected");
Serial.println(WiFi.localIP());

server.on("/", hR);
server.on("/on", switchLED(true));
server.on("/on", switchLED(false));
server.begin();
Serial.println("Start Server!");
}

void loop() {
server.handleClient();
}

void hR(){
  String html ="<html>\
  <head><title> ESP 32 LED</head></title>\
  <body>\
  <p> Светодиод сейчас: "+ String(state ? "ВКЛ" : "ВЫКЛ") + " </p>\
  <a herf= \"/on\"><button>LED ON</button></a>\
  <a herf= \"/off\"><button>LED OFF</button></a>\
  </body>\
  </html>";

  server.send(200, "text/html", html);
}

void switchLED(bool status){
  digitalWrite(LED_PIN, status);
  state = status;
}