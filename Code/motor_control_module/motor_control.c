/*
 * Motor control 
 *
 * Created 09/04/2016
 * Author: Mark Hocevar
 * DNV GL Fuel Fighter 2016
 **
 *
 */
#include "motor_control.h"

//Variables for screen reading (TODO in struct)
int screen_idx = 0; //Idx of screen cursor 
int reading = 0; 	// 0 -> not reading;  1-> reading (found first byte)
unsigned char screen[34];
esc myesc;

/* Init of motor control module  */
void init(void){
	printf("Init complete\n\r");
	//Serial comunication with computer init
	USART_init(MYUBRR, true);
	//Esc telemetry init
	esc_telemetry_init(MYUBRR_ESC);
	//Timer3 for pwm control init
	pwm_init();
	//Can setup
	//TODO id of package and mask for can
	can_init();
	prepare_rx(0, ID_steeringWheel, 0xff, can_recv);
	//Onboard led init
	RGB_init();
	//Timer for sending can
	timer_init();
	sei();
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

/* Decode function for screen of telemetry (data reveived with telemetry) */
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




/* Set speed (pwm value) of motors  */
void set_speed(uint16_t target){	
	if(((MAXPWM_CNT - target) > MAXSPEED) && ((MAXPWM_CNT - target) < MINSPEED)){
		OCR3B = MAXPWM_CNT - target;
	}
}

/* Function called on can receive package  */
void can_recv(CAN_packet *p, unsigned char mob){
	//Parse data from message
	//TODO do some checking of data so we don't get stupid stuff
	uint8_t recv =  p->data[1] ;
	if((tar_speed + 10) < (1024 + recv*4) || (tar_speed - 10) > 1024+recv*4){
		tar_speed = 1024 + recv*4;		
	}
	printf("Package received: %d\n\r", tar_speed);
}

/* Send telemetry data from esc to can */
//TODO make a timer to send this every x seconds =)
void can_send(){
	CAN_packet msg;
	msg.id = ID_esc_telemetry;
	msg.length = 5;
	msg.data[0] = myesc.percentage;
	msg.data[1] = myesc.temp;
	msg.data[2] = myesc.voltage;
	msg.data[3] = (uint8_t) (myesc.rpm >> 8);
	msg.data[4] = (uint8_t) (myesc.rpm);
	can_packet_send(4, &msg);
}

/* Timer for sending can message every x seconds */
void timer_init(){
	//1024prescaler -> 128us
	TCCR1A = 0;
	TCCR1B = (1<<WGM12);
	TCCR1C = 0;
	TIMSK1 = (1 << OCIE1A);


	OCR1A = 15625; //2s 	

	TCCR1B |= (1<<CS10) | (1<<CS12); 
}

ISR(TIMER1_COMPA_vect){
	cli();
	printf("Sending package\n\r");
	can_send();
	sei();
}
