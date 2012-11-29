/****************************************************************
 *
 * Author: Ella Atkins modified by Justin Bradley
 * Title: test_adc.c
 * Date: Sunday, April 24, 2011
 * Description: Test bench for ADC code
 *
 ****************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "../include/adc.h"

static unsigned int halt = 0;

// would like to gracefully shutdown
void sig_handler(int)
{
        halt = 1;
}

int main()
{
        int chnum;
        unsigned int adcounts;
        double adsample;
        int init_success;
        int sleep_time = 1;
        float angular_vel;

        // install the signal handlers
        signal(SIGABRT, &sig_handler);
        signal(SIGTERM, &sig_handler);
	signal(SIGINT, &sig_handler);
        
        printf("Initializing ADC channels\n");
        init_success = initADC();
	if(init_success != ADC_INIT_SUCCESS)    
        {
                printf("ADC initialization failed\n");
                return -1;
        }
		
        printf("Enter channel number to read (2-7):\n");
        scanf("%d",&chnum);

        while(!halt)
        {
                adcounts = readADC(chnum);
                adsample = convertADCValueToVolts(adcounts);

                printf("ADC channel %d:  %u counts, %f volts\n",
                       chnum, adcounts, adsample);
                // assuming reading rate gyro (channel 7)
                angular_vel = (180/3.14159)*(-1.4721799*adsample + 1.82256189);
                printf("Angular Velocity = %f deg/sec\n",angular_vel);

                // need to sleep one second
                sleep(sleep_time);
        }
        printf("Deinitializing ADC channels...\n");
        // gracefully shutdown
        init_success = deinitADC();
        if(init_success != ADC_DEINIT_SUCCESS)
        {
                printf("ADC deinitialization failed...bailing out...\n");
                return -1;
        }
        return 0; // nominal execution
}
