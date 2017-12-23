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
//
// 
// #define PWM0_PIN	12
// #define PWM1_PIN	13
// 
// #define LMOTOR_PWM 	12
// #define LMOTOR_DIR	16
// #define RMOTOR_PWM	13
// #define RMOTOR_DIR	26

#include "raz_follower_defs.h"

using namespace std;

long int rc_values[3]={0,0,0};
uint16_t rc_charge_us = 50;

long int* qtr3rc_read(){
	gpioSetPad(0, 16); // the sensors want 17mA, let's at least get 16 (the max).
	// set the RC sensor pins to output, and set HIGH to charge
	gpioSetMode(RC_2, PI_OUTPUT);
	gpioSetMode(RC_1, PI_OUTPUT);
	gpioSetMode(RC_3, PI_OUTPUT);
	gpioWrite(RC_2, 1);
	gpioWrite(RC_1, 1);
	gpioWrite(RC_3, 1);
	gpioDelay(rc_charge_us);

	gpioSetMode(RC_2, PI_INPUT);
	gpioSetMode(RC_1, PI_INPUT);
	gpioSetMode(RC_3, PI_INPUT);
	
	long int ticks[3]{0,0,0};
	long unsigned int tickStart = gpioTick();
	printf("tickStart: %lu\n", (tickStart));
	
	// keep reading whichever ones are HIGH, until they're all low.
	while(ticks[0] == 0 || ticks[1] == 0 || ticks[2] == 0){
		if(ticks[0] == 0 && (gpioRead(RC_1) == 0)){
			ticks[0] = gpioTick();
		}
		if(ticks[2] == 0 && (gpioRead(RC_3) == 0)){
			ticks[2] = gpioTick();
		}
		if(ticks[1] == 0 && (gpioRead(RC_2) == 0)){
			ticks[1] = gpioTick();
		}
	}
	
	rc_values[0] = (int) (ticks[0] - tickStart);
	rc_values[1] = (int) (ticks[1] - tickStart);
	rc_values[2] = (int) (ticks[2] - tickStart);
	
	
	
	return &(rc_values[0]);
}


int main(){
	// surround with silent try-catch-move-on?

	std::cout << "hello and welcome." << std::flush;

	if(initGpio() == 1){ return 1; }
	if(atexit(closeGpio) != 0){
		cerr << "could not register closeGpio() to atexit()." << endl; return 1;
	}
// 	cout << "\rgoing forward..    " << flush;
// 	motors_forward();
// 	time_sleep(2.0f);
// 	
// 	leftMotor(0);
// 	rightMotor(0);
// 	
// 	cout << "\rnow going backward." << flush;
// 	motors_backward();
// 	time_sleep(2.0f);
// 	
// 	motors_stop();
	gpioSetPad(0, 16);
	cout << "reading sensors:  " << endl;
	for(int i = 0; i < 100; i++){
		qtr3rc_read();
		printf("%lu\t%lu\t%lu\t   \n", rc_values[0], rc_values[1], rc_values[2] );
		time_sleep(0.250f);
	}


	cout << ("\rbye.                    \r\n");
	
	return 0;
}


class Raz_Motors {

	// 
	Raz_Motors(uint8_t leftPwm, uint8_t leftDir, 
			  uint8_t rightPwm, uint8_t rightDir,
			  uint8_t notsleep){
		
	}
	
	
};



