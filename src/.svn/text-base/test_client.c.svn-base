/******************************************************************************
  test_client.c

  Written:  E. Atkins, 7/30/01
******************************************************************************/

#include <stdio.h>
#include "../include/simple_sock.h"

#define MAXLOOP 10

struct blah 
{
  float x, y, z;
};
  
int main() {
  struct blah b;
  char Buffer[BUFFSIZE];
  int sock;
  int loopcount=0; 

  b.x=10.1; b.y=3.14; b.z = -567; 

  if (client_init(&sock, DEFAULT_PORT) < 0) {
    fprintf(stderr,"Socket initialization error.\n");
    exit(-1);
  }

  while(1) {
    b.x++; b.y++; b.z++;
    memcpy(Buffer, &b, sizeof(b));
    if (sock_send(sock,Buffer, 0) < 0) exit(-1);
    if (sock_receive(sock,Buffer,0) <=0) exit(-1);
    printf("Msg:  %s\n",Buffer);
    if (loopcount++ >= MAXLOOP) break;
  }
  
  exit(0);
}
