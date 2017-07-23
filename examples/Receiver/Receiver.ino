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
  pinMode(rx_pin, INPUT_PULLUP);
  Serial.println("PwmTransceiver - Receiver");
  Transceiver.begin(1200);           // Bits per second
}

void loop() {
  while (1)
  {
    while (Transceiver.isReceiving());
    if (Transceiver.available())
    {
      Serial.println(Transceiver.readString());
    }
    // Put non-blocking code here
  }
}
