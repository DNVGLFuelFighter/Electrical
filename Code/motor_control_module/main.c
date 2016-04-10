/*
 * Motor control with pwm to esc
 *
 * Created: 03/04/2016 
 * Author : Mark Hocevar
 */ 
#include "motor_control.h"
uint16_t tar_speed = 1024; //10bit
uint16_t old_speed = 1024;

int main(void){
	init();

	RGB_init();
	RGB_led_on(LED_BLUE);

	while(1){
		esc_telemetry_recv();
		if(tar_speed < old_speed){
			set_speed(tar_speed);
			old_speed = tar_speed;
		}else if(tar_speed > old_speed){
			while(tar_speed > old_speed){	
				old_speed = old_speed + 10;
				set_speed(old_speed);	
				_delay_ms(20);
			}
		}

	}
}



