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
#include "project_hardware_init.h"
#include "project_images.h"


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
	char welcome[] = "Welcome to Polar Plunge! Goal: avoid snowmen and tree stumps while trying to collect coins To play: Press the right button to jump and move the joystick left to slow down or right to speed up.";
	int len = strlen(welcome);
	int i;
	
	init_hardware();
	
	
	//Render instructions on screen character by character
	for(i = 0; i < len; i++){
		int descriptorOffset = welcome[i] - '!';
		int bitmapOffset = microsoftSansSerif_8ptDescriptors[descriptorOffset].offset;
		
		int width_bits = microsoftSansSerif_8ptDescriptors[descriptorOffset].widthBits;
		int height_pixels = 11;
		
		lcd_draw_image(0, width_bits, 0, height_pixels, &microsoftSansSerif_8ptBitmaps[bitmapOffset], LCD_COLOR_MAGENTA, LCD_COLOR_BLACK);
	}
	//lcd_draw_image(16, 16, 16, 16, &microsoftSansSerif_8ptBitmaps[0], LCD_COLOR_MAGENTA, LCD_COLOR_BLACK);
	
	//Wait for touch interrupt to go to level selections screen
	
	//Reach Infinite Loop
	while(1){};
}
