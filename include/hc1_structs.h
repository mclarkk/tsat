/****************************************************************
 *
 * Author: Justin Bradley
 * Title: hc1_structs.h
 * Date: Thursday, April  7, 2011
 * Description: This contains the definitions of all the globally
 *              shared structures
 *
 ****************************************************************/

#ifndef HC1_STRUCTS_H
#define HC1_STRUCTS_H

#include <sys/time.h>

typedef double TimeStamp;

typedef struct
{
        // local positions
        float x_position;
        float y_position;
        // local velocities
        float x_velocity;
        float y_velocity;
        // local orientation
        float angle; 
        float angular_velocity;
        TimeStamp timestamp;
} StateInfo;

typedef struct
{
        StateInfo target; // destination target
        int achieved;
        TimeStamp timestamp;
} Command;

typedef struct
{
        StateInfo state;
        // fan information
        float thrust_magnitude;
        float thrust_angle;
        // lifting fan
        int lift_magnitude;
        // current command
        Command command;
        // integrator states
        float x_error_integral;
        float y_error_integral;
        float angle_error_integral;
        // timestamp
        TimeStamp timestamp;
} HovercaftInfo;

typedef struct
{
        float thrust_magnitude;
        float thrust_angle;
        int lift_magnitude;//On/off parameter
        TimeStamp timestamp;
} ControlInfo;



#endif
