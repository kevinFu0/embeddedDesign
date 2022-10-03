#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "DE1SoCfpga.h"
using namespace std;




//constructor
DE1SoCfpga::DE1SoCfpga() {

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

//destructor
DE1SoCfpga::~DE1SoCfpga() {

if( munmap (pBase, LW_BRIDGE_SPAN) != 0) {


cout << "error" << endl;
exit(1);


}

close (fd);    
    
}

void DE1SoCfpga::RegisterWrite(unsigned int reg_offset, int value) {

* (volatile unsigned int *) (pBase + reg_offset) = value;


}

int DE1SoCfpga::RegisterRead(unsigned int reg_offset) 
{ 
	return * (volatile unsigned int *)(pBase + reg_offset); 
} 

