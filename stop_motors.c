/****************** main.c ******************/

#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include <stdio.h>

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

#include "raz_follower_defs.h"
#include <strings.h>

void gpioInit();
void gpioEnd();

int main(){
	// surround with silent try-catch-move-on?
	if(gpioInitialise() < 0){
		puts("couldn't initialize gpio.\n");
		return 1;
	}

	puts("stopping motors"/* on pins... */".\n");
	
	gpioWrite(LMOTOR_PWM, 0);
	gpioWrite(RMOTOR_PWM, 0);
	gpioWrite(LMOTOR_NSLP, 1);
	
	gpioTerminate();
	return 0;
}
