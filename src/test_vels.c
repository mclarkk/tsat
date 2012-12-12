/****************************************************************
 *
 * Author: Justin Bradley
 * Title: GetVelocitiesTime.c
 * Date: Wednesday, November 28, 2012
 * Description: Code to determine how long it takes to complete a
 *              full revolution. This is used to find a function relating
 *              fan speed to angular velocity (in steady state).
 * NOTE: For TableSat, our pwm channels are as follows:
 * PWM10 = fan at 270 degrees (west) from bubble levels on TSat
 * PWM11 = fan at 90 degrees (east) from bubble levels on TSat
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
        int duty_cycle;
        double start_time, stop_time, elapsed_time;
        char quit[5];

        pwminit();
        
        printf("Enter PWM speed for west fan (21-638), or 'q' for quit: ");
        gets(quit);
        if(quit[0] == 'q')
        {
                pwmdeinit();
                return 0;
        }
        duty_cycle = atoi(quit);
        pwm10(duty_cycle);

        printf("Enter PWM speed for east fan (21-638), or 'q' for quit: ");
        gets(quit);
        if(quit[0] == 'q')
        {
                pwmdeinit();
                return 0;
        }
        duty_cycle = atoi(quit);
        pwm11(duty_cycle);

        while(1)
        {
                printf("Press Enter to START timer...'q' for quit\n");
                gets(quit);
                // start the timer
                start_time = get_timestamp();
                //printf("Start time = %f\n",start_time);
                // check to see if we should quit
                if(quit[0] == 'q')
                {
                        break;
                }
                printf("Press Enter to STOP timer...'q' for quit\n");
                gets(quit);
                // stop the timer
                stop_time = get_timestamp();
                //printf("Stop time = %f\n",stop_time);
                // check to see if we should quit
                if(quit[0] == 'q')
                {
                        break;
                }
                elapsed_time = time_subtract(stop_time,start_time);
                printf("Elapsed time = %f\n",elapsed_time);
        }
	pwm10(FAN_STOP);
	pwm11(FAN_STOP);
        pwmdeinit();
        return 0;
}
