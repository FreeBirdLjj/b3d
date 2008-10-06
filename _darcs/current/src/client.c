/* Much of this code is copied verbatim from 
 *
 * http://users.actcom.co.il/~choo/lupg/tutorials/internetworking/internet-programming.html#clients
 * 
 */

#include <stdio.h>
#include <sys/types.h>

#ifdef WIN32
# include <winsock2.h>
#else
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
#endif

#include "config.h"
#include "globals.h"
#include "utils.h"
#include <ctype.h>

int parse_labels_from_string(char str[]);
void print_win32_network_error(void);
int parse_labels_from_string(char str[]);

#include <stdlib.h>
#include <string.h>

#define HOSTNAMELEN     80
#define BUFLEN          1000*80
#define PORT            80      /* HTTP server port */

int l_http_get(lua_State* L)
{
    int rc; 
    SOCKET s;
    char buf[BUFLEN+1];
    char* pc;
    struct sockaddr_in sa;
    struct hostent* hen;
    const char *server_name;
    const char *filename;

    server_name=luaL_checkstring(L,1);
    filename=luaL_checkstring(L,2);

    /* Set up connection */
    {
        /* hen=gethostbyname("brainmaps.org"); */
        hen=gethostbyname(server_name); 
        if(!hen) { 
            /* ( perror("Couldn't resolve host name."); */
            fprintf(stderr,"Could not resolve host name.\n");
#ifdef WIN32
            print_win32_network_error();
#endif
            return 0;
        }
        memset(&sa,0,sizeof(sa));
        sa.sin_family=AF_INET;  
        sa.sin_port=htons(PORT);
        memcpy(&sa.sin_addr.s_addr, hen->h_addr_list[0], hen->h_length);
        s=socket(AF_INET, SOCK_STREAM, 0);
        if(s<0) { perror("socket: allocation failed"); }
        rc = connect(s, (struct sockaddr*)&sa, sizeof(sa));
        if(rc) { perror("connect failed"); }
    }

    /* Send a request to the server */
    {
        char request[2*80];
        /* snprintf(request,sizeof(request),"GET /%s\n",filename); */
        snprintf(request,sizeof(request),"GET /%s\r\n\r\n",filename); 
        send(s, request, strlen(request), /*flags*/ 0);
    }

    /* Start reading the socket till read() returns 0, meaning that the server
     * closed the connection. */
    {
        pc = buf;
        while((rc=recv(s,pc,BUFLEN-(pc-buf), /*flags*/ 0))!=0) {
            pc += rc;
            if(pc-buf>=BUFLEN) { break; }
        }
#ifdef WIN32
        shutdown(s, SD_BOTH);
#else
        shutdown(s, SHUT_RDWR);
#endif
    }

    *pc='\0'; /* Append null terminator to string. */

    lua_pushstring(L,buf);
    return 1;
}


/* http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winsock/winsock/gethostbyname_2.asp
 * */
void print_win32_network_error(void)
{
#ifdef WIN32
    int e=WSAGetLastError();
    char* s=
 e==WSANOTINITIALISED?"A successful WSAStartup call must occur before using this function."
:e==WSAENETDOWN?"The network subsystem has failed."
:e==WSAHOST_NOT_FOUND?"Authoritative answer host not found."
:e==WSATRY_AGAIN?"Nonauthoritative host not found, or server failure."
:e==WSANO_RECOVERY?"A nonrecoverable error occurred."
:e==WSANO_DATA?"Valid name, no data record of requested type."
:e==WSAEINPROGRESS?"A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function."
:e==WSAEFAULT?"The name parameter is not a valid part of the user address space."
:e==WSAEINTR?"A blocking Windows Socket 1.1 call was canceled through WSACancelBlockingCall."
:"unknown";
    fprintf(stderr,"%s\n",s);
    fflush(stderr);
#endif
}

