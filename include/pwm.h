/****************************************************************
 *
 * Author: Yiying with modifications by Ella Atkins and Justin Bradley
 * Title: pwm.h
 * Date: Saturday, April 23, 2011
 * Description: Provides the entire PWM library.
 *
 ****************************************************************/
#ifndef PWM_H
#define PWM_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>

#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
  __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)
/*
 * PWM Values are defined here for the servos and the motors.
 * The motors max and min values are the value for 0% speed to 100% speed
 * while the stop value is the value required to stop the motor completely
 * requiring the start up sequence again.
 */
#define MOTOR_MAX 	638 			// 100% thrust
#define MOTOR_MIN 	21 			// 0% thrust
#define MOTOR_STOP 	0 			//stops the motor from pulling current

/*
 * Define frequency for the PWMs here
 */
#define PWM8FREQ        50 // most RC servos assume 50 Hz
#define PWM9FREQ 	50
#define PWM10FREQ 	50
#define PWM11FREQ 	50

//#define PWM8FCLK        13000000
#define PWM8FCLK        32000
#define PWM9FCLK 	32000
#define PWM10FCLK 	32000
#define PWM11FCLK 	32000

#define PWM8ADR         0x4800217A
#define PWM9ADR 	0x48002174
#define PWM10ADR 	0x48002176
#define PWM11ADR 	0x48002178
#define PORTTYPE 	2 	     //sets the with this address 2=PWM 4=GPIO

//Controls optional features specific to the timer functionality
#define PWM8TCLR        0x4903E024
#define PWM9TCLR 	0x49040024
#define PWM10TCLR 	0x48086024
#define PWM11TCLR 	0x48088024
//Holds the value of the internal counter
#define PWM8TCRR        0x4903E028
#define PWM9TCRR	0x49040028
#define PWM10TCRR	0x48086028
#define PWM11TCRR	0x48088028
//Holds the timer load values
#define PWM8TLDR        0x4903E02C
#define PWM9TLDR	0x4904002C
#define PWM10TLDR	0x4808602C
#define PWM11TLDR	0x4808802C
//Holds the value to be compared with the counter value
#define PWM8TLMAR       0x4903E038
#define PWM9TLMAR	0x49040038
#define PWM10TLMAR	0x48086038
#define PWM11TLMAR	0x48088038

#define TIMERSTOP 	0
#define TIMERSTART 	0x01843

unsigned int TLDRVAL8=0, TLDRVAL9=0, TLDRVAL10=0, TLDRVAL11=0;

typedef struct 
{
	double pwm8;
	double pwm9;
	double pwm10;
	double pwm11;
} pwmQuery;


int checkmem(char,int,unsigned int);
int changemem(char,int,int);
void pwm8(double);
void pwm9(double);
void pwm10(double);
void pwm11(double);
void pwm8init(void);
void pwm9init(void);
void pwm10init(void);
void pwm11init(void);
void pwminit();

int fd; //sets the place that the /dev/mem is opened to input the PWM

int changemem(char t, int a, int b) {

    void *map_base, *virt_addr;
	unsigned long read_result, writeval;
	off_t target;
	int access_type = t;

	target = a;

    /* Map one page */
    map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target & ~MAP_MASK);
    if(map_base == (void *) -1) FATAL;

    virt_addr = (void *) ((off_t) map_base + (target & MAP_MASK));
    /*
    switch(access_type) {
		case 'h':
			read_result = *((unsigned short *) virt_addr);
			break;
		case 'w':
			read_result = *((unsigned long *) virt_addr);
			break;
		default:
			fprintf(stderr, "Illegal data type '%c'.\n", access_type);
			exit(2);
	}
	*/
    fflush(stdout);
	writeval = b;
	switch(access_type) {
		case 'h':
			*((unsigned short *) virt_addr) = writeval;
			read_result = *((unsigned short *) virt_addr);
			break;
		case 'w':
			*((unsigned long *) virt_addr) = writeval;
			read_result = *((unsigned long *) virt_addr);
			break;
		}
	if(munmap(map_base, MAP_SIZE) == -1) FATAL; //removes mapping
    return 0;
}
/*
* Function pwminit: Initializes the PWM
*/
void pwminit()
{
	if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) FATAL; //open the memory for changing the PWM
        // TableSat doesn't use 8 and 9
	// pwm8init();
	// pwm9init();
	pwm10init();
	pwm11init();
	sleep(1); // The motor has to sit at 0 for 1 second before recieving
                  // commands
	pwm10(MOTOR_STOP);
	pwm11(MOTOR_STOP);
}

/*
* Function pwmdeinit: Deinitializes the PWM (basically just stops the motors)
*/
void pwmdeinit()
{
	pwm10(MOTOR_STOP);
	pwm11(MOTOR_STOP);
        close(fd);
}


void pwm8(double duty) {
	int temp=duty;
	changemem('w',PWM8TLMAR,TLDRVAL8+temp);
}
void pwm8init(void) {
	TLDRVAL8 = -((PWM8FCLK/PWM8FREQ)-1-0xFFFFFFFF);
	// Always false if (TLDRVAL8 < 0x00000000) 		TLDRVAL8 = 0x00000000;
	changemem('w',PWM8TCLR,TIMERSTOP);
	changemem('h',PWM8ADR,PORTTYPE);
	changemem('w',PWM8TLDR,TLDRVAL8);
        // switch these depending on whether you have a motor or a servo connected
	changemem('w',PWM8TLMAR,TLDRVAL8+MOTOR_STOP);
	changemem('w',PWM8TCRR,TLDRVAL8);
	changemem('w',PWM8TCLR,TIMERSTART);
	printf("In Order: %X %X %X %X %X %X\n",TLDRVAL8,PWM8TCLR,PWM8ADR,PWM8TLDR,PWM8TLMAR,PWM8TCRR);
}

void pwm9(double duty) {
	int temp=duty;
	changemem('w',PWM9TLMAR,TLDRVAL9+temp);
}
void pwm9init(void) {
	TLDRVAL9 = -((PWM9FCLK/PWM9FREQ)-1-0xFFFFFFFF);
	// Always false if (TLDRVAL9 < 0x00000000) 		TLDRVAL9 = 0x00000000;
	changemem('w',PWM9TCLR,TIMERSTOP);
	changemem('h',PWM9ADR,PORTTYPE);
	changemem('w',PWM9TLDR,TLDRVAL9);
        // switch these depending on whether you have a motor or a servo connected
	changemem('w',PWM9TLMAR,TLDRVAL9+MOTOR_STOP);
	changemem('w',PWM9TCRR,TLDRVAL9);
	changemem('w',PWM9TCLR,TIMERSTART);
	printf("In Order: %X %X %X %X %X %X\n",TLDRVAL9,PWM9TCLR,PWM9ADR,PWM9TLDR,PWM9TLMAR,PWM9TCRR);
}
void pwm10(double duty) {
	int temp=duty;
	changemem('w',PWM10TLMAR,TLDRVAL10+temp);
}
void pwm10init(void) {
	TLDRVAL10 = -((PWM10FCLK/PWM10FREQ)-1-0xFFFFFFFF);
	// Always false if (TLDRVAL10 < 0x00000000) 		TLDRVAL10 = 0x00000000;
	changemem('w',PWM10TCLR,TIMERSTOP);
	changemem('h',PWM10ADR,PORTTYPE);
	changemem('w',PWM10TLDR,TLDRVAL10);
        // switch these depending on whether you have a motor or a servo connected
	changemem('w',PWM10TLMAR,TLDRVAL10+MOTOR_STOP);
	changemem('w',PWM10TCRR,TLDRVAL10);
	changemem('w',PWM10TCLR,TIMERSTART);
	printf("In Order: %X %X %X %X %X %X\n",TLDRVAL10,PWM10TCLR,PWM10ADR,PWM10TLDR,PWM10TLMAR,PWM10TCRR);
}
void pwm11(double duty) {
	int temp=duty;
	changemem('w',PWM11TLMAR,TLDRVAL11+temp);
}
void pwm11init(void) {
	TLDRVAL11 = -((PWM11FCLK/PWM11FREQ)-1-0xFFFFFFFF);
	// Always false if (TLDRVAL11 < 0x00000000) 		TLDRVAL11 = 0x00000000;
	changemem('w',PWM11TCLR,TIMERSTOP);
	changemem('h',PWM11ADR,PORTTYPE);
	changemem('w',PWM11TLDR,TLDRVAL11);
        // switch these depending on whether you have a motor or a servo connected
	changemem('w',PWM10TLMAR,TLDRVAL11+MOTOR_STOP);
	changemem('w',PWM11TCRR,TLDRVAL11);
	changemem('w',PWM11TCLR,TIMERSTART);
}
#endif // PWM_H
