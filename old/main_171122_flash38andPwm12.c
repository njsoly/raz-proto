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


#define J8_11 17
#define J8_8 4
#define J8_10 15
#define J8_12 18
#define J8_38 20
#define J8_40 21
#define J8_37 26

int main(){
	// surround with silent try-catch-move-on?
	if(gpioInitialise() < 0){
		return 1;
	}
	puts("aoesunth\n");
	
	gpioSetMode(J8_10, PI_OUTPUT);
	gpioSetMode(J8_12, PI_ALT5); // ALT5==PWM1
	gpioSetMode(J8_38, PI_OUTPUT);
	gpioSetMode(J8_40, PI_OUTPUT);

	for(int i = 0; i < 10; i++){
		gpioWrite(J8_38, i % 2);
		time_sleep(0.1f);		
	}	
	
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 255; j++){
			gpioPWM(J8_12, (i % 2 == 0 ? j : 255 - j));
			time_sleep(0.002f);
			
		}
	}
	
	gpioTerminate();
	return 0;
}
