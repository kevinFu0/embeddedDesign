#ifndef DE1SOCFPGA_H
#define DE1SOCFPGA_H

// Cyclone V FPGA device addresses
const unsigned int LEDR_OFFSET =  0x00000000; //Physical ADDRESS of RED LED - LW_BRIDGE_BASE ; Write the value in Hex here
const unsigned int SW_OFFSET   =  0x00000040;//Physical ADDRESS of SWITCH - LW_BRIDGE_BASE  ; Write the value in Hex here
const unsigned int KEY_OFFSET  =  0x00000050;//Physical ADDRESS of PUSH BUTTON - LW_BRIDGE_BASE ; Write the value in Hex here
const unsigned int HEX3_HEX0_OFFSET = 0x20;
const unsigned int HEX5_HEX4_OFFSET = 0x30;
// Physical base address of FPGA Devices
const unsigned int LW_BRIDGE_BASE  = 0xFF200000;  // Base offset

// Length of memory-mapped IO window
const unsigned int LW_BRIDGE_SPAN  = 0x00005000;  // Address map size



class DE1SoCfpga {

public:
char *pBase;
int fd;

//constructor
DE1SoCfpga();
//destructor
~DE1SoCfpga();

void RegisterWrite(unsigned int reg_offset, int value);
int RegisterRead(unsigned int reg_offset);

};

#endif



