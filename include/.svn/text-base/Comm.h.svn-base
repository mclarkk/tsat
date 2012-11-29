/****************************************************************
 *
 * Author: Justin Bradley & Gerardo Cruz
 * Title: Comm.h
 * Date: Thursday, April  7, 2011
 * Description: Functional spec for Comm
 * There are actually two mechanisms for comm within the hovercraft software
 * architecture. One mechanism (implemented here) sends and/or receives
 * information that is not urgent and simply uses the left over time within the
 * loop in the main function.
 *
 * The other mechanism for communications is a separate thread kicked off during
 * the initialization phase. This thread is responsible for receiving any
 * communication from the aerial vision system having updates for our position
 * and orientation.
 *
 * This file is the header file for the former.
 *
 ****************************************************************/

#ifndef COMM_H
#define COMM_H

#include "Config.h"

//#define COMM_DEBUG

#define COMM_VISION_INIT_FAILED -1
#define COMM_HOVERCRAFT2GROUND_INIT_FAILED -2
#define COMM_GROUND2HOVERCRAFT_INIT_FAILED -3
#define COMM_HALT_INIT_FAILED -4

#define VISION_PORT  9191
#define HOVERCRAFT2GROUND_PORT 1445
#define GROUND2HOVERCRAFT_PORT 1334
#define HALT_PORT  9190


#define HOVERCRAFT_NAME  "192.168.0.108"
#define VISION_NAME  "192.168.0.100"
#define GROUND_NAME "richard-mobile"
//#define GROUND_NAME "192.168.0.104"
#define HC_BUFFSIZE 1024             // Send/receive message buffer size

#if defined (SIMULATION)
#define VISION_NAME  "richard-mobile"
#define VISION_PORT  9191
#endif
typedef struct 
{
        float PosX, PosY, VelX, VelY, Angle, Rate;
	double TStamp;
} CommData;

/***********************************************************
 * Function: Comm_init - takes care of any initialization needed by the
 *           Communications
 * Parameters: none
 * Return Value: 0 on success, -1 on failure
 ***********************************************************/
int Comm_init();
//int Comm_init(int* SOCK_Vis, int SOCK_Gnd);

/***********************************************************
 * Function: Comm_loop - communicates with the groundstation
 * Parameters: none
 * Return Value: none
 ***********************************************************/
void Comm_loop();
//void Comm_loop(int SOCK_Vis, int SOCK_Gnd);

/***********************************************************
 * Function: Comm_deinit - deinitializes anything needed by communications
 * Parameters: none
 * Return Value: 0 on success, -1 on failure
 ***********************************************************/
int Comm_deinit();

#endif // COMM_H
