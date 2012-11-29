/******************************************************************************
 * simple_sock.h
 * - Change SERVER_NAME below to be the machine where you run the server socket
 *
 ******************************************************************************/

#ifndef SIMPLE_SOCK_H
#define SIMPLE_SOCK_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>

//#define SIMPLE_SOCK_DEBUG         // Comment out this line to remove message printing to screen


#define SERVER_NAME   "192.168.0.102"
#define BUFFSIZE 128             // Send/receive message buffer size
#define DEFAULT_PROTO SOCK_DGRAM // TCP = SOCK_STREAM; UDP = SOCK_DGRAM




// Function declarations

int client_init(int *conn_socket, unsigned short port);
int client_init_extended(int *conn_socket, unsigned short port, char *server_name);

int server_init(int *msgsock, unsigned short port);

int sock_send(int sock, char *Buffer, char server_flag); // server_flag: 1=server; 0=client

int sock_receive(int sock, char *Buffer, char server_flag); // server_flag:1=server;0=client
int sock_receive_extended(int sock, char *Buffer, char server_flag, int recvflag);


#endif
