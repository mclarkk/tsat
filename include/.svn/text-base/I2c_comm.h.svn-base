/****************************************************************
 *
 * Author: Justin Bradley
 * Title: I2c_comm.h
 * Date: Thursday, April  7, 2011
 * Description: header file for I2c_comm
 *
 ****************************************************************/

#ifndef I2C_COMM_H
#define I2C_COMM_H

/** 
 * Function: I2c_comm_init - initializes the I2C resource
 * Parameters: none
 * Return Value: 0 on success, -1 on failure
 */
int I2c_comm_init();

/**
 * Function: I2c_comm_loop - reads data from all sensors and stores
 *           the information in the appropriate structures
 * Parameters: none
 * Return Value: none (stores information in shared memory structures
 */
void I2c_comm_loop();

/**
 * Function: I2c_comm_deinit - deinitializes the I2c bus by closing the handle
 *           to the resource
 * Parameters: none
 * Return Value: 0 on success, -1 on failure
 */
int I2c_comm_deinit();

#endif
