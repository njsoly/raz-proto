#define J8_8	4
#define J8_10	15
#define J8_11	17
#define J8_12	18
#define J8_13	27
#define J8_15	22
#define J8_16	23
#define J8_18	24
#define J8_22	25
#define J8_24	8
#define J8_26	7
#define J8_28	1
#define J8_32	12
#define J8_33	13
#define J8_36	16
#define J8_37	26
#define J8_38	20
#define J8_40	21

#define FULL_BORE	255
#define HALF_SPEED	127

#define PWM0_PIN	12
#define PWM1_PIN	13

#define LMOTOR_PWM 	PWM0_PIN
#define LMOTOR_DIR	J8_36
#define RMOTOR_PWM	PWM1_PIN
#define RMOTOR_DIR	J8_37
#define MOTOR_NSLP	J8_24

#define RC_1	17
#define RC_2	27
#define RC_3	22

int initGpio();
void delay(int millis);
void pwmDoubleBreather(int, int);

void nl();
int initGpio(){
	
	if(gpioInitialise() < 0){
		puts("couldn't initialize gpio.\n");
		return 1;
	}

	gpioSetMode(LMOTOR_DIR, PI_OUTPUT);
	gpioSetMode(RMOTOR_DIR, PI_OUTPUT);
	gpioSetMode(LMOTOR_PWM, PI_ALT0);
	gpioSetMode(RMOTOR_PWM, PI_ALT0);
	gpioSetMode(MOTOR_NSLP, PI_OUTPUT);
	
	return 0;
}

void closeGpio(){
	gpioTerminate();
}

void delay(int millis){
	time_sleep(0.001f * ((float) millis));
}

void pwmDoubleBreather(int gpioA, int gpioB){
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 255; j++){
			gpioPWM(PWM0_PIN, (i % 2 == 0 ? j : 255 - j));
			gpioPWM(PWM1_PIN, (i % 2 == 1 ? j : 255 - j));
			time_sleep(0.002f);
		}
	}
}

void nl(){ puts("\n"); }
