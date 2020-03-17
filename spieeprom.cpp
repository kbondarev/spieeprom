/*
 * spieeprom.cpp - library for SPI EEPROM IC's
 * https://bitbucket.org/trunet/spieeprom/
 * 
 * This library is based on code by Heather Dewey-Hagborg
 * available on http://www.arduino.cc/en/Tutorial/SPIEEPROM
 * 
 * by Wagner Sartori Junior <wsartori@gmail.com>
 */

#include <WProgram.h>
#include <SPI.h>
#include "spieeprom.h"

SPIEEPROM::SPIEEPROM()
{
	eeprom_type = 0;
	address = 0;
	cs_pin = CS_PIN_DEFAULT;
}

SPIEEPROM::SPIEEPROM(byte type, int cs)
{
	if (type > 1)
	{
		eeprom_type = 0;
	}
	else
	{
		eeprom_type = type;
	}
	address = 0;
	cs_pin = cs;
}

void SPIEEPROM::setup()
{
	pinMode(cs_pin, OUTPUT);
	SPI.begin();
}

void SPIEEPROM::send_address(long addr)
{
	if (eeprom_type == 1)
	{
		SPI.transfer((byte)(addr >> 16));
	}
	SPI.transfer((byte)(addr >> 8));
	SPI.transfer((byte)(addr));
}

void SPIEEPROM::start_write()
{
	digitalWrite(cs_pin, LOW);
	SPI.transfer(WREN); //send WREN command
	digitalWrite(cs_pin, HIGH);
	digitalWrite(cs_pin, LOW);
	SPI.transfer(WRITE); //send WRITE command
}

bool SPIEEPROM::isWIP()
{
	byte data;

	digitalWrite(cs_pin, LOW);
	SPI.transfer(RDSR); // send RDSR command

	data = SPI.transfer(0xFF); //get data byte

	digitalWrite(cs_pin, HIGH);

	return (data & (1 << 0));
}

void SPIEEPROM::write(long addr, byte data)
{
	start_write();

	send_address(addr); // send address
	SPI.transfer(data); // transfer data

	digitalWrite(cs_pin, HIGH);

	while (isWIP())
	{
		delay(1);
	}
}

void SPIEEPROM::write(long addr, byte data[], int arrLength)
{
	start_write();

	send_address(addr); // send address

	for (int i = 0; i < arrLength; i++)
	{
		SPI.transfer(data[i]); // transfer data
	}

	digitalWrite(cs_pin, HIGH);
	while (isWIP())
	{
		delay(1);
	}
}

void SPIEEPROM::write(long addr, char data[], int arrLength)
{
	start_write();

	send_address(addr); // send address

	for (int i = 0; i < arrLength; i++)
	{
		SPI.transfer(data[i]); // transfer data
	}

	digitalWrite(cs_pin, HIGH);
	while (isWIP())
	{
		delay(1);
	}
}

byte SPIEEPROM::read_byte(long addr)
{
	byte data;

	digitalWrite(cs_pin, LOW);
	SPI.transfer(READ); // send READ command

	send_address(addr);		   // send address
	data = SPI.transfer(0xFF); //get data byte

	digitalWrite(cs_pin, HIGH); //release chip, signal end transfer

	return data;
}

char SPIEEPROM::read_char(long addr)
{
	char data;

	digitalWrite(cs_pin, LOW);
	SPI.transfer(READ); // send READ command

	send_address(addr);		   // send address
	data = SPI.transfer(0xFF); //get data byte

	digitalWrite(cs_pin, HIGH); //release chip, signal end transfer
	return data;
}
