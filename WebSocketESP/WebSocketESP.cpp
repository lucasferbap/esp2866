#include "WebSocketESP.h"

WebSocketESP::WebSocketESP(const char* ssid, const char* password) : _ssid(ssid), _password(password) {}

void WebSocketESP::begin() {
  // Connect to WiFi
  WiFi.begin(_ssid, _password);

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  // You can get the IP address assigned to ESP

  _server.listen(80);
  Serial.print("Is the server live? ");
  Serial.println(_server.available());
}

void WebSocketESP::handleClient() {
  WebsocketsClient client = _server.accept();
  if(client.available()) {
    // Recebe dados do cliente
    WebsocketsMessage msg = client.readBlocking();
    Serial.print("Got Message: ");
    Serial.println(msg.data());

    // Enviar dados para o cliente
    if (Serial.available()) {
      String encodedData = base64::encode((uint8_t*)Serial.readString().c_str(), Serial.readString().data().length());
      client.send(encodedData);
    }
    

    // close the connection
    client.close();
  }
}

