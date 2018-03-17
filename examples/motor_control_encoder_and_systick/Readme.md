Control of two pololu motors with encoders.

Pinout:
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

