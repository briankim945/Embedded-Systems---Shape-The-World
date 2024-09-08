// BranchingFunctionsDelays.c Lab 6
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to 
// toggle an LED while a button is pressed and 
// turn the LED on when the button is released.  
// This lab will use the hardware already built into the LaunchPad.
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// built-in connection: PF0 connected to negative logic momentary switch, SW2
// built-in connection: PF1 connected to red LED
// built-in connection: PF2 connected to blue LED
// built-in connection: PF3 connected to green LED
// built-in connection: PF4 connected to negative logic momentary switch, SW1

#include "TExaS.h"

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control

// Declarations Section
//   Global Variables
unsigned long SW1;  // input from PF4

// basic functions defined at end of startup.s
void Delay100ms(unsigned long time); // delay for set time
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void PortF_Init(void);				// Initialize port connections

int main(void){ unsigned long volatile delay;
  TExaS_Init(SW_PIN_PF4, LED_PIN_PF2);  // activate grader and set system clock to 80 MHz
  // initialization goes here
	PortF_Init();

  EnableInterrupts();           // enable interrupts for the grader
  while(1){
		Delay100ms(1); 						// Delay for 1 second
    SW1 = !(GPIO_PORTF_DATA_R&0x10);  // read PF4 into SW1
		
		if (SW1) { // If reset is pressed
			GPIO_PORTF_DATA_R ^= 0x04;       // Toggle PF2
		} else {
			GPIO_PORTF_DATA_R = 0x04;       // PF2 is turned on
		}
  }
}

// Subroutine to initialize port F pins for input and output
// PF4 is input SW1 and PF2 is output Blue LED
// Inputs: None
// Outputs: None
// Notes: ...
void PortF_Init(void){
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;     // 1) Turn on the clock for Port F
	delay = SYSCTL_RCGC2_R;           // allow time for clock to start
	GPIO_PORTF_AMSEL_R = 0x00;				// 2) Clear the PF4 and PF2 bits in Port F AMSEL to disable analog
	GPIO_PORTF_PCTL_R = 0x00000000;		// 3) Clear the PF4 and PF2 bit fields in Port F PCTL to configure as GPIO
	GPIO_PORTF_DIR_R = 0x0F;          // 4) Set the Port F direction register so PF4 is an input and
																		//		PF2 is an output (01111)
	GPIO_PORTF_AFSEL_R &= 0x0B;       // 5) Clear the PF4 and PF2 bits in Port F AFSEL to disable alternate functions (01011)
	GPIO_PORTF_DEN_R |= 0x14;         // 6) Set the PF4 and PF2 bits in Port F DEN to enable digital (10100)
	GPIO_PORTF_PUR_R |= 0x10;         // 7) Set the PF4 bit in Port F PUR to activate an internal pullup resistor (10000)
	GPIO_PORTF_DATA_R = 0x04;       	// 8) Set the PF2 bit in Port F DATA so the LED is initially ON (00100)
} 

// Delays the running of the program by 100ms * an input time
// Input:  a long time that determins how many 100ms intervals to wait
// Output: None
// Notes:  
void Delay100ms(unsigned long time){
  unsigned long i;
  while(time > 0){
    i = 1333333;  // this number means 100ms
    while(i > 0){
      i = i - 1;
    }
    time = time - 1; // decrements every 100 ms
  }
}
