# Shelf Edge Clock powered by ESP32

> The Shelf Edge Clock is heavily inspired by a post on social media showing the awesome project of [DIY-Machines](https://github.com/DIY-Machines/DigitalClockSmartShelving). After some consideration i decided to build the clock with the given 3D parts but adopt the software to an ESP32. sss

## Table of Contents

* [General Information](#general-information)
* [Features](#features)
* [Screenshots](#screenshots)
* [Setup](#setup)
  * [Hardware](#hardware)
  * [Software](#software)
* [Usage](#usage)
  * [Clock](#clock)
  * [Date](#date)
  * [Temperature](#temperature)
  * [Humidity](#humidity)
  * [Scoreboard](#scoreboard)
  * [Countdown](#countdown)
  * [Downlights](#downlights)
  * [there is more](#there-is-more)
* [Project Status](#project-status)
* [Room for Improvement](#room-for-improvement)
* [Technologies and Software used](#technologies-and-software-used)
  * [Libraries](#libraries)
* [Acknowledgements](#acknowledgements)
* [Contact](#contact)
* [License](#license)

## General Information

[![C/C++ CI](https://github.com/CaptSnus/Shelf-Edge-Clock/actions/workflows/c-cpp.yml/badge.svg?branch=main)](https://github.com/CaptSnus/Shelf-Edge-Clock/actions/workflows/c-cpp.yml)

The Shelf Edge Clock is powered by an ESP32 devkit and more than 250 neopixels. The clock itself is mostly 3D printed, some wood for a proper frame and minimal additional parts.

* folder `data` contains the files stored on the ESP32
* folder `docs`contains the documentation

## Features

* Powered by an ESP32 devkit out of the box.
* More than 250 neopixels across two strands.
* Webserver providing a GUI to control the clock.
* Webclient making GET request with your home automation (e.g. openHAB)

## Screenshots

![Example screenshot](./docs/images/shelfEdgeClock.png)

## Setup

Setup instructions will be completed after all stuff is completed.

### Hardware

1. Build a frame/background for your clock
2. Print all the required parts
3. Wire all the electronics and get the leds installed

### Software

1. Load the software to your ESP32
2. Setup all the functions
3. Get your self a drink and do nothing in the afternoon

## Usage

This shelf edge clock must be connected to your wifi network, otherwise it won't work. All functions rely on data acquired from the internet (e.g. NTP) or your local network (e.g. home automation/sensors)

### Clock

Display the current time whichs is updated and verified by calling an external NTP server.

* two clock formats
  * 24 hours: 00:00 to 23:59
  * 12 hours: 12:00 to 11:59 AM/PM
* two color options: user defined colors or random colors
* both color options use two colors: one for hours and one for minutes

### Date

Display the current date which is updated and verified by calling an external NTP server.

* two date formats
  * european style: 2903 -> march 29th
  * american style: 0329 -> march 29th
* two color options: defined colors or random colors
* both color options use two colors: one for days and one for months

### Temperature

Display the current outside temperature and regularly fetch the value from openHAB.

* requires a home automation system (e.g. openHAB or similar with a rest api)
* settings provide a field to enter a string for the get call
* display type is fixed to XXYS
  * XX -> degree figure
  * Y -> decimal figure
  * S - symbol
    * C -> abbreviation for celsius
    * F -> abbreviation for fahrenheit
* two color options: defined colors or random colors
* both color options use three colors: one for XX, one for Y and one for S

### Humidity

Display the current outside humidity and regularly fetch the value from openHAB.

* requires a home automation system (e.g. openHAB or similar with a rest api)
* settings provide a field to enter a string for the get call
* display type is fixed to XXYH
  * XX -> percentage figure
  * Y -> decimal figure
  * H -> abbreviation for humidity
* two color options: defined colors or random colors
* both color options use three colors: one for XX, one for Y and one for H

### Scoreboard

Display a scoreboard for table soccer or whatever game you're playing that requires a scoreboard. You can also show the latest score of your favorite team.

* display type is fixed to HHAA: HH -> home team, AA -> away team
* one color option: user defined colors
* the color option uses two colors: one for HH and one for AA

### Countdown

Display a countdown and flash the word "EndE" at the end

* display type is fixed
  * hhmm, if there is more than 60 minutes to countdown,
  * and switches to mmss, if the countdown is less than 60 minutes
* one color option: user defined colors
* the color option uses four colors: one for hours, one for minutes, one for seconds and one for the final 10 seconds

### Downlights

The 14 boxes created by the design of the clock are an ideal storage for small things. These items are highlighted by leds in the piece above.

* four color options
  * 1 user defined color
  * 14 user defined colors
  * 1 random color for all boxes
  * 14 random colors for each box

### there is more

* the brightness of the leds is either in auto mode, controlled by the brightness outside, or manual mode configured in the settings
* first connection to clock for configuration of the required wifi via access point generated by the ESP32 (see Setup above)
* the necessary data is stored in SPIFFS and therefore reducing the required parts
* integrated update mechanism for future updates and bugfixes

## Project Status

Project is: _in progress_! The new release is currently on our our prototype running 24/7 and all issues will be remediated before completion.

This README will receive continuous updates throughout the development.

## Room for Improvement

Planned improvements:

* add additional scroll mode: show clock, date, temperature in a perpetual cycle
* add buzzer for the countdown (it's very silent at the moment)
* maybe design a pcb as plug'n'play solution
* make website translation ready

To Do:

* update `index.html` in order to reduce the need to use the menu
* add check for an established wifi connection at certain functions

## Technologies and Software used

* Visual Studio Code (VSCode)
* PlatformIO - version 6.1.7
* Bootstrap - version 5.2.3
* jQuery - version 3.6.3

### Libraries

* Adafruit_NeoPixel Library by adafruit - version 1.11.0
* ArduinoHttpClient by Arduino - version 0.4.0
* AsyncElegantOTA by ayushsharma82 - version 2.2.7
* AsyncTCP by me-no-dev - version 1.1.1
* ESPAsyncWebServer by me-no-dev - version 1.2.3
* ESPmDNS by Hristo Gochkov - version 2.0.0
* NonBlockingRTTTL by Antoine Beauchamp - version 1.3.0
* Preferences by Hristo Gochkov - version 2.0.0
* SPIFFS by Hristo Gochkov - version 2.0.0
* WiFi by Hristo Gochkov - version 2.0.0

## Acknowledgements

* This project was inspired by the [Hidden Digital Clock Smart Shelving](https://github.com/DIY-Machines/DigitalClockSmartShelving) from [DIY-Machines](https://www.diymachines.co.uk).
* The code is created based on the excellent examples of [Random Nerd Tutorials](https://randomnerdtutorials.com).
* Many thanks to my family for the ongoing lovely support and encouragement, their wonderful ideas and help in creating this shelf edge clock.

## Contact

Created by [@CaptSnus](https://github.com/CaptSnus/) - feel free to contact me! Please report bugs, enhancements or feature request directly on github.

## License

This project is open source and available under the [... License](./LICENSE.md).
