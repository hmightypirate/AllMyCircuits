# Intro
The idea is compile some tests using Unity unit test framework for C
(http://www.throwtheswitch.org/unity) and run them on quemu for STM32 or the
MCU, as the output is performed on UART1.

Quemu maps the serial port to a telnet server, so you need to connect to quemu
to read the tests status.

In the example the test is performed in a loop so, if you miss the test output
you only have to wait.

The unit test binary can also be programed into a blue pill and see the output
in the serial port. The serial port configuration is placed in
tests/lib/uart_putchar. Unity is placed in tests/lib/Unity.


# How to run the tests on quemu:
AllMyCircuits $ make build_quemu_for_stm32_image
AllMyCircuits $ make unit_tests_build
AllMyCircuits $ make unit_tests_run

In other terminal:
telnet localhost 5900

CTRL + C in the quemu terminal to finish

# References
 - quemu for stm32 code https://github.com/beckus/qemu_stm32
 - This is a very good article regarding unit testing for the stm32
https://medium.com/@ly.lee/stm32-blue-pill-unit-testing-with-qemu-blue-pill-emulator-9d88002a68b2
