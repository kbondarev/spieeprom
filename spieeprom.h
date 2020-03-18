#ifndef SPIEEPROM_h
#define SPIEEPROM_h

#include <Arduino.h>
#include <SPI.h> // relies on arduino SPI library

// type of the eeprom address (check the datasheet of the chip)
// defaults to 16 bit
#define EEPROM_TYPE_16BIT 0
#define EEPROM_TYPE_24BIT 1

// default digital pin for Chip Select (aka "Slave Select")
#define CS_PIN_DEFAULT 10

// opcodes
#define WREN 6
#define WRDI 4
#define RDSR 5
#define WRSR 1
#define READ 3
#define WRITE 2

class SPIEEPROM
{
private:
	long address;
	byte eepromType;
	int csPin; // chip select

	void sendAddress(long addr);
	void startWrite();
	bool isWIP(); // is write in progress?

public:
	SPIEEPROM();					 // default to type 0
	SPIEEPROM(byte type, int csPin); // type=0: 16-bits address
									 // type=1: 24-bits address
									 // type>1: defaults to type 0

	void setup();

	void write(long addr, byte data);
	void write(long addr, byte data[], int arrLength);

	byte readByte(long addr);
	void readByteArray(long addr, byte data[], int arrLength);
};

#endif // SPIEEPROM_h
