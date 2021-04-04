# keyboard-firmware

The firmware has been tested on Teensy 3.2 and Teensy LC for a split keyboard.
It is meant to be compiled with Teensyduino which can be downloaded from
[PJRC website](http://www.pjrc.com/teensy/teensyduino.html). In the Arduino IDE
(1.6.12 and above), load `keyboard-firmware.ino`, set USB type to have both
keyboard and mouse (Tools -> USB Type), compile (Sketch -> Verify/Compile) and
upload (Sketch -> Upload) to the Teensy board.

The development environment runs on Linux Mint. Other variants in the Debian
family, like Ubuntu, should be fine too but I didn't verify.

To create a new layout, look at the files in `layouts/` directory for examples,
particularly [`layouts/darknight2.h`](https://github.com/macroxue/keyboard-firmware/blob/master/layouts/darknight2.h)
that defines multiple layers including a VI-like one. You can copy and modify
one of them to start. After a new layout is defined, include it in
`keyboard-firmware.ino` and comment out all other layouts.

## Switching between layers

Each keyboard can have multiple layers. Each layer has an associated Fn layer.
There are three ways to switch to another layer.

1. Hit a layer key, e.g. switching to layer 2 by hitting 'l2' key in the
   current layer.
2. Press and hold the 'fn' key to shift to the Fn layer of the current layer.
   Once the 'fn' key is released, it's back to the previous layer. This is
   similar to how Shift key works.
3. Hit the 'fnl' key and lock to the Fn layer of the current layer. Hit the
   'fnl' key again to go back to the previous layer. This is similar to the
   use of CapsLock key.

## Using VI translator for a layer

A layer can optionally have a translator to convert a sequence of keys to
another sequence of keys. This is similar to having macros, but more generic.

For now, there is one built-in translator to provide VI-like editing commands.
I have long been missing VI's power in a browser environment, say, when
writing an email. Now with the VI translator, I can move the cursor to the
beginning of the line with "0" and then change the three words there with "c3w".
Or, I can fix a typo with "xp" quickly.

The translator can't do everything that VI does but it does provide a usable
VI-like environment almost everywhere.

## Dumping debug info

A debug layer can be installed to dump info about the keyboard. The following
keys are supported by the debug layer. Make sure you're in an editor before
invoking the dumps.

* The 'p' key shows column pins and row pins.
* The 'l' key shows all layers.

See [`layouts/darknight2.h`](https://github.com/macroxue/keyboard-firmware/blob/master/layouts/darknight2.h)
for an example of specifying the debug layer.
