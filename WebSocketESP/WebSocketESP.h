#ifndef WebSocketESP_h
#define WebSocketESP_h

#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <base64.h>

using namespace websockets;

class WebSocketESP {
public:
  WebSocketESP(const char* ssid, const char* password);
  void begin();
  void handleClient();

private:
  const char* _ssid;
  const char* _password;
  WebsocketsServer _server;
};

#endif
