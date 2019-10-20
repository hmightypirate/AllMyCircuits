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

The project can be flashed in a Blue/Black pill boards or in one of the mightybuga boards. All the mightybuga boards are compatible with the latest version of the software. To flash the software - TODO


## Cli Commands

TODO


## Relevant files - (Needs Update)

- setup.h: Microcontroller setup and pinout
- commons.h: basic configuration of the car behaviour
- motor.h: motor configuration (basically there is a flag when the motor velocities are inverted in the microncontroller)
- pid.h: pid configuration
- sensors.h: black/white sensor values can be configured here. The FOLLOW_BLACK_LINE flag can be used to configure the car to follow white lines (if it is set to 0)


## Changelog

You have the changelog of the project in this [link](./Changelog.md)




