//
//  main.m
//  brainmaps
//
//  Created by Issac Trotts on 10/3/05.
//  Copyright Regents of the University of California 2005. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include "view.h"
#include "globals.h"

char* get_resource_path(const char resource[])
{
    NSString *ns_path;
    NSString *ns_resource;
    char c_path[4*80];

    ns_resource=[[NSString alloc] initWithUTF8String:resource];
    ns_path=[[NSBundle mainBundle] pathForResource:ns_resource ofType:nil
        inDirectory:nil];		
    if(!ns_path) {
        fprintf(stderr,"Can't get path to %s.\n",resource);
        return NULL;
    }	
    [ns_path getCString:c_path maxLength:(sizeof(c_path)-1)];

    [ns_path release];
    [ns_resource release];
    return strdup(c_path);	
}

int main(int argc, char *argv[])
{
    //NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    glutInit(&argc,argv);	
    progname=argv[0];
    init_filename=get_resource_path("bminit.lua");

    /* cd into the Resource directory of the .app bundle so the init script can find the files
       it depends on, such as the mesh. */
    {
        char dirname[10*80],*c;
        strncpy(dirname, init_filename, sizeof(dirname));
        c=dirname+strlen(dirname); 
        while(c-dirname>=0 && *c!='/') { c--; }
        if(c-dirname<0) { c=dirname; }
        *c='\0';
        if(chdir(dirname)!=0) {
            fprintf(stderr,"Warning: Could not cd into the Resource directory of brainmaps.app.\n");
        }
    }

    /* Launch the viewing window. */
    view("b3d");

    //[pool release];
    return 0;
}

