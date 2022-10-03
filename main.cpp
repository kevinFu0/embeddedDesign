#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>

#include "SevenSegment.h"
#include "DE1SoCfpga.h"

using namespace std;


int main()
{
	// creating new DE1SoCfgpa object
	// creating a pointer

	SevenSegment *display = new SevenSegment;
	int index;
	int value, value2;
	cout << "Enter index" << endl;
	cin >> index;
	cout << "Enter value" << endl;
	cin >> value;
	display-> Hex_WriteSpecific(index, value);
	cout << "Enter a value to write to hex" << endl;
	cin >> value2;
	display-> Hex_WriteNumber(value2);
	cout << "Destroying object..." << endl;
}

