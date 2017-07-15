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
const int en_pin = 11;  // To power up/down the module

PwmTransceiver Transceiver(rx_pin, tx_pin);

String str;

void setup() {
  Serial.begin(9600);
  Transceiver.begin(1200);              // 1200 bit per second
  Transceiver.setEnablePin(en_pin);     // Set power up/down pin
  Transceiver.powerDown();              // Power down module
}

void loop() {

  // Alternative way: send a string
  if (Serial.available() > 0) {
    Transceiver.powerUp();            	// Power up module
    delay(1000);                        // Radio-module is powering up
    str = Serial.readString();          // One second timeOut
    Transceiver.print(str);
    Serial.println(str);
    delay(1000);                        
    Transceiver.powerDown();          	// Power down module
  }

}
