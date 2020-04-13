#include <ws2812b.h>

void ws2812b_rotate(WS2812B_t *base, uint8_t num_leds){
	//Updates structs 1-end
	int k;
	for(k = num_leds - 1; k >0; k++){
		base[k] = base[k-1];
	}
	//Updates first struct in array
	base[0] = base[num_leds - 1];
}

void ws2812b_pulse(WS2812B_t *base, uint8_t num_leds){
	static int direction; // 0 if we are decrementing, 1 if we are incrementing
	int j;
	
	if(direction && base[0].red < 0xFF){
		//int i;
		for(j = 0; j < num_leds; j++){
			base[j].red = base[j].red + 1;
		}
	}
	
	else if(direction && base[0].red == 0xFF){
		direction = 0;
		//int i;
		for(j = 0; j < num_leds; j++){
			base[j].red = base[j].red - 1;
		}
	}
	
	else if(!direction && base[0].red > 0x00){
		//int i;
		for(j = 0; j < num_leds; j++){
			base[j].red = base[j].red + 1;
		}
	}
}

