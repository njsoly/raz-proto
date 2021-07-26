/****************** main.c ******************/

#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>

// gpioSetMode(unsigned gpio, unsigned mode)
// gpio 0-53, mode 0-7
//
// uses GPIO## numbering
// examples: 
//	set GPIO17 as input:
//		gpioSetMode(17, PI_INPUT); 
//	set GPIO18 as output:
//		gpioSetMode(18, PI_OUTPUT);
//
// int gpioRead (unsigned gpio); // read it.
// 
// int gpioWrite(unsigned gpio, unsigned level);
// 	sets GPIO level, on or off (1 or 0)
//	also: if PWM or servo pulses are active on 
// 	the GPIO, they are switched off.
//
// gpioTick - get current microsecond tick number
// gpioTime - get current time
// gpioSleep - sleep for a time. (what unit?)
// time_sleep - sleeps for a float number of seconds.
//
//	in order to use PWM on BCM12 and BCM13, need to set gpio mode to PI_ALT0
//	in order to use PWM on BCM18, need to set gpio mode to PI_ALT5

// #define J8_8	4
// #define J8_10	15
// #define J8_11	17
// #define J8_12	18
// #define J8_13	27
// #define J8_15	22
// #define J8_32	12
// #define J8_33	13
// #define J8_36	16
// #define J8_37	26
// #define J8_38	20
// #define J8_40	21
// 
// #define PWM0_PIN	12
// #define PWM1_PIN	13
// 
// #define LMOTOR_PWM 	PWM0_PIN
// #define LMOTOR_DIR	J8_36
// #define RMOTOR_PWM	PWM1_PIN
// #define RMOTOR_DIR	J8_37
// 
// // these may or may not need to be used.
// #define MOTOR_NSLP	

#include "raz_follower_defs.h"

using namespace std;

void blinkGpio(int gpio){
	for(int i = 0; i < 10; i++){
		gpioWrite(gpio, i % 2);
		time_sleep(0.5f);		
	}	
  
}
void leftMotor(int l){
	gpioWrite(MOTOR_NSLP, 1);
	if(l < 0){
		gpioWrite(LMOTOR_DIR, 0);
		l = 0 - l;
	}
	else
		gpioWrite(LMOTOR_DIR, 1);
	
	gpioPWM(LMOTOR_PWM, l);
}
void rightMotor(int r){
	gpioWrite(MOTOR_NSLP, 1);
	if(r < 0){
		gpioWrite(RMOTOR_DIR, 1);
		r = 0 - r;
	}
	else
		gpioWrite(RMOTOR_DIR, 0);
	
	gpioPWM(RMOTOR_PWM, r);
	
}

void motors_forward(){
	leftMotor(FULL_BORE);
	rightMotor(FULL_BORE);
}
void motors_backward(){
	leftMotor(-FULL_BORE);
	rightMotor(-FULL_BORE);
}
void motors_stop(){
	leftMotor(0);
	rightMotor(0);
	gpioWrite(MOTOR_NSLP, 0);
	
}

int main(){
	// surround with silent try-catch-move-on?

	std::cout << "hello and welcome." << std::flush;

	if(initGpio() == 1){ return 1; }
	if(atexit(closeGpio) != 0){
		cerr << "could not register closeGpio() to atexit()." << endl;
		return 1;
	}
	cout << "\rgoing forward..    " << flush;
	motors_forward();
	time_sleep(2.0f);
	
	leftMotor(0);
	rightMotor(0);
	
	cout << "\rnow going backward." << flush;
	motors_backward();
	time_sleep(2.0f);
	
	motors_stop();
	
	cout << ("\rbye.                    \r\n");
	//closeGpio();
	return 0;
}


