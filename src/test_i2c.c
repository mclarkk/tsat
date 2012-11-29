/*************************
 * Test file for I2c
 *************************/

#include <stdio.h>

#include "../include/I2c_comm.h"

int main() 
{
        int i = 5;
        
        if (I2c_comm_init()) {
                printf("I2c init error.\n");
                return -1;
        }
        
        /* nanosleep(&sleeptime, NULL); */
        
        while (i--) {
                fflush(stdout);
                printf("Before I2c\n");
                I2c_comm_loop();
                printf("After I2c\n");
                fflush(stdout);
        }

        I2c_comm_deinit();

        return 0;
}
