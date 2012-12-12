/****************************************************************
 *
 * Author: Justin Bradley and Richard Choroszucha
 * Title: Control.h
 * Date: Thursday, April  7, 2011
 * Description: Functional spec for Control
 *
 ****************************************************************/

#ifndef CONTROL_H
#define CONTROL_H

/***********************************************************
 * Function: Control_init - takes care of any initialization needed by the
 *           controller 
 * Parameters: none
 * Return Value: 0 on success, -1 on failure
 ***********************************************************/
int Control_init(double angular_velocity);

/***********************************************************
 * Function: Control_loop - executes the control
 * Parameters: none
 * Return Value: none
 ***********************************************************/
void Control_loop();

/***********************************************************
 * Function: Control_deinit - deinitializes anything needed by the controller 
 * Parameters: none
 * Return Value: 0 on success, -1 on failure
 ***********************************************************/
int Control_deinit();

#endif // CONTROL_H
