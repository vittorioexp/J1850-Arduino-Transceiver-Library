/*
	Created by:		Esposito Vittorio
	GitHub:			https://github.com/VittorioEsposito	
	
	J1850-Arduino-Transceiver-Library
*/

#define LIBCALL_ENABLEINTERRUPT
#define EI_ARDUINO_INTERRUPTED_PIN
#include <stdint.h>
#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <digitalWriteFast.h>
#include <EnableInterrupt.h>
#define BUFFER_SIZE			64

class PwmTransceiver 
{
		
	public:
		PwmTransceiver(uint8_t RX_PIN, uint8_t TX_PIN);
		PwmTransceiver(uint8_t RX_PIN, uint8_t TX_PIN, uint8_t MODE_PIN, uint8_t EN_PIN);
		void setRxPin(uint8_t RX_PIN);
		void setTxPin(uint8_t TX_PIN);
		void setEnablePin(uint8_t EN_PIN);
		void setTxRxModePin(uint8_t MODE_PIN);
		void setLogic(bool logic);
		uint8_t getRxPin();
		uint8_t getTxPin();
		
		void begin(uint32_t BITRATE);
		void powerDown();
		void powerUp();
		void enableTxMode();
		void enableRxMode();
		
		void println(String str);
		void print(String str);
		void print(char txt[]);
		void print(char c);
		void print(uint8_t n);
		void send (bool b);
		
		bool isReceiving();				
		bool available();
		String readString();
		
		byte CRC8(String str);
		byte CRC8(byte data[], byte len);

};
