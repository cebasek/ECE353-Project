#ifndef __PROJECT_IMAGES_H__
#define __PROJECT_IMAGES_H__

#include <stdint.h>
#include "bitmap_db.h"

//Bitmap info for welcome screen text
// Font data for Lucida Calligraphy 12pt

extern const uint8_t lucidaCalligraphy_12ptBitmaps[];
extern const FONT_INFO lucidaCalligraphy_12ptFontInfo;
extern const FONT_CHAR_INFO lucidaCalligraphy_12ptDescriptors[];

// Bitmap info for bear
extern const uint8_t bearBitmaps[];
extern const uint8_t bearWidthPixels;
extern const uint8_t bearHeightPixels;


// Font data for Tahoma 28pt
extern const uint8_t tahoma_28ptBitmaps[];
extern const FONT_INFO tahoma_28ptFontInfo;
extern const FONT_CHAR_INFO tahoma_28ptDescriptors[];

// Font data for Tahoma 36 pt for 3 in countdown
extern const uint8_t tahoma_36ptBitmaps[];
extern const FONT_INFO tahoma_36ptFontInfo;
extern const FONT_CHAR_INFO tahoma_36ptDescriptors[];

#endif
