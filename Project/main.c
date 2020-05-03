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

#include "main.h"

volatile uint8_t CUR_SCORE = 0;
volatile uint8_t HIGH_SCORE = 0;
volatile bool BLINK_LAUNCHPAD_LED = true;
volatile bool GAME_OVER = false;
volatile bool GAME_RUNNING = false;

//*****************************************************************************
//*****************************************************************************
void DisableInterrupts(void)
{
  __asm {
         CPSID  I
  }
}

//*****************************************************************************
//*****************************************************************************
void EnableInterrupts(void)
{
  __asm {
    CPSIE  I
  }
}


//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
	uint8_t get_score;
	
	init_hardware();
	
	//Print welcome screen  
	print_welcome();
	
	// print high score on start screen
	eeprom_byte_read(I2C1_BASE, EEPROM_ADDR, &get_score); // get cur high score
	//eeprom_byte_write(I2C1_BASE, EEPROM_ADDR, 0); // initialize/reset high score
	if (HIGH_SCORE < CUR_SCORE) {
		eeprom_byte_write(I2C1_BASE, EEPROM_ADDR, CUR_SCORE);
		HIGH_SCORE = CUR_SCORE;
	} else {
		HIGH_SCORE = get_score;
	}
	print_high_score();
	
	//Wait for touch interrupt to go to start game screen
	print_ready();
	print_countdown();
	
	//Begin Game
	while(!GAME_OVER){
		GAME_RUNNING = true;
		eeprom_byte_write(I2C1_BASE, EEPROM_ADDR, HIGH_SCORE);
		game_main();
		print_cur_score();
	}
	
	//Reach Infinite Loop
	while(1){};
}

