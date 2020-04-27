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

//*****************************************************************************
// TIMER2 ISR is triggered every 3 seconds and will add one point to the player's score
//*****************************************************************************
void TIMER2A_Handler(void){
	TIMER2->ICR |= TIMER_ICR_TATOCINT; // Clear the interrupt
}


//*****************************************************************************
// TIMER3 ISR is used to determine when to move the bear and the ready screen
//*****************************************************************************

void TIMER3A_Handler(void){
	ALERT_BEAR = true;
	ALERT_READY_SCREEN = true;
	//move_image(PS2_DIR, &INVADER_X_COORD, &INVADER_Y_COORD, invaderHeightPixels, invaderWidthPixels);

	// Clear the interrupt
	TIMER3->ICR |= TIMER_ICR_TATOCINT;
}

