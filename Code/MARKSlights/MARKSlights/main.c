#define F_CPU 8000000UL  // 1 MHz
#include "universal_module.h"
#include "lights.h"
#include <util/delay.h>
void init_timer(void);

int main(){
	init_lights();
	RGB_init();
	set_angel_eyes(1);
//	RGB_led_on(LED_RED);
	int cntr = 0;
	while(1){
		switch(cntr){
			case 1:
					set_headlights(128);
					break;
			case 2:
					set_eye_brows(1);
					break;
			case 3:
					set_left_turn(1);
					break;
			case 4:
					set_right_turn(1);
					break;
			case 5:
					set_angel_eyes(1);
					break;
		}
		RGB_toggle(LED_BLUE);
		_delay_ms(5000);		
		cntr = cntr%5;
		cntr++;
		set_headlights(0);
		set_eye_brows(0);
		set_angel_eyes(0);
		set_left_turn(0);
		set_right_turn(0);
	}
}

