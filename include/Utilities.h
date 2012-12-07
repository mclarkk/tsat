/****************************************************************
 *
 * Author: Justin Bradley
 * Title: Utilities.h
 * Date: Saturday, April 23, 2011
 * Description: Header file for important utility functions
 *
 ****************************************************************/

#ifndef UTILITIES_H
#define UTILITIES_H

typedef struct timeval TimeVal;
typedef struct timespec TimeSpec;

/***********************************************************
 * Function: get_timestamp - gets a timestamp and converts it to a double
 * Parameters: none
 * Return Value: returns a double representing seconds since the epoch
 ***********************************************************/
double get_timestamp();

/***********************************************************
 * Function: get_timestamp - populates the timestamp struct with the current
 *           time since the epoch
 * Parameters: timestamp - a timeval struct (typecasted above) to be populated
 * Return Value: none
 ***********************************************************/
void get_timestamp(TimeVal *timestamp);

int hc_sleep(double sleeptime);

int convert_double_to_timespec(TimeSpec * ts, double time);

int convert_double_to_timeval(TimeVal * tv, double time);

/***********************************************************
 * Function: copy_timeval - copies the contents of tv_from into tv_to
 * Parameters: tv_to - copy to
 *             tv_from - copy from
 * Return Value: 0 on success
 ***********************************************************/
int copy_timeval(TimeVal *tv_to, TimeVal *tv_from);

/***********************************************************
 * Function: timeval_subtract - subtract value y from x
 * Parameters: result - the resulting TimeVal
 *             x - first time val (should be greater than y)
 *             y - second time val
 * Return Value: returns 1 if diff is negative, 0 otherwise
 ***********************************************************/
int timeval_subtract(TimeVal *result, TimeVal *x, TimeVal *y);

/***********************************************************
 * Function: timeval_subtract - same as above except converts result to a double
 *           and returns it
 * Parameters: x - first time val (should be greater than y)
 *             y - second time val
 * Return Value: difference in seconds and fractions of seconds
 ***********************************************************/
double timeval_subtract(TimeVal *x, TimeVal *y);

/***********************************************************
 * Function: timeval_subtract - subtract y from x
 * Parameters: x - first time value (should be greater than y)
 *             y - second time value
 * Return Value: difference in seconds and fractions of seconds
 *               returns -1 if x<y
 ***********************************************************/
double time_subtract(double x, double y);

#endif // UTILITIES_H
