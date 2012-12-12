/****************************************************************
 *
 * Author: Meghan Clark, based on hc1_main by Justin Bradley
 * Title: tsat_main.c
 * Date: December 10, 2012
 * Description: The main round robin function for the TableSat
 *
 ****************************************************************/

#include <stdio.h>
#include <time.h>
#include <signal.h>
// no longer using I2C...sniff sniff
//#include "../include/I2c_comm.h"
#include "../include/tsat_globals.h"
#include "../include/Camera.h"
#include "../include/Control.h"
#include "../include/Utilities.h"
#include "../include/Config.h"

#define THREE_MIN 180 //seconds

void sig_handler(int);

//int main(int argc, char* argv[])
int main()
{
        double angular_velocity = 2.418;
        int control_init_failed;
        int camera_init_failed;

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
    //    printf("CONTROL_LOOP_TIME=%f\n",CONTROL_LOOP_TIME);        
        printf("CAMERA_LOOP_TIME=%f\n",CAMERA_LOOP_TIME);
#endif

        /******************************************************
         * Initialize
         ******************************************************/
        if((control_init_failed = Control_init(angular_velocity)) != 0)
        {
                printf("Control_init failed with error=%d",control_init_failed);
                return -1;
        }
        if((camera_init_failed = Camera_init()) != 0)
        {
                printf("Camera_init failed with error=%d",camera_init_failed);
                return -1;
        }

        Control_loop();
        hc_sleep(THREE_MIN);

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
                Camera_loop();

                // sleep until Comm_loop's time is up
                get_timestamp(&timestamp_now);
                curr_time_diff = timeval_subtract(&timestamp_now,&timestamp_begin_loop);
                hc_sleep(time_subtract(CAMERA_LOOP_TIME,curr_time_diff));

                //hc_sleep(SYSTEM_LOOP_TIME - curr_time_diff); 
                
                //get_timestamp(&timestamp_now);

        /******************************************************
         * Deinitialize (attempt to shutdown gracefully)
         ******************************************************/
#if defined(DEBUG)
        printf("Stopping Main Loop...Starting Deinitialization\n");
#endif
        if((control_init_failed = Control_deinit()) != 0)
        {
                printf("Control_deinit failed with error=%d",control_init_failed);
        }
        if((camera_init_failed = Camera_deinit()) != 0)
        {
                printf("Camera_deinit failed with error=%d",camera_init_failed);
        }
        return 0;
}

void sig_handler(int)
{
        // set the halt flag to 1 so we break out of the loop
        halt_G = 1;
}
