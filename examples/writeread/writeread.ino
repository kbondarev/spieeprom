#include <SPI.h>
#include <spieeprom.h>

#define CHIP_SELECT 7

byte buffer[256];
long address;

SPIEEPROM disk1(EEPROM_TYPE_16BIT, CHIP_SELECT); // parameter is type
                    // type=0: 16-bits address
                    // type=1: 24-bits address
                    // type>1: defaults to type 0

void fill_buffer()
{
  for (int i=0;i<256;i++)
  {
    buffer[i]=i;
  }
}

void setup() {
	Serial.begin(9600);
	
	disk1.setup(); // setup disk1
	
	fill_buffer();
	address = 0;
	
	Serial.print("Starting to write on EEPROM:");
	Serial.print(millis());
	Serial.println();
	
	disk1.write(address, buffer, (sizeof(buffer)/sizeof(byte)));
	
	Serial.print("Finish to write:");
	Serial.print(millis());
	Serial.println();
	
	delay(1000);
}

void loop() {
	Serial.print("Address:|");
	Serial.print(address);
	Serial.print("| - Value:|");
	Serial.print(disk1.readByte(address), DEC);
	Serial.println("|");
	address++;
	if (address == 256)
		address = 0;
	delay(500);
}
