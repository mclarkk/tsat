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
#include "../include/hc1_structs.h"
#include "../include/Config.h"
#include "../include/pwm.h"
#include "../include/Utilities.h"


/******************************************************
 * Extern variables
 ******************************************************/
// command from the planner
extern Command planner_cmd_G;
// complete hovercraft information
extern HovercaftInfo hc_info_G;
//complete control info
extern ControlInfo hc_control_G;
//Time Step
extern float CONTROL_LOOP_TIME;
// goal distance
extern const float goal_threshold_G;

/******************************************************
 * Local variables
 ******************************************************/
float Fmax=1;
float phimax=M_PI/3;
int stopLift=0;
pwmQuery pwmPrevious; // from pwm.h

/******************************************************
 * Local functions
 ******************************************************/
void PWM_Commands(void);
void pwmServo(void);
void pwmThruster(void);
void pwmLift(void);


int Control_init()
{	
        pwminit();
        return 0;
}
  
void Control_loop()
{
	stopLift=0;
	float FGains[]={-0.1,-1,-5,  -0.2,-1,-3,  -0.03,-0.1,-3};//Just for error dynamics
	float phiGains[]={-0.1,-1,-3};//{Integral,Proportional,Derivative}
	float F=0;//Force [N]
	float phi=0;//Fan angle [rad]
	float distance=sqrt(pow(planner_cmd_G.target.x_position-hc_info_G.state.x_position,2)+
                            pow(planner_cmd_G.target.y_position-hc_info_G.state.y_position,2));

//	float haveABall=0.5;
        // determine the norm of the phiGains
	float aNorm=0;
	for(int a1=0;a1<3;a1++)
	{
		aNorm=aNorm+pow(phiGains[a1],2);
	}
	aNorm=sqrt(aNorm);
	
	if (distance>goal_threshold_G)//Distance from desired
	{

		F=Fmax;
                float termI = phiGains[0]*atan2(hc_info_G.y_error_integral,hc_info_G.x_error_integral);
                float termP = phiGains[1]*(atan2(planner_cmd_G.target.y_position - hc_info_G.state.y_position, planner_cmd_G.target.x_position - hc_info_G.state.x_position) - hc_info_G.state.angle);
                // need to put in theta_d_dot
                float termD = phiGains[2]*(planner_cmd_G.target.angular_velocity - hc_info_G.state.angular_velocity);
                phi = termI + termP + termD;
	}
	else if (distance<=goal_threshold_G) // if we are within goal_threshold_G[m] of our target
	{
		F=FGains[0]*(hc_info_G.x_error_integral)+
			FGains[1]*(planner_cmd_G.target.x_position-hc_info_G.state.x_position)+
			FGains[2]*(planner_cmd_G.target.x_velocity-hc_info_G.state.x_velocity)+
			FGains[3]*(hc_info_G.y_error_integral)+
			FGains[4]*(planner_cmd_G.target.y_position-hc_info_G.state.y_position)+
			FGains[5]*(planner_cmd_G.target.y_velocity-hc_info_G.state.x_velocity)+
			FGains[6]*(hc_info_G.angle_error_integral)+
			FGains[7]*(planner_cmd_G.target.angle-hc_info_G.state.angle)+
			FGains[8]*(planner_cmd_G.target.angular_velocity-hc_info_G.state.angular_velocity);
		phi=phiGains[0]*(atan2(hc_info_G.y_error_integral,hc_info_G.x_error_integral)-hc_info_G.angle_error_integral)+
			phiGains[1]*atan2(planner_cmd_G.target.y_position-hc_info_G.state.y_position,
                                          planner_cmd_G.target.x_position-hc_info_G.state.x_position)+
                        phiGains[2]*atan2(planner_cmd_G.target.y_velocity-hc_info_G.state.y_velocity,
                                          planner_cmd_G.target.x_velocity-hc_info_G.state.x_velocity)-hc_info_G.state.angle;             
	}


	//Wait for next command
/*
	if (distance<=goal_threshold_G)
	{
		F=0;
		phi=0;
		stopLift=1;
	}
	else
	{
		stopLift=0;
	}

*/
	/*
          if (F<0) //If overshoot happens, shutdown lift fan.
          {
          F=0;
          stopLift=1;
          }
	*/

	//Check and Obey Constraints
	if (F>Fmax)
	{
		F=Fmax;
	}
	
	if (fabs(phi)>=phimax)
	{
		phi=phi/fabs(phi)*phimax;
	}
	
	//F=DT*F;
	hc_control_G.thrust_magnitude=F;
	hc_info_G.thrust_magnitude=F;
	hc_control_G.thrust_angle=phi;
	hc_info_G.thrust_angle=phi;
	hc_control_G.lift_magnitude=stopLift;
	hc_control_G.timestamp=get_timestamp();
        PWM_Commands();
}


void PWM_Commands()
{
        // NOTE:
        // PWM8 = servo
        // PWM10 = thrust
        // PWM11 = lift
	pwmServo();
        pwmThruster();
	pwmLift();
}


void pwmServo()
{
	// calculate PWM value for servo
	double servoSlope=(SERVO_MAX*1.0-SERVO_MIN*1.0)/2;
	double servoIntercept=SERVO_MID;
	double servo=-servoSlope*(hc_control_G.thrust_angle/phimax)+servoIntercept;
	if (servo>SERVO_MAX)
	{
		servo=SERVO_MAX;
	}
	if (servo<=SERVO_MIN)
	{
		servo=SERVO_MIN;
	}
	pwm8(servo);
	pwmPrevious.pwm8=servo;
}


void pwmThruster()
{
	// calculate PWM value for thrust
	//double thrustSlope=MOTOR_MAX-MOTOR_MIN;
        double thrustSlope=(MOTOR_MAX*1.0f-MOTOR_MIN*1.0f)/2;
	double thrustIntercept=MOTOR_MIN*1.0f;
	double motor=thrustSlope*(hc_control_G.thrust_magnitude/Fmax)+thrustIntercept;
	if (motor>MOTOR_MAX)
	{
		motor=MOTOR_MAX;
	}
	if (motor<MOTOR_MIN)
	{
		motor=MOTOR_MIN;
	}
        //motor = MOTOR_STOP; // just for test
	pwm10(motor);
	pwmPrevious.pwm10=motor;
}


void pwmLift()
{
	double motor=0;
	// calculate PWM value for lift
        if (stopLift || fabs(hc_info_G.state.angular_velocity)>2)
	{
		motor = MOTOR_MIN;                
		//motor=30.0;
	}
	else
	{
                motor = 55; // magic number for use in testing
	}
        //printf("motor = %f\n",motor);
        printf("Angular Rate: %f\n",fabs(hc_info_G.state.angular_velocity));
        //motor = MOTOR_STOP; // just for test
        pwm11(motor);
        pwmPrevious.pwm11 = motor;
}


int Control_deinit()
{
        pwminit();
        return 0;
}

