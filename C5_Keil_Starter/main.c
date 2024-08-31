//**** 0. Documentation Section
// This Program calculates the area of  square shaped rooms
// Author: Ramesh Yerraballi  and Jon Valavano
// Date:   6/28/2013

// 1. Pre-processorr Directives Section
//  INCLUDES: Lists any external libraries we will refer to using
//  a #include tag
#include <stdio.h>  // Diamond braces for sys lib: Standard I/O
#include "uart.h"   // Quotes for user lib: UART lib

// 2. Global Declarations
// Global variables
unsigned long error;

// Function Protoypes
// Compiler aid for "type checking"
void Initialize(void);
unsigned long Calc_Area(unsigned long s);	// Says Calc_Area expects
						// an unsigned long and returns an unsigned long

// 3. Subroutines section
// MAIN: Mandatory subroutine for a C Program to be executable
int main(void) {
	unsigned long side;	// room wall meters
	unsigned long area;	// size squared meters
	
	UART_Init();   // call subroutine to init uart lib
	Initialize();  // Set global variables
	
	printf("This program calculates areas of square shaped rooms\n");
	
	while (side != 0) {
		printf("\nGive room side (zero to quit):");
		scanf("%ld", &side);
		
		if (side != 0) {
			area = Calc_Area(side);
			
			if (area != 0) {
				printf("\nArea of the room with side of %ld m is %ld square m\n", side, area);
			} else {
				printf("\nRoom side size cannot exceed 25 meters\n");
			}
		}
	}
	
	printf("\nGoodbye (Mistake count: %ld)", error);
}

// Initialize global variables
// Input:   None
// Output:  None
// Notes:   ...
void Initialize(void) {
	error = 0;
}

// Calculates Area
// Input:   side of a room (unsigned long)
// Output:  area of a room (unsigned long)
// Notes:   ...
unsigned long Calc_Area(unsigned long s) {
	unsigned long result;
	
	if (s <= 25) {
		result = s * s;  // Working fine
	} else {
		result = 0;      // mistake, too big
		error = error + 1;
	}
	return(result);
}
