#ifndef UartLibrary_h
#define UartLibrary_h

#include <Arduino.h>
#include <HardwareSerial.h>

class UartLibrary {
public:
  UartLibrary(int rxPin, int txPin);
  
  void setup();
  String readMessage();
  void writeMessage(const String& message);

private:
  int rxPin;
  int txPin;
  HardwareSerial uart;
  String inputString;
};

#endif
