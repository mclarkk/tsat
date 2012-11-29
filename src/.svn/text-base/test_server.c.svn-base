/******************************************************************************
  test_server.c

******************************************************************************/

#include <stdio.h>
#include "../include/simple_sock.h"

typedef struct
{
        double x, y, z;
} foo;

int main() 
{
        foo bar;
        char Buffer[BUFFSIZE];
        int sock, retval;
        int i;

        // Initialize server socket
        if (server_init(&sock, DEFAULT_PORT) < 0) 
        {
                fprintf(stderr,"Socket initialization error.\n");
                exit(-1);
        }

        for(i=0;i<MAX_COMM_TRIES;i++)
        {
                retval = sock_receive(sock, Buffer, 1);
                if (retval < 0)  
                {
                        continue;
                }
                else if (retval == 0)
                {
                        printf("return == 0.\n");
                        exit(0);    // Client closed connection so exit (TCP only)
                }
                memcpy(&bar, Buffer, sizeof(bar));
                printf("Msg:  %s\n",Buffer);
                printf("bar is: x=%f, y=%f, z=%f\n",bar.x, bar.y, bar.z);
                /* sprintf(Buffer,"Your assimilation attempt has been rejected.  Try again.\0"); */
                /* if (sock_send(sock, Buffer, 1) < 0) exit(-1); */
        }
}
