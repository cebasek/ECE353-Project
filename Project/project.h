#ifndef __PROJECT_H__
#define __PROJECT_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "TM4C123.h"
#include "gpio_port.h"
#include "ps2.h"
#include "lcd.h"
#include "timers.h"
#include "project_images.h"
#include "project_hardware_init.h"
#include "project_interrupts.h"
#include "main.h"

extern volatile uint16_t BEAR_X_COORD; 
extern volatile uint16_t BEAR_Y_COORD;
extern volatile bool ALERT_BEAR;
 
extern volatile uint8_t HIGH_SCORE;
extern volatile bool ALERT_READY_SCREEN;

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
// Will be the main game driver
//************************************************************************
void game_main();

#endif
