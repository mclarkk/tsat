/****************************************************************
 *
 * Author: Yiying with mods by Justin Bradley
 * Title: test_pwm.c
 * Date: Wednesday, November 28, 2012
 * Description: Small bit of test code to test pwm signals
 * NOTE: For TableSat, our pwm channels are as follows:
 * PWM8 = not used
 * PWM9 = not used
 * PWM10 = fan at 90 degrees from bubble levels on TSat
 * PWM11 = fan at 270 degrees from bubble levels on TSat
 *
 ****************************************************************/

#include "../include/pwm.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

#define FAN_STOP 0

int main () {
        pwminit();
//if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) FATAL; //open the memory for changing the PWM
       	int pwm8duty, pwm9duty,pwm10duty,pwm11duty,choice;
        char quit[5]; 
        while(quit[0] != 'q')
        {
                printf("Enter which pwm 10=east, or 11=west, or 'q' for quit: ");
                gets(quit);
                if(quit[0] == 'q')
                {
                        break;
                }
                choice = atoi(quit);
                switch(choice) {
                case 8:printf("Enter a number for 8: ");
                        scanf("%d",&pwm8duty);getchar();
                        pwm8(pwm8duty);
                        break;
                case 9:printf("Enter a number for 9: ");
                        scanf("%d",&pwm9duty);getchar();
                        pwm9(pwm9duty);
                        break;
                case 10:printf("Enter a number for 10: ");
                        scanf("%d",&pwm10duty);getchar();
                        pwm10(pwm10duty);
                        break;
                case 11:printf("Enter a number for 11: ");
                        scanf("%d",&pwm11duty);getchar();
                        pwm11(pwm11duty);
                        break;
                default:printf("Wrong Number\n");break;
                }
        }
	pwm10(FAN_STOP);
	pwm11(FAN_STOP);
        return 0;
}
