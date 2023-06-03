#include <WiFi.h>
#include <WebSocketsServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "UartLibrary.h"

#define RX_PIN 16
#define TX_PIN 17

UartLibrary uartInstance(RX_PIN, TX_PIN);

const char* ssid = "a";
const char* password = "japa3211";
const int webSocketPort = 81;
const size_t JSON_BUFFER_SIZE = JSON_OBJECT_SIZE(9) + JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(2);
WebSocketsServer webSocket = WebSocketsServer(webSocketPort);

void JsonConvert() {
  
}
void postTimeKeeping() {
  HTTPClient http;
  // Cria um objeto JSON
  StaticJsonDocument<JSON_BUFFER_SIZE> doc;

  // Definir os valores no objeto JSON
  doc["TopSpeed"] = 0;
  doc["Threshold"] = 0;
  doc["InverseMaximum"] = 0;
  doc["ProportionalSpeed"] = 0;
  doc["Speed"] = 0;
  doc["BreakingTime"] = 0;
  doc["Score"] = 0;
  doc["SensorTelemetries"][0]["SensorValue"] = 0;
  doc["SensorTelemetries"][0]["SensorWeight"]["SensorWeight"] = 0;

  String jsonString;
  serializeJson(doc, jsonString);

  http.begin("https://zeri.azurewebsites.net/api/TimeKeeping");  // Substitua pela URL do servidor de destino
  http.addHeader("Content-Type", "application/json");


  Serial.println(jsonString);
  int httpResponseCode = http.POST(jsonString);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(response);
  } else {
    Serial.println("Erro na requisição.");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Desconectado!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Conectado de %d.%d.%d.%d\n", num, ip[0], ip[1],
                      ip[2], ip[3]);
      }
      break;
    case WStype_TEXT:{
        Serial.printf("[%u] Mensagem recebida: %s\n", num, payload);
        size_t payloadSize = length; // Usar o tamanho fornecido pelo parâmetro 'length'
        String payloadString(reinterpret_cast<const char*>(payload), payloadSize);
        Serial.print(payloadString);
        uartInstance.writeMessage(payloadString);
        webSocket.sendTXT(num, "Mensagem recebida com sucesso!");
      }
      break;
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conectado ao WiFi!");
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  delay(500);
  uartInstance.setup();
}

void loop() {
  webSocket.loop();
}
