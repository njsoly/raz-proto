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

#define J8_11 17
#define J8_8 4
#define J8_10 15
#define J8_12 18
#define J8_38 20
#define J8_40 21
#define J8_36 16
#define J8_37 26
#define J8_32 12
#define J8_33 13

#define PWM0_PIN 12
#define PWM1_PIN 13

#define LMOTOR_DIR J8_36

void nl(){ puts("\n"); }


int main(){
	// surround with silent try-catch-move-on?
	if(gpioInitialise() < 0){
		puts("couldn't initialize gpio.\n");
		return 1;
	}

	puts("hello and welcome.");
	nl();
	
	gpioSetMode(J8_10, PI_OUTPUT);
	gpioSetMode(J8_38, PI_OUTPUT);
	gpioSetMode(J8_40, PI_OUTPUT);
	gpioSetMode(LMOTOR_DIR, PI_OUTPUT);
	gpioSetMode(PWM0_PIN, PI_ALT0);
	gpioSetMode(PWM1_PIN, PI_ALT0);

	for(int i = 0; i < 10; i++){
		gpioWrite(LMOTOR_DIR, i % 2);
		time_sleep(0.1f);		
	}	
	

	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 255; j++){
			gpioPWM(PWM0_PIN, (i % 2 == 0 ? j : 255 - j));
			gpioPWM(PWM1_PIN, (i % 2 == 1 ? j : 255 - j));
			time_sleep(0.002f);
		}
	}
	
	gpioTerminate();
	return 0;
}
