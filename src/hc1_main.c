/****************************************************************
 *
 * Author: Justin Bradley
 * Title: hc1_main.c
 * Date: Thursday, April  7, 2011
 * Description: The main round robin function for the hc1
 *
 ****************************************************************/

#include <stdio.h>
#include <time.h>
#include <signal.h>
// no longer using I2C...sniff sniff
//#include "../include/I2c_comm.h"
#include "../include/Plan.h"
#include "../include/Comm.h"
#include "../include/Estimation.h"
#include "../include/Control.h"
#include "../include/hc1_globals.h"
#include "../include/Utilities.h"
#include "../include/Config.h"

void sig_handler(int);

//int main(int argc, char* argv[])
int main()
{
        int plan_init_failed;
        int estimation_init_failed;
        int control_init_failed;
        int comm_init_failed;

        TimeVal timestamp_begin_loop;
        TimeVal timestamp_prev;
        TimeVal timestamp_now;
        double curr_time_diff;

        // install the signal handlers
        signal(SIGABRT, &sig_handler);
        signal(SIGTERM, &sig_handler);
	signal(SIGINT, &sig_handler);

#if defined(DEBUG)
        printf("Starting Initialization...\n");
        printf("SYSTEM_LOOP_TIME=%f\n",SYSTEM_LOOP_TIME);
        printf("COMM_LOOP_TIME=%f\n",COMM_LOOP_TIME);
        printf("PLAN_LOOP_TIME=%f\n",PLAN_LOOP_TIME);
        printf("ESTIMATION_LOOP_TIME=%f\n",ESTIMATION_LOOP_TIME);
        printf("CONTROL_LOOP_TIME=%f\n",CONTROL_LOOP_TIME);        
#endif

        /******************************************************
         * Initialize
         ******************************************************/
        if((plan_init_failed = Plan_init()) != 0)
        {
                printf("Plan_init failed with error=%d",plan_init_failed);
                return -1;
        }
        if((estimation_init_failed = Estimation_init()) != 0)
        {
                printf("Estimation_init failed with error=%d",estimation_init_failed);
                return -1;
        }
        if((control_init_failed = Control_init()) != 0)
        {
                printf("Control_init failed with error=%d",control_init_failed);
                return -1;
        }
        if((comm_init_failed = Comm_init()) != 0)
        {
                printf("Comm_init failed with error=%d",comm_init_failed);
                return -1;
        }

        /******************************************************
         * Main Loop
         ******************************************************/
#if defined(DEBUG)
        printf("Starting Main Loop...\n");
#endif
        while(!halt_G)
        {
                // get the timestamp at the beginning of the loop
                get_timestamp(&timestamp_begin_loop);

                // Comm to groundstation
                Comm_loop();

                // sleep until Comm_loop's time is up
                get_timestamp(&timestamp_now);
                curr_time_diff = timeval_subtract(&timestamp_now,&timestamp_begin_loop);
                /* printf("approximate execution time of Comm = %g\n",curr_time_diff); */
                /* printf("remaining = %g\n",time_subtract(COMM_LOOP_TIME,curr_time_diff)); */
                hc_sleep(time_subtract(COMM_LOOP_TIME,curr_time_diff));

                // plan
                Plan_loop();

                // sleep until Plan_loop's time is up
                copy_timeval(&timestamp_prev, &timestamp_now);
                get_timestamp(&timestamp_now);
                curr_time_diff = timeval_subtract(&timestamp_now,&timestamp_prev);
                /* printf("approximate execution time of Plan = %g\n",curr_time_diff); */
                /* printf("remaining = %g\n",time_subtract(PLAN_LOOP_TIME,curr_time_diff)); */
                hc_sleep(PLAN_LOOP_TIME - curr_time_diff);

                // read sensors and estimate
                Estimation_loop();

                // sleep until Estimation_loop's time is up
                copy_timeval(&timestamp_prev, &timestamp_now);
                get_timestamp(&timestamp_now);
                curr_time_diff = timeval_subtract(&timestamp_now,&timestamp_prev);
                /* printf("approximate execution time of Estimation = %g\n",curr_time_diff); */
                /* printf("remaining = %g\n",time_subtract(ESTIMATION_LOOP_TIME,curr_time_diff)); */
                hc_sleep(ESTIMATION_LOOP_TIME - curr_time_diff);

                // control
                Control_loop();

                // sleep until the entire system time step is up
                get_timestamp(&timestamp_now);
                curr_time_diff = timeval_subtract(&timestamp_now,&timestamp_begin_loop);
                /* printf("approximate execution time of all tasks = %g\n",curr_time_diff); */
                /* printf("remaining = %g\n",time_subtract(SYSTEM_LOOP_TIME,curr_time_diff)); */
                hc_sleep(SYSTEM_LOOP_TIME - curr_time_diff);
                
                get_timestamp(&timestamp_now);
                printf("Approximate sampling time = %g\n\n",timeval_subtract(&timestamp_now,&timestamp_begin_loop));
        }

        /******************************************************
         * Deinitialize (attempt to shutdown gracefully)
         ******************************************************/
#if defined(DEBUG)
        printf("Stopping Main Loop...Starting Deinitialization\n");
#endif
        if((plan_init_failed = Plan_deinit()) != 0)
        {
                printf("Plan_deinit failed with error=%d",plan_init_failed);
        }
        if((estimation_init_failed = Plan_deinit()) != 0)
        {
                printf("Estimation_deinit failed with error=%d",estimation_init_failed);
        }
        if((control_init_failed = Control_deinit()) != 0)
        {
                printf("Control_deinit failed with error=%d",control_init_failed);
        }
        if((comm_init_failed = Control_deinit()) != 0)
        {
                printf("Comm_deinit failed with error=%d",comm_init_failed);
        }
        return 0;
}

void sig_handler(int)
{
        // set the halt flag to 1 so we break out of the loop
        halt_G = 1;
}
