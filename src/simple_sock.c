/******************************************************************************
 * simple_sock.c
 *
 ******************************************************************************/
#include <ctype.h>
#include <arpa/inet.h>
#include "../include/simple_sock.h"

// Socket global variables:  used in initialization and UDP send functions
struct sockaddr_in sock_from;
unsigned int sock_fromlen;


/**********
 * client_init:  Initialize socket client
 **********/
int client_init(int *conn_socket, unsigned short port)     
{
        char *server_name = (char *) SERVER_NAME;
        unsigned int addr;
        int socket_type = DEFAULT_PROTO;
        struct sockaddr_in server;
        struct hostent *hp;
  
        //
        // Attempt to detect if we should call gethostbyname() or
        // gethostbyaddr()
        if (isalpha(server_name[0])) {   /* server address is a name */
                hp = gethostbyname(server_name);
        }
        else  { /* Convert nnn.nnn address to a usable one */
                addr = inet_addr(server_name);
                hp = gethostbyaddr((char *)&addr,4,AF_INET);
        }
        if (hp == NULL ) {
                fprintf(stderr,"Client: Cannot resolve address [%s]\n",
                        server_name);
                exit(1);
        }
  
        //
        // Copy the resolved information into the sockaddr_in structure
        //
        memset(&server,0,sizeof(server));
        memcpy(&(server.sin_addr),hp->h_addr,hp->h_length);
        server.sin_family = hp->h_addrtype;
        server.sin_port = htons(port);
  
        *conn_socket = socket(AF_INET,socket_type,0); /* Open a socket */
        if (*conn_socket <0 ) {
                fprintf(stderr,"Client: Error Opening socket\n");
                return -1;
        }
  
        //
        // Notice that nothing in this code is specific to whether we 
        // are using UDP or TCP.
        // We achieve this by using a simple trick.
        //    When connect() is called on a datagram socket, it does not 
        //    actually establish the connection as a stream (TCP) socket
        //    would. Instead, TCP/IP establishes the remote half of the
        //    ( LocalIPAddress, LocalPort, RemoteIP, RemotePort) mapping.
        //    This enables us to use send() and recv() on datagram sockets,
        //    instead of recvfrom() and sendto()
  
  
        printf("Client connecting to: %s\n",hp->h_name);
        if (connect(*conn_socket,(struct sockaddr*)&server,sizeof(server))
            < 0) {
                fprintf(stderr,"connect() failed\n");
                return -1;
        }
        return 0;
}


int client_init_extended(int *conn_socket, unsigned short port, char *server_name) {
	unsigned int addr;
	int socket_type = DEFAULT_PROTO;
	struct sockaddr_in server;
	struct hostent *hp;

	//
	// Attempt to detect if we should call gethostbyname() or
	// gethostbyaddr()
	if (isalpha(server_name[0])) { /* server address is a name */
		hp = gethostbyname(server_name);
	} else { /* Convert nnn.nnn address to a usable one */
		addr = inet_addr(server_name);
		hp = gethostbyaddr((char *) &addr, 4, AF_INET);
	}
	if (hp == NULL) {
		fprintf(stderr, "Client: Cannot resolve address [%s]\n", server_name);
		return -1;
	}

	//
	// Copy the resolved information into the sockaddr_in structure
	//
	memset(&server, 0, sizeof(server));
	memcpy(&(server.sin_addr), hp->h_addr, hp->h_length);
	server.sin_family = hp->h_addrtype;
	server.sin_port = htons(port);

	*conn_socket = socket(AF_INET, socket_type, 0); /* Open a socket */
	if (*conn_socket < 0) {
		fprintf(stderr, "Client: Error Opening socket\n");
		return -1;
	}

	//
	// Notice that nothing in this code is specific to whether we
	// are using UDP or TCP.
	// We achieve this by using a simple trick.
	//    When connect() is called on a datagram socket, it does not
	//    actually establish the connection as a stream (TCP) socket
	//    would. Instead, TCP/IP establishes the remote half of the
	//    ( LocalIPAddress, LocalPort, RemoteIP, RemotePort) mapping.
	//    This enables us to use send() and recv() on datagram sockets,
	//    instead of recvfrom() and sendto()


	printf("Client connecting to: %s\n", hp->h_name);
	if (connect(*conn_socket, (struct sockaddr*) &server, sizeof(server)) < 0) {
		fprintf(stderr, "connect() failed\n");
		return -1;
	}
	return 0;
}


/**********
 * server_init:  Initialize socket server
 **********/
int server_init(int *msgsock, unsigned short port)
{
	char *interface= NULL;
	int socket_type = DEFAULT_PROTO;
	struct sockaddr_in local;
	int listen_socket;

	memset(&local,0,sizeof(local));
       	local.sin_family = AF_INET;
	local.sin_addr.s_addr = (!interface)?INADDR_ANY:inet_addr(interface); 

	/* 
	 * Port MUST be in Network Byte Order
	 */
	local.sin_port = htons(port);
	listen_socket = socket(AF_INET, socket_type,0);
	
	if (listen_socket < 0){
		fprintf(stderr,"socket() failed\n");
		return -1;
	}
	//
	// bind() associates a local address and port combination with the
	// socket just created. This is most useful when the application is a 
	// server that has a well-known port that clients know about in advance.
	//

	if (bind(listen_socket,(struct sockaddr*)&local,sizeof(local) ) 
            < 0) {
		fprintf(stderr,"bind() failed\n");
		return -1;
	}

	//
	// So far, everything we did was applicable to TCP as well as UDP.
	// However, there are certain steps that do not work when the server is
	// using UDP.
	//

	// We cannot listen() on a UDP socket.

	if (socket_type != SOCK_DGRAM) {
		if (listen(listen_socket,5) < 0) {
			fprintf(stderr,"listen() failed\n");
			return -1;
		}
	}
#ifdef SIMPLE_SOCK_DEBUG
	printf("Listening on port %d, protocol %s\n",port,(socket_type == SOCK_STREAM)?"TCP":"UDP");
#endif              
	sock_fromlen = sizeof(sock_from);
	//
	// accept() doesn't make sense on UDP, since we do not listen()
	//
	if (socket_type != SOCK_DGRAM) {
		*msgsock = accept(listen_socket,(struct sockaddr*)&sock_from, &sock_fromlen);
		if (*msgsock < 0) {
			fprintf(stderr,"accept() error\n");
			return -1;
		}
#ifdef SIMPLE_SOCK_DEBUG
		printf("accepted connection from %s, port %d\n", 
                       inet_ntoa(sock_from.sin_addr),
                       htons(sock_from.sin_port)) ;
#endif		
	}
	else
        {
		*msgsock = listen_socket;
        }
	return 0;
}




/**********
 * sock_send:  Send data buffer; check for errors
 **********/
int sock_send(int sock, char *Buffer, char server_flag)
{
        int retval;

        if ((DEFAULT_PROTO == SOCK_DGRAM) && server_flag)            // UDP on server
                retval = sendto(sock,Buffer,BUFFSIZE,0,(struct sockaddr *)&sock_from,sock_fromlen);
        else                                        
                retval = send(sock,Buffer,BUFFSIZE,0);

        if (retval < 0) {
#if defined(SIMPLE_SOCK_DEBUG)
                fprintf(stderr,"send() failed.\n");
#endif
                return -1;
        }
#ifdef SIMPLE_SOCK_DEBUG
        printf("Sent Data [%s]\n",Buffer);
#endif
        return retval;
}

int sock_receive(int sock, char *Buffer, char server_flag)
{
        int retval;

        if ((DEFAULT_PROTO == SOCK_DGRAM) && server_flag) {          // UDP on server
                retval = recvfrom(sock,Buffer,BUFFSIZE,0,(struct sockaddr *)&sock_from,&sock_fromlen);
#ifdef SIMPLE_SOCK_DEBUG
                printf("Received datagram from %s\n",inet_ntoa(sock_from.sin_addr));
#endif

        } else  
                retval = recv(sock,Buffer,BUFFSIZE,0 );

        if (retval < 0) {
#if defined(SIMPLE_SOCK_DEBUG)
                fprintf(stderr,"recv() failed\n");
#endif
                // closesocket(sock);
                return -1;
        }
        //
        // We are not likely to see this with UDP, since there is no
        // 'connection' established. 
        //
        if (retval == 0) {
                printf("Server closed connection\n");
                // closesocket(sock);
                return retval;
        }
#ifdef SIMPLE_SOCK_DEBUG
        printf("Received %d bytes, data [%s]\n", retval, Buffer);
#endif
        return retval;
}

int sock_receive_extended(int sock, char *Buffer, char server_flag, int recvflag)
{
        int retval;

        if ((DEFAULT_PROTO == SOCK_DGRAM) && server_flag) {          // UDP on server
                retval = recvfrom(sock,Buffer,BUFFSIZE,recvflag,(struct sockaddr *)&sock_from,&sock_fromlen);
#ifdef SIMPLE_SOCK_DEBUG
                printf("Received datagram from %s\n",inet_ntoa(sock_from.sin_addr));
#endif

        } else  
                retval = recv(sock,Buffer,BUFFSIZE,0 );

        if (retval < 0) {
                fprintf(stderr,"recv() failed\n");
                // closesocket(sock);
                return -1;
        }
        //
        // We are not likely to see this with UDP, since there is no
        // 'connection' established. 
        //
        if (retval == 0) {
                printf("Server closed connection\n");
                // closesocket(sock);
                return retval;
        }
#ifdef SIMPLE_SOCK_DEBUG
        printf("Received %d bytes, data [%s]\n", retval, Buffer);
#endif
        return retval;
}

