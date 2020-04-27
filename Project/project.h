#ifndef __PROJECT_H__
#define __PROJECT_H__
 
#include "main.h"
#include "project_hardware_init.h"
#include "project_images.h"
 
extern volatile uint8_t HIGH_SCORE;
extern volatile bool ALERT_READY_SCREEN;

//************************************************************************
// Prints a welcome message to the screen upon reset of game
//************************************************************************
void print_welcome(void);

//************************************************************************
// Prints different levels for user to select from
//************************************************************************
void print_ready(void);

#endif
