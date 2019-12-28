# Introduction

Configures USART1 as a serial port to receive only one digit (from 0 to 9). The internal LED reacts blinking faster when the digit is lower.


## Compilation

With the Blue Pill

```
make clean && make all
```

With the Black Pill

```
make clean && PILL=BLACK make all
```

## Known Issues

The black pill needs that both jumpers are in 0 position and it is reseted in order to work properly. This is not necessary with the Blue Pill
