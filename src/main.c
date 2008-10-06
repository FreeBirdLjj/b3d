#ifdef WIN32
# include <windows.h>
# include <direct.h> /* _chdir */
#else 
# include "config.h"
# include <unistd.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifndef DARWIN
#include <malloc.h>
#endif

#ifdef HAVE_GETOPT
#include <getopt.h>
#endif

#include "utils.h"
#include "view.h"
#include "mesh.h"
#include "globals.h"
#include "my_lua.h"

void init_network_libs(void);

int main(int argc, char * argv[])
{
    int c = 0;

    /* Init some global vars */
    progname=argv[0];

    /* Init some libraries */
    glutInit(&argc,argv);
    init_network_libs();

    argc--; 
    argv++;

#ifdef WIN32
    /* Make sure to start in the directory where the executable lives. */
    {
        int i,status;
        char dir[3*80];
        strncpy(dir,progname,sizeof(dir));
        for(i=strlen(dir)-1;i>=0;i--) {
            if(dir[i]=='\\') { 
                dir[i]='\0';
                status=_chdir(dir);
                if(status!=0) {
                    b3d_warn(
"Warning: Could not chdir to executable directory.\n");
                }
                break;
            }
        }
    }
#endif

    /* Launch the viewing window. */
    view("b3d");

    return 0;
}

/* http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winsock/winsock/wsastartup_2.asp
 * */
void init_network_libs(void)
{
#ifdef WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD( 2, 2 );
    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 ) {
        b3d_fatal_error("Could not find a usable WinSock DLL.");
    }

    /* Confirm that the WinSock DLL supports 2.2.*/
    /* Note that if the DLL supports versions greater    */
    /* than 2.2 in addition to 2.2, it will still return */
    /* 2.2 in wVersion since that is the version we      */
    /* requested.                                        */
    if ( LOBYTE( wsaData.wVersion ) != 2 ||
            HIBYTE( wsaData.wVersion ) != 2 ) {
        WSACleanup( );
        b3d_fatal_error("Could not find a usable WinSock DLL.\n");
    }
#endif
}

