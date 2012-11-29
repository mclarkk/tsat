/****************************************************************
 *
 * Author: Justin Bradley
 * Title: Estimation.c
 * Date: Saturday, April 23, 2011
 * Description: Estimation loop
 *
 ****************************************************************/

#include <stdio.h>
#include <math.h>
#include "../include/Estimation.h"
#include "../include/hc1_structs.h"
#include "../include/Config.h"
#include "../include/adc.h"
#include "../include/Utilities.h"

/******************************************************
 * Extern variables
 ******************************************************/
// current best estimate of what the HC is doing
extern HovercaftInfo hc_info_G;
// current best estimate of HC position, velocity, orientation from machine
// vision system
extern StateInfo state_from_vision_G;
// control input
extern ControlInfo hc_control_G;
// current command from the planner
extern Command planner_cmd_G;

// timing
extern const float ESTIMATION_LOOP_TIME;

#define GYRO_CHANNEL 2
//#define SONAR0_CHANNEL 3
//#define SONAR90_CHANNEL 4
//#define SONAR180_CHANNEL 5

// My sensor struct
typedef struct
{
        float gyro;
        // 0 degrees points directly from the CG to the nacelle
        // numbers are degrees relative to 0 assuming degrees increase rotating
        // to the right
        float sonar0;
        float sonar90;
        float sonar180;
        TimeStamp timestamp;
} SensorValues;

// my local sensor readings
SensorValues sensors;
StateInfo state_from_sensors;

// table calibration and other variables
const float hc_radius = 0.17f;
const float x_vision_0 = 5.0f;
const float x_vision_max = 549.0f;
const float y_vision_0 = 78.0f;
const float y_vision_max = 404.0f;
const float table_length = 93.75*2.54/100; // inches->cm->m
//const float table_length = 93.75*2.54/100 -2.0*hc_radius; // inches->cm->m
const float x_max = table_length;
const float table_width = 54*2.54/100; // inches->cm->m
//const float table_width = 54*2.54/100 -2.0*hc_radius; // inches->cm->m
const float y_max = table_width;
const float convert_to_rad = M_PI/180;
const float convert_to_deg = 180/M_PI;
static unsigned int flag = 0; // flag to indicate we now have a vision update


/***********************************************************
 * Function: read_sensors
 * Parameters: 
 * Return Value: 0 on success, -1 on failure
 ***********************************************************/
unsigned int read_sensors();

unsigned int estimate_state();

float in_range(float);

int Estimation_init()
{
#if !defined(SIMULATION)
        int adc_init_success = initADC();
        if(adc_init_success != ADC_INIT_SUCCESS)
        {
                return -1;
        }
#endif
        return 0;
}

void Estimation_loop()
{
#if !defined(SIMULATION)
        int read_success = read_sensors();
        if(read_success != 0) // 0 means success
        {
                // the whole function just bails if any of the reads fail
                return;
        }
#endif
        estimate_state();
}

int Estimation_deinit()
{
#if !defined(SIMULATION) // only read from the sensors if this isn't a simulation
        int adc_deinit_success = deinitADC();
        if(adc_deinit_success != ADC_DEINIT_SUCCESS)
        {
                return -1;
        }
#endif
        return 0;
}

unsigned int read_sensors()
{
        // read each channel and convert it to something useful
        // read gyro
        int gyro_count = readADC(7);
        if(gyro_count == ADC_READ_ERROR)
        {
                return -1;
        }
        float gyro_volts = convertADCValueToVolts(gyro_count);
        // magic numbers below come from gyro calibration
        // and give results in rad/sec
        sensors.gyro = -1.0f*(-1.4721799* gyro_volts+ 1.82256189);

        // not using SONARS right now
        //// read sonar0
        //int sonar0_count = readADC(SONAR0_CHANNEL);
        //if(sonar0_count == ADC_READ_ERROR)
        //{
        //        return -1;
        //}
        //float sonar0_volts = convertADCValueToVolts(sonar0_count);
        //// Need to add code to convert to meters or whatever
        //sensors.sonar0 = 0;

        //// read sonar90
        //int sonar90_count = readADC(SONAR90_CHANNEL);
        //if(sonar90_count == ADC_READ_ERROR)
        //{
        //        return -1;
        //}
        //float sonar90_volts = convertADCValueToVolts(sonar90_count);
        //// Need to add code to convert to meters or whatever
        //sensors.sonar90 = 0;

        //// read sonar180
        //int sonar180_count = readADC(SONAR180_CHANNEL);
        //if(sonar180_count == ADC_READ_ERROR)
        //{
        //        return -1;
        //}
        //float sonar180_volts = convertADCValueToVolts(sonar180_count);
        //// Need to add code to convert to meters or whatever
        //sensors.sonar180 = 0;

        // set the timestamp
        sensors.timestamp = get_timestamp();

        return 0; // nominal
}

unsigned int estimate_state()
{
        // note the vision system assumes a different coordinate system (image
        // coordinates) and positions/velocities are in pixels
	float x_prev = hc_info_G.state.x_position;
	//float x_vision = table_length*(1.0f - (state_from_vision_G.x_position - x_vision_0)/(x_vision_max - x_vision_0));
	float x_vision = table_length*(1.0f - (state_from_vision_G.x_position - x_vision_0)/(x_vision_max - x_vision_0)) +hc_radius;
	float y_prev = hc_info_G.state.y_position;
//	float y_vision = table_width*(state_from_vision_G.y_position - y_vision_0)/(y_vision_max - y_vision_0) ;
	float y_vision = table_width*(state_from_vision_G.y_position - y_vision_0)/(y_vision_max - y_vision_0) +hc_radius;
	float x_velocity_prev = hc_info_G.state.x_velocity;
	float y_velocity_prev = hc_info_G.state.y_velocity;
	float x_velocity_vision = table_length*(-1.0f*(state_from_vision_G.x_velocity)/(x_vision_max - x_vision_0));
	float y_velocity_vision = table_width*((state_from_vision_G.y_velocity)/(y_vision_max - y_vision_0));
	float angle_prev = hc_info_G.state.angle;
        // angle from vision comes in degrees and in a different coordinate
        // frame. we need to convert it
	float angle_vision = (-1.0f*(convert_to_rad * state_from_vision_G.angle)) + M_PI_2;
        // now we need to limit the angle to +-180 degrees
        angle_vision = in_range(angle_vision);
	//float angular_velocity_vision = convert_to_rad * state_from_vision_G.angular_velocity;
        float angular_velocity_prev = hc_info_G.state.angular_velocity;

        // tuning knob
        float with_vision_weight = 0.8f; // weights new info from the vision
        // system
        float without_vision_weight = 0.2f; // weights integrated terms without
                                            // vision
        float weight_angle = 0.3f;

        float temp;

	double delta_t = ESTIMATION_LOOP_TIME;

        double time_last_vision = state_from_vision_G.timestamp;

        // check to see if we got a new vision update
	if (time_subtract(hc_info_G.timestamp,time_last_vision) < 0.0) 
	{
                // got new vision update
                hc_info_G.state.x_position = (1.0f-with_vision_weight)*(x_prev+x_velocity_vision*delta_t) + with_vision_weight*x_vision;
                hc_info_G.state.y_position = (1.0f-with_vision_weight)*(y_prev+y_velocity_vision*delta_t) + with_vision_weight*y_vision;
                hc_info_G.state.x_velocity = x_velocity_vision;
                hc_info_G.state.y_velocity = y_velocity_vision;

                // need to set the angle for the first time I've gotten a angle measurement
                if(flag==0)
                {
                        hc_info_G.state.angle = angle_vision;
                }
                else
                {
                        temp = ((1.0f - weight_angle) * angle_prev) + (weight_angle * angle_vision);
                        hc_info_G.state.angle = in_range(temp);
                }
                hc_info_G.state.angular_velocity = sensors.gyro;
                
                // set the flag
                flag = 1;
	}
	else 
        {
                // no new vision information
                // limit x and y position to constraints of the laser table
                temp = x_prev + without_vision_weight * (x_velocity_prev*delta_t);
                hc_info_G.state.x_position = (temp>(x_max-hc_radius) ? (x_max-hc_radius) : (temp<(0.0f+hc_radius) ? (0.0f+hc_radius) : temp));

                temp = y_prev + without_vision_weight * (y_velocity_prev*delta_t);
                hc_info_G.state.y_position = (temp>(y_max-hc_radius) ? (y_max-hc_radius) : (temp<(0.0f+hc_radius) ? (0.0f+hc_radius) : temp));

                // I need an initial angle to start with, so don't do any of
                // this until I get at least one vision update
                if(flag==0)
                {
                        hc_info_G.state.angle = 0.0f;
                }
                else
                {
                        temp = angle_prev + (angular_velocity_prev*delta_t);
                        hc_info_G.state.angle = in_range(temp);
                }

                hc_info_G.state.x_velocity = x_velocity_prev;
                hc_info_G.state.y_velocity = y_velocity_prev;
                hc_info_G.state.angular_velocity = sensors.gyro;
	}
        // set the integral states
        // control is calculated assuming desired minus actual (e.g. x_desired -
        // x)
        if(!planner_cmd_G.achieved)
        {
                hc_info_G.x_error_integral += (planner_cmd_G.target.x_position - hc_info_G.state.x_position)*delta_t;
                hc_info_G.y_error_integral += (planner_cmd_G.target.y_position - hc_info_G.state.y_position)*delta_t;
                hc_info_G.angle_error_integral += (planner_cmd_G.target.angle - hc_info_G.state.angle)*delta_t;
        }
        else
        {
                hc_info_G.x_error_integral = 0.0f;
                hc_info_G.y_error_integral = 0.0f;
                hc_info_G.angle_error_integral = 0.0f;
        }
        
        // also set the command and the desired control inputs
        hc_info_G.thrust_magnitude = hc_control_G.thrust_magnitude;
        hc_info_G.thrust_angle = hc_control_G.thrust_angle;
        hc_info_G.lift_magnitude = hc_control_G.lift_magnitude;
        hc_info_G.command = planner_cmd_G;

        printf("est x_pos = %f\n",hc_info_G.state.x_position);
        printf("est y_pos = %f\n",hc_info_G.state.y_position);
        printf("est angle = %f\n",convert_to_deg*hc_info_G.state.angle);
        printf("est angular_vel = %f\n", hc_info_G.state.angular_velocity);

        // set the timestamp
        hc_info_G.timestamp = get_timestamp();
        return 0; // nominal
}

// convert our angle to plus or minus 180 degrees
float in_range(float angle)
{
        return fmod(angle+M_PI,2.0*M_PI) - M_PI;
}
