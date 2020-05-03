#ifndef __PROJECT_IMAGES_H__
#define __PROJECT_IMAGES_H__

#include <stdint.h>
#include "bitmap_db.h"

// Font data for Lucida Calligraphy 12pt for welcome screen text
extern const uint8_t lucidaCalligraphy_12ptBitmaps[];
extern const FONT_INFO lucidaCalligraphy_12ptFontInfo;
extern const FONT_CHAR_INFO lucidaCalligraphy_12ptDescriptors[];

// Bitmap info for bear
extern const uint8_t bearBitmaps[];
extern const uint8_t bearWidthPixels;
extern const uint8_t bearHeightPixels;

// Font data for Tahoma 28pt for GET READY
extern const uint8_t tahoma_28ptBitmaps[];
extern const FONT_INFO tahoma_28ptFontInfo;
extern const FONT_CHAR_INFO tahoma_28ptDescriptors[];

// Font data for Tahoma 48pt for countdown
extern const uint8_t tahoma_48ptBitmaps[];
extern const FONT_INFO tahoma_48ptFontInfo;
extern const FONT_CHAR_INFO tahoma_48ptDescriptors[];

// Bitmap info for stump
extern uint8_t stumpBitmaps[];
extern uint8_t stumpWidthPixels;
extern uint8_t stumpHeightPixels;

// Bitmap info for snowball
extern uint8_t snowballBitmaps[];
extern uint8_t snowballWidthPixels;
extern uint8_t snowballHeightPixels;

//These will change as the current enemy on the screen changes
extern uint8_t *EnemyBitmaps;
extern uint8_t enemyWidthPixels;
extern uint8_t enemyHeightPixels;

// Font data for Tahoma 16pt for high score
extern const uint8_t tahoma_16ptBitmaps[];
extern const FONT_INFO tahoma_16ptFontInfo;
extern const FONT_CHAR_INFO tahoma_16ptDescriptors[];

// Font data for Tahoma 10pt for high score words
extern const uint8_t tahoma_10ptBitmaps[];
extern const FONT_INFO tahoma_10ptFontInfo;
extern const FONT_CHAR_INFO tahoma_10ptDescriptors[];

#endif
