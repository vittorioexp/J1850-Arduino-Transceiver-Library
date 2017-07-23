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
  Serial.println("PwmTransceiver");
  Transceiver.begin(1200);
}

void loop() {
  while (Transceiver.isReceiving());
  if (Transceiver.available()) {
    String str = Transceiver.readString();
    if (str.indexOf("##") != (-1) && str.indexOf(";;") != (-1)) {
      uint8_t indexOne = str.indexOf("##");
      uint8_t indexTwo = str.indexOf(";;");
      String msg = str.substring(indexOne + 2, indexTwo);
      Serial.println(msg);
    }
  }
}
