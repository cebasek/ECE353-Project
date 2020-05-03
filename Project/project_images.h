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

// Font data for Tahoma 16pt for high score
extern const uint8_t tahoma_16ptBitmaps[];
extern const FONT_INFO tahoma_16ptFontInfo;
extern const FONT_CHAR_INFO tahoma_16ptDescriptors[];

#endif
