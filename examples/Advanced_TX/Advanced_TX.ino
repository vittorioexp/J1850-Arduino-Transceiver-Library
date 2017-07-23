/*
  Created by:   Esposito Vittorio
  GitHub:       https://github.com/VittorioEsposito

  J1850-Arduino-Transceiver-Library
*/
#include <EnableInterrupt.h>
#include <PwmTransceiver.h>

const int rx_pin = 3;   // https://goo.gl/nHC95v
const int tx_pin = 13;

String str;
const int mode_pin = 12;
uint16_t count = 0;
uint8_t num = random(10, 20);

PwmTransceiver Transceiver(rx_pin, tx_pin);

void setup() {
  Serial.begin(9600);
  Serial.println("Transmitter");
  Transceiver.begin(1200);   // 1200 bit per second
  Transceiver.setTxRxModePin(mode_pin);
  Transceiver.enableRxMode();
}

void loop() {

  delay(500 * num);
  
  // Alternative way: send a string
  Transceiver.enableTxMode();           	  // Enable TX
  delay(800);                         	    // Load serial buffer
  str = "## ";							
  str += String(count++);
  str += ". Message from Arduino n.1 ;;";
  Transceiver.println(str);
  Serial.println(str);
  delay(800);
  Transceiver.enableRxMode();           	  // Disable TX (free the channel)

  
}
