# espCoProcessor
> Firmware for communicating with ESP8266 Wi-Fi module using AT commands.

espCoProcessor is a generic, platform independent, bare metal (do not use any RTOS)
library for communicating with ESP8266 Wi-Fi module using AT commands.
Module is written in C99 and is independent from used platform. Its main targets
are embedded system devices like ARM Cortex-M and others.

This firmware develop for host MCU device which drives ESP8266 devices
running official `AT commands` firmware written and maintained by Espressif System.
Source of official firmware is publicly available at
[esp32-at](https://github.com/espressif/esp-at) repository.


## Features

- Supports latest ESP8266 RTOS-SDK based AT commands firmware
- Platform independent and very easy to port
- Available examples for STM32F407VG Discovery board.
- Written in C language (C99)


## Code Structure

* `examples` contains a range of example source files.
* `include` contains header files.
* `projects` contains a range of example projects for TrueStudio IDE.
* `src` contains source files.
* `tests` unit tests source files.



## Third Party Libs

In projects use next third party libs:

* Super light lib for work with regular expression - [slre](https://github.com/cesanta/slre)
* Unit tests framework - [cmockery](https://github.com/google/cmockery)

## Contribute

Contributions are always welcome. Simple instructions to proceed:

1. Fork Github repository
3. Create a pull request to develop branch with new features or bug fixes


## Running unit tests

Building and running the tests requires the following:

* install the cmockery framework (https://github.com/google/cmockery).
* integrate files in folder **tests** in your project and run it.

## License

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the
use of this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications.
