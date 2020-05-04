#include "project.h"
#include "main.h"
#include "io_expander.h"
#include "project_images.h"
#include "project_interrupts.h"

const uint16_t BEAR_X_COORD = 50; //THIS WILL NEVER CHANGE
volatile uint16_t BEAR_Y_COORD = 200;

volatile uint8_t HIGH_SCORE = 0;
bool WAIT_SCORE = false;

volatile uint16_t ENEMY_X_COORD = 320;
const uint16_t ENEMY_Y_COORD = 230; //THIS WILL NEVER CHANGE

volatile SPEED_t currSPEED;

volatile bool NEW_ENEMY = false;


typedef struct
{
    int top;
    int bottom;
    int left;
    int right;
} Rectangle;


//CHANGE based on which enemy is currently on the screen
uint8_t *EnemyBitmaps; 
uint8_t enemyWidthPixels = 0;
uint8_t enemyHeightPixels = 0;
uint16_t ENEMY_COLOR;

// indicates if at the start of a new game
bool game_begin = true;

volatile bool ALERT_BEAR = true; //Set to true when we want to update the bear's position
volatile bool ALERT_BUTTON = true; //Set to true when the push button is pressed
volatile bool ALERT_ENEMY = true;
volatile bool ALERT_SPEED = false;

//Decrease when the bear hits an enemy
volatile uint8_t SCORE = 0x1F;

//USED IN MOVE_BEAR FOR JUMPING LOGIC
volatile bool JUMPING = false; //Indicates if we are currently in a jump
volatile bool ASCENDING = false; //Indicates if we are moving up in a jump

volatile BUTTON_t BUTTON_PRESSED;
volatile SPEED_t SPEED = SPEED_MEDIUM;

//************************************************************************
// Prints a welcome message to the screen upon reset of game
//************************************************************************
void print_welcome(){
	char welcome[] = "Welcome to Polar *Plunge!*";
	char instructions[] = "Goal: avoid balls* and tree stumps*To play: Press the*right button to jump and move the*joystick left to slow down or right to* speed up*";
	char cont[] = "Touch the screen to*continue";
	
	int len_w = strlen(welcome);
	int len_i = strlen(instructions);
	int len_c = strlen(cont);
	
	int i, j, k;
	int x_start = 0;
	int y_start = 0;
	
//Start with blank blue screen
lcd_clear_screen(LCD_COLOR_BLUE2);

//Prints our welcome message in blue
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
	   
			lcd_draw_char(x_start, width_bits, y_start, height_pixels, lucidaCalligraphy_12ptBitmaps + bitmapOffset, LCD_COLOR_BLUE, LCD_COLOR_BLUE2, 0);
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

//Prints our continue message in blue
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
	   
			lcd_draw_char(x_start, width_bits, y_start, height_pixels, lucidaCalligraphy_12ptBitmaps + bitmapOffset, LCD_COLOR_BLUE, LCD_COLOR_BLUE2, 0);
			x_start = x_start + width_bits + 2;
	
			if(x_start >= 220){
				x_start = 0;
				y_start = y_start + 25;
			}
	 }
}
}

//*****************************************************************************
// Prints "GET READY!" to alert users to get ready to play game
//*****************************************************************************
void print_ready() {
	uint8_t touch_event;
	char get_ready[] = "GET READY!";
	int i;
	int x_start = 20;
	int y_start = 146;
	
	// wait until a touch event to continue
	while (1) {
		touch_event = ft6x06_read_td_status();
	
		if (touch_event > 0) {

			lcd_clear_screen(LCD_COLOR_BLUE2);
			
			for (i = 0; i < strlen(get_ready); i++) {
				if(get_ready[i] == ' '){ // increment x for space
					x_start = x_start + 14;
				} else { // write character to screen
					int descriptorOffset = get_ready[i] - '!'; // subtract start character ('!') to get offset
					int bitmapOffset = tahoma_28ptDescriptors[descriptorOffset].offset;
					int width_bits = tahoma_28ptDescriptors[descriptorOffset].widthBits;
					int height_pixels = 28;
				 
					lcd_draw_char(x_start, width_bits, y_start, height_pixels, tahoma_28ptBitmaps + bitmapOffset, LCD_COLOR_MAGENTA, LCD_COLOR_BLUE2, 0);
					x_start = x_start + width_bits + 2;
				}
			}	
			break;
		}
	}
}

//*****************************************************************************
// Print countdown until game start... 3, 2, 1
//*****************************************************************************
void print_countdown() {
	char countdown[] = "3 2 1";
	int x_start = 40;
	int y_start = 136;
	int i;
	
	for (i = 0; i < 10000000; i++) {
	}
	
	lcd_clear_screen(LCD_COLOR_BLUE2);
	
	for (i = 0; i < strlen(countdown); i++) {
		if(countdown[i] == ' '){ // increment x for space
			x_start = x_start + 40;
		} else { // write character to screen
			int descriptorOffset = countdown[i] - '1'; // subtract start character ('1') to get offset
			int bitmapOffset = tahoma_48ptDescriptors[descriptorOffset].offset;
			int width_bits = tahoma_48ptDescriptors[descriptorOffset].widthBits;
			int height_pixels = 48;
			
			lcd_draw_char(x_start, width_bits, y_start, height_pixels, tahoma_48ptBitmaps + bitmapOffset, LCD_COLOR_MAGENTA, LCD_COLOR_BLUE2, 0);
			x_start = x_start + width_bits + 2;					
		}
	}
	
	// Begin countdown and wait between counts
	for (i = 0; i < 6000000; i++) {}
	lcd_draw_rectangle(40, 28, 136, 48, LCD_COLOR_BLUE2); // draw over 3
	for (i = 0; i < 6000000; i++) {}
	lcd_draw_rectangle(110, 29, 136, 48, LCD_COLOR_BLUE2); // draw over 2
	for (i = 0; i < 6000000; i++) {}
	lcd_draw_rectangle(181, 22, 136, 48, LCD_COLOR_BLUE2); // draw over 1
	for (i = 0; i < 10000000; i++) {}
}

//*****************************************************************************
// Displays high score on startup
//*****************************************************************************
void print_high_score() {
	int i, num;
	int x_start = 190;
	int y_start = 295;
	int score_x_start = 90;
	int score_y_start = 299;
	int num_digits = 0;
	char* score_array;
	char high_score_words[] = "HIGH SCORE: ";
	
	if (HIGH_SCORE == 0) {
		num_digits = 1; // one digit
	} else {
		// get number of digits in high score
		num = HIGH_SCORE;
		while (num != 0) { // get number of digits
			num /= 10;
			num_digits++;
		}
	}
	
	score_array = malloc(num_digits * sizeof(char*));
	
	// print "HIGH SCORE: " words
	for (i = 0; i < strlen(high_score_words); i++) {
		if(high_score_words[i] == ' '){ // increment x for space
			score_x_start = score_x_start + 10;
		} else { // write character to screen
			int descriptorOffset = high_score_words[i] - ':'; // subtract start character (':') to get offset
			int bitmapOffset = tahoma_10ptDescriptors[descriptorOffset].offset;
			int width_bits = tahoma_10ptDescriptors[descriptorOffset].widthBits;
			int height_pixels = 9;
			
			lcd_draw_char(score_x_start, width_bits, score_y_start, height_pixels, tahoma_10ptBitmaps + bitmapOffset, LCD_COLOR_MAGENTA, LCD_COLOR_BLUE2, 0);
			score_x_start = score_x_start + width_bits + 2;					
		}
	}
	
	// turn high score to char array
	snprintf(score_array, num_digits + 1, "%d", HIGH_SCORE);
	
	// print out score
	for (i = 0; i < strlen(score_array); i++) {
		int descriptorOffset = score_array[i] - '0'; // subtract start character ('0') to get offset
		int bitmapOffset = tahoma_16ptDescriptorsNum[descriptorOffset].offset;
		int width_bits = tahoma_16ptDescriptorsNum[descriptorOffset].widthBits;
		int height_pixels = 16;
		
		lcd_draw_char(x_start, width_bits, y_start, height_pixels, tahoma_16ptBitmapsNum + bitmapOffset, LCD_COLOR_MAGENTA, LCD_COLOR_BLUE2, 0);
		x_start = x_start + width_bits + 2;
	}
	
	free(score_array); // array not needed anymore
}

//*****************************************************************************
// Displays current score
//*****************************************************************************
void print_cur_score() {
	int i, num;
	int x_start = 190;
	int y_start = 295;
	int score_x_start = 132;
	int score_y_start = 299;
	int num_digits = 0;
	char* score_array;
	char score_words[] = "SCORE: ";
	
	if (CUR_SCORE == 0) {
		num_digits = 1; // one digit
	} else {
		// get number of digits in high score
		num = CUR_SCORE;
		while (num != 0) { // get number of digits
			num /= 10;
			num_digits++;
		}
	}
	
	score_array = malloc(num_digits * sizeof(char*));
	
	// turn high score to char array
	snprintf(score_array, num_digits + 1, "%d", CUR_SCORE);
	
	// print "SCORE: " words
	for (i = 0; i < strlen(score_words); i++) {
		if(score_words[i] == ' '){ // increment x for space
			score_x_start = score_x_start + 10;
		} else { // write character to screen
			int descriptorOffset = score_words[i] - ':'; // subtract start character (':') to get offset
			int bitmapOffset = tahoma_10ptDescriptors[descriptorOffset].offset;
			int width_bits = tahoma_10ptDescriptors[descriptorOffset].widthBits;
			int height_pixels = 9;
			
			lcd_draw_char(score_x_start, width_bits, score_y_start, height_pixels, tahoma_10ptBitmaps + bitmapOffset, LCD_COLOR_MAGENTA, LCD_COLOR_BLUE2, 0);
			score_x_start = score_x_start + width_bits + 2;					
		}
	}
	
	// print out score
	for (i = 0; i < strlen(score_array); i++) {
		int descriptorOffset = score_array[i] - '0'; // subtract start character ('0') to get offset
		int bitmapOffset = tahoma_16ptDescriptorsNum[descriptorOffset].offset;
		int width_bits = tahoma_16ptDescriptorsNum[descriptorOffset].widthBits;
		int height_pixels = 16;
		
		lcd_draw_rectangle(x_start, width_bits + 20, y_start, height_pixels, LCD_COLOR_BLUE2);
		lcd_draw_char(x_start, width_bits, y_start, height_pixels, tahoma_16ptBitmapsNum + bitmapOffset, LCD_COLOR_MAGENTA, LCD_COLOR_BLUE2, 0);
		x_start = x_start + width_bits + 2;
	}
	
	free(score_array); // array not needed anymore
}

//*****************************************************************************
// Displays "dead" screen, after player dies
//*****************************************************************************
void print_dead_screen() {
	uint8_t touch_event;
	char game_over[] = "GAME OVER!";
	char play_again[] = "Play again?";
	char yes[] = "YES";
	char no[] = "NO";
	int i;
	int game_over_x_start = 18;
	int game_over_y_start = 125;
	int play_again_x_start = 73;
	int play_again_y_start = game_over_y_start + 27 + 20;
	int yes_x_start = 52;
	int no_x_start = yes_x_start + 47 + 52;
	int yesno_y_start = 210;
	
	bool yes_box;
	bool no_box;
	bool y_box;
	uint16_t touch_x, touch_y;
	
	lcd_clear_screen(LCD_COLOR_BLUE2);
	
	// print "GAME OVER!" text
	for (i = 0; i < strlen(game_over); i++) {
				if(game_over[i] == ' '){ // increment x for space
					game_over_x_start = game_over_x_start + 14;
				} else { // write character to screen
					int descriptorOffset = game_over[i] - '!'; // subtract start character ('!') to get offset
					int bitmapOffset = tahoma_28ptDescriptors[descriptorOffset].offset;
					int width_bits = tahoma_28ptDescriptors[descriptorOffset].widthBits;
					int height_pixels = 28;
				 
					lcd_draw_char(game_over_x_start, width_bits, game_over_y_start, height_pixels, tahoma_28ptBitmaps + bitmapOffset, LCD_COLOR_MAGENTA, LCD_COLOR_BLUE2, 0);
					game_over_x_start = game_over_x_start + width_bits + 2;
				}
	}
	
	// print "Play again?" text
	for (i = 0; i < strlen(play_again); i++) {
		if(play_again[i] == ' '){ // increment x for space
			play_again_x_start = play_again_x_start + 7;
		} else { // write character to screen
			int descriptorOffset = play_again[i] - '?'; // subtract start character ('?') to get offset
			int bitmapOffset = tahoma_16ptDescriptors[descriptorOffset].offset;
			int width_bits = tahoma_16ptDescriptors[descriptorOffset].widthBits;
			int height_pixels = 21;
		 
			lcd_draw_char(play_again_x_start, width_bits, play_again_y_start, height_pixels, tahoma_16ptBitmaps + bitmapOffset, LCD_COLOR_MAGENTA, LCD_COLOR_BLUE2, 0);
			play_again_x_start = play_again_x_start + width_bits + 2;
		}
	}
	
	lcd_draw_box(yes_x_start - 5, 47, yesno_y_start - 6, 30, LCD_COLOR_MAGENTA, LCD_COLOR_BLUE2, 2);
	
	// print "YES" text
	for (i = 0; i < strlen(yes) + 1; i++) {
		// write character to screen
		int descriptorOffset = yes[i] - '?'; // subtract start character ('?') to get offset
		int bitmapOffset = tahoma_16ptDescriptors[descriptorOffset].offset;
		int width_bits = tahoma_16ptDescriptors[descriptorOffset].widthBits;
		int height_pixels = 21;
	 
		lcd_draw_char(yes_x_start, width_bits, yesno_y_start, height_pixels, tahoma_16ptBitmaps + bitmapOffset, LCD_COLOR_MAGENTA, LCD_COLOR_BLUE2, 0);
		yes_x_start = yes_x_start + width_bits + 2;
	}
	
	lcd_draw_box(no_x_start - 5, 37, yesno_y_start - 6, 30, LCD_COLOR_MAGENTA, LCD_COLOR_BLUE2, 2);
	
	// print "NO" text
	for (i = 0; i < strlen(no) + 1; i++) {
		// write character to screen
		int descriptorOffset = no[i] - '?'; // subtract start character ('?') to get offset
		int bitmapOffset = tahoma_16ptDescriptors[descriptorOffset].offset;
		int width_bits = tahoma_16ptDescriptors[descriptorOffset].widthBits;
		int height_pixels = 21;
	 
		lcd_draw_char(no_x_start, width_bits, yesno_y_start, height_pixels, tahoma_16ptBitmaps + bitmapOffset, LCD_COLOR_MAGENTA, LCD_COLOR_BLUE2, 0);
		no_x_start = no_x_start + width_bits + 2;
	}
	
	// wait until a touch event to continue
	while (1) {			
		touch_event = ft6x06_read_td_status();
		
		// get coordinates of touch and check if in boxes
		if (touch_event > 0) {
			touch_x = ft6x06_read_x();
			touch_y = ft6x06_read_y();
			printf("X: %d\t, Y: %d\n", touch_x, touch_y);
			
			// yes and no boxes
			yes_box = (touch_x >= 47) && (touch_x <= 104);
			no_box = (touch_x >= 146) && (touch_x <= 198);
			y_box = (touch_y > (yesno_y_start - 6)) && (touch_y < (yesno_y_start + 30 + 6));
			
			if (yes_box && y_box) { // if in "Yes"
				GAME_OVER = false;
				PLAY_AGAIN = true;
				DEAD_SCREEN = false;
				lcd_clear_screen(LCD_COLOR_BLUE2);
				return;
			} else if (no_box && y_box) { // if in "No"
				PLAY_AGAIN = false;
				DEAD_SCREEN = false;
				lcd_clear_screen(LCD_COLOR_BLUE2);
				return;
			} else { // in neither box
				continue;
			}
		}
	}
}

//*****************************************************************************
// Moves the bear one pixel every time it's called to create the motion of a jump
// when the right button is pressed
//*****************************************************************************
void move_bear(volatile uint16_t *y_coord){
					
	//If we're currently jumping, won't restart the jump when we click the right button
	if((BUTTON_PRESSED == BUTTON_RIGHT) &&!JUMPING){
		BUTTON_PRESSED = NONE_PRESSED; // so we can wait for another press
		JUMPING = true;
		ASCENDING = true; //start jump by asscending
		currSPEED = SPEED; //Don't want to change jump speed mid-jump
	}
	
	//////////////////////IF WE ARE JUMPING//////////////////////////////////
	if(JUMPING){
		
		//If we hit jump's peak, descend
		if(*y_coord <= 130)
			ASCENDING = false;
		
		///////////////JUMP DEPENDS ON SPEED///////////////////////////////////
		switch(currSPEED){
			case SPEED_FAST:
			  //Ascent of jump 
		    if(ASCENDING)
			    *y_coord = *y_coord - 3.5;
		    //Descent of jump
		    else if(!ASCENDING)
			    *y_coord = *y_coord + 4.5;
				break;
					
			case SPEED_SLOW:
		    if(ASCENDING)
			    *y_coord = *y_coord - 0.75;
		    //Descent of jump
		    else if(!ASCENDING)
			    *y_coord = *y_coord + 1;
				break;

			default: //For medium speed
		    if(ASCENDING)
			    *y_coord = *y_coord - 1.5;
		    //Descent of jump
		    else if(!ASCENDING)
			    *y_coord = *y_coord + 2;
			 	break;
		}
		////////////////////////////////////////////////////////////////
		
		//DONE with jump once we are at our original position
		if(*y_coord == 200){
			JUMPING = false;
			BUTTON_PRESSED = NONE_PRESSED;
		}
		
	}
}
//*****************************************************************************
// Moves the enemy one pixel every time it's called to create the motion across
// the screen
//*****************************************************************************
void move_enemy(volatile uint16_t *x_coord){
	int randNum;
	int i;
	
	//If the last enemy left the screen, generate a random new one
	if(contact_edge_enemy()){
		//NEW_ENEMY will remove our "ouch" rendering in main
		NEW_ENEMY = true;
		//Picks either 0 or 1
		randNum = rand() % 2; 
		
		if(randNum == 0){ //It'll be a stump!
			ENEMY_X_COORD = 240 - (stumpWidthPixels /2);
			EnemyBitmaps = stumpBitmaps;
			enemyWidthPixels = stumpWidthPixels;
			enemyHeightPixels = stumpHeightPixels;
			ENEMY_COLOR = LCD_COLOR_BROWN;
		}
		
		else{ //It'll be a snowball!
		  ENEMY_X_COORD = 240 - (snowballWidthPixels /2);
			EnemyBitmaps = snowballBitmaps;
			enemyWidthPixels = snowballWidthPixels;
			enemyHeightPixels = snowballHeightPixels;
			ENEMY_COLOR = LCD_COLOR_WHITE;
		}
		
	}
	
	
	switch(SPEED){
		case SPEED_FAST:
			*x_coord = *x_coord - 3;
			break;
		case SPEED_SLOW:
			*x_coord = *x_coord - 0.5;
		  break;	
		default: //USED for medium speed case
			*x_coord = *x_coord - 1.25;
		}
}

//*****************************************************************************
// Determines if any part of the enemy image would be off the screen as it continues on
// Returns true if it would be off screen, false if on screen
//*****************************************************************************

bool contact_edge_enemy(){
  if((ENEMY_X_COORD - (enemyWidthPixels / 2)) <= 10) { //If we are about to be off screen
		return true;
	}
	
	return false;
}

//*****************************************************************************
// Updates speed if joystick is right or left
//*****************************************************************************
SPEED_t update_speed()
{
	SPEED_t s;
	switch(PS2_DIR){
		case PS2_DIR_RIGHT:
			s = SPEED_FAST;
		  break;
		case PS2_DIR_LEFT:
			s = SPEED_SLOW;
		  break;
		default: //used for center case
			s = SPEED_MEDIUM;
		  break;
	
		return s;
	}
}

//*****************************************************************************
// Checks if there's an overlap between enemy and player and deducts from score
// if so
//*****************************************************************************
void recalculate_score()
{
	//Represents the rectangle perimeter of the bear
	Rectangle bear;
	//Represents the rectangle perimeter of the current enemy
	Rectangle enemy;
	
	if (game_begin) {
		SCORE = 0x1F;
		return;
	}
	//////////////////////////////////////////////////////////////////////
	//I'm going to take off a border of 10 pixels from each edge of both
	//images to account for the white space padding in their bitmaps
	//////////////////////////////////////////////////////////////////////
	
	bear.top = BEAR_Y_COORD - (bearHeightPixels/2) + 10;
	bear.bottom = BEAR_Y_COORD + (bearHeightPixels/2) - 10;
	bear.left = BEAR_X_COORD - (bearWidthPixels/2) + 10;
	bear.right= BEAR_X_COORD + (bearWidthPixels/2) - 10;
	
	enemy.top = ENEMY_Y_COORD - (enemyHeightPixels/2) + 10;
	enemy.bottom = ENEMY_Y_COORD + (enemyHeightPixels/2) - 10;
	enemy.left = ENEMY_X_COORD - (enemyWidthPixels/2) + 10;
	enemy.right= ENEMY_X_COORD + (enemyWidthPixels/2) - 10;
	
	//If one rectangle is on the complete left side of the other, no overlap
	if(enemy.left > bear.right || bear.left > enemy.right)
		return;

	//If one rectangle is completely above the other, no overlap
	if(enemy.top > bear.bottom || bear.top > enemy.bottom)
		return;
	
	if (SCORE == 0) {
		GAME_OVER = true;
		DEAD_SCREEN = true;
	}
	
	//If the above conditions are not met, they are overlapping
	if (!WAIT_SCORE) {
		SCORE = (int)SCORE >> (int)1;
		WAIT_SCORE = true;
		//Render the "ouch" picture
		lcd_draw_image(140, ouchWidthPixels, 90, ouchHeightPixels, ouchBitmaps, LCD_COLOR_MAGENTA, LCD_COLOR_BLUE2);
	}


  return;
}

//*****************************************************************************
// Draws the snow at the bottom of the screen
//*****************************************************************************
void draw_snow(void){
	lcd_draw_rectangle_centered(120, 238, 270, 35, LCD_COLOR_WHITE);
	
}

//*****************************************************************************
// Our main driver that is consitently called until the player loses
//*****************************************************************************
void game_main(void) {
	int wait; //use in loop below
	GAME_RUNNING = true;
	
  //Updates the score on the RED leds 
	io_expander_write_reg(MCP23017_GPIOA_R, SCORE);

	//Renders our constant background
	draw_snow();
	
	////////////////////////////////////////////////////////////////////////////
	/////////////////If GPIOF detected a push button was pressed////////////////
	////////////////////////////////////////////////////////////////////////////
	if (ALERT_BUTTON) {
		ALERT_BUTTON = false;
		io_expander_debounce();
	}
	
  ////////////////////////////////////////////////////////////////////////////
	////////If ADC0 is triggered by Timer4A , update the bear's speed///////////
	////////////////////////////////////////////////////////////////////////////
	if (ALERT_SPEED) {
		ALERT_SPEED = false;
		SPEED = update_speed();
	}

  ////////////////////////////////////////////////////////////////////////////
	//////If Timer3A detects its time to re-render the bear and enemy///////////
	////////////////////////////////////////////////////////////////////////////
	if(ALERT_ENEMY){
		ALERT_ENEMY = false;
		//Don't render the enemies right away
		if (game_begin) {
			game_begin = false;
			for (wait = 0; wait < 9000000; wait++) {}
			ENEMY_X_COORD = 238 - (enemyWidthPixels / 2);
		}
		lcd_draw_image(ENEMY_X_COORD, enemyWidthPixels, ENEMY_Y_COORD, enemyHeightPixels, EnemyBitmaps, ENEMY_COLOR, LCD_COLOR_BLUE2);
		//Doing this here bc move_bear does not necessarily get called at the same time as this ldc_draw_image
		//So it needs to disappear a bit before it hits the edge
    //If it's about to hit the edge, "erase" the image
		if((ENEMY_X_COORD - (enemyWidthPixels / 2)) <= 20){
			lcd_draw_image(ENEMY_X_COORD, enemyWidthPixels, ENEMY_Y_COORD, enemyHeightPixels, EnemyBitmaps, LCD_COLOR_BLUE2, LCD_COLOR_BLUE2);
		}
	}
	
	if(ALERT_BEAR){
		ALERT_BEAR = false;
		lcd_draw_image(BEAR_X_COORD, bearWidthPixels, BEAR_Y_COORD, bearHeightPixels, bearBitmaps, LCD_COLOR_BLUE, LCD_COLOR_BLUE2);
	}
	
	////////////////////////////////////////////////////////////////////////////
	//////Internal signal asserted to indicate removal of "ouch" image//////////
	////////////////////////////////////////////////////////////////////////////
	if(NEW_ENEMY){
		lcd_draw_image(140, ouchWidthPixels, 90, ouchHeightPixels, ouchBitmaps, LCD_COLOR_BLUE2, LCD_COLOR_BLUE2);
		NEW_ENEMY = false;
	}
	
	//Recalculates score if an enemy is overlapping with the bear
	recalculate_score();
	
	//Re-renders the red LEDs on the left to indicate how many lives are left
	io_expander_write_reg(MCP23017_GPIOA_R, SCORE);
}
