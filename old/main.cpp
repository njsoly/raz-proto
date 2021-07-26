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

// defines the GPIO numbers and other things.
// defines include RC_L, RC_R, MOTORL_PWM, ETC.
#include "raz_follower_defs.h"

using namespace std;

uint32_t rc_values[3]={0,0};
uint16_t rc_charge_us = 30;
uint32_t ticks[2]={0,0};

uint32_t* qtr1rc2_read();



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

// 	motors_backward();// 	time_sleep(2.0f);// 	motors_stop();

	gpioSetPad(0, 16);
	cout << "reading sensors:  " << endl;
	for(int i = 0; i < 80; i++){
		qtr1rc2_read();
		printf("%d\t%d\t   \n", rc_values[0], rc_values[1]);
		time_sleep(0.250f);
	}


	cout << ("\rbye.                    \r\n");
	
	return 0;
}


/** just read a left and right sensor.
 */
uint32_t* qtr1rc2_read(){
	uint32_t tickStart = gpioTick();
	ticks[0] = 0;
	ticks[1] = 0;
	
	printf("tickStart: %d\n", (tickStart));
	
	gpioSetPad(0, 16); // the sensors want 17mA, let's at least get 16 (the max).
	if(gpioGetPad(0) != 16) {
		printf("ERROR: pad strength set didn't work.\n");
	}
	
	gpioSetMode(RC_L, PI_INPUT);
	gpioSetMode(RC_R, PI_INPUT);

	// read sensor L
	tickStart = gpioTick();
	gpioSetMode(RC_L, PI_OUTPUT);
	gpioWrite(RC_L, 1);
	gpioDelay(rc_charge_us);
	gpioSetMode(RC_L, PI_INPUT);
	while(ticks[0] == 0 || gpioRead(RC_L) == 1){
		if((gpioRead(RC_L) == 0) && gpioRead(RC_L) == 0){
			ticks[0] = gpioTick();
		}
	}
	rc_values[0] = (ticks[0] - tickStart);
	
	printf("sensor L: %u = %u - %u\n", rc_values[0], ticks[0], tickStart);

	// read sensor R
	tickStart = gpioTick();
	gpioSetMode(RC_R, PI_OUTPUT);
	gpioWrite(RC_R, 1);
	gpioDelay(rc_charge_us);
	gpioSetMode(RC_R, PI_INPUT);
	while(ticks[1] == 0){
		if((gpioRead(RC_R) == 0) && gpioRead(RC_R) == 0){
			ticks[1] = gpioTick();
		}
	}
	rc_values[1] = (ticks[1] - tickStart);
	printf("sensor R: %u = %u - %u\n", rc_values[1], ticks[1], tickStart);

	gpioWrite(RC_L, 0);
	gpioWrite(RC_R, 0);
	return &(rc_values[0]);
}



