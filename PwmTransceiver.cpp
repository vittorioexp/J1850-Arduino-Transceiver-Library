/*
	Created by:		Esposito Vittorio
	GitHub:			https://github.com/VittorioEsposito	
	
	J1850-PWM-Transceiver-Library
*/

#include <Arduino.h>
#include "avr/interrupt.h"
#include <PwmTransceiver.h>

PwmTransceiver::PwmTransceiver(uint8_t RX_PIN, uint8_t TX_PIN)
{
	_RX_PIN = RX_PIN;
	pinMode(_RX_PIN, INPUT);
	_TX_PIN = TX_PIN;
	pinMode(_TX_PIN, OUTPUT);
}

PwmTransceiver::PwmTransceiver(uint8_t RX_PIN, uint8_t TX_PIN, uint8_t MODE_PIN, uint8_t EN_PIN)
{
	_RX_PIN = RX_PIN;
	pinMode(_RX_PIN, INPUT);
	_TX_PIN = TX_PIN;
	pinMode(_TX_PIN, OUTPUT);
	_MODE_PIN = MODE_PIN;
	pinMode(_MODE_PIN, OUTPUT);
	_EN_PIN = EN_PIN;
	pinMode(_EN_PIN, OUTPUT);
}

void PwmTransceiver::setRxPin(uint8_t RX_PIN)
{
	_RX_PIN = RX_PIN;
	pinMode(_RX_PIN, INPUT);
}

void PwmTransceiver::setTxPin(uint8_t TX_PIN)
{
	_TX_PIN = TX_PIN;
	pinMode(_TX_PIN, OUTPUT);
}

void PwmTransceiver::setEnablePin(uint8_t EN_PIN)
{
	_EN_PIN = EN_PIN;
	pinMode(_EN_PIN, OUTPUT);
}

void PwmTransceiver::setTxRxModePin(uint8_t MODE_PIN)
{
	_MODE_PIN = MODE_PIN;
	pinMode(_MODE_PIN, OUTPUT);
}

void PwmTransceiver::setLogic(bool logic)
{
	_logic = logic;
}

uint8_t PwmTransceiver::getRxPin()
{
	return _RX_PIN;
}

uint8_t PwmTransceiver::getTxPin()
{
	return _TX_PIN;
}

void PwmTransceiver::begin(uint32_t BITRATE)
{
	_BITRATE  = BITRATE;
	if (_BITRATE > 15) {
		// Use microseconds
		_BIT_TIME  = 1000000 / _BITRATE;  // uS
		_high_speed = true;
	} else  {
		// Use milliseconds
		_BIT_TIME  = 1000 / _BITRATE;  	// mS
		_high_speed = false;
	}
	_HALF_BIT_TIME = _BIT_TIME / 2;
	_ONE_THIRD_BIT_TIME = _BIT_TIME / 3;
	_TWO_THIRD_BIT_TIME = _BIT_TIME * 2 / 3;
	if (_logic) attachInterrupt(digitalPinToInterrupt(_RX_PIN), isrPwmDecoder, RISING);
	else attachInterrupt(digitalPinToInterrupt(_RX_PIN), isrPwmDecoder, FALLING);
	flagDecoder = false;
}

void PwmTransceiver::powerDown()
{
	digitalWrite(_EN_PIN, LOW);
	delay(15);
}

void PwmTransceiver::powerUp()
{
	digitalWrite(_EN_PIN, HIGH);
	delay(15);
}

void PwmTransceiver::enableTxMode()
{
	digitalWrite(_MODE_PIN, HIGH);
	delayMicroseconds(500);
}

void PwmTransceiver::enableRxMode()
{
	digitalWrite(_MODE_PIN, LOW);
	delayMicroseconds(500);
}

void PwmTransceiver::println(String str)
{
	str += '\n';
	print(str);
}

void PwmTransceiver::print(String str)
{
	uint16_t size = str.length() + 1;
	char txt[size];
	str.toCharArray(txt, size);
	uint16_t i;
	for (i = 0; i < size; i++) 
	{
		PwmTransceiver::print(txt[i]);
	}
}

void PwmTransceiver::print(char txt[])
{
	uint16_t size = strlen(txt);
	uint16_t i;
	for (i = 0; i < size; i++) 
	{
		PwmTransceiver::print(txt[i]);
	}
}

void PwmTransceiver::print(char c)
{
	int i;
	for (i = 7; i >= 0; i--) 
	{
		PwmTransceiver::send((c >> i) % 2);
	}
}

void PwmTransceiver::print(uint8_t n)
{
	int i;
	for (i = 7; i >= 0; i--) 
	{
		PwmTransceiver::send((n >> i) % 2);
	}
}

void PwmTransceiver::send(bool b)
{
	if (!_logic) b = !b;
	if (_high_speed) 
	{
		if (b) 
		{
			/* Send 66% HIGH, 33% LOW  */
			digitalWrite(_TX_PIN, HIGH);
			delayMicroseconds(_TWO_THIRD_BIT_TIME);
			digitalWrite(_TX_PIN, LOW);
			delayMicroseconds(_ONE_THIRD_BIT_TIME);
		} 
		else 
		{
			/* Send 33% HIGH, 66% LOW  */
			digitalWrite(_TX_PIN, HIGH);
			delayMicroseconds(_ONE_THIRD_BIT_TIME);
			digitalWrite(_TX_PIN, LOW);
			delayMicroseconds(_TWO_THIRD_BIT_TIME);
		}
	} 
	else 
	{
		if (b) 
		{
			/* Send 66% HIGH, 33% LOW  */
			digitalWrite(_TX_PIN, HIGH);
			delay(_TWO_THIRD_BIT_TIME);
			digitalWrite(_TX_PIN, LOW);
			delay(_ONE_THIRD_BIT_TIME);
		} 
		else 
		{
			/* Send 33% HIGH, 66% LOW  */
			digitalWrite(_TX_PIN, HIGH);
			delay(_ONE_THIRD_BIT_TIME);
			digitalWrite(_TX_PIN, LOW);
			delay(_TWO_THIRD_BIT_TIME);
		}
	}
}

static void PwmTransceiver::isrPwmDecoder()
{
	timeOld = millis();
	flagDecoder = true;
}

void PwmTransceiver::receive()
{
	if (flagDecoder)
	{
		flagDecoder = false;	
		
		if (_high_speed) 
		{
			delayMicroseconds(_HALF_BIT_TIME);
		}
		else 
		{
			delay(_HALF_BIT_TIME);
		}
		
		//https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit

		if (_logic) 
		{
			_rx_char ^= (-digitalRead(_RX_PIN) ^ _rx_char) & (1 << _count);
		}
		else 
		{
			_rx_char ^= (-(!digitalRead(_RX_PIN)) ^ _rx_char) & (1 << _count);
		}
		
		if (_count > 0) 
		{
			_count--;
		}
		else 
		{
			_count = 7;
			_RX_BUFFER[_BUFFER_INDEX] = _rx_char;
			_BUFFER_INDEX++;
			_rx_char = 0x00;
		}
    }
}

bool PwmTransceiver::isReceiving()
{
	if (_BUFFER_INDEX > BUFFER_SIZE - 1)
	{
		// Buffer full
		_BUFFER_INDEX = BUFFER_SIZE - 1;
		return false;
	}
	// Check if millis has resetted
	if (millis() < timeOld) timeOld = millis();
	
	// Check the lost of synchronicity
	if (millis() > (timeOld + _timeOut))
	{
		_rx_char = 0x00;
		_count = 7;
		return false;
	}
	else
	{
		return true;
	}
}
			
bool PwmTransceiver::available()
{
	return (_BUFFER_INDEX > 0);
}

String PwmTransceiver::readString()
{
	String str = "";
	int i;
	for (i=0; i<_BUFFER_INDEX; i++)
	{
		str += _RX_BUFFER[i];
		_RX_BUFFER[i] = 0x00;
	}
	_BUFFER_INDEX = 0;
	return str;
}

byte PwmTransceiver::CRC8(String str)
{
  byte size = str.length() + 1;
  char txt[size];
  str.toCharArray(txt, size);
  return CRC8(txt, size - 1);
}

byte PwmTransceiver::CRC8(byte data[], byte len) 
{
  byte crc = 0x00;
  while (len--) {
    byte extract = *data++;
    for (byte tempI = 8; tempI; tempI--) {
      byte sum = (crc ^ extract) & 0x01;
      crc >>= 1;
      if (sum) {
        crc ^= 0x8C;
      }
      extract >>= 1;
    }
  }
  return crc;
}
