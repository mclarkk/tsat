/****************************************************************
 *
 * Author: Justin Bradley
 * Title: Utilities.c
 * Date: Saturday, April 23, 2011
 * Description: Contains implementation for important utility functions
 *
 ****************************************************************/

#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include "../include/Utilities.h"
#include "../include/Config.h"

#define MICROSECONDS_CONVERT 1000000
#define NANOSECONDS_CONVERT  1000000000
//#define NULL 0

double get_timestamp()
{
        double seconds, microseconds;
        TimeVal tp;
        gettimeofday(&tp,NULL);
        seconds = (double)tp.tv_sec;
        microseconds = ((double)(tp.tv_usec))/MICROSECONDS_CONVERT;
        return seconds + microseconds;
}

void get_timestamp(TimeVal *timestamp)
{
        gettimeofday(timestamp,NULL);
}

int hc_sleep(double sleeptime)
{
        // ummm, can't sleep for negative time duh!
        if(sleeptime < 0.0)
        {
                return -1;
        }
        TimeSpec sleeptime_struct;
        TimeSpec remaining;
        convert_double_to_timespec(&sleeptime_struct, sleeptime);
        nanosleep(&sleeptime_struct, &remaining);
        return 0;
}

int convert_double_to_timespec(TimeSpec * ts, double time)
{
        int seconds = (int)time;
        int nanoseconds = (int)((time - seconds)*NANOSECONDS_CONVERT);
        ts->tv_sec = seconds;
        ts->tv_nsec = nanoseconds;
        return 0;
}

int convert_double_to_timeval(TimeVal * tv, double time)
{
        int seconds = (int)time;
        int microseconds = (int)((((double)seconds) - time)*MICROSECONDS_CONVERT);
        tv->tv_sec = seconds;
        tv->tv_usec = microseconds;
        return 0;
}

int copy_timeval(TimeVal *tv_to, TimeVal *tv_from)
{
        tv_to->tv_sec = tv_from->tv_sec;
        tv_to->tv_usec = tv_from->tv_usec;
        return 0;
}


int timeval_subtract(TimeVal *result, TimeVal *x, TimeVal *y)
{
        /* Perform the carry for the later subtraction by updating y. */
        if (x->tv_usec < y->tv_usec) {
                int nsec = (y->tv_usec - x->tv_usec) / MICROSECONDS_CONVERT + 1;
                y->tv_usec -= MICROSECONDS_CONVERT * nsec;
                y->tv_sec += nsec;
        }
        if (x->tv_usec - y->tv_usec > MICROSECONDS_CONVERT) {
                int nsec = (x->tv_usec - y->tv_usec) / MICROSECONDS_CONVERT;
                y->tv_usec += MICROSECONDS_CONVERT * nsec;
                y->tv_sec -= nsec;
        }

        /* Compute the time remaining to wait.
           tv_usec is certainly positive. */
        result->tv_sec = x->tv_sec - y->tv_sec;
        result->tv_usec = x->tv_usec - y->tv_usec;

        /* Return 1 if result is negative. */
        return x->tv_sec < y->tv_sec;
}

double timeval_subtract(TimeVal *x, TimeVal *y)
{
        double seconds, microseconds;
        /* Perform the carry for the later subtraction by updating y. */
        if (x->tv_usec < y->tv_usec) {
                int nsec = (y->tv_usec - x->tv_usec) / MICROSECONDS_CONVERT + 1;
                y->tv_usec -= MICROSECONDS_CONVERT * nsec;
                y->tv_sec += nsec;
        }
        if (x->tv_usec - y->tv_usec > MICROSECONDS_CONVERT) {
                int nsec = (x->tv_usec - y->tv_usec) / MICROSECONDS_CONVERT;
                y->tv_usec += MICROSECONDS_CONVERT * nsec;
                y->tv_sec -= nsec;
        }

        /* Compute the time remaining to wait.
           tv_usec is certainly positive. */
        seconds = (double)x->tv_sec - (double)y->tv_sec;
        microseconds = ((double)x->tv_usec - (double)y->tv_usec)/MICROSECONDS_CONVERT;

        /* Return -1 if result is negative. */
        if(x->tv_sec < y->tv_sec)
        {
                return -1.0;
        }
        return seconds + microseconds;
}

double time_subtract(double x, double y)
{
        if(x<y)
        {
                return -1.0;
        }
        return x-y;
}
