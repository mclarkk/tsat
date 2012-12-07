/****************************************************************
 *
 * Author: Justin Bradley
 * Title: GetVelocitiesTime.c
 * Date: Wednesday, November 28, 2012
 * Description: Code to determine how long it takes to complete a
 *              full revolution. This is used to find a function relating
 *              fan speed to angular velocity (in steady state).
 * NOTE: For TableSat, our pwm channels are as follows:
 * PWM10 = fan at 90 degrees from bubble levels on TSat
 * PWM11 = fan at 270 degrees from bubble levels on TSat
 *
 ****************************************************************/

#include "../include/pwm.h"
#include "../include/Utilities.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

#define FAN_STOP 0

int main () 
{

        int pwm10duty,pwm11duty,choice;
        double start_time, stop_time, elapsed_time;
        char quit[5];

        pwminit();
        
        printf("Enter PWM speed for fans (21-638), or 'q' for quit: ");
        gets(quit);
        if(quit[0] == 'q')
        {
                return 0;
        }
        choice = atoi(quit);
        pwm10(pwm10duty);
        pwm11(pwm11duty);

        while(1)
        {
                printf("Press Enter to START timer...'q' for quit\n");
                gets(quit);
                // start the timer
                start_time = get_timestamp();
                // check to see if we should quit
                if(quit[0] == 'q')
                {
                        break;
                }
                printf("Press Enter to STOP timer...'q' for quit\n");
                gets(quit);
                // stop the timer
                stop_time = get_timestamp();
                // check to see if we should quit
                if(quit[0] == 'q')
                {
                        break;
                }
                elapsed_time = time_subtract(stop_time,start_time);
                printf("Elapsed time = %G\n",elapsed_time);
        }
	pwm10(FAN_STOP);
	pwm11(FAN_STOP);
        pwmdeinit();
        return 0;
}
