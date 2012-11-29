#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <math.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include "../include/adc.h"

#define ADC_FILENAME "/dev/twl4030-madc"
#define ADC_VOLTAGE_CONVERSION 0.00244379277;

// Global A/D variables, I guess defined in twl4030-madc.h
int adc_device;
struct twl4030_madc_user_parms *adc_par;

int initADC()
{
	adc_device = open(ADC_FILENAME, O_RDWR | O_NONBLOCK);
	if (adc_device == ADC_INIT_ERROR)
        {
                return ADC_INIT_ERROR;
        }
	adc_par = (struct twl4030_madc_user_parms *)malloc(sizeof(struct twl4030_madc_user_parms));
	return ADC_INIT_SUCCESS; // nominal
}

int deinitADC()
{
        int adc_deinit_success = close(adc_device);
        if(adc_deinit_success != ADC_DEINIT_SUCCESS)
        {
                return adc_deinit_success;
        }
        return ADC_DEINIT_SUCCESS;
}

double convertADCValueToVolts(unsigned int counts)
{
	//This constant comes from 2.5 volts/1023 maximum count.
	return ((double) counts)*ADC_VOLTAGE_CONVERSION;
}

int readADC(int channel)
{
	memset(adc_par, 0, sizeof(struct twl4030_madc_user_parms));
	adc_par->channel = channel;
	ioctl(adc_device, TWL4030_MADC_IOCX_ADC_RAW_READ, adc_par);
	if(adc_par->status != ADC_READ_ERROR)
	{
		return (unsigned int)adc_par->result;
	}
	printf("Failed to read channel %d.\n", channel);
	return ADC_READ_ERROR;
}
