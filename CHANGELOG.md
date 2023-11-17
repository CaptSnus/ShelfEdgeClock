# Change Log

* [V1.1.3](#v113)
* [V1.1.2](#v112)
* [V1.1.1](#v111)
* [V1.1.0](#v110)
* [V1.0.0](#v100)

## V1.1.3

### Enhancements<!-- omit from toc -->

* Updated some libraries
  * Adafruit Neopixel: 1.11.0 -> 1.12.0
  * Arduino HttpClient: 0.4.0 -> 0.5.0
* Replaced one library
  * AsyncElegantOTA -> ElegantOTA

### Bugfixes<!-- omit from toc -->

* Display mode will be reloaded after reboot or reset. The value was saved before but not updated on reload of the settings.

## V1.1.2

### Enhancements<!-- omit from toc -->

* Added scroll mode which changes every couple of seconds from time -> date -> temperature

### Bugfixes<!-- omit from toc -->

* Small tweaks here and there to make the experience more enjoyable.

## V1.1.1

After more hours of testing, I found a necessary improvement in the code and fixed some minor bugs.

### Enhancements<!-- omit from toc -->

* Brightness mapping editable (issue [#7](https://github.com/CaptSnus/ShelfEdgeClock/issues/7)): you can now change the _knx - min value_, _knx - max value_, _clock - min value_, _clock - max value_, _downlight - min value_ and _downlight - max value_.

### Bugfixes<!-- omit from toc -->

* Small tweaks here and there to make the experience more enjoyable.

## V1.1.0

### Enhancements<!-- omit from toc -->

* Hide color fields based on color mode (issue [#1](https://github.com/CaptSnus/ShelfEdgeClock/issues/1)): unused fields are hidden.
* suppress calls to resources before wifi connection (issue [#3](https://github.com/CaptSnus/ShelfEdgeClock/issues/3)): the respective funtions just `return;` now
* Color preview (issue [#8](https://github.com/CaptSnus/ShelfEdgeClock/issues/8)): you can now see a preview of the color code entered on the right side of the input field.

### Bugfixes<!-- omit from toc -->

* Color change (per minute) not working (issue [#6](https://github.com/CaptSnus/ShelfEdgeClock/issues/6)): changed the processing of setting and clearing the flag for a color change.

## V1.0.0

Initial upload of the code
