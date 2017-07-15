/*
	Created by:		Esposito Vittorio
	GitHub:			https://github.com/VittorioEsposito	
	
	J1850-Arduino-Transceiver-Library
*/

#include <Arduino.h>
#include "avr/interrupt.h"

#define BUFFER_SIZE			1526

class PwmTransceiver 
{
	
	static volatile bool flagDecoder;
	static volatile unsigned long timeOld;	
	
	private:
		uint8_t _RX_PIN;
		uint8_t _TX_PIN;
		uint8_t _EN_PIN;
		uint8_t _MODE_PIN;
		uint32_t _BITRATE;
		uint32_t _BIT_TIME;
		uint32_t _HALF_BIT_TIME;
		uint32_t _ONE_THIRD_BIT_TIME;
		uint32_t _TWO_THIRD_BIT_TIME;
		bool _high_speed = false;
		bool _logic = true;		// 1: Direct; 0: Inverse
		char _RX_BUFFER[BUFFER_SIZE];
		uint16_t _BUFFER_INDEX = 0;
		int _count = 7;
		
		uint16_t _timeOut = 100;		// mS	
		char _rx_char = 0x00;
		
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
		
		static void isrPwmDecoder();
		void receive();
		bool isReceiving();				
		bool available();
		String readString();
		
		byte CRC8(String str);
		byte CRC8(byte data[], byte len);

};
