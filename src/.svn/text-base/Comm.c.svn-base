/****************************************************************
 *
 * Author: Gerardo Cruz
 * Title: Comm.c
 * Date: Thursday, April  7, 2011
 * Description: Communications implementation for Hovercraft
 *
 ****************************************************************/

#include <stdio.h>
#include "../include/Comm.h"
#include "../include/Config.h"
#include "../include/hc1_structs.h"
#include "../include/simple_sock.h"
#include <unistd.h>
#include <fcntl.h>


/******************************************************
 * Extern variables
 ******************************************************/
// complete hovercraft information
extern HovercaftInfo hc_info_G;
// info from machine vision system
extern StateInfo state_from_vision_G;
// command from user
extern Command user_cmd_G;
// Halt Command
extern unsigned int halt_G;

int SOCK_Vis, SOCK_Gnd2Hov, SOCK_Hov2Gnd, SOCK_Hlt;

    
//int Comm_init(int * SOCK_Vis, int SOCK_Gnd)
int Comm_init()
{
int flags;

        // Initialize FROM Vison SERVER 
        if (server_init(&SOCK_Vis, VISION_PORT) < 0)
        {
                fprintf(stderr,"Vision socket initialization error.\n");
                return COMM_VISION_INIT_FAILED;
        }
	else
	{
		flags = fcntl(SOCK_Vis, F_GETFL);
		flags |= O_NONBLOCK;
		fcntl(SOCK_Vis, F_SETFL, flags);
	}
    
	// Initialize from Hovercraft TO Ground 
      	if (client_init_extended(&SOCK_Hov2Gnd, HOVERCRAFT2GROUND_PORT, (char*)GROUND_NAME) < 0) {
                fprintf(stderr,"Ground Send socket initialization error.\n");            
                return COMM_HOVERCRAFT2GROUND_INIT_FAILED;
        } 
	else
	{
		flags = fcntl(SOCK_Hov2Gnd, F_GETFL);
		flags |= O_NONBLOCK;
		fcntl(SOCK_Hov2Gnd, F_SETFL, flags);    
	}

	// Initialize FROM Ground to Hovercraft
	if (server_init(&SOCK_Gnd2Hov, GROUND2HOVERCRAFT_PORT) < 0) 
	{
                fprintf(stderr,"Ground Receive socket initialization error.\n");    
                return COMM_GROUND2HOVERCRAFT_INIT_FAILED;
		}
		else
		{
			flags = fcntl(SOCK_Gnd2Hov, F_GETFL);
			flags |= O_NONBLOCK;
			fcntl(SOCK_Gnd2Hov, F_SETFL, flags);
	}

	// Initialize HALT SOCKET
	if (server_init(&SOCK_Hlt, HALT_PORT) < 0) 
	{
                fprintf(stderr,"Ground Receive socket initialization error.\n");    
                return COMM_HALT_INIT_FAILED;
		}
		else
		{
			flags = fcntl(SOCK_Hlt, F_GETFL);
			flags |= O_NONBLOCK;
			fcntl(SOCK_Hlt, F_SETFL, flags);
	}

  	return 0; // nominal
}

void Comm_loop()
{
        CommData h_info, c_info;
        int retval;
        char BUFF_Vis[HC_BUFFSIZE];
        char BUFF_Gnd2Hov[HC_BUFFSIZE];
        char BUFF_Hov2Gnd[HC_BUFFSIZE];
        char BUFF_Hlt[HC_BUFFSIZE];
        unsigned int halt_flag;

	// Receive Data from HALT SOCKET //////////////
	retval = sock_receive(SOCK_Hlt, BUFF_Hlt, 1);
	if (retval > 0)  
	{
		#if defined(COMM_DEBUG)
		printf("Received Halt Data \n");
		#endif

                // Copy Data to local variable      
                memcpy(&halt_flag, BUFF_Hlt, sizeof(halt_flag));

		// Copy Data to global Halt Variable
		halt_G = halt_flag;
	}		
	///////////////////////////////////////////////////

	// Receive Data from Ground Station //////////////
	retval = sock_receive(SOCK_Gnd2Hov, BUFF_Gnd2Hov, 1);
	if (retval > 0)  
	{
                // Copy Data to local structure      
                memcpy(&c_info, BUFF_Gnd2Hov, sizeof(c_info));

		#if defined(COMM_DEBUG)
		printf("Command Received: \n x=%f\n y=%f\n x_dot=%f\n y_dot=%f\n theta=%f\n theta_dot=%f\n time=%f\n \n", 			c_info.PosX,c_info.PosY,c_info.VelX,c_info.VelY,c_info.Angle,c_info.Rate,c_info.TStamp);
		#endif

		// Copy Data to global command structure      
                user_cmd_G.target.x_position = c_info.PosX;
                user_cmd_G.target.x_velocity = c_info.VelX;

                user_cmd_G.target.y_position = c_info.PosY;
                user_cmd_G.target.y_velocity = c_info.VelY;

                user_cmd_G.target.angle = c_info.Angle;
                user_cmd_G.target.angular_velocity = c_info.Rate;

                user_cmd_G.target.timestamp = c_info.TStamp;

		user_cmd_G.timestamp = c_info.TStamp;

		// New User Command, set achievement flag to 0
		user_cmd_G.achieved = 0;

	}		
	///////////////////////////////////////////////////


	// Send Data To Ground Station ///////////////////
	// Get Current HC State to send to ground station
        h_info.PosX =  hc_info_G.state.x_position;
        h_info.VelX = hc_info_G.state.x_velocity;

        h_info.PosY = hc_info_G.state.y_position;
        h_info.VelY = hc_info_G.state.y_velocity;

        h_info.Angle = hc_info_G.state.angle;
        h_info.Rate = hc_info_G.state.angular_velocity;
        h_info.TStamp = hc_info_G.timestamp;

	// SEND DATA TO GROUND
	memcpy(BUFF_Hov2Gnd, &h_info, sizeof(h_info));
	sock_send(SOCK_Hov2Gnd, BUFF_Hov2Gnd, 0);

	#if defined(COMM_DEBUG)
//	printf("Sent Data to GROUND \n");
	#endif
	///////////////////////////////////////////////////	


	// Receive Data From Vision  /////////////////////       
	retval = sock_receive(SOCK_Vis, BUFF_Vis, 1);	
        if (retval >0)
        {
                // Copy Data onto local structure   
                memcpy(&h_info, BUFF_Vis, sizeof(h_info));

		#if defined(COMM_DEBUG)
		printf("Vision Received: \n x=%f\n y=%f\n x_dot=%f\n y_dot=%f\n theta=%f\n theta_dot=%f\n time=%f\n \n",
		h_info.PosX,h_info.PosY,h_info.VelX,h_info.VelY,h_info.Angle,h_info.Rate,h_info.TStamp);
		#endif

                // Copy Data onto Global Structure
                state_from_vision_G.x_position = h_info.PosX;
                state_from_vision_G.x_velocity = h_info.VelX;

                state_from_vision_G.y_position = h_info.PosY;
                state_from_vision_G.y_velocity = h_info.VelY;

                state_from_vision_G.angle = h_info.Angle;
                state_from_vision_G.angular_velocity = h_info.Rate;
                state_from_vision_G.timestamp = h_info.TStamp;
		
        }
	/////////////////////////////////////////////////



}

int Comm_deinit()
{
	if (close(SOCK_Vis) < 0){return -1;}
	if (close(SOCK_Hov2Gnd) < 0){return -1;}
	if (close(SOCK_Gnd2Hov) <0){return -1;}
	if (close(SOCK_Hlt) < 0){return -1;}

        return 0; //Nominal Behavior
}


