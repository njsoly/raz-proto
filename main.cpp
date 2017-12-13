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


