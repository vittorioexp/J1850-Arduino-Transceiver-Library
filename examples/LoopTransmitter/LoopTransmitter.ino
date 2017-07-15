/*
  Created by:   Esposito Vittorio
  GitHub:       https://github.com/VittorioEsposito

  J1850-Arduino-Transceiver-Library
*/
#include <PwmTransceiver.h>

volatile bool PwmTransceiver::flagDecoder;
volatile unsigned long PwmTransceiver::timeOld;
const int rx_pin = 3;   // https://goo.gl/nHC95v
const int tx_pin = 13;

PwmTransceiver Transceiver(rx_pin, tx_pin);

void setup() {
  Serial.begin(2000000);
  Serial.println("PwmTransceiver");
  //Transceiver.setLogic(false);    // Inverse logic
  Transceiver.begin(1200);
}

void loop() {
  Serial.println("123456789012345678901234567890123456789012345678901234567890");
  Transceiver.println("123456789012345678901234567890123456789012345678901234567890");
  delay(1000);
}
