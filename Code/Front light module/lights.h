
/*********************************************************************************************************************

	Driver for PWM controlled constant current high power LED drivers              
                                                                                                   
	Author:		Jostein Furseth						03.05.2013					                

**********************************************************************************************************************/


#ifndef LED_DRIVERS_H_
#define LED_DRIVERS_H_

void init_lights(void);
void set_headlights(int);
void set_eye_brows(int);
void set_left_turn(int);
void set_angel_eyes(int);
void set_right_turn(int);



#endif /* LED_DRIVERS_H_ */