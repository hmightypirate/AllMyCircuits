# Introduction

Control of two pololu motors with their respective encoders

The encoder ticks are read periodically using the Systick timer.

* Timer 2 is used to count the number of ticks of the left encoder
* Timer 3 is used to count the number of ticks of the right encoder (TODO)
* Timer 4 is used for the PWM of the two motors: CH3 for the left motor and CH4 for the right motor

## Pinout (FIXME: check and complete)
PC13 -> Internal LED.
PB08 and PB09 -> Timer 4 PWM CH3 (left motor) and CH4 (right motor). 
PB12 and PB13 -> Motor driver control pins for left motor.
   L        L  -> brake low
   H        L  -> forward
   L        H  -> reverse
Encoders:
PA15 -> Out A (left motor encoder)
PB3 -> Out B (left motor encoder)

PA09 and PB10 -> Serial port Tx/Rx.

