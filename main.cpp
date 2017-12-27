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

uint32_t rc_values[3]={0,0,0};
uint16_t rc_charge_us = 50;

uint32_t* qtr3rc_read_parallel(){
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
	
	uint32_t ticks[3]{0,0,0};
	uint32_t tickStart = gpioTick();
	printf("tickStart: %d\n", (tickStart));
	
	// keep reading whichever ones are HIGH, until they're all low.
	while(ticks[0] == 0 || ticks[1] == 0 || ticks[2] == 0){
		if(ticks[0] == 0 && (gpioRead(RC_1) == 0)){
			ticks[0] = gpioTick();
		}
		if(ticks[1] == 0 && (gpioRead(RC_2) == 0)){
			ticks[1] = gpioTick();
		}
		if(ticks[2] == 0 && (gpioRead(RC_3) == 0)){
			ticks[2] = gpioTick();
		}
	}
	
	rc_values[0] = (ticks[0] - tickStart);
	rc_values[1] = (ticks[1] - tickStart);
	rc_values[2] = (ticks[2] - tickStart);
	
	
	
	return &(rc_values[0]);
}
/** this version of the method doesn't try to charge or read the sensors
 * in a parallel fashion
 */
uint32_t* qtr3rc_read(){
	uint32_t ticks[3]{0,0,0};
	uint32_t tickStart = gpioTick();
	
	printf("tickStart: %d\n", (tickStart));
	
	gpioSetPad(0, 16); // the sensors want 17mA, let's at least get 16 (the max).
	if(gpioGetPad(0) != 16) {
		printf("ERROR: pad strength set didn't work.\n");
	}
	
	gpioSetMode(RC_2, PI_INPUT);
	gpioSetMode(RC_1, PI_INPUT);
	gpioSetMode(RC_3, PI_INPUT);

	// read sensor 1
	tickStart = gpioTick();
	gpioSetMode(RC_1, PI_OUTPUT);
	gpioWrite(RC_1, 1);
	gpioDelay(rc_charge_us);
	gpioSetMode(RC_1, PI_INPUT);
	while(ticks[0] == 0){
		if((gpioRead(RC_1) == 0)){
			ticks[0] = gpioTick();
		}
	}
	rc_values[0] = (ticks[0] - tickStart);
	printf("sensor 1: %u = %u - %u\n", rc_values[0], ticks[0], tickStart);

	// read sensor 2
	tickStart = gpioTick();
	gpioSetMode(RC_2, PI_OUTPUT);
	gpioWrite(RC_2, 1);
	gpioDelay(rc_charge_us);
	gpioSetMode(RC_2, PI_INPUT);
	while(ticks[1] == 0){
		if(ticks[1] == 0 && (gpioRead(RC_2) == 0)){
			ticks[1] = gpioTick();
		}
	}
	rc_values[1] = (ticks[1] - tickStart);
	printf("sensor 2: %u = %u - %u\n", rc_values[1], ticks[1], tickStart);

	
	// read sensor 3
	tickStart = gpioTick();
	gpioSetMode(RC_3, PI_OUTPUT);
	gpioWrite(RC_3, 1);
	gpioDelay(rc_charge_us);
	gpioSetMode(RC_3, PI_INPUT);
	while(ticks[2] == 0){
		if((gpioRead(RC_3) == 0)){
			ticks[2] = gpioTick();
		}
	}
	
	rc_values[2] = (ticks[2] - tickStart);
	printf("sensor 3: %u = %u - %u\n", rc_values[2], ticks[2], tickStart);
		
	
	return &(rc_values[0]);
}

int gpioInitAndRegisterClose(){
	if(initGpio() == 1){ 
		return 1; 
	}
	
	if(atexit(closeGpio) != 0){
		cerr << "could not register closeGpio() to atexit()." << endl; return 1;
	}
	return 0;
}

int main(){
	std::cout << "hello and welcome." << std::flush;
	
	if(gpioInitAndRegisterClose() != 0){
		cerr << "failed gpio init and register close at exit." << endl;
		return 1;
	}

// 	motors_backward();
// 	time_sleep(2.0f);
// 	motors_stop();

	gpioSetPad(0, 16);
	cout << "reading sensors:  " << endl;
	for(int i = 0; i < 80; i++){
		qtr3rc_read();
		printf("%d\t%d\t%d\t   \n", rc_values[0], rc_values[1], rc_values[2] );
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



