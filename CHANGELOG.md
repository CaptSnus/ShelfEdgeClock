# Change Log

## V1.1.1

### Enhancements

* Brightness mapping editable (issue [#7](https://github.com/CaptSnus/ShelfEdgeClock/issues/7)): you can now change the _knx - min value_, _knx - max value_, _clock - min value_, _clock - max value_, _downlight - min value_ and _downlight - max value_.

### Bugfixes

* Small tweaks here and there to make the experience more enjoyable.

## V1.1.0

### Enhancements

* Hide color fields based on color mode (issue [#1](https://github.com/CaptSnus/ShelfEdgeClock/issues/1)): unused fields are hidden.
* suppress calls to resources before wifi connection (issue [#3](https://github.com/CaptSnus/ShelfEdgeClock/issues/3)): the respective funtions just `return;` now
* Color preview (issue [#8](https://github.com/CaptSnus/ShelfEdgeClock/issues/8)): you can now see a preview of the color code entered on the right side of the input field.

### Bugfixes

* Color change (per minute) not working (issue [#6](https://github.com/CaptSnus/ShelfEdgeClock/issues/6)): changed the processing of setting and clearing the flag for a color change.

## V1.0.0

Initial upload of the code
