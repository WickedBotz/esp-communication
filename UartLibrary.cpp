#include "UartLibrary.h"

UartLibrary::UartLibrary(int rxPin, int txPin) : rxPin(rxPin), txPin(txPin), uart(2) {}

void UartLibrary::setup() {
  Serial.begin(115200);
  uart.begin(115200, SERIAL_8N1, rxPin, txPin);
}

String UartLibrary::readMessage() {
  if (uart.available()) {
    String receivedData = uart.readString();
    return receivedData;
  }
}

void UartLibrary::writeMessage(const String& message) {
  uart.write(message.c_str());
}
