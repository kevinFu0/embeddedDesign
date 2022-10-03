#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
using namespace std;

// Physical base/starting address of FPGA Devices 
const unsigned int LW_BRIDGE_BASE = 0xFF200000;  // Base offset 

// Physical final address of FPGA Devices 


// Length of memory-mapped IO window 
const unsigned int LW_BRIDGE_SPAN = 0x00DEC700;


// Cyclone V FPGA device addresses
const unsigned int LEDR_OFFSET =  0x00000000; //Physical ADDRESS of RED LED - LW_BRIDGE_BASE ; Write the value in Hex here
const unsigned int SW_OFFSET   =  0x00000040;//Physical ADDRESS of SWITCH - LW_BRIDGE_BASE  ; Write the value in Hex here
const unsigned int KEY_OFFSET  =  0x00000050;//Physical ADDRESS of PUSH BUTTON - LW_BRIDGE_BASE ; Write the value in Hex here
const unsigned int HEX3_HEX0_OFFSET = 0x20;
const unsigned int HEX5_HEX4_OFFSET = 0x30;

//KEVIN FU
// K->C and V->U
// corresponding bit values below

const unsigned int bit_values[14] = {57, 121, 62, 48, 55, 113, 62, 57, 121, 62, 48, 55, 113, 62};

// base is 0xFF200000
const unsigned int MPCORE_PRIV_TIMER_LOAD_OFFSET = 0xDEC600;
const unsigned int MPCORE_PRIV_TIMER_COUNTER_OFFSET = 0xDEC604;
const unsigned int MPCORE_PRIV_TIMER_CONTROL_OFFSET = 0xDEC608;
const unsigned int MPCORE_PRIV_TIMER_INTERRUPT_OFFSET = 0xDEC60C; 









class DE1SoCfpga {

public:
char *pBase;
int fd;


DE1SoCfpga() {

	// Open /dev/mem to give access to physical addresses
	fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if (fd == -1) // check for errors in openning /dev/mem}
	{
        cout << "ERROR: could not open /dev/mem..." << endl;
        exit(1);
	}
	   char *virtual_base = (char *)mmap (NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, LW_BRIDGE_BASE); 
   if (virtual_base == MAP_FAILED) // check for errors exit(1);
   {
	close(fd);
	exit(1);
   }
   pBase = virtual_base;

}


~DE1SoCfpga() {

if( munmap (pBase, LW_BRIDGE_SPAN) != 0) {


cout << "error" << endl;
exit(1);


}

close (fd);
}



void RegisterWrite(unsigned int reg_offset, int value) {

* (volatile unsigned int *) (pBase + reg_offset) = value;


}




int RegisterRead(unsigned int reg_offset) 
{ 
	return * (volatile unsigned int *)(pBase + reg_offset); 
} 






}; // end class

// class SevenSegement inherits the property of base class DE1SoCfpga

class SevenSegment : public DE1SoCfpga {




private:
unsigned int reg0_hexValue;
unsigned int reg1_hexValue;
unsigned int initialvalueLoadMPCore;
unsigned int initialvalueControlMPCore;
unsigned int initialvalueInterruptMPCore;

public:



  //creates an DE1SoCfpga object to use its methods
  //initializes private data memebers
  SevenSegment() {
	reg0_hexValue = 0;
	reg1_hexValue = 0;
	RegisterWrite(HEX3_HEX0_OFFSET, reg0_hexValue);
	RegisterWrite(HEX5_HEX4_OFFSET, reg1_hexValue);
	initialvalueLoadMPCore = RegisterRead(MPCORE_PRIV_TIMER_LOAD_OFFSET);
	initialvalueControlMPCore = RegisterRead(MPCORE_PRIV_TIMER_CONTROL_OFFSET);
	initialvalueInterruptMPCore = RegisterRead(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET);
	
  }
  //clears the hex values with Hex_ClearAll();
  ~SevenSegment() {
	Hex_ClearAll();
	RegisterWrite(MPCORE_PRIV_TIMER_LOAD_OFFSET, initialvalueLoadMPCore);
	RegisterWrite(MPCORE_PRIV_TIMER_CONTROL_OFFSET, initialvalueControlMPCore);
	RegisterWrite(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET, initialvalueInterruptMPCore);

  }

// turn off all the 7 -segements displays
  void Hex_ClearAll() {
	reg0_hexValue = 0;
	reg1_hexValue = 0;
  	RegisterWrite(HEX3_HEX0_OFFSET, reg0_hexValue);
	RegisterWrite(HEX5_HEX4_OFFSET, reg1_hexValue);



  }


// turns off a specified 7-segment display specified by index(0 to 5)
void Hex_ClearSpecific(int index) {
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


// writes a digit or number to the specified 7-segment display
void Hex_WriteSpecific(int index, int value) {
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


//writes a positive or negative number
//USING 2 COMPLEMENT

void Hex_WriteNumber(int number) {

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







  void WriteAllLeds(int value)
{
    RegisterWrite(LEDR_OFFSET, value);
}





// changes the state of an Given Led to on or off
// unsets the given led position to 0
// sets the given led position to state
void Write1Led(int ledNum, int state) {
	// gets current value
	int currentValue = RegisterRead(LEDR_OFFSET);
	currentValue = (currentValue & ~(1<<ledNum)) | (state << ledNum);
	RegisterWrite(LEDR_OFFSET, currentValue);
}

// reads all the switches using RegisterRead
int ReadAllSwitches() {

	int value = RegisterRead(SW_OFFSET);
	return value;


}


// Reads the value of a Switch
int Read1Switch(int switchNum) {

int currentValue = RegisterRead(SW_OFFSET);
// checks if an nth bit is set or unset
// using bitwise manipulation
int check = (currentValue & (1<<switchNum));
if(check > 0) {
	return 1;
}
else {
	return 0;

}



}


// reads the value of a push button
int PushButtonGet() {

int value = RegisterRead(KEY_OFFSET);
return value;
}
//
















}; // end class





int main(void)
{
  SevenSegment *display = new SevenSegment;
  cout << "Program Starting...!" << endl;
  int counter= 100000000;// changed to 0.5 sec///timeout = 1/(200 MHz) x 200x10^6 = 1 sec
  display->RegisterWrite(MPCORE_PRIV_TIMER_LOAD_OFFSET, counter);
  display->RegisterWrite(MPCORE_PRIV_TIMER_CONTROL_OFFSET, 3);
  int entervalue = 0;
  int enterindex = 0;
  int value;
  int reg0_hexValue;

  display->Hex_ClearAll();
  // matches the bit_values array
  int nameLetters[14] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
  int i = 0;
  while (display->Read1Switch(0) == 0)
  {

    if (display->RegisterRead(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET) != 0)
    {
      display->RegisterWrite(MPCORE_PRIV_TIMER_INTERRUPT_OFFSET, 1); // reset timer flag
      // loop through the 14 value array
      if(i < 14)
      {
	// starts at 0
        value = display->RegisterRead(HEX3_HEX0_OFFSET);
        // bitshift by 8 to get to the next seven segment display 
        value = value << 8;
        // loops through HEX1 - HEX5 to copy the previous value to the right
        for(int j = 1; j < 5; j++) {
          if(i - j > 0) {
            display ->Hex_WriteSpecific(j, nameLetters[i-j]);
          }
        }
        // displays the current value to HEX0
        display ->Hex_WriteSpecific(0, nameLetters[i]);
        // priting the current value of HEX0
        // cout << nameLetters[i] << endl;
        // increment i to loop 
        i = i + 1;
      } else {
        // reset i to loop until switch0 is 1
        i = 0;
      }

    }
  }
  delete display;

  cout << "Terminating...!" << endl;
  return 0;
    }





