/*
	Created by:		Esposito Vittorio
	GitHub:			https://github.com/VittorioEsposito	
	
	J1850-PWM-Transceiver-Library
*/

#include <PwmTransceiver.h>

volatile uint8_t _TX_PIN;
volatile uint8_t _RX_PIN;
volatile uint8_t _EN_PIN;
volatile uint8_t _MODE_PIN;
volatile uint32_t _BITRATE;
volatile uint32_t _BIT_TIME;
volatile uint32_t _HALF_BIT_TIME;
volatile uint32_t _ONE_THIRD_BIT_TIME;
volatile uint32_t _TWO_THIRD_BIT_TIME;
volatile bool _high_speed = false;
volatile char _RX_BUFFER[BUFFER_SIZE];
volatile uint16_t _BUFFER_INDEX = 0;
volatile uint32_t _timeOut = 500;		// uS	
volatile unsigned long timeOldMicros;
volatile unsigned long timeOldMillis = 0;
volatile int _count = 7;
volatile char _rx_char = 0x00;
volatile bool _logic = true;		// 1: Direct; 0: Inverse

void isrPwmDecoder()
{
	if (digitalReadFast(_RX_PIN)==_logic)
	{
		timeOldMicros = micros();
		timeOldMillis = millis();
	}
	else 
	{
		bool bit = micros() > timeOldMicros + _HALF_BIT_TIME;
		_rx_char ^= (-bit ^ _rx_char) & (1 << _count);
		//Serial.print(bit);
		if (_count > 0) _count--;
		else  {
			_count = 7;
			_RX_BUFFER[_BUFFER_INDEX] = _rx_char;
			_BUFFER_INDEX++;
			_rx_char = 0x00;
		}
	}
}

/*
void isrPwmDecoder()
{
	timeOldMicros = micros();
	while (digitalReadFast(_RX_PIN) == _logic);
	bool bit = (micros() - timeOldMicros) > _HALF_BIT_TIME;
	_rx_char ^= (-bit ^ _rx_char) & (1 << _count);
	if (_count > 0) _count--;
	else  {
		_count = 7;
		_RX_BUFFER[_BUFFER_INDEX] = _rx_char;
		_BUFFER_INDEX++;
		_rx_char = 0x00;
	}
	timeOldMillis = millis();
}
*/
PwmTransceiver::PwmTransceiver(uint8_t RX_PIN, uint8_t TX_PIN)
{
	_RX_PIN = RX_PIN;
	pinModeFast(_RX_PIN, INPUT);
	_TX_PIN = TX_PIN;
	pinModeFast(_TX_PIN, OUTPUT);
}

PwmTransceiver::PwmTransceiver(uint8_t RX_PIN, uint8_t TX_PIN, uint8_t MODE_PIN, uint8_t EN_PIN)
{
	_RX_PIN = RX_PIN;
	pinModeFast(_RX_PIN, INPUT);
	_TX_PIN = TX_PIN;
	pinModeFast(_TX_PIN, OUTPUT);
	_MODE_PIN = MODE_PIN;
	pinModeFast(_MODE_PIN, OUTPUT);
	_EN_PIN = EN_PIN;
	pinModeFast(_EN_PIN, OUTPUT);
}

void PwmTransceiver::setRxPin(uint8_t RX_PIN)
{
	_RX_PIN = RX_PIN;
	pinModeFast(_RX_PIN, INPUT);
}

void PwmTransceiver::setTxPin(uint8_t TX_PIN)
{
	_TX_PIN = TX_PIN;
	pinModeFast(_TX_PIN, OUTPUT);
}

void PwmTransceiver::setEnablePin(uint8_t EN_PIN)
{
	_EN_PIN = EN_PIN;
	pinModeFast(_EN_PIN, OUTPUT);
}

void PwmTransceiver::setTxRxModePin(uint8_t MODE_PIN)
{
	_MODE_PIN = MODE_PIN;
	pinModeFast(_MODE_PIN, OUTPUT);
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
	enableInterrupt(_RX_PIN, isrPwmDecoder, CHANGE);
	//if (_logic) enableInterrupt(_RX_PIN, isrPwmDecoder, RISING);
	//else enableInterrupt(_RX_PIN, isrPwmDecoder, FALLING);
}

void PwmTransceiver::powerDown()
{
	digitalWriteFast(_EN_PIN, LOW);
	delay(15);
}

void PwmTransceiver::powerUp()
{
	digitalWriteFast(_EN_PIN, HIGH);
	delay(15);
}

void PwmTransceiver::enableTxMode()
{
	digitalWriteFast(_MODE_PIN, HIGH);
	delayMicroseconds(500);
}

void PwmTransceiver::enableRxMode()
{
	digitalWriteFast(_MODE_PIN, LOW);
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
	for (i = 0; i < size - 1; i++) 
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
			digitalWriteFast(_TX_PIN, HIGH);
			delayMicroseconds(_TWO_THIRD_BIT_TIME);
			digitalWriteFast(_TX_PIN, LOW);
			delayMicroseconds(_ONE_THIRD_BIT_TIME);
		} 
		else 
		{
			/* Send 33% HIGH, 66% LOW  */
			digitalWriteFast(_TX_PIN, HIGH);
			delayMicroseconds(_ONE_THIRD_BIT_TIME);
			digitalWriteFast(_TX_PIN, LOW);
			delayMicroseconds(_TWO_THIRD_BIT_TIME);
		}
	} 
	else 
	{
		if (b) 
		{
			/* Send 66% HIGH, 33% LOW  */
			digitalWriteFast(_TX_PIN, HIGH);
			delay(_TWO_THIRD_BIT_TIME);
			digitalWriteFast(_TX_PIN, LOW);
			delay(_ONE_THIRD_BIT_TIME);
		} 
		else 
		{
			/* Send 33% HIGH, 66% LOW  */
			digitalWriteFast(_TX_PIN, HIGH);
			delay(_ONE_THIRD_BIT_TIME);
			digitalWriteFast(_TX_PIN, LOW);
			delay(_TWO_THIRD_BIT_TIME);
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
	
	// Check the lost of synchronicity
	
	if (millis() > timeOldMillis + _timeOut)
	{
		_rx_char = 0x00;
		_count = 7;
		return false;
	}
	
	return true;
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
