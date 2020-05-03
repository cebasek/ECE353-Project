// Copyright (c) 2015-19, Joe Krachey
// All rights reserved.
//
// Redistribution and use in source or binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in source form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "project_interrupts.h"

volatile uint16_t PS2_X_DATA = 0;
volatile uint16_t PS2_Y_DATA = 0;
volatile PS2_DIR_t PS2_DIR = PS2_DIR_CENTER;

//*****************************************************************************
// Returns the most current direction that was pressed.
//*****************************************************************************
PS2_DIR_t ps2_get_direction(void)
{
  if (PS2_X_DATA > 0xC00) {
		PS2_DIR = PS2_DIR_LEFT; // joystick left
	} else if (PS2_X_DATA < 0x400) {
		PS2_DIR = PS2_DIR_RIGHT; // joystick right
	} else if (PS2_Y_DATA > 0xC00) {
		PS2_DIR = PS2_DIR_UP; // joystick up
	} else if (PS2_Y_DATA < 0x400) {
		PS2_DIR = PS2_DIR_DOWN; // joystick down
	} else {
		PS2_DIR = PS2_DIR_CENTER;
	}
	return PS2_DIR;
}

//*****************************************************************************
// TIMER1 blinks the LED on the launchpad every 1 second
//*****************************************************************************
void TIMER1A_Handler(void){
	// blink launchpad LED blue
	if (BLINK_LAUNCHPAD_LED) {
		lp_io_set_pin(BLUE_BIT); // LED on
		BLINK_LAUNCHPAD_LED = false;
	} else {
		lp_io_clear_pin(BLUE_BIT); // LED off
		BLINK_LAUNCHPAD_LED = true;
	}
	
	// Clear the interrupt
	TIMER1->ICR |= TIMER_ICR_TATOCINT;
}

//*****************************************************************************
// TIMER2 ISR is triggered every 3 seconds and will add one point to the player's score
//*****************************************************************************
void TIMER2A_Handler(void){
	if (!GAME_RUNNING) {
		CUR_SCORE = 0;
	} else {
		CUR_SCORE = CUR_SCORE + 1;
	}
	
	if (CUR_SCORE > HIGH_SCORE) {
		HIGH_SCORE = CUR_SCORE;
	}
	
	printf("CUR SCORE: %d\t", CUR_SCORE);
	printf("HIGH SCORE: %d\n", HIGH_SCORE);
	
	// Clear the interrupt
	TIMER2->ICR |= TIMER_ICR_TATOCINT;
}

//*****************************************************************************
// TIMER3 ISR is used to determine when to move the bear and its obstacles
// ALSO - it checks if the pause button on the keyboard was pressed and if so it
// pauses the game
//*****************************************************************************

void TIMER3A_Handler(void){	
	//Pauses game if space bar is hit, and prints to Putty interface the current status of the game
	if(fgetcNB(stdin) != ' '){
    //printf("Running...\n\r");
	}
	else{
		while(fgetcNB(stdin) != ' '){
			printf("Paused...\n\r");
		}
	}
	
	//Move Bear
	ALERT_BEAR = true;
	move_bear(&BEAR_Y_COORD);
	
	//Move Enemy
	ALERT_ENEMY = true;
	move_enemy(&ENEMY_X_COORD);

	// Clear the interrupt
	TIMER3->ICR |= TIMER_ICR_TATOCINT;
}
//*****************************************************************************
// TIMER4 ISR to trigger the ADC
//*****************************************************************************
void TIMER4A_Handler(void) {
	// Trigger ADC Sample Sequencer 2 conversion
	ADC0->PSSI |= ADC_PSSI_SS2;
	
	// Clear the interrupt
	TIMER4->ICR |= TIMER_ICR_TATOCINT;
}



//*****************************************************************************
// ADC0 Sample Sequencer 2 ISR
//*****************************************************************************
void ADC0SS2_Handler(void)
{	
	PS2_X_DATA = ADC0->SSFIFO2;
	PS2_Y_DATA = ADC0->SSFIFO2;
	PS2_DIR = ps2_get_direction();
	
  // Clear the interrupt
  ADC0->ISC |= ADC_ISC_IN2;
	
	//Time to change the speed of the bear
	ALERT_SPEED = true;
}

//*****************************************************************************
// GPIOF Sample Sequencer 2 ISR
//*****************************************************************************
void GPIOF_Handler(void){
	ALERT_BUTTON = true; //To call debounce in main
	//Clear the interrupt
	GPIOF->ICR |= 0x01; 
}

