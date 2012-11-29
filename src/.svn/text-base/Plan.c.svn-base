/****************************************************************
 *
 * Author: Justin Bradley & Gerardo Cruz
 * Title: Plan.c
 * Date: Thursday, April  7, 2011
 * Description: Planning implementation for Hovercraft
 *
 ****************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "../include/hc1_structs.h"
#include "../include/Plan.h"
#include "../include/Config.h"
#include "../include/Utilities.h"

/******************************************************
 * Extern variables
 ******************************************************/
// command from the planner
extern Command planner_cmd_G;
// complete hovercraft information
extern HovercaftInfo hc_info_G;
// state from vision
extern StateInfo state_from_vision_G;
// command from user
extern Command user_cmd_G;
// goal threshold

extern const float goal_threshold_G;

// plan
static Command goal1 = {{2.4f, 1.4f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 0, 0.0};
//static Command goal1 = {{1.4f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, 0, 0.0};
static Command goal2 = {{0.2f, 0.2f, 0.0f, 0.0f, 0.0f, 0.75498f, 0.0f}, 0, 0.0};

// Wall avoidance parameters
const float hc_radius = 0.17f;
const float table_length = 93.75*2.54/100 -2.0*hc_radius; // inches->cm->m
const float x_max = table_length;
const float x_min = 0;
const float table_width = 54*2.54/100 -2.0*hc_radius; // inches->cm->m
const float y_max = table_width;
const float y_min = 0;
const float escape_angle = 1.57;
const float wall_threshold = 0.05;


int Plan_init()
{
        return 0;
}

void Plan_loop()
{        
	float distance;
	int wall_flag = 0;

        if(state_from_vision_G.timestamp == 0.0)
        {
                return;
        }
 

        // do goal 1 first
        if(!goal1.achieved && user_cmd_G.achieved)
        {
                planner_cmd_G.achieved = 0;
                planner_cmd_G = goal1;
                if(planner_cmd_G.timestamp == 0.0)
                {
                        planner_cmd_G.timestamp = get_timestamp();
                }
                distance=sqrt(pow(planner_cmd_G.target.x_position-hc_info_G.state.x_position,2)+
                                    pow(planner_cmd_G.target.y_position-hc_info_G.state.y_position,2));
                if(distance <= goal_threshold_G)
                {
			printf("REACHED GOAL 1 \n");
                        planner_cmd_G.achieved = 1;
                        goal1.achieved = 1;
                }

                printf("Command in goal1{x=%f, y=%f}\n",planner_cmd_G.target.x_position,planner_cmd_G.target.y_position);
        }

        // once goal1 achieved, do goal 2
        if(!goal2.achieved && goal1.achieved && user_cmd_G.achieved)
        {
                planner_cmd_G.achieved = 0;
                planner_cmd_G = goal2;
                if(planner_cmd_G.timestamp == 0.0)
                {
                        planner_cmd_G.timestamp = get_timestamp();
                }
                distance=sqrt(pow(planner_cmd_G.target.x_position-hc_info_G.state.x_position,2)+
                                    pow(planner_cmd_G.target.y_position-hc_info_G.state.y_position,2));
                if(distance <= goal_threshold_G)
                {
			printf("REACHED GOAL 2 \n");
                        planner_cmd_G.achieved = 1;
                        goal2.achieved = 1;
                }
                printf("Command in goal2 {x=%f, y=%f}\n",planner_cmd_G.target.x_position,planner_cmd_G.target.y_position);

        }

	// Interrupt for user commands (Use 2 for indicating a new command)        
        if(!user_cmd_G.achieved)
        {
                planner_cmd_G.achieved = 0;
		planner_cmd_G.target = user_cmd_G.target;
		planner_cmd_G.timestamp = user_cmd_G.timestamp;

		printf("User Command {x=%f, y=%f}\n",planner_cmd_G.target.x_position,planner_cmd_G.target.y_position);
                
		distance=sqrt(pow(planner_cmd_G.target.x_position-hc_info_G.state.x_position,2)+
                                    pow(planner_cmd_G.target.y_position-hc_info_G.state.y_position,2));

                if(distance <= goal_threshold_G)
                {
			printf("REACHED USER GOAL\n");
                        planner_cmd_G.achieved = 1;
			user_cmd_G.achieved = 0;
                }
	
	}

	// For walls
	if (fabs(hc_info_G.state.x_position-x_min) < wall_threshold || fabs(hc_info_G.state.x_position-x_max) < wall_threshold)
	{
		planner_cmd_G.target = hc_info_G.state;
		planner_cmd_G.target.angle = escape_angle;
		printf("Near X Wall \n");
		wall_flag = 1;
	}

	if (fabs(hc_info_G.state.y_position-y_min) < wall_threshold || fabs(hc_info_G.state.y_position-y_max) < wall_threshold)
	{
		planner_cmd_G.target = hc_info_G.state;
		planner_cmd_G.target.angle = escape_angle;
		printf("Near Y Wall \n");
		wall_flag = 1;
	}

	// Print Planner Information
	if (!planner_cmd_G.achieved && !wall_flag)
	{
		printf("Distance to Goal: %f \n", distance);
		printf("Command{x=%f, y=%f}\n",planner_cmd_G.target.x_position,planner_cmd_G.target.y_position);
	}
	else if (planner_cmd_G.achieved && !wall_flag)
	{
		printf("Waiting for Command\n");
	}





}

int Plan_deinit()
{
        return 0;
}

// FXB ATRIUM PLANNER
/* struct node { */
/*         float x_coord; */
/*         float y_coord; */
/* }; */

/* struct link { */
/*         int link_a; */
/*         int link_b; */
/* }; */

/* const int coord_length = 94; */
/* const int link_length = 94; */
/* const int waypoint_length = 55; */

/* const struct node coords[] = */
/* {{                        0,                         0}, */
/*  {                        0,                       340}, */
/*  {                      480,                       340}, */
/*  {                      480,                       600}, */
/*  {                      325,                       600}, */
/*  {                      325,                       638}, */
/*  {                       91,                       638}, */
/*  {                       91,                       583}, */
/*  {                      -10,                       583}, */
/*  {                      -10,                       940}, */
/*  {                      265,                       940}, */
/*  {                      265,                      1561}, */
/*  {                      488,                      1561}, */
/*  {                      488,                      1683}, */
/*  {                      316,                      1683}, */
/*  {                      316,                      1863}, */
/*  {                      219,                      1863}, */
/*  {                      219,                      2337}, */
/*  {                      316,                      2337}, */
/*  {                      316,                      2742}, */
/*  {                      178,                      2742}, */
/*  {                      178,                      3106}, */
/*  {                      316,                      3106}, */
/*  {                      316,                      3678}, */
/*  {                      238,                      3678}, */
/*  {                      238,                      3955}, */
/*  {                      316,                      3955}, */
/*  {                      316,                      4396}, */
/*  {                       75,                      4396}, */
/*  {                       75,                      3950}, */
/*  {                       -6,                      3950}, */
/*  {                       -6,                      3695}, */
/*  {                       75,                      3695}, */
/*  {                       75,                      3643}, */
/*  {                     -515,                      3643}, */
/*  {        -564.159540931188,          4022.56309530887}, */
/*  {         -888.21104723794,          3978.74961267709}, */
/*  {        -871.060815504398,          3851.90376311359}, */
/*  {        -901.781294695558,          3847.75019136562}, */
/*  {        -868.150762155567,           3599.0134082372}, */
/*  {        -837.430282964407,          3603.16697998516}, */
/*  {         -790.13315951175,          3253.34991048583}, */
/*  {         -820.85363870291,          3249.19633873786}, */
/*  {        -787.223106162918,          3000.45955560944}, */
/*  {        -756.502626971759,           3004.6131273574}, */
/*  {         -709.33948970452,          2655.78704105778}, */
/*  {         -738.07800249625,          2651.90144168065}, */
/*  {         -704.31348377084,          2402.17367535251}, */
/*  {         -675.57497097911,          2406.05927472964}, */
/*  {        -668.741675522777,          2355.51913154418}, */
/*  {        -635.048246732473,          2360.07466184841}, */
/*  {        -601.417714192482,          2111.33787871998}, */
/*  {        -635.111142982786,          2106.78234841576}, */
/*  {        -628.277847526453,           2056.2422052303}, */
/*  {        -658.998326717613,          2052.08863348234}, */
/*  {        -625.233807992203,           1802.3608671542}, */
/*  {        -594.513328801043,          1806.51443890217}, */
/*  {        -466.288549355738,          858.143516775067}, */
/*  {        -731.872046879314,          822.235219082964}, */
/*  {        -708.290478245695,          647.822175933153}, */
/*  {        -646.849519863375,          656.129319429087}, */
/*  {        -638.676362552859,          595.679344246482}, */
/*  {        -700.117320935179,          587.372200750548}, */
/*  {        -691.542205068408,          523.949275968799}, */
/*  {        -734.154482656146,          518.187869995812}, */
/*  {        -701.595839599501,          277.378952465107}, */
/*  {        -665.920444409767,          282.202455140165}, */
/*  {        -657.747287099251,           221.75247995756}, */
/*  {        -465.496546354573,          247.745799928709}, */
/*  {        -472.329841810906,          298.285943114166}, */
/*  {        -392.060202634004,          309.138824133047}, */
/*  {        -385.226907177671,          258.598680947591}, */
/*  {        -465.496546354573,          247.745799928709}, */
/*  {                     -432,                         0}, */
/*  {                      -75,                      1193}, */
/*  {                      -75,                      1245}, */
/*  {                        6,                      1245}, */
/*  {                        6,                      1193}, */
/*  {                      -75,                      1498}, */
/*  {                      -75,                      2465}, */
/*  {                        6,                      2465}, */
/*  {                        6,                      1498}, */
/*  {                      -75,                      2717}, */
/*  {                      -75,                      3076}, */
/*  {                        6,                      3076}, */
/*  {                        6,                      2717}, */
/*  {                      -75,                      3326}, */
/*  {                      -75,                      3380}, */
/*  {                        6,                      3380}, */
/*  {                        6,                      3326}, */
/*  {        -425.690735173995,          557.875607261471}, */
/*  {        -432.658016815747,          609.406733646643}, */
/*  {        -512.927655992648,          598.553852627761}, */
/*  {        -505.960374350897,           547.02272624259}}; */

/* const struct link links[] = */
/* {{    1,     2}, */
/*  {    2,     3}, */
/*  {    3,     4}, */
/*  {    4,     5}, */
/*  {    5,     6}, */
/*  {    6,     7}, */
/*  {    7,     8}, */
/*  {    8,     9}, */
/*  {    9,    10}, */
/*  {   10,    11}, */
/*  {   11,    12}, */
/*  {   12,    13}, */
/*  {   13,    14}, */
/*  {   14,    15}, */
/*  {   15,    16}, */
/*  {   16,    17}, */
/*  {   17,    18}, */
/*  {   18,    19}, */
/*  {   19,    20}, */
/*  {   20,    21}, */
/*  {   21,    22}, */
/*  {   22,    23}, */
/*  {   23,    24}, */
/*  {   24,    25}, */
/*  {   25,    26}, */
/*  {   26,    27}, */
/*  {   27,    28}, */
/*  {   28,    29}, */
/*  {   29,    30}, */
/*  {   30,    31}, */
/*  {   31,    32}, */
/*  {   32,    33}, */
/*  {   33,    34}, */
/*  {   34,    35}, */
/*  {   35,    36}, */
/*  {   36,    37}, */
/*  {   37,    38}, */
/*  {   38,    39}, */
/*  {   39,    40}, */
/*  {   40,    41}, */
/*  {   41,    42}, */
/*  {   42,    43}, */
/*  {   43,    44}, */
/*  {   44,    45}, */
/*  {   45,    46}, */
/*  {   46,    47}, */
/*  {   47,    48}, */
/*  {   48,    49}, */
/*  {   49,    50}, */
/*  {   50,    51}, */
/*  {   51,    52}, */
/*  {   52,    53}, */
/*  {   53,    54}, */
/*  {   54,    55}, */
/*  {   55,    56}, */
/*  {   56,    57}, */
/*  {   57,    58}, */
/*  {   58,    59}, */
/*  {   59,    60}, */
/*  {   60,    61}, */
/*  {   61,    62}, */
/*  {   62,    63}, */
/*  {   63,    64}, */
/*  {   64,    65}, */
/*  {   65,    66}, */
/*  {   66,    67}, */
/*  {   67,    68}, */
/*  {   68,    69}, */
/*  {   69,    70}, */
/*  {   70,    71}, */
/*  {   71,    72}, */
/*  {   72,    73}, */
/*  {   73,    74}, */
/*  {   74,     1}, */
/*  {   75,    76}, */
/*  {   76,    77}, */
/*  {   77,    78}, */
/*  {   78,    75}, */
/*  {   79,    80}, */
/*  {   80,    81}, */
/*  {   81,    82}, */
/*  {   82,    79}, */
/*  {   83,    84}, */
/*  {   84,    85}, */
/*  {   85,    86}, */
/*  {   86,    83}, */
/*  {   87,    88}, */
/*  {   88,    89}, */
/*  {   89,    90}, */
/*  {   90,    87}, */
/*  {   91,    92}, */
/*  {   92,    93}, */
/*  {   93,    94}, */
/*  {   94,    91}}; */

/* const struct node waypoints[] = */
/* {{                      -20,                       360}, */
/*  {                      305,                       580}, */
/*  {                      111,                       563}, */
/*  {                      -30,                       563}, */
/*  {                      -30,                       960}, */
/*  {                      245,                      1581}, */
/*  {                      296,                      1663}, */
/*  {                      199,                      1843}, */
/*  {                      199,                      2357}, */
/*  {                      158,                      2722}, */
/*  {                      158,                      3126}, */
/*  {                      218,                      3658}, */
/*  {                      218,                      3975}, */
/*  {                       95,                      3930}, */
/*  {                       95,                      3715}, */
/*  {                       95,                      3623}, */
/*  {                     -535,                      3623}, */
/*  {        -851.060815504398,          3831.90376311359}, */
/*  {        -817.430282964407,          3623.16697998516}, */
/*  {         -770.13315951175,          3233.34991048583}, */
/*  {        -736.502626971759,           3024.6131273574}, */
/*  {         -689.33948970452,          2635.78704105778}, */
/*  {         -655.57497097911,          2426.05927472964}, */
/*  {        -615.048246732473,          2380.07466184841}, */
/*  {        -581.417714192482,          2091.33787871998}, */
/*  {        -608.277847526453,           2036.2422052303}, */
/*  {        -574.513328801043,          1826.51443890217}, */
/*  {        -446.288549355738,          838.143516775067}, */
/*  {        -626.849519863375,          676.129319429087}, */
/*  {        -618.676362552859,          575.679344246482}, */
/*  {        -671.542205068408,          503.949275968799}, */
/*  {        -645.920444409767,          302.202455140165}, */
/*  {        -492.329841810906,          318.285943114166}, */
/*  {        -372.060202634004,          329.138824133047}, */
/*  {        -365.226907177671,          238.598680947591}, */
/*  {                      -95,                      1173}, */
/*  {                      -95,                      1265}, */
/*  {                       26,                      1265}, */
/*  {                       26,                      1173}, */
/*  {                      -95,                      1478}, */
/*  {                      -95,                      2485}, */
/*  {                       26,                      2485}, */
/*  {                       26,                      1478}, */
/*  {                      -95,                      2697}, */
/*  {                      -95,                      3096}, */
/*  {                       26,                      3096}, */
/*  {                       26,                      2697}, */
/*  {                      -95,                      3306}, */
/*  {                      -95,                      3400}, */
/*  {                       26,                      3400}, */
/*  {                       26,                      3306}, */
/*  {        -405.690735173995,          537.875607261471}, */
/*  {        -412.658016815747,          629.406733646643}, */
/*  {        -532.927655992648,          618.553852627761}, */
/*  {        -525.960374350897,           527.02272624259}}; */

/* int obstacle(StateInfo Current, struct node Goal); */

/* int Plan_init() */
/* { */
/*         return 0; */
/* } */

/* void Plan_loop() */
/* { */
        


/*         int i; */
/*         float x_current = hc_info_G.state.x_position; */
/*         float y_current = hc_info_G.state.y_position; */
/*         float x_goal = user_cmd_G.target.x_position; */
/*         float y_goal = user_cmd_G.target.y_position; */
/*         struct node Goal_node; */
/*         Goal_node.x_coord = x_goal; */
/*         Goal_node.y_coord = y_goal; */
/*         if (obstacle(hc_info_G.state, Goal_node) == 0) */
/*         { */
/*                 planner_cmd_G.target.x_position = x_goal; */
/*                 planner_cmd_G.target.y_position = y_goal; */
/*         } */
/*         else  */
/*         { */
/*                 float distances[waypoint_length]; */
/*                 int best_waypoint=0; */
/*                 for (i=0;i<waypoint_length;i++) */
/*                 { */
/*                         if (obstacle(hc_info_G.state, waypoints[i]) == 0) */
/*                         { */
/*                                 distances[i] = pow((waypoints[i].y_coord-y_current),2)+pow((waypoints[i].x_coord-x_current),2)+pow((waypoints[i].y_coord-y_goal),2)+pow((waypoints[i].x_coord-x_goal),2);} */
/*                         else  */
/*                         { */
/*                                 distances[i] = 10e20; // magic number, what is this? */
/*                         } */
/*                         if (distances[i] < distances[best_waypoint]) */
/*                         { */
/*                                 best_waypoint = i; */
/*                         } */
/*                 } */
/*                 planner_cmd_G.target.x_position = waypoints[best_waypoint].x_coord; */
/*                 planner_cmd_G.target.y_position = waypoints[best_waypoint].y_coord;	   */
/* 	} */
/* //        printf("Target x_coord: %5.1f\n",TargetState.x_position); */
/* //        printf("Target y_coord: %5.1f\n",TargetState.y_position); */
/* } */

/* int Plan_deinit() */
/* { */
/*         return 0; */
/* } */

/* int obstacle(StateInfo Current, struct node Goal) */
/* { */
/*         float x1 = Current.x_position; */
/*         float y1 = Current.y_position; */
/*         float x2 = Goal.x_coord; */
/*         float y2 = Goal.y_coord; */
/*         int i; */
/*         for (i=0;i<coord_length;i++) */
/*         { */
/*                 float x3 = coords[links[i].link_a-1].x_coord; */
/*                 float y3 = coords[links[i].link_a-1].y_coord; */
/*                 float x4 = coords[links[i].link_b-1].x_coord; */
/*                 float y4 = coords[links[i].link_b-1].y_coord; */
/*                 if (fmax(y1,y2) < fmin(y3,y4) || fmin(y1,y2) > fmax(y3,y4) || fmax(x1,x2) < fmin(x3,x4) || fmin(x1,x2) > fmax(x3,x4)) */
/*                 { */
/*                         // why is this empty? */
/*                 }  */
/*                 else  */
/*                 { */
/*                         float m1 = (y2-y1)/(x2-x1); */
/*                         if (x2 == x1) */
/*                         { */
/*                                 m1 = 1000000000; // magic numbers why are you */
/*                                                  // here, what do you mean? I */
/*                                                  // have no idea, infinity? */
/*                         } */
/*                         float m2 = (y4-y3)/(x4-x3);	     */
/*                         if (x3 == x4) */
/*                         { */
/*                                 m2 = 1000000000; // magic number why are you here? */
/*                         } */
/*                         if (m1 == m2) */
/*                         { */
/*                                 // why is this empty? */
/*                         }  */
/*                         else  */
/*                         { */
/*                                 if (x1 == x2) */
/*                                 { */
/*                                         float b2 = y3-m2*x3; */
/*                                         float y_common = m2*x1+b2; */
/*                                         if (y_common <= fmax(y1,y2) && y_common >= fmin(y1,y2) && y_common <= fmax(y3,y4) && y_common >= fmin(y3,y4)) */
/*                                         { */
/*                                                 return(1); */
/*                                         } */
/*                                 } */
/*                                 else  */
/*                                 { */
/*                                         if (x3 == x4) */
/*                                         { */
/*                                                 float b1 = y1-m1*x1; */
/*                                                 float y_common = m1*x3+b1; */
/*                                                 if (y_common <= fmax(y1,y2) && y_common >= fmin(y1,y2) && y_common <= fmax(y3,y4) && y_common >= fmin(y3,y4)) */
/*                                                 { */
/*                                                         return(1); */
/*                                                 } */
/*                                         } */
/*                                         else  */
/*                                         { */
/*                                                 float b1 = y1-m1*x1; */
/*                                                 float b2 = y3-m2*x3; */
/*                                                 float x_common = (b2-b1)/(m1-m2); */
/*                                                 if (x_common <= fmax(x1,x2) && x_common >= fmin(x1,x2) && x_common <= fmax(x3,x4) && x_common >= fmin(x3,x4)) */
/*                                                 { */
/*                                                         return(1); */
/*                                                 } */
/*                                         } */
/*                                 } */
/*                         } */
/*                 } */
/*         } */
/*         return(0); */
/* } */
