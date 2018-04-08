Example of a simple jukebox using a buzzer controller. Plays a list of melodies.

The playlist file can be made using https://github.com/punkto/RTTTL_python/blob/master/rtttl_dir_to_c_format.py

Pinout:
PC13 -> Internal LED.
PB04 -> Timer 3 PWM CH1 alternative pin (buzzer in).
PA09 and PB10 -> Serial port Tx/Rx.
