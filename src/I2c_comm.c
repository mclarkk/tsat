/****************************************************************
 *
 * Author: Justin Bradley adapted from code by Ryan D. Eubank and Stephanie
 *         Ogura 
 * Title: I2c_comm.c
 * Date: Thursday, April  7, 2011
 * Description: Contains the I2C implementation
 *
 ****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "../include/i2c.h"
#include "../include/i2c-dev.h"
#include "../include/i2c-api.h"
#include "../include/I2c_comm.h"
#include "../include/hc1_structs.h"

// Control-Bytes (channels)
#define ADC_CHANNEL_0    0x80
#define ADC_CHANNEL_1    0x90
#define ADC_CHANNEL_2    0xA0
#define ADC_CHANNEL_3    0xB0
#define ADC_CHANNEL_4    0xC0
#define ADC_CHANNEL_5    0xD0
#define ADC_CHANNEL_6    0xE0
#define ADC_CHANNEL_7    0xF0
// Range and Polarity selection
#define ADC_VREF_RANGE      0x08
#define ADC_HALF_VREF_RANGE 0x00
#define ADC_BIP             0x04
#define ADC_POWERDOWN       0x03

// Error information
#define I2C_WRITE_ERROR  -100.0 // returned by I2c_read when a write fails

// this is the slave address it is 0b0101A2A1A0 where A2 A1 A0 are the logic
// state of the A2-A0 pins on the chip itself
static unsigned int I2c_address = 0x28; // I've chosen 0b00101000 so A2=0 A1=0 A0=0
// this is the bus ID default value (which is invalid)
static int I2c_busID = -1;

/******************************************************
 * Extern global variables
 ******************************************************/
// sensor readings
extern SensorValues sensors;

// function prototype only needs to be here since only I2c_comm_loop uses it
/**
 * Function: I2c_read - reads 2 bytes from the I2c bus as designated
 *           by the control_byte and the addr parameters. This is done
 *           by writing a byte and then reading 2 back.
 * Parameters: control_byte - 
 * Return Value: the data read
 */
double I2c_read(unsigned char control_byte, unsigned char addr);

/******************************************************
 * Functions
 ******************************************************/

int I2c_comm_init()
{
        //
        // Open i2c bus; check for error
        // Hard-coded to Gumstix (/dev/i2c-3)
        //
        I2c_busID = open("/dev/i2c-3",O_RDWR);
        if (I2c_busID < 0) {
                //printf("%.3lf\tERROR: busID=%d\n",gettimestamp(),I2c_busID);
                // don't worry about the timestamp for now
                printf("\tERROR: busID=%d\n",I2c_busID);
                /* pthread_mutex_lock(&log_mutex); */
                /* fprintf(fp_log, "%.3lf\tERROR: busID=%d\n", */
                /*         gettimestamp(), I2c_busID); */
                /* fflush(fp_log); */
                /* pthread_mutex_unlock(&log_mutex); */
                return -1;
        }
        printf("I2c_busID=%d\n",I2c_busID);

        printf("New:  setting slave address\n");
        if (ioctl(I2c_busID, I2C_SLAVE, I2c_address) < 0) {
                fprintf(stderr, "Error trying to set slave address to 0x%02x (%d %s)\n",
                        I2c_address, errno, strerror( errno ));
        }

        return 0; // Nominal execution
}

void I2c_comm_loop()
{
        //struct timespec sleeptime;
        //double ms = 0.001;
        //sleeptime.tv_sec = (long)(ms);
        //sleeptime.tv_nsec = (long)(1.0E9*(ms - sleeptime.tv_sec));
        // Probing all channels in each loop  
        double temp;
        printf("in I2c_comm_loop attempting to read\n"); fflush(stdout);
        printf("I2c_address=%x\n",I2c_address);
        // Channel 0 - ultrasonic altimeter
        if ((temp = I2c_read(ADC_CHANNEL_1|ADC_HALF_VREF_RANGE, I2c_address)) != I2C_WRITE_ERROR) {
                /* pthread_mutex_lock(&altimeterdata_mutex); */
                /* altimeterdata.altitude = (temp*ADC_10V_CAL*ADC_ULTRASONIC_CAL)+ADC_ULTRASONIC_OFFSET; */
                /* pthread_mutex_unlock(&altimeterdata_mutex); */
                printf("Behold, we have data=%f",temp);
        }

        /* pthread_mutex_lock(&adsdata_mutex); */
        /* adsdata.airspeed = (adsdata.airspeed_left + adsdata.airspeed_right)/2.0; */
        /* pthread_mutex_unlock(&adsdata_mutex); */
        return;
}

double I2c_read(unsigned char control_byte, unsigned char addr)
{
        unsigned char buffer[5];
        double data;
        buffer[0] = control_byte;
        printf("in I2c_read before writing first write\n"); fflush(stdout);
        printf("args:  busID=%d, addr=%x, WRITE=%u, control_byte=%x\n",
               I2c_busID, addr, I2C_WRITE, buffer[0]);
        if (I2cTransfer(I2c_busID, addr, I2C_WRITE, buffer, 1) < 0)
        {
                return I2C_WRITE_ERROR;
        }
        printf("in I2c_read before read\n"); fflush(stdout);
        I2cTransfer(I2c_busID, addr, I2C_READ, buffer, 2);
        // shift the first byte over, add the second byte,
        // then convert to a double
        data = (double) (((buffer[0] << 8) + buffer[1]) >> 4);
        // data *= (5.0/4096.0);
        return data;
}

int I2c_comm_deinit()
{
        int failed = close(I2c_busID);
        if(failed)
        {
                printf("Unable to close I2c bus with ID=%d",I2c_busID);
                return -1; // something went wrong
        }
        return 0; // nominal execution
}
