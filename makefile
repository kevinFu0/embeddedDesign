output: main.o DE1SoCfpga.o SevenSegment.o 
	g++ main.o DE1SoCfpga.o SevenSegment.o -o output

main.o: main.cpp DE1SoCfpga.h SevenSegment.h
	g++ -g -Wall -c main.cpp

SevenSegment.o: SevenSegment.cpp SevenSegment.h
	g++ -g -Wall -c SevenSegment.cpp

DE1SoCfpga.o: DE1SoCfpga.cpp DE1SoCfpga.h
	g++ -g -Wall -c DE1SoCfpga.cpp

clean:
	rm SevenSegment.o DE1SoCfpga.o main.o output

