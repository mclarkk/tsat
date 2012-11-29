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
*   @file   i2c-api.h
*
*   @brief  This file contains definitions for performing i2c operations 
*           on the gumstix.
*
****************************************************************************/

#if !defined( I2C_API_H )
#define I2C_API_H

// ---- Include Files -------------------------------------------------------

#include <inttypes.h>
#include <stdio.h>
#include "i2c.h"

// ---- Constants and Types -------------------------------------------------

#define I2C_USE_CRC 1
#define I2C_NO_CRC  0

#define I2C_READ  0x01 //read data flag (duplicates I2C_M_RD in i2c-dev.h, but this name is more descriptive and allows changes to be confined to i2c-api.h/c
#define I2C_WRITE 0x00

// ---- Variable Externs ----------------------------------------------------

// ---- Function Prototypes -------------------------------------------------

void I2cSetSlaveAddress
(
    int         i2cDev,     ///< Handle to i2c-dev file
    I2C_Addr_t  i2cAddr,    ///< 7 bit i2c address to use
    int         useCrc );   ///< Should CRC's be used?

int I2cTransfer(int       i2cDev,     //< Handle to i2c-dev file
		I2C_Addr_t I2cAddr,  //< Address
		uint8_t   i2cFlag,    //< I2C_READ or I2C_WRITE flag
		void     *i2cData,    //< Data to read/write
		uint8_t   i2cLen      //< Number of bytes to read/write
		);

#endif  // I2C_API_H

