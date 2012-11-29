#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/* Note: (very weird) the madc driver seems to crash if you've got driver debugging
 * turned on in your defconfig... */
/* 
 * Channel numbering:
 * ADC0-1 : to do with battery charging, not relevant on Overo
 * ADC2-7 : general purpose, input range = 0 - 2.5V.
 * ADC8 : USB OTG port bus voltage.
 * ADC9-11 : more battery charging stuff, not relevant.
 * ADC12 : main battery voltage.
 * ADC13-15: reserved or not relevant.
 */
#define ADC_INIT_SUCCESS   0
#define ADC_INIT_ERROR     -1
#define ADC_DEINIT_SUCCESS 0
#define ADC_DEINIT_ERROR   -1
#define ADC_READ_ERROR     -1

/******************************************************
 * NOTE: the following ordering of code (before the function declarations)
 * is critical to success (don't ask)
 ******************************************************/
#ifndef _LINUX
#define _LINUX
#endif

typedef uint8_t u8;
typedef uint16_t u16;

#include <twl4030-madc.h>

/***********************************************************
 * Function: initADC - initializes the ADC
 * Parameters: none
 * Return Value: 0 on success, else failure
 ***********************************************************/
int initADC();

/***********************************************************
 * Function: deinitADC - deinitializes the ADC
 * Parameters: none
 * Return Value: 0 on success, else failure
 ***********************************************************/
int deinitADC();

/***********************************************************
 * Function: convertADCValueToVolts - converts the ADC counts to volts
 * Parameters: counts - the number of counts
 * Return Value: voltage
 ***********************************************************/
double convertADCValueToVolts(unsigned int counts);

/***********************************************************
 * Function: readADC - reads the ADC
 * Parameters: channel - the channel to be read 2-7
 * Return Value: counts
 ***********************************************************/
int readADC(int channel);

#endif // ADC_H
