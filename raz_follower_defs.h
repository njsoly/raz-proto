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

#define LMOTOR_PWM 	12
#define LMOTOR_DIR	16
#define RMOTOR_PWM	13
#define RMOTOR_DIR	26
#define MOTOR_NSLP	8

#define RC_1	4
#define RC_2	27
#define RC_3	24

// defines for two-sensor config of QTR-1RC sensors.
#define RC_L	20
#define RC_R	21

int initGpio();
void delay(int millis);
void pwmDoubleBreather(int, int);

void blinkGpio(int gpio);
void leftMotor(int l);
void rightMotor(int r);
void nl();
int initGpio();
void closeGpio();
void motors_stop();
void motors_forward(int speed);
void motors_backward(int speed);
void set_motors(int lspeed, int rspeed);



void set_motors(int lspeed, int rspeed){
	leftMotor(lspeed);
	rightMotor(rspeed);
}

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
	
	gpioSetMode(RC_1, PI_OUTPUT);
	gpioSetMode(RC_2, PI_OUTPUT);
	gpioSetMode(RC_3, PI_OUTPUT);
	gpioWrite(MOTOR_NSLP, 0);
	
	return 0;
}

void closeGpio(){
	gpioTerminate();
}

void delay(int millis){
	time_sleep(0.001f * ((float) millis));
}

void pwmDoubleBreather(int gpioA=PWM0_PIN, int gpioB=PWM1_PIN){
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 255; j++){
			gpioPWM(gpioA, (i % 2 == 0 ? j : 255 - j));
			gpioPWM(gpioB, (i % 2 == 1 ? j : 255 - j));
			time_sleep(0.002f);
		}
	}
}




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

void motors_forward(int speed=HALF_SPEED){
	leftMotor(speed);
	rightMotor(speed);
}
void motors_backward(int speed=HALF_SPEED){
	leftMotor(-speed);
	rightMotor(-speed);
}
void motors_stop(){
	leftMotor(0);
	rightMotor(0);
	gpioWrite(MOTOR_NSLP, 0);
	
}
void nl(){ puts("\n"); }
