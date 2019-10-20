## Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

Use the following tags: Added, Changed, Fixed and Removed

## [Electronic Experience 1.4.0] - Unreleased

### Added

- adding PID for selecting target velocity during corners
- linear mapping of the circuit
- circular mapping of the circuit
- a docker container to build the project


### Changed
- using printf for embedded systems
- changed cli commands 

### Fixed
- full code refactor

### Removed
TODO

## [Disable Drive 1.3.0] - 24/02/2019

### Added
- Preliminary circuit mapping
- Turbo Pickle
- Using leds as feedback for state
- Adding Pitch for feedback 
- Accelerating/Braking in corners
- antiwheelie heuristics (limiting velocity during acceleration/brake)

### Changed
- Refactoring Code
- Using encoders for changing states (corner detection)

### Fixed
- Fixed CLI commands
- Delaying derivative in PID

### Removed
TODO

## [Candid Car 1.2.0] - 21/07/2018

### Added
- Added example for storing data in flash
- Added example of DMA usage
- Basic telemetry using blocking messages
- Keypad menu
- Different mappings of buzzers
- New optional functionality: stopping the car if all sensors detect a line
- Idle state (sending car to ilde state from running state)
- Resetting calibration from idle state

### Changed
- Pinout of encoders and motor PWMs
- PID and vel mappings (now they are performed jointly)

### Fixed
- Battery consumption of buzzer in idle mode
- PWM in timer1 of the STM32F1
- Bugs in the readings of sensors and pid
- Prints of cli messages

### Removed
- Disabling the option of moving backwards


## [Bat Bolido: 1.1.0] - 05/05/2018

### Added
- FSM for car state: calibration, running and out of line detection
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
