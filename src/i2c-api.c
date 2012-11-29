/****************************************************************************
 *
 *   Copyright (c) 2006 Dave Hylands     <dhylands@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2 as
 *   published by the Free Software Foundation.
 *
 *   Alternatively, this software may be distributed under the terms of BSD
 *   license.
 *
 *   See README and COPYING for more details.
 *
 ****************************************************************************/
/**
 *
 *   @file   i2c-api.c
 *
 *   @brief  This file contains the implementation for performing I2C operations
 *           on the gumstix.
 *
 ****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <string.h>
#include <errno.h>

#include "../include/i2c.h"
#include "../include/i2c-dev.h"
#include "../include/i2c-api.h"

//#include "Crc8.h"
//#include "DumpMem.h"
//#include "Log.h"

// ---- Public Variables ----------------------------------------------------

// ---- Private Constants and Types -----------------------------------------

// ---- Private Variables ---------------------------------------------------

static  I2C_Addr_t  gI2cAddr;
static  int         gUseCrc;

// ---- Private Function Prototypes -----------------------------------------

// ---- Functions -----------------------------------------------------------

//***************************************************************************
/**
 *
 *   Sets the I2C address that we'll be communicating with, as well as whether
 *   the device uses smbus PEC (CRC).
 */

void I2cSetSlaveAddress( int i2cDev, I2C_Addr_t i2cAddr, int useCrc)
{
        gI2cAddr = i2cAddr;
        gUseCrc  = useCrc;

        // fprintf(stderr,"----- I2cSetSlaveAddress i2cAddr:0x%02x useCrc:%d -----\n",i2cAddr, useCrc );

        // Indicate which slave we wish to speak to

        if ( ioctl( i2cDev, I2C_SLAVE, gI2cAddr ) < 0 )
        {
                fprintf(stderr,"%I2cSetSlaveAddress: Error trying to set slave address to 0x%02x (%d %s)\n", gI2cAddr, errno, strerror( errno ));
        }

        // We do the CRC calculation ourself, so we don't need to tell the driver
        // that we're using it.

#if 0
        // Indicate that we use PEC (aka CRCs)

        if ( ioctl( i2cDev, I2C_PEC, 1 ) < 0 )
        {
                fprintf(stderr,"%I2cSetSlaveAddress: Error trying to set PEC mode\n");
        }
#endif

} // I2cSetSlaveAddress

/***************************************************************************
 *   Transfer data to/from an i2c device.
 *
 *   This function implements the equivalent of the smbus functions using
 *   I2C_RDWR.
 *
 *   The PXA driver doesn't support the smbus transfers.
 *
 *   This function can perform the following SMBUS transactions:
 *
 *       Write Byte:     i2cLen == 1,                i2cFlag = I2C_WRITE
 *       Read Byte:      i2cLen == 1,                i2cFlag = I2C_READ
 *       Write Word:     i2cLen == 2,                i2cFlag = I2C_WRITE
 *       Read Word:     i2cLen == 2,                i2cFlag = I2C_READ
 */

int I2cTransfer(int       i2cDev,     //< Handle to i2c-dev file
		I2C_Addr_t I2cAddr,  //< Address
		uint8_t   i2cFlag,    //< I2C_READ or I2C_WRITE flag
		void     *i2cData,    //< Data to read/write
		uint8_t   i2cLen      //< Number of bytes to read/write
        )
{
        struct i2c_rdwr_ioctl_data  rdwr;
        struct i2c_msg              msg;
        uint8_t                     i2cBuf[ I2C_MAX_DATA_LEN ];

        printf("in I2cTransfer start transfer\n"); fflush(stdout);

        msg.addr  = I2cAddr; //set slave address for message (set by I2cSetSlaveAddress())
        msg.len   = i2cLen;   //set length of message 
        msg.flags = i2cFlag; //set flag for read or write
        msg.buf   = (char *)i2cBuf; //point message to data

        printf("in I2cTransfer msg.addr=0x%x\n", msg.addr);

        if( (i2cData != NULL) && (i2cLen) ) { //if buffer allocated/passed to function and we have data to read/write
                
                if(i2cFlag == I2C_WRITE){
                        printf("in I2cTransfer trying to write\n");
                        memcpy(i2cBuf, i2cData, i2cLen); //copy data to buffer
                }
                printf("in I2cTransfer after writing\n");
                rdwr.msgs = &msg; //point to message
                rdwr.nmsgs = 1; //number of message always = 1 (no combined write-then-read cycles)
      
                printf("in I2cTransfer before ioctl\n");
                if (ioctl(i2cDev, I2C_RDWR, &rdwr) < 0 ){                        
                        fprintf(stderr,"%I2cTransfer: ioctl faileds: %s (%d)\n", 
                                strerror( errno ), errno );
                        printf("stop transfer -1\n"); fflush(stdout);
                        return -1;
                }
                printf("in I2cTransfer after ioctl\n");
                if (i2cFlag == I2C_READ){
                        memcpy(i2cData, i2cBuf, i2cLen); //copy data from buffer
                }
        }
        else{
                printf("in I2cTransfer stop transfer -2\n"); fflush(stdout);
                return(-2); //no buffer or no length
        }
        printf("in I2cTransfer stop transfer 0\n"); fflush(stdout);
        return 0;

} // I2cTransfer

