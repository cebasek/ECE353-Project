#ifndef __PROJECT_H__
#define __PROJECT_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "TM4C123.h"
#include "gpio_port.h"
#include "ps2.h"
#include "lcd.h"
#include "timers.h"
#include "project_images.h"
#include "project_hardware_init.h"
#include "project_interrupts.h"
#include "serial_debug.h"

#include "main.h"

extern volatile uint16_t BEAR_X_COORD; 
extern volatile uint16_t BEAR_Y_COORD;

extern volatile uint16_t ENEMY_X_COORD;
extern volatile uint16_t ENEMY_Y_COORD;


extern volatile bool ALERT_BEAR;

extern volatile bool ALERT_BUTTON;
extern volatile bool ALERT_ENEMY;
extern volatile bool ALERT_SPEED;

extern volatile uint8_t CUR_SCORE;
extern volatile uint8_t HIGH_SCORE;
extern volatile bool GAME_OVER;
extern volatile bool GAME_RUNNING;


//************************************************************************
// Prints a welcome message to the screen upon reset of game
//************************************************************************
void print_welcome(void);

//************************************************************************
// Prints GET READY screen
//************************************************************************
void print_ready(void);

//************************************************************************
// Prints countdown
//************************************************************************
void print_countdown(void);

//************************************************************************
// Displays high score on startup
//************************************************************************
void print_high_score(void);

//*****************************************************************************
// Displays current score
//*****************************************************************************
void print_cur_score(void);
	
//************************************************************************
// Will be the main game driver
//************************************************************************
void game_main(void);

//************************************************************************
// Determines when to move the bear
//************************************************************************
void move_bear(volatile uint16_t *y_coord);

//************************************************************************
// Determines when to move the bear
//************************************************************************
void move_enemy(volatile uint16_t *x_coord);


//************************************************************************
// Determines when to move the bear
//************************************************************************
bool contact_edge_enemy();


#endif
