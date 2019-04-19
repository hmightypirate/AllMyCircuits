Example of use of the i2c interface as well as a VL6180 time of flight range finder.


Bluepill Pinout:
PC13 -> Internal LED.
PA09 and PB10 -> Serial port Tx/Rx.
PB06 and PB07 -> i2c SCL/SDA (both of them pulled up with 2.2k resistors)

VL6180 Pinout:
I01 no connected
I00  ->  3.3V from FTDI (needed to turn on the VL6180)
SCL  ->  Bluepill PB06 (and a 2.2k pull up resistor)
SDA  ->  Bluepill PB07 (and a 2.2k pull up resistor)
2.8v ->  3.3V from FTDI
GND  ->  GND
