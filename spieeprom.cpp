#include <Arduino.h>
#include <SPI.h>
#include "spieeprom.h"

SPIEEPROM::SPIEEPROM()
{
	eepromType = 0;
	address = 0;
	csPin = CS_PIN_DEFAULT;
}

SPIEEPROM::SPIEEPROM(byte type, int cs)
{
	if (type > EEPROM_TYPE_24BIT)
	{
		// select default type
		eepromType = EEPROM_TYPE_16BIT;
	}
	else
	{
		eepromType = type;
	}
	address = 0;
	csPin = cs;
}

void SPIEEPROM::setup()
{
	pinMode(csPin, OUTPUT);
	SPI.begin();
}

void SPIEEPROM::sendAddress(long addr)
{
	if (eepromType == EEPROM_TYPE_24BIT)
	{
		SPI.transfer((byte)(addr >> 16));
	}
	SPI.transfer((byte)(addr >> 8));
	SPI.transfer((byte)(addr));
}

void SPIEEPROM::startWrite()
{
	digitalWrite(csPin, LOW);
	SPI.transfer(WREN); //send WREN command
	digitalWrite(csPin, HIGH);
	digitalWrite(csPin, LOW);
	SPI.transfer(WRITE); //send WRITE command
}

bool SPIEEPROM::isWIP()
{
	byte data;

	digitalWrite(csPin, LOW);
	SPI.transfer(RDSR); // send RDSR command

	data = SPI.transfer(0xFF); //get data byte

	digitalWrite(csPin, HIGH);

	return (data & (1 << 0));
}

void SPIEEPROM::write(long addr, byte data)
{
	startWrite();

	sendAddress(addr); // send address
	SPI.transfer(data); // transfer data

	digitalWrite(csPin, HIGH);

	while (isWIP())
	{
		delay(1);
	}
}

void SPIEEPROM::write(long addr, byte data[], int arrLength)
{
	startWrite();

	sendAddress(addr); // send address

	for (int i = 0; i < arrLength; i++)
	{
		SPI.transfer(data[i]); // transfer data
	}

	digitalWrite(csPin, HIGH);
	while (isWIP())
	{
		delay(1);
	}
}

byte SPIEEPROM::readByte(long addr)
{
	byte data;

	digitalWrite(csPin, LOW);
	SPI.transfer(READ); // send READ command

	sendAddress(addr);		   // send address
	data = SPI.transfer(0xFF); //get data byte

	digitalWrite(csPin, HIGH); //release chip, signal end transfer

	return data;
}

void SPIEEPROM::readByteArray(long addr, byte data[], int arrLength)
{

	digitalWrite(csPin, LOW);
	SPI.transfer(READ); // send READ command

	sendAddress(addr);		   // send address
	for (int i = 0; i < arrLength; i++)
	{
		// the EEPROM chip has an internal counter and will continue sending the
		// next bytes
		data[i] = SPI.transfer(0xFF); //get data byte
	}

	digitalWrite(csPin, HIGH); //release chip, signal end transfer
}
