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

extern volatile uint16_t BEAR_X_COORD; 
extern volatile uint16_t BEAR_Y_COORD;
extern volatile bool ALERT_BEAR;
 
//************************************************************************
// Prints a welcome message to the screen upon reset of game
//************************************************************************
void print_welcome();

//************************************************************************
// Prints different levels for user to select from
//************************************************************************
void print_levels();


//************************************************************************
// Will be the main game driver
//************************************************************************
void game_main();

#endif