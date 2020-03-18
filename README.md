# SPI EEPROM

This is an Arduino library which helps to read and write to an EEPROM over SPI.


I forked this library from https://bitbucket.org/trunet/spieeprom/ and modified it to add these features:

- Ability to set the pin on the Arduino to be used as the CS (chips select, aka "slave select")
- A function to read an array of bytes


This was tested with the 25LC640A EEPROM chip and the Arduino MKR1010 board. However, it should work with any other arduino and probably with other SPI EEPROM chips.

## Install

Download or clone this repo into your Arduino libraries directory


## How to use

Include these

```cpp
#include <SPI.h>
#include <spieeprom.h>
```

Define the object SPIEEPROM with the type of eeprom (16 or 24 bit) and the pin for chip select

```cpp
SPIEEPROM eep(EEPROM_TYPE_16BIT, 7);
```

Call the `SPIEEPROM::setup()` function once in the `setup` function of your sketch:

```cpp
void setup() {
    // your setup code

    eep.setup();

    // more setup code
}
```

Then you can use one of the read/write methods (see [spieeprom.h](./spieeprom.h) for options)