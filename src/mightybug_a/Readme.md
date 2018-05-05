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

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

Use the following tags: Added, Changed, Fixed and Removed

## [Bat bolido: 1.1.0] - 05/05/2018

### Added
- FSM for car state: callibration, running and out of line detection
- commands for changing PID ctes, car state, target velocity of motors, etc.
- buzzer library. Changing musics regarding car state.
- measure battery voltage
- systick (buzzer, battery measure)
- serial communication

### Fixed
- Bug corrections in pid


## [Agonizing Automobile: 1.0.0] - 09/04/2018

### Added
- initial structure: sensor + pid + motor control
- white and black line (line follower)
- added basic pid	

	
## Known issues:
- car stops if it is out of line
- no encoders


