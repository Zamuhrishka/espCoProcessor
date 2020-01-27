# espCoProcessor
> Firmware for communicating with ESP8266 Wi-Fi module using AT commands.

espCoProcessor is a generic, platform independent, **bare metal** (do not use any RTOS)
library for communicating with ESP8266 Wi-Fi module using AT commands.
Module is written in C99 and is independent from used platform. Its main targets
are embedded system devices like ARM Cortex-M and others.

This firmware develop for host MCU device which drives ESP8266 devices
running official `AT commands` firmware written and maintained by Espressif System.
Source of official firmware is publicly available at
[esp32-at](https://github.com/espressif/esp-at) repository.


## Features

- Written in C language (C99)
- Supports latest ESP8266 RTOS-SDK based AT commands firmware
- Platform independent and very easy to port
- Available examples for STM32F407VG Discovery board.
- Almost all function (except for TCP / UDP communication) are blocking,
  but consists of timeouts.

## Code Structure
Path				        | Description
--------------------|---------------
examples            | Example applications available for all ports
include             | Header files
projects            | A range of example projects for TrueStudio IDE
src                 | Source files
tests               | Unit tests source files

## Third Party Libs

In projects use next third party libs:

* Super light lib for work with regular expression - [slre](https://github.com/cesanta/slre)
* Unit tests framework - [cmockery](https://github.com/google/cmockery)

## Supported Chipsets

- STM32F4

## Install

Choose your platform in port folder. If your platform not support then you may
add it support according to [instruction](https://github.com/Zamuhrishka/espCoProcessor/tree/develop/src/port/README.md).

## Running unit tests

Building and running the tests requires the following:

* install the cmockery framework (https://github.com/google/cmockery).
* integrate files in folder **tests** in your project and run it.

## Contribute

Contributions are always welcome. Simple instructions to proceed:

1. Fork Github repository
3. Create a pull request to develop branch with new features or bug fixes

## License

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the
use of this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications.
