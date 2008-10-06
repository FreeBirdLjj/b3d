# Microsoft Developer Studio Project File - Name="b3d" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=b3d - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "b3d.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "b3d.mak" CFG="b3d - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "b3d - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "b3d - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "b3d - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\lua-5.0.2\include" /I "..\freeglut-2.4.0\include" /I "..\jpeg-6b" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 glut32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "b3d - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\jpeg-6b" /I "..\lua-5.0.2\include" /I "..\freeglut-2.4.0\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 glut32.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "b3d - Win32 Release"
# Name "b3d - Win32 Debug"
# Begin Group "lua"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\lua-5.0.2\src\lapi.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lapi.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lib\lauxlib.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lib\lbaselib.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lcode.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lcode.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lib\ldblib.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\ldebug.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\ldebug.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\ldo.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\ldo.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\ldump.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lfunc.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lfunc.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lgc.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lgc.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lib\liolib.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\llex.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\llex.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\llimits.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lib\lmathlib.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lmem.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lmem.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lib\loadlib.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lobject.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lobject.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lopcodes.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lopcodes.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lparser.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lparser.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lstate.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lstate.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lstring.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lstring.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lib\lstrlib.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\ltable.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\ltable.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lib\ltablib.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\ltests.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\ltm.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\ltm.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lundump.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lundump.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lvm.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lvm.h"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lzio.c"
# End Source File
# Begin Source File

SOURCE="..\lua-5.0.2\src\lzio.h"
# End Source File
# End Group
# Begin Group "b3d"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\client.c
# End Source File
# Begin Source File

SOURCE=..\src\client.h
# End Source File
# Begin Source File

SOURCE=..\src\globals.c
# End Source File
# Begin Source File

SOURCE=..\src\globals.h
# End Source File
# Begin Source File

SOURCE=..\src\image.c
# End Source File
# Begin Source File

SOURCE=..\src\image.h
# End Source File
# Begin Source File

SOURCE=..\src\lbitlib.c
# End Source File
# Begin Source File

SOURCE=..\src\lua_bind.h
# End Source File
# Begin Source File

SOURCE=..\src\lua_gl.c
# End Source File
# Begin Source File

SOURCE=..\src\lua_gl.h
# End Source File
# Begin Source File

SOURCE=..\src\lua_glu.c
# End Source File
# Begin Source File

SOURCE=..\src\lua_glu.h
# End Source File
# Begin Source File

SOURCE=..\src\lua_glut.c
# End Source File
# Begin Source File

SOURCE=..\src\lua_glut.h
# End Source File
# Begin Source File

SOURCE=..\src\main.c
# End Source File
# Begin Source File

SOURCE=..\src\mesh.c
# End Source File
# Begin Source File

SOURCE=..\src\mesh.h
# End Source File
# Begin Source File

SOURCE=..\src\my_lua.c
# End Source File
# Begin Source File

SOURCE=..\src\my_lua.h
# End Source File
# Begin Source File

SOURCE=..\src\my_opengl.h
# End Source File
# Begin Source File

SOURCE=..\src\utils.c
# End Source File
# Begin Source File

SOURCE=..\src\utils.h
# End Source File
# Begin Source File

SOURCE=..\src\view.c
# End Source File
# Begin Source File

SOURCE=..\src\view.h
# End Source File
# End Group
# Begin Group "jpeg"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\jpeg-6b\jcapimin.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jcapistd.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jccoefct.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jccolor.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jcdctmgr.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jchuff.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jchuff.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jcinit.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jcmainct.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jcmarker.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jcmaster.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jcomapi.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jconfig.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jcparam.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jcphuff.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jcprepct.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jcsample.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jctrans.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdapimin.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdapistd.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdatadst.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdatasrc.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdcoefct.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdcolor.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdct.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jddctmgr.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdhuff.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdhuff.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdinput.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdmainct.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdmarker.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdmaster.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdmerge.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdphuff.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdpostct.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdsample.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jdtrans.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jerror.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jerror.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jfdctflt.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jfdctfst.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jfdctint.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jidctflt.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jidctfst.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jidctint.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jidctred.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jinclude.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jmemansi.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jmemmgr.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jmemsys.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jmorecfg.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jpegint.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jpeglib.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jquant1.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jquant2.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jutils.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\jversion.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\rdbmp.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\rdcolmap.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\rdgif.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\rdppm.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\rdrle.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\rdswitch.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\rdtarga.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\transupp.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\transupp.h"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\wrbmp.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\wrgif.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\wrppm.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\wrrle.c"
# End Source File
# Begin Source File

SOURCE="..\jpeg-6b\wrtarga.c"
# End Source File
# End Group
# End Target
# End Project
