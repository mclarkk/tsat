/****************************************************************
 *
 * Author: Justin Bradley and Richard Choroszucha
 * Title: Control.c
 * Date: Thursday, April  7, 2011
 * Description: Controller implementation for Hovercraft
 *
 ****************************************************************/

#include <stdio.h>
#include <math.h>
#include "../include/Config.h"
#include "../include/pwm.h"
#include "../include/Utilities.h"

#define FAN_STOP 0

/******************************************************
 * Extern variables
 ******************************************************/
//Time Step
extern float CONTROL_LOOP_TIME;

/******************************************************
 * Local variables
 ******************************************************/
double angular_velocity;

/******************************************************
 * Local functions
 ******************************************************/


int Control_init(double av)
{	
        angular_velocity = av;
        pwminit();
        return 0;
}
  
void Control_loop()
{
        // From fitted functions
        double p1  = 36.4;
        double p2 = 209.3;
        int pwm11duty = (int) p1 * angular_velocity + p2;
        pwm11(pwm11duty);
}


int Control_deinit()
{
        pwminit();
        return 0;
}

