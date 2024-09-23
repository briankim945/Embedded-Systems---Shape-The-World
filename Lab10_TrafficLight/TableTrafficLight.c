// ***** 0. Documentation Section *****
// TableTrafficLight.c for Lab 10
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void PortB_Init(void);				// initialize B ports
void PortE_Init(void);				// initialize E ports
void PortF_Init(void);				// initialize F ports
void SysTick_Init(void);			// Initialize SysTick;
void SysTick_Wait(unsigned long delay);		// Delay functionality
void SysTick_Wait10ms(unsigned long delay);		// Delay in increments of 10ms

//Shortcuts to refer to the various states in the FSM array
#define longWait 300 // extending time to pass test
#define shortWait 20
#define westRed 0x20
#define westYellow 0x10
#define westGreen 0x08
#define southRed 0x04
#define southYellow 0x02
#define southGreen 0x01
#define walk 0x08
#define nowalk 0x02
#define walkoff 0x00

// represents a State of the FSM 
struct State {
   unsigned long pedOut;   // output for the pedestrian state
	 unsigned long nsOut;   // output for the North-South state
	 unsigned long ewOut;   // output for the East-West state
   unsigned short wait;     // Time to wait when in this state
   unsigned char next[8]; // Next state array
};

typedef const struct State StateType;

unsigned char cState; //Current state is between 0-11, inclusive

//The data structure that captures the FSM state transition graph
StateType Fsm[11] = { // output pattern: pedestrian, south, west
   {nowalk, southRed, westGreen, longWait, {0,0,1,1,1,1,1,1}},
   {nowalk, southRed, westYellow, longWait, {0,0,2,2,4,4,2,2}},
	 {nowalk, southGreen, westRed, longWait, {2,3,2,3,3,3,3,3}},
	 {nowalk, southYellow, westRed, longWait, {2,0,2,0,4,4,4,4}},
	 {walk, southRed, westRed, longWait, {4,5,5,5,4,5,5,5}},
	 // blinking:
	 {walkoff, southRed, westRed, shortWait, {4,6,6,6,4,6,6,6}},
	 {walk, southRed, westRed, shortWait, {4,7,7,7,4,7,7,7}},
	 {walkoff, southRed, westRed, shortWait, {4,8,8,8,4,8,8,8}},
	 {walk, southRed, westRed, shortWait, {4,9,9,9,4,9,9,9}},
	 {walkoff, southRed, westRed, shortWait, {4,10,10,10,4,10,10,10}},
	 {walk, southRed, westRed, shortWait, {4,0,2,0,4,0,2,0}},
}; 

// ***** 3. Subroutines Section *****

int main(void){ 
  TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff); // activate grader and set system clock to 80 MHz
	PortB_Init();			// Initialize port B
	PortE_Init();			// Initialize port E
  PortF_Init();			// Initialize port F
	SysTick_Init();		// Initialize SysTick
	
	unsigned char input;
  
  EnableInterrupts();
	cState = 0;			// Initial state
  while(1){
		// output based on current state
    GPIO_PORTB_DATA_R = Fsm[cState].ewOut + Fsm[cState].nsOut;		// output for west and south
		GPIO_PORTF_DATA_R = Fsm[cState].pedOut;	// output for pedestrian
		
		// wait for time relevant to state
    SysTick_Wait10ms(Fsm[cState].wait);
		
		// get input     
		input = GPIO_PORTE_DATA_R & 0x07;		// Input 0/1: pedestrian, west, south detector all pressed/not pressed
		
		// change state based on input and current state
		cState = Fsm[cState].next[input]; 
  }
}

// Subroutine to initialize port E pins for input and output
// PB5-0 is output LED
// Inputs: None
// Outputs: None
// Notes: ...
void PortB_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000002;      // 1) B clock
  delay = SYSCTL_RCGC2_R;            // delay to allow clock to stabilize     
  GPIO_PORTB_AMSEL_R &= 0x00;        // 2) disable analog function
  GPIO_PORTB_PCTL_R &= 0x00000000;   // 3) GPIO clear bit PCTL  
	GPIO_PORTB_DIR_R &= 0x00;          // 4.1) No input
  GPIO_PORTB_DIR_R |= 0x3F; 				 // 4) PB5-0 output  
  GPIO_PORTB_AFSEL_R &= 0x00;        // 5) no alternate function
  //GPIO_PORTB_PUR_R |= 0x00;          // 6) enable pullup resistor on PE0       (double check)
  GPIO_PORTB_DEN_R |= 0x3F;					 // 7) enable digital pins PB5-0
}

// Subroutine to initialize port E pins for input and output
// PE2-0 is input
// Inputs: None
// Outputs: None
// Notes: ...
void PortE_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000010;      // 1) E clock
  delay = SYSCTL_RCGC2_R;            // delay to allow clock to stabilize     
  GPIO_PORTE_AMSEL_R &= 0x00;        // 2) disable analog function
  GPIO_PORTE_PCTL_R &= 0x00000000;   // 3) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R &= ~0x07;         // 4.1) PE2-0 input
  GPIO_PORTE_AFSEL_R &= 0x00;        // 5) no alternate function
  //GPIO_PORTE_PUR_R |= 0x00;          // 6) enable pullup resistor on PE0       (double check)
  GPIO_PORTE_DEN_R |= 0x07;          // 7) enable digital pins PE2-0
}

// Subroutine to initialize port F pins for input and output
// PF3,1 is output LED
// Inputs: None
// Outputs: None
// Notes: ...
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // Activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // Allow time for clock to start
  GPIO_PORTF_AMSEL_R = 0x00;        // Disable analog
  GPIO_PORTF_PCTL_R = 0x00000000;   // PCTL GPIO on PF4-0
	GPIO_PORTF_DIR_R &= 0x00;         // 4.1) No input
	GPIO_PORTF_DIR_R |= 0x0A;         //   PF3,1 output
  GPIO_PORTF_AFSEL_R &= 0x00;      	// no alternate function
  //GPIO_PORTF_PUR_R |= 0x10;         // Enable pull-up on PF4
  GPIO_PORTF_DEN_R |= 0x0A;         // Enable digital I/O on PF3,1
}

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = 0x00FFFFFF;        // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it             
  NVIC_ST_CTRL_R = 0x00000005;          // enable SysTick with core clock
}

// Time delay using busy wait.
// The delay parameter is in units of the core clock. 
void SysTick_Wait(unsigned long delay){
  volatile unsigned long elapsedTime;
  unsigned long startTime = NVIC_ST_CURRENT_R;
  do{
    elapsedTime = (startTime-NVIC_ST_CURRENT_R)&0x00FFFFFF;
  }
  while(elapsedTime <= delay);
}

// Time delay using busy wait.
// This assumes 16 MHz system clock.
void SysTick_Wait10ms(unsigned long delay){
  unsigned long i;
  for(i=0; i<delay; i++){
    SysTick_Wait(160000);  // wait 10ms (assumes 16 MHz clock)
  }
}


