#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <setjmp.h>
#import <Cocoa/Cocoa.h>

@interface Delegate : NSObject {
    lua_State* L;
};
- (id)initWithLuaState:(lua_State*)newL;
- (void)openPanelDidEnd:(NSOpenPanel *)sheet returnCode:(int)returnCode contextInfo:(void *)contextInfo;
@end


@implementation Delegate
- (id)initWithLuaState:(lua_State*)newL
{
    self=[super init];
    L=newL;
    return self;
}

- (void)openPanelDidEnd:(NSOpenPanel *)sheet returnCode:(int)returnCode 
    contextInfo:(void *)contextInfo
{
    if(returnCode==NSOKButton) {
        /* Call the Lua function on_filename. */
        const char* utf8_filename = [[[sheet filenames] objectAtIndex:0] UTF8String];
        lua_getglobal(L, "on_filename");
        if(!lua_isfunction(L,-1)) {
            luaL_error(L,"on_filename is not a function in openPanelDidEnd");
            lua_pop(L,1);
        }
        lua_pushstring(L, utf8_filename);
        if(lua_pcall(L,1,0,0)!=0) {
            warn("on_filename callback failed.");
        }
    }

    lua_pushnil(L); lua_setglobal(L,"on_filename"); /* on_filename = nil */
}

@end

int l_get_filename(lua_State* L)
{
    NSOpenPanel* panel = [NSOpenPanel openPanel];
    Delegate* delegate = [[Delegate alloc] initWithLuaState:L];
    [panel beginSheetForDirectory:nil file:nil types:nil 
        modalForWindow:[[NSApplication sharedApplication] keyWindow]
        modalDelegate:delegate
        didEndSelector:@selector(openPanelDidEnd:returnCode:contextInfo:)
        contextInfo:NULL];
    return 0;
}


