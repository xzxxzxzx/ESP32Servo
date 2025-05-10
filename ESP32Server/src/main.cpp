#include <WiFi.h>
#include <WebServer.h>

#define LED_PIN 2

const char *ssid = "Galaxy";
const char *password = "12344321";

WebServer server(80);

void hR();
void switchLED();

bool state = false;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, false);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  Serial.println(WiFi.localIP());

  server.on("/", hR);
  server.on("/led", switchLED);
  server.onNotFound([]() {
    server.send(404, "text/plain", "404: Not found");
  });

  server.begin();
  Serial.println("Server started!");
}

void loop()
{
  server.handleClient();
}

void hR()
{
  String html = "<html>\
  <head><title>ESP32 LED</title><meta charset='UTF-8'></head>\
  <body>\
  <p>Светодиод сейчас: " +
                String(state ? "ВКЛ" : "ВЫКЛ") + "</p>\
  <a href=\"/led?state=on\"><button>LED ON</button></a>\
  <a href=\"/led?state=off\"><button>LED OFF</button></a>\
  </body>\
  </html>";

  server.send(200, "text/html", html);
}

void switchLED()
{
  if (server.hasArg("state"))
  {
    String value = server.arg("state");
    if (value == "on")
    {
      digitalWrite(LED_PIN, true);
      state = true;
    }
    else if (value == "off")
    {
      digitalWrite(LED_PIN, false);
      state = false;
    }
  }
  server.sendHeader("Location", "/");
  server.send(303);
}
