# Introduction

This subproject contains the code for a line follower car using the (bluepill/blackpill) microcontroller.

The car uses a PID controller to adapt the velocity of the motors to the current position of the car in the line.

## Relevant files

- setup.h: Microcontroller setup and pinout
- commons.h: basic configuration of the car behaviour
- motor.h: motor configuration (basically there is a flag when the motor velocities are inverted in the microncontroller)
- pid.h: pid configuration
- sensors.h: black/white sensor values can be configured here. The FOLLOW_BLACK_LINE flag can be used to configure the car to follow white lines (if it is set to 0)

## Changelog
c
See Changelog.md

## Known issues:

** initial loop: sensor + pid + motor control
** white and black line (line follower)
** car stops if it is out of line
** no parameter adaptation
** no serial communication
** no interruptions
** no encoders
** no systick


