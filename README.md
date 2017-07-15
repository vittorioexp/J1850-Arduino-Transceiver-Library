# J1850-Arduino-Transceiver-Library
Arduino library which allow to communicate on J1850-PWM mode.
It works with wire, radio and laser transmissions.
You just have to choose your bit-rate (lower is better), tx and rx pins.
It doesn't have either a header nor a parity check. These functions could be implemented at a higher protocol layer.
This library has been tested with:
1. Wire connection between two Arduino
2. Radio connection (multipoint) with FSK modulation (and OOK too), module RTX-MID-868. Datasheet: https://goo.gl/UEv8ii
3. Laser connection (experimental circuit)

## 1. Wire connection between two Arduino (DEFAULT)
Arduino Uno (TRX)  |  Arduino Uno (TRX)  |    Notes  
-------------|-------------|------------
13 | 3 | Intelligent signal Tx
3 | 13 | Intelligent signal Rx
GND | GND | Common GND 

## 2.  Radio connection with RTX-MID-868 (DEFAULT)
Arduino Uno (TX)  |  RTX-MID-868 module  |    Notes  
-------------|-------------|------------
13 | 4 | Intelligent signal Tx
12 | 5 | Enable TX/RX
11 | 6 | Power Up/Down
3 | 9 | Intelligent signal Rx
+5V | 10 | +2,5V ÷ +6V 
GND | 2, 7 | Common GND 

#### Remember: 
In order to keep the transmission medium free (ether) after transmission, it is recommended to use the "setTxRxModePin" method (see example sketch "Advanced_TX"). The "mode_pin" is connected to the radio module and is used to choose the transmission directions (to select transmission or reception, see datasheet of RTX-MID-868 module https://goo.gl/UEv8ii).
