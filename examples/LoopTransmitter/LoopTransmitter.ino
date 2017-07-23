/*
  Created by:   Esposito Vittorio
  GitHub:       https://github.com/VittorioEsposito

  J1850-Arduino-Transceiver-Library
*/
#define EI_ARDUINO_INTERRUPTED_PIN
#include <EnableInterrupt.h>
#include <PwmTransceiver.h>

const int rx_pin = 3;   // https://goo.gl/nHC95v
const int tx_pin = 13;

PwmTransceiver Transceiver(rx_pin, tx_pin);

void setup() {
  Serial.begin(2000000);
  Serial.println("PwmTransceiver - LoopTransmitter");
  //Transceiver.setLogic(false);    // Inverse logic
  Transceiver.begin(1200);         // Bits per second
  delay(5000);                      // Receiver needs to prepare
}

void loop() {
  String str = "Hello, world!";
  str += Transceiver.CRC8(str);
  Serial.println(str);
  Transceiver.print(str);
  delay(2000);
}
