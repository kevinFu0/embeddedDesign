#ifndef SEVENSEGMENT_H
#define SEVENSEGMENT_H
#include "DE1SoCfpga.h"

const unsigned int bit_values[16] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 103, 119, 124, 57, 94, 121, 113};



class SevenSegment: public DE1SoCfpga {




unsigned int reg0_hexValue;
unsigned int reg1_hexValue;

public:
// constructor
SevenSegment();
//destructor
~SevenSegment();
//clear all
void Hex_ClearAll();
//clear specific
void Hex_ClearSpecific(int index);
// write specific
void Hex_WriteSpecific(int index, int value);
// write number
void Hex_WriteNumber(int number);
 
};

#endif


