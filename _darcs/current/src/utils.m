#include "utils.h"
#include "my_opengl.h"
#include <stdio.h>
#include <stdlib.h>
#import <Cocoa/Cocoa.h>

void unpack_color(GLuint color, float* r, float* g, float* b, float* a)
{
	*a=(color&0xff)/255.f;
	*b=((color>>8)&0xff)/255.f;
	*g=((color>>16)&0xff)/255.f;
	*r=((color>>24)&0xff)/255.f;
}

void warn_or_error(NSAlertStyle style, NSString* pre_text, 
        const char msg[])
{
    NSAlert *alert = [[NSAlert alloc] init];
    NSString *nsmsg = 
        [pre_text stringByAppendingString:
            [[NSString alloc] initWithCString:msg encoding:NSASCIIStringEncoding]];
    [alert addButtonWithTitle:@"OK"];
    [alert setMessageText:nsmsg];
    // [alert setInformativeText:@"b3d warning"];
    [alert setAlertStyle:style];
    [alert runModal];
    [alert release];
}

void b3d_warn(const char msg[])
{
    warn_or_error(NSWarningAlertStyle, @"Warning: ", msg);
}

void b3d_fatal_error(const char msg[])
{
    warn_or_error(NSCriticalAlertStyle, @"Error: ", msg);
    exit(-1);
}

const char* get_filename(const char* types[], int ntypes)
{
    int i;
    static char filename[10*80];
    NSString* nsfilename;
    NSOpenPanel* panel = [NSOpenPanel openPanel];
    NSMutableArray* nstypes = [NSMutableArray arrayWithCapacity:ntypes];
    for(i=0;i<ntypes;i++) { [nstypes addObject:[NSString stringWithUTF8String:types[i]]]; }
    // [panel beginSheetForDirectory:nil file:nil types:nstypes modalForWindow:nil modalDelegate:(id) didEndSelector:(SEL)didEndSelector contextInfo:(void *)contextInfo];
    switch([panel runModalForDirectory:nil file:nil types:nstypes]) {
        case NSOKButton:
            nsfilename=[[panel filenames] objectAtIndex:0];
            strncpy(filename, [nsfilename UTF8String], sizeof(filename));
            break;
        case NSCancelButton:
            filename[0]='\0'; /* result is empty string */
            break;
        default:
            warn("Unrecognized result code from [NSPanel runModalForDirectory]");
    }
    [nstypes release];
    return filename;
}

