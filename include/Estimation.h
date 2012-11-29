/****************************************************************
 *
 * Author: Justin Bradley and Richard Choroszucha
 * Title: Estimation.h
 * Date: Saturday, April 23, 2011
 * Description: Functional spec for Estimation
 *
 ****************************************************************/

#ifndef ESTIMATION_H
#define ESTIMATION_H

/***********************************************************
 * Function: Estimation_init - takes care of any initialization needed by the
 *           estimator
 * Parameters: none
 * Return Value: 0 on success, -1 on failure
 ***********************************************************/
int Estimation_init();

/***********************************************************
 * Function: Estimation_loop - executes the estimation - bails if it fails to
 *           read any of the sensors. this isn't optimal but...
 * Parameters: none
 * Return Value: none
 ***********************************************************/
void Estimation_loop();

/***********************************************************
 * Function: Estimation_deinit - deinitializes anything needed by the estimator
 * Parameters: none
 * Return Value: 0 on success, -1 on failure
 ***********************************************************/
int Estimation_deinit();

#endif // ESTIMATION_H
