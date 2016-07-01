/*
 * Motor control 
 *
 * Created 09/04/2016
 * Author: Mark Hocevar
 * DNV GL Fuel Fighter 2016
 * Edited: 7/1/2016
 * Edited by: Vilius Ciuzelis
 *
 */
#include "motor_control.h"

// Cruise control values
BOOL cc = FALSE;
uint16_t ref = 0;
uint16_t y; //output
uint16_t curr_speed = 100;
double Ppart = 0;
double Ipart = 0;
double Kp = 100;
double Ki = 10;
double delta_t = 0.0005;

//Variables for screen reading (TODO in struct)
int screen_idx = 0; //Idx of screen cursor 
int reading = 0; 	// 0 -> not reading;  1-> reading (found first byte)
unsigned char screen[34];
esc myesc;


uint8_t brake = 0; 	// 0 -> brake not pressed; 1-> brake pressed do not run motors
uint8_t can_lost = 0;  // 0 -> we have can;  1 -> no can communication
/* Init of motor control module  */
void init(void){
	//Serial communication with computer init
	USART_init(MYUBRR, true);
	//Esc telemetry init
	//esc_telemetry_init(MYUBRR_ESC);
	//Timer3 for pwm control init
	pwm_init();
	//Can setup
	//TODO id of package and mask for can
	can_init();
	prepare_rx(0, ID_steeringWheel, MASK_MOTOR_MODULE, can_recv_no_cc());
	//On-board led init
	RGB_init();
	//Timer for sending can
	timer_init();
	
	printf("Init complete\n\r");
	sei();
}

uint16_t PI(uint16_t curr_speed){
	int error = ref - curr_speed;
	printf("\r\nerror %u",error);
	Ppart = error * Kp;
	if ((Ppart > 0 && y < 255 )||(Ppart < 0 && y > -255 )) {/**antiwindup condition**/
		Ipart = Ki*(Ipart + error*delta_t);
		//Dpart= (kd *(error - lasterror ))/deltaT;
	}
	y = Ppart+Ipart; //total output
	//lasterror = error
	return y;
}

void can_recv_cc(CAN_packet *p, unsigned char mob){
	//Parse data from message
	//TODO do some checking of data so we don't get stupid stuff
	uint16_t recv;
	switch(p->id) {
		case ID_brakes:
			//Set brake variable
			can_lost = 0; //safety if can is lost we kill the motors
			if(p->data[0] == 0){
				brake = 0;
				}else if(p->data[0] == 1){
				brake = 1;
				}else {
				brake = 1;
			}
			break;
		case ID_steeringWheel:
			if(!test_bit(p->data[0], (1<<6))) { // if cruise control on
				cc = TRUE;
				ref = p->data[2]; // set reference value for the controller
			} else {
				cc = FALSE;
				recv =  p->data[1];
// 				if((tar_speed + 10) < (1024 + recv*4) || (tar_speed - 10) > 1024+recv*4){
// 					tar_speed = 1024 + recv*4;
// 				}
			}
			break;
		case ID_hallSensor:
			curr_speed = p->data[0]*256 + p->data[1];
			curr_speed = curr_speed/100;
			break;
	}
	//PI-controller
	if(cc)
		recv = PI(curr_speed);
	set_speed(recv);
}


/* Set speed (pwm value) of motors  */
void set_speed(uint16_t target){
	//printf("\r\ntarget - %u",target);
	if((tar_speed + 10) < (1024 + target*4) || (tar_speed - 10) > 1024+target*4){
		tar_speed = 1024 + target*4;
	}
	//printf("\r\ntar_speed = %u", tar_speed);
// 	if(brake == 1){ //should be 1
// 		//Braking do no turn motors
// 		OCR3B = MINSPEED;
// 		}else{
// 		if(((MAXPWM_CNT - tar_speed) > MAXSPEED) && ((MAXPWM_CNT - tar_speed) < MINSPEED)){
// 			OCR3B = MAXPWM_CNT - tar_speed;
// 		}
// 	}
}

/* Init hardware PWM on timer 3, output compare B (pin PE4) */ 
void pwm_init(void){
	/*
	 * Timer3, output compare B, clear on match, set on top
	 * Prescaler = 8Mhz / 1 (fuse) / 8 (timer prescaler)
	 * mode = 15 (fast pwm with top on)
	 * ocr3a
	 * */
	TCCR3A = (1 << COM3B1) | (1 << COM3B0) | (1 << WGM31) | (1 << WGM30);
	TCCR3B = (1 << WGM33) | (1 << WGM32) ;
	TCCR3C = 0;

	uint16_t duty = MAXPWM_CNT - 1024;
	OCR3A = MAXPWM_CNT; 
	OCR3B = duty;
	
	DDRE |= (1 << DDE4); //set pin as output

	TCCR3B |= (1<<CS31); //start timer with prescaler 1;
}

/* Init serial communication for telemetry */
void esc_telemetry_init(unsigned int baud){	
	uint8_t rx_data_ready = 0;
	UBRR1H = (unsigned char) (baud>>8);
	UBRR1L = (unsigned char) baud;
	/* format 9600baud; 9bit; 2stop bit; odd parity*/
	UCSR1C = (1<<UPM11) | (1<<UPM10) | (1<<USBS1) | (1<<UCSZ11) | (1<<UCSZ10);	
	UCSR1B = (1<<RXEN1) | (1<<UCSZ12);
	rx_data_ready = UDR1; 
	rx_data_ready = 0;	
}

/* Receive function for telemetry (serial port 1) */
void esc_telemetry_recv(void){
	unsigned char status, data, bit8;
	//Wait for data
	while(!(UCSR1A & (1<<RXC1))); 

	status = UCSR1A;
	bit8 = UCSR1B;
	data = UDR1;
	bit8 = (bit8 >> 1) & 0x01;
	if(status &((1<<FE1) | (1<<DOR1) | (1<<UPE1))){
		RGB_toggle(LED_RED);
		screen_idx = 0;
		return;
	}
	if(bit8 == 0){
		if(data == 0xfe){
			//Start of the screen
			reading = 1;
			screen_idx = 0;
		}else if(data == 0xff && screen_idx == 32){
			//End of the screen
			reading = 0;
			screen[33] = '\0';
			decode_screen();
			screen_idx = 0;
		}
	}else if(reading){
		screen[screen_idx] = data;
		screen_idx++;
	}
}

/* Decode function for screen of telemetry (data received with telemetry) */
//TODO check esc pointers 
void decode_screen(){
	RGB_led_on(LED_BLUE);
	char tmpstr[6];
	//Get throttle percentage
	memcpy(tmpstr, &screen[1], 3);
	tmpstr[3] = '\0';
	myesc.percentage = atoi(tmpstr);
	//Get rpm
	memcpy(tmpstr, &screen[9], 4);
	tmpstr[4] = '\0';
	myesc.rpm = atoi(tmpstr);
	//Get voltage 
	memcpy(tmpstr, &screen[16], 3);
	tmpstr[3] = '\0';
	myesc.voltage = atof(tmpstr);
	//Get temperature
	memcpy(tmpstr, &screen[27], 3);
	tmpstr[3] = '\0';
	myesc.temp = atoi(tmpstr);
	/*printf("----------\n\r");
	printf("Perc: %d\n\r", myesc.percentage);
	printf("rpm : %d\n\r", myesc.rpm);
	printf("volt: %d\n\r", myesc.voltage);
	printf("temp: %d\n\r", myesc.temp);
*/
	RGB_led_off(LED_BLUE);
}

/* Function called on can receive package  */
void can_recv_no_cc(CAN_packet *p, unsigned char mob){
	//Parse data from message
	//TODO do some checking of data so we don't get stupid stuff
// 	if(p->id == ID_brakes){
// 		//Set brake variable
// 		can_lost = 0; //safety if can is lost we kill the motors
// 		if(p->data[0] == 0){
// 				brake = 0;
// 			}else if(p->data[0] == 1){
// 				brake = 1;
// 			}else{
// 				brake = 1;
// 		}
// 		// 	printf("got brake data %d\n\r", brake);
// 		} else 
	if(p->id == ID_steeringWheel){
		printf("\r\nSpeed ref - %d",p->data[1]);
		uint8_t recv =  p->data[1] ;
		if((tar_speed + 10) < (1024 + recv*4) || (tar_speed - 10) > 1024+recv*4){
			tar_speed = 1024 + recv*4;
		}
	}
		//printf("Package received: %d\n\r", tar_speed);
	/*}*/
	//printf("Target speed: %d\n\r", tar_speed);
	set_speed(tar_speed);
}

/* Send telemetry data from esc to can */
//TODO make a timer to send this every x seconds =)
// void can_send(){
// 	CAN_packet msg;
// 	msg.id = ID_esc_telemetry;
// 	msg.length = 5;
// 	msg.data[0] = myesc.percentage;
// 	msg.data[1] = myesc.temp;
// 	msg.data[2] = myesc.voltage;
// 	msg.data[3] = (uint8_t) (myesc.rpm >> 8);
// 	msg.data[4] = (uint8_t) (myesc.rpm);
// 	can_packet_send(4, &msg);
// }

/* Timer for sending can message every x seconds */
void timer_init(){
	//1024prescaler -> 128us
	TCCR1A = 0;
	TCCR1B = (1<<WGM12);
	TCCR1C = 0;
	TIMSK1 = (1 << OCIE1A);

	OCR1A = 15625; //2s 
//	OCR1A = 65000;

	TCCR1B |= (1<<CS10) | (1<<CS12); 
}

void brake_timer_init(){
	//1024prescaler -> 128us
	TCCR1A = 0;
	TCCR1B = (1<<WGM12);
	TCCR1C = 0;
	TIMSK1 = (1 << OCIE1A);

	OCR1A = 15625; //2s 
//	OCR1A = 65000;

	TCCR1B |= (1<<CS10) | (1<<CS12); 
}

ISR(TIMER1_COMPA_vect){
	cli();
	if(can_lost == 1){
		//printf("Game (of thrones) over\n\r");
		//If you want it to work remove comment
		//brake = 1;
	}
	can_lost = 1;
	/*can_send();*/
	sei();
}


