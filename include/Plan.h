/****************************************************************
 *
 * Author: Justin Bradley
 * Title: Plan.h
 * Date: Thursday, April  7, 2011
 * Description: Functional spec for Planning
 *
 ****************************************************************/

#ifndef PLAN_H
#define PLAN_H

/***********************************************************
 * Function: Plan_init - takes care of any initialization needed by the
 *           planner 
 * Parameters: none
 * Return Value: 0 on success, -1 on failure
 ***********************************************************/
int Plan_init();

/***********************************************************
 * Function: Plan_loop - creates the plan
 * Parameters: none
 * Return Value: none
 ***********************************************************/
void Plan_loop();

/***********************************************************
 * Function: Plan_deinit - deinitializes anything needed by the planner
 * Parameters: none
 * Return Value: 0 on success, -1 on failure
 ***********************************************************/
int Plan_deinit();

#endif // PLAN_H
