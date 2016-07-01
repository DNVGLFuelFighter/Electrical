/*
 * Motor control with pwm to esc
 *
 * Created: 03/04/2016 
 * Author : Mark Hocevar
 * Edited: 06/28/2016
 * Editor: Vilius Ciuzelis
 */ 

#include "motor_control.h"
uint16_t tar_speed = 1024; //10bit
uint16_t old_speed = 1024;

int main(void){
	init();
	printf("\r\nHello world");
	RGB_init();
	RGB_led_on(LED_BLUE);
	
	

	int cnt = 0;

	while(1){
		//printf("\tTarget speed: %d\n\r", tar_speed);
		//_delay_ms(100);
		
	/*	
		cnt++;
		if(tar_speed < old_speed){
			set_speed(tar_speed);
			old_speed = tar_speed;
		}else if(tar_speed >= old_speed){
			set_speed(tar_speed);
			old_speed = tar_speed;
			//slow speed
			 while(tar_speed > old_speed){
				printf("Slow speed change %d -> %d\n\r", old_speed, tar_speed);
				old_speed = old_speed + 10;
				set_speed(old_speed);	
				_delay_ms(20);
			/
		}*/

	}
}



