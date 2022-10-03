#include "SevenSegment.h"
#include "DE1SoCfpga.h"

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
using namespace std;






// constructor
SevenSegment::SevenSegment() {

	reg0_hexValue = 0;
	reg1_hexValue = 0;
	RegisterWrite(HEX3_HEX0_OFFSET, reg0_hexValue);
	RegisterWrite(HEX5_HEX4_OFFSET, reg1_hexValue);
}


void SevenSegment::Hex_ClearAll() {


	reg0_hexValue = 0;
	reg1_hexValue = 0;
  	RegisterWrite(HEX3_HEX0_OFFSET, reg0_hexValue);
	RegisterWrite(HEX5_HEX4_OFFSET, reg1_hexValue);

}


//destructor
SevenSegment::~SevenSegment() {
        Hex_ClearAll();
}





void SevenSegment::Hex_ClearSpecific(int index) {

	reg0_hexValue = RegisterRead(HEX3_HEX0_OFFSET);
        reg1_hexValue = RegisterRead(HEX5_HEX4_OFFSET);

	if(index >= 0 && index < 4) {
		reg0_hexValue = reg0_hexValue & ~(127<<(index*8));
		RegisterWrite(HEX3_HEX0_OFFSET, reg0_hexValue);

	}
	else {
		index = index - 4;
		reg1_hexValue = reg1_hexValue & ~(127<<(index*8));
		RegisterWrite(HEX5_HEX4_OFFSET, reg1_hexValue);

	}

}


void SevenSegment::Hex_WriteSpecific(int index, int value) {


	reg0_hexValue = RegisterRead(HEX3_HEX0_OFFSET);
        reg1_hexValue = RegisterRead(HEX5_HEX4_OFFSET);

	Hex_ClearSpecific(index);
	// flips negative number to positive
	value = (value & 0xF);
	int arrValue = bit_values[value];
	// similar to write 1 led
	if(index >= 0 && index < 4) {
		reg0_hexValue = reg0_hexValue  | (arrValue << (index*8));
		RegisterWrite(HEX3_HEX0_OFFSET, reg0_hexValue);
	}
	else {
		index = index - 4;
		reg1_hexValue = reg1_hexValue  | (arrValue << (index*8));
		RegisterWrite(HEX5_HEX4_OFFSET, reg1_hexValue);

	}

}


void SevenSegment::Hex_WriteNumber(int number) {


// flips the number if negative
// doesn't change the number if positive
number = (number & 0xFFFFFF);
reg0_hexValue = RegisterRead(HEX3_HEX0_OFFSET);
reg1_hexValue = RegisterRead(HEX5_HEX4_OFFSET);


// iterates through each of the 6 seven segment displays
     int realNum=0;
      for (int i=5; i>=0; i--) 
      {
        if(i==5)
        {
          realNum=(number & 0xF00000);
          realNum=realNum>>20;
          Hex_WriteSpecific(5, realNum);
        }
        else if(i==4)
        {
          realNum=(number & 0x0F0000);
          realNum=realNum>>16;
          Hex_WriteSpecific(4, realNum);
        }
        else if(i==3)
        {
          realNum=(number & 0x00F000);
          realNum=realNum>>12;
          Hex_WriteSpecific(3, realNum);
        }
        else if(i==2)
        {
          realNum=(number & 0x000F00);
          realNum=realNum>>8;
          Hex_WriteSpecific(2, realNum);
        }
        else if(i==1)
        {
          realNum=(number & 0x0000F0);
          realNum=realNum>>4;
          Hex_WriteSpecific(1, realNum);
        }
        else if(i==0)
        {
          realNum=(number & 0x00000F);
          Hex_WriteSpecific(0, realNum);
        }
      }
}

