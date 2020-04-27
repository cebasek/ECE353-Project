#include "project.h"


volatile uint16_t BEAR_X_COORD = 50; //THIS WILL NEVER CHANGE
volatile uint16_t BEAR_Y_COORD = 200;
volatile bool ALERT_BEAR = true; //Set to true when we want to update the bear's position
volatile uint8_t HIGH_SCORE = 0;
volatile bool ALERT_READY_SCREEN = true;


//************************************************************************
// Prints a welcome message to the screen upon reset of game
//************************************************************************
void print_welcome(){
	char welcome[] = "Welcome to Polar *Plunge!*";
	char instructions[] = "Goal: avoid snowmen and tree stumps*To play: Press the*right button to jump and move the*joystick left to slow down or right to*speed up*";
	char cont[] = "Touch the screen to*continue";
	
	int len_w = strlen(welcome);
	int len_i = strlen(instructions);
	int len_c = strlen(cont);
	
	int i, j, k;
	int x_start = 0;
	int y_start = 0;
	
//Start with blank blue screen
lcd_clear_screen(LCD_COLOR_BLUE2);

//Prints our welcome message in white
for(i = 0; i < len_w; i++){
	
	  //If it's a space, just increment x and move to the next character
	  if(welcome[i] == ' '){
			x_start = x_start + 8;
		}
		//If we want to go to a new line
		else if(welcome[i] == '*'){
				x_start = 0;
				y_start = y_start + 25;
		}
		//If it's an actual letter, write it
		else{
			int descriptorOffset = welcome[i] - '!'; //'!' is my start character defined by the output
			int bitmapOffset = lucidaCalligraphy_12ptDescriptors[descriptorOffset].offset;

			int width_bits = lucidaCalligraphy_12ptDescriptors[descriptorOffset].widthBits;
			int height_pixels = 20;
	   
			lcd_draw_char(x_start, width_bits, y_start, height_pixels, lucidaCalligraphy_12ptBitmaps + bitmapOffset, LCD_COLOR_WHITE, LCD_COLOR_BLUE2, 0);
			x_start = x_start + width_bits + 2;
	
			if(x_start >= 220){
				x_start = 0;
				y_start = y_start + 25;
			}
	 }

}	

//Prints our instructions in Magenta
for(j = 0; j < len_i; j++){
	
	  //If it's a space, just increment x and move to the next character
	  if(instructions[j] == ' '){
			x_start = x_start + 8;
		}
		//If we want to go to a new line
		else if(instructions[j] == '*'){
				x_start = 0;
				y_start = y_start + 25;
		}
		//If it's an actual letter, write it
		else{
			int descriptorOffset = instructions[j] - '!'; //'!' is my start character defined by the output
			int bitmapOffset = lucidaCalligraphy_12ptDescriptors[descriptorOffset].offset;

			int width_bits = lucidaCalligraphy_12ptDescriptors[descriptorOffset].widthBits;
			int height_pixels = 20;
	   
			lcd_draw_char(x_start, width_bits, y_start, height_pixels, lucidaCalligraphy_12ptBitmaps + bitmapOffset, LCD_COLOR_MAGENTA, LCD_COLOR_BLUE2, 0);
			x_start = x_start + width_bits + 2;
	
			if(x_start >= 220){
				x_start = 0;
				y_start = y_start + 25;
			}
	 }
}

//Prints our continue message in white
for(k = 0; k < len_c; k++){
	
	  //If it's a space, just increment x and move to the next character
	  if(cont[k] == ' '){
			x_start = x_start + 8;
		}
		//If we want to go to a new line
		else if(cont[k] == '*'){
				x_start = 0;
				y_start = y_start + 25;
		}
		//If it's an actual letter, write it
		else{
			int descriptorOffset = cont[k] - '!'; //'!' is my start character defined by the output
			int bitmapOffset = lucidaCalligraphy_12ptDescriptors[descriptorOffset].offset;

			int width_bits = lucidaCalligraphy_12ptDescriptors[descriptorOffset].widthBits;
			int height_pixels = 20;
	   
			lcd_draw_char(x_start, width_bits, y_start, height_pixels, lucidaCalligraphy_12ptBitmaps + bitmapOffset, LCD_COLOR_WHITE, LCD_COLOR_BLUE2, 0);
			x_start = x_start + width_bits + 2;
	
			if(x_start >= 220){
				x_start = 0;
				y_start = y_start + 25;
			}
	 }
}
}

void print_ready() {
	uint8_t touch_event;
	char get_ready[] = "GET READY!";
	char countdown3[] = "3";
	char countdown2[] = "2";
	char countdown1[] = "1";
	uint8_t current_message = 0;
	int i;
	int x_start = 20;
	int y_start = 146;
	
	// wait until a touch event to continue
	while (1) {
		touch_event = ft6x06_read_td_status();
	
		if (touch_event > 0) {

			lcd_clear_screen(LCD_COLOR_BLUE2);
			lcd_draw_box(150, 80, 10, 25, LCD_COLOR_WHITE, LCD_COLOR_BLACK, 2);
			
			for (i = 0; i < strlen(get_ready); i++) {
				if(get_ready[i] == ' '){ // increment x for space
					x_start = x_start + 14;
				} else { // write character to screen
					int descriptorOffset = get_ready[i] - '!'; // subtract start character ('!') to get offset
					int bitmapOffset = tahoma_28ptDescriptors[descriptorOffset].offset;
					int width_bits = tahoma_28ptDescriptors[descriptorOffset].widthBits;
					int height_pixels = 28;
				 
					lcd_draw_char(x_start, width_bits, y_start, height_pixels, tahoma_28ptBitmaps + bitmapOffset, LCD_COLOR_MAGENTA, LCD_COLOR_BLACK, 0);
					x_start = x_start + width_bits + 2;
				}
			}
			current_message = 1;
			
			if (ALERT_READY_SCREEN && (current_message == 1)) {
				ALERT_READY_SCREEN = false;
				
				
			} else if (ALERT_READY_SCREEN && (current_message == 2)) {
				ALERT_READY_SCREEN = false;
				
				
			} else if (ALERT_READY_SCREEN && (current_message == 3)) {
				ALERT_READY_SCREEN = false;
				
				
			}
			break;
		}
	}
}

void game_main(void){
	//If it's time to re-render the bear
	if(ALERT_BEAR){
		ALERT_BEAR = false;
		lcd_draw_image(BEAR_X_COORD, bearWidthPixels, BEAR_Y_COORD, bearHeightPixels, bearBitmaps, LCD_COLOR_WHITE, LCD_COLOR_BLUE2);
	
	}
}
