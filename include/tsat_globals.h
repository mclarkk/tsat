/****************************************************************
 *
 * Author: Meghan Clark
 * Title: tsat_globals.h
 * Date: Dec 10, 2012
 * Description: Contains all the global variables for sharing of information
 * NOTE: This should ONLY be #included by tsat_main.c
 *
 ****************************************************************/

#ifndef TSAT_GLOBALS_H
#define TSAT_GLOBALS_H

#include "hc1_structs.h"
#include "Config.h"

// NOTE: the "_G" after each global indicates it is a global variable

// this is the StateInfo received from the aerial vision system
StateInfo state_from_vision_G = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0};

// command from the user on the ground station 
// NOTE: Initialize achieve flag to 1 since there are no user commands -Gerardo Cruz)
Command user_cmd_G = {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0}, 1, 0.0};

// command from the planner
Command planner_cmd_G = {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0}, 0, 0.0};

// current best estimate of what the HC is doing including 
// complete hovercraft information
//HovercaftInfo hc_info_G = {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0}, 0.0f, 0.0f, 0, {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0}, 0.0}, 0.0f, 0.0f, 0.0f, 0.0};

//current control inputs for what the hovercraft should do.
ControlInfo hc_control_G = {0.0f, 0.0f, 0, 0.0};

// halt
// this gets populated by the comm thread who gets a message from the
// groundstation
volatile int halt_G = 0;

// this is the threshold for determining whether or not we have met our goal state
extern const float goal_threshold_G = 0.1;

// System timing constants
// NOTE: when declaring constant global values to be shared across files, you
// must declare them as extern const everywhere. This gives the linker access to
// the variable, otherwise it has no access to it
//extern const float SYSTEM_LOOP_TIME =     1.0f/SYSTEM_RATE;     // seconds
extern const float CAMERA_LOOP_TIME =    0.5;// SYSTEM_LOOP_TIME/2.0; // seconds
//extern const float CONTROL_LOOP_TIME =    SYSTEM_LOOP_TIME/2.0; // seconds

#endif // TSAT_GLOBALS_H
