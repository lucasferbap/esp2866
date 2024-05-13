#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <SoftwareSerial.h>

const char* ssid = "Lucas Fernandes";
const char* password = "33463186";

WebSocketsServer webSocket = WebSocketsServer(80);

// Comunicação esp -> arduino
const int tx = 0; // GPIO0
const int rx = 2;// GPIO2
SoftwareSerial arduino(rx, tx);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
      }
      break;
    case WStype_TEXT:
      // Recebendo Dados do Cliente e enviando para o arduino 
      arduino.println((const char*)payload);

      break;
  }
}

void setup() {
  Serial.begin(115200);
  arduino.begin(115200);
 
  // Conectar ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");

  // Inicializar WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();

  // Verificar se há dados na porta serial
  while (Serial.available() > 0) {
    String serialData = Serial.readStringUntil('\n');
    webSocket.broadcastTXT(serialData);
  }
}
