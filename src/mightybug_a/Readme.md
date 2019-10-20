# Introduction

This subproject contains the code for a line follower car using the (bluepill/blackpill) microcontroller STM32F103C8T6. The code is compatible with the mightybug-a boards that you can find in this [link](https://github.com/pcounhago/mightybuga-board) which have everything you need to get the line follower up and running. You can find more information of the concrete robots in the [wiki of the project](https://github.com/hmightypirate/AllMyCircuits/wiki/)

The software implements, among other things, a PID controller to adapt the velocity of the motors to the current position of the car in the line.

Wiki contains some troubleshooting issues so check it if you run into some problems with the configurations of your car.


## Bootstrapping the project

### Cloning the project

Initialise submodules as the first step if you did not do it (in the root of the project)

```
git submodule init
git submodule update
```

### Using Make

To compile the project with make you need to install the following dependencies:

* gcc-arm-none-eabi

To compile the project for the mightybuga boards or blue pill just type:

```
make clean && make all
```

Note: if you are compiling for the black pill board use this command instead:

```
make clean && TARGET=BLACK make all
```

If everything goes ok you will find the .bin file in the bin folder:

```
bin/electronic_experience.bin

```
electronic_experience was the version of the software at the moment this manual was made. Name is updated with every version. Thus check the "bin/" folder to get the real .bin file.

### Docker

You can use the Dockerfile included in the root of the project to compile the code. We highly recommend to use the dockerfile as the arm-none-eabi version works with the current software.

The additional dependency if you are using this option is "docker".

In the root of the project type:

```
docker build
```

and run the container with

```
docker run
```

TODO - Instructions 

## Flashing the project

The project can be flashed in a Blue/Black pill boards or in one of the mightybuga boards. All the mightybuga boards are compatible with the latest version of the software. To flash the software



## Cli Commands


## Relevant files - (Needs Update)

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

	
## Known issues:
- car stops if it is out of line
- no encoders


