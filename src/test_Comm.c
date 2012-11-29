/****************************************************************
 *
 * Author: Gerardo Cruz
 * Title: test_Comm.c
 * Date: Saturday, April 23, 2011
 * Description: Tests the Comm
 *
 ****************************************************************/

#include <stdio.h>
#include "../include/hc1_globals.h"
#include "../include/Comm.h"
#include "../include/hc1_structs.h"
#include "../include/simple_sock.h"


int main()
{

// Halt Command


      if(Comm_init() < 0){
        return -1;
        
        };

   
	while(1)
	{
		Comm_loop();

		if(halt_G > 0)
		{
		printf("Halt Command Received \n");
		break;
		}
	}
	
	if(Comm_deinit() < 0)
	{
	return -1;
	printf("Sockets were not closed \n");
	}
	printf("Sockets closed succesfully \n");
	return 0;
}
