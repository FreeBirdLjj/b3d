# b3d

## Version 1i

- Update for lua-5.2.3
- Update for libjpeg-9a
- Bugfix: float can't be arshifted.

## Version 1h

- Optimize some codes.
- The 'F' key now turns on full screen mode.
- Some keymap are changed.

## Version 1g

- The 3D view and thumbnail view are now in separate viewports, making the display more pleasing.
- The camera interaction has been changed to make it more intuitive. The pivot stays in the center of the object even after panning. However, the pivot can be changed by doing Zoom in.

## Version 1f

- The 'l' key now locks the thumbnails to the position where the mouse is pointing, and shows the three planes of section in 3D through that point. Pressing it again unlocks the thumbnails and makes the planes disappear.
- The horizontal thumbnails have been rotated 90 degrees for greater viewing enjoyment.
- The ordering of the sagittal sections has been reversed by a change in bm.get\_sagittal\_index\_from\_mouse().

## Version 1e

- New, higher-res sagittal thumbnails have been added.
- Thumbnails are now displayed on the right hand side of the window.
- The external browser can now be launched on any or all of the three planes of section, not just coronal.

## Version 1d

- \`Toggle transparency' menu item has been added.
- Stripes are now four times as wide.
- \`Toggle thumbnails' menu item has been added.
- Thumbnails for the three planes of section are now included.

## Version 1c 

- The program was crashing when I minimized it, but this has been fixed. The trouble was that the reshape callback is called by GLUT with arguments of zero width and height.  When these are passed to glViewport, you get a divide by zero error, which makes the program crash on Windows.

- Restart (r) menu item has been added.

- Lua commands can now be entered directly into the graphics window.

- Warnings are now displayed as dialog boxes.

- Removed disconcerting curved camera motion during seek.

- The \`Run Lua script' menu item has been added, letting the user choose a script to run from a Windows file dialog.

## Version 1b

- \`View section in detail' has been added, launching an external web browser on the zoomify viewer for the section where the mouse is pointing.
- A \`Help' menu item has been added, which just brings up notepad.  This is kludgey, but it's a start.




# Former life as brainmaps3d


## Version 0.6.3

Camera panning has been added.  Just hold down the x key while dragging.

The mesh diffuse color (and transparency) can now be set like this:
```lua
bm.mesh_color = { 0.8, 0.8, 0.8, 1.0 }
```

The executable now searches in parent directories for bminit.lua, stopping at the tenth parent directory.

src/calc\_deps.pl and src/calc\_all\_deps.sh have been added, to help keep the header dependencies in the Makefile up to date.

## Version 0.6.2

Most top level definitions in bminit.lua are now in the bm table.

The menu item \`Move camera center' has been changed to \`Zoom in' and the action has been changed accordingly.

Partial Lua bindings for GLU and GLUT have been added.  The GLU binding only includes the most useful functions.  The GLUT binding includes almost everything except for the functions that set up event handlers.

lbitlib for Lua is now included, for bitwise operations.

Mouse and keyboard even handling is now handled on the Lua side.

## Version 0.6.1

Shadows have been put back in, along with striping.

Menu items have been added for `View all' and `Move camera center'.

## Version 0.6

A partial OpenGL binding has been added so that a useful subset of GL functionality can be accessed from Lua.  Some parts of the binding are in doubt and should be tested though.

Meshes can now be loaded and displayed directly in Lua.

## Version 0.5.5

An Inventor-like targeted camera recentering function has been added.Move the mouse to the position you would like to focus on and press the 'c' key.

## Version 0.5.4

A newly-introduced bug in the label-making callback for the \`p' key has been removed.  add_labels() was incorrectly being called as bm.add_labels().

The default texture for the stripes has been changed to mostly light gray with small black stripes.

3D coordinates are now displayed in a little heads-up display at the bottom left of the screen.  They are updated whenever the mouse is moved unless the user calls bm.set\_updating\_mouse\_location(false) in bmconf.lua, as may be necessary on systems without hardware-accelerated OpenGL.

## Version 0.5.3

All the builtin brainmaps functions are now in a Lua library called 'bm' instead of global.  These functions can be listed with the function list_bm_functions(), defined in bminit.lua.

Errors are now reported on the command line when they occur.  Before this version, syntax errors were not reported.

Error checking in the bound C functions has been changed to use the official Lua error checking functions as much as possible.

## Version 0.5.2

Rudimentary help has been added, accessible from the menu.  However, it depends on the terminal.

Stripes can now be done in any direction, with any frequency.  Menu items for X, Y, and Z stripes have been added.  The stripes are now sharper as well.  Additionally, an optional fifth argument can be supplied to do_candy_stripes: the 1D RGBA texture to be used for the stripes.

Idle callbacks can now be specified from Lua.  Passing \`nil' as the argument stops idling.

Command line mode is now terminated by entering a blank line, instead of only taking a single statement and then returning to 3D interaction.

## Version 0.5.1

Multiple meshes can now be loaded.  The cerebellum is now loaded by default along with the cortex.

Candy striping has been added (see context menu).

## Version 0.5

An Xcode project has been added for Mac OS X, and its settings have been adjusted so that the resulting application can be launched just by clicking on it instead of having to run it from the command line.  The files bmconf.lua, bminit.lua, and cortex.mesh are included as resources in the application bundle.

A Mac icon has also been added (created with /Developer/Applications/IconComposer).  For now, it's just a screenshot.

init.lua and config.lua have been renamed to bminit.lua and bmconf.lua.


## Version 0.4.4

brainmaps.bat has been added, so that we can launch the program by clicking on the .bat file on Windows.

An MSVC build error was removed.

## Version 0.4.3

Another bug was fixed in the Makefile on OS X.

Most of the contents of config.lua have been moved to a new file called init.lua, and this file is now the one that is initially invoked when brainmaps starts up.  init.lua in turn executes config.lua after all the functions etc. have been defined.  config.lua is the file that casual users of the program should edit if they want to adjust settings.

Some needless printouts have been removed.

## Version 0.4.2

The Makefile has been updated to work with OS X.

## Version 0.4.1

Points are now shown next to the labels again, fixing a bug that came up in Version 0.4.

A serious bug in label file parsing, having to do with calling the downloaded string \`labels' and not realizing that this overwrote the global table also called \`labels' because of lua's scoping rules, has been fixed.

## Version 0.4

It is now possible to press the 'p' key to get a little star (\*) label at the 3D location of the cursor.

The label file is now a sequence of lua commands.

## Version 0.3

Keybindings can now be set in the configuration file.

Set/get lua functions have been registered for most viewing parameters.

Menu items can now be added from the config file, with lua callbacks.

It is now possible to define functions on_display and on_idle in the config file that will be called whenever a redraw occurs or an idle event occurs, respectively.

The brain now can cast a shadow, if the 's' key is pressed.

## Version 0.2

The release build configuration has been corrected for MSVC.

Missing WSAStartup call has been put in to support sockets on win32.

The source code for lua-5.0.2 has been brought in, making it so we no longer have to worry about whether the system has lua support or where to find it.

The Makefile has been updated to recurse into the lua source tree when necessary.

A configuration file called brainmaps-config.lua is now loaded from the current directory on startup.  The next step is to add some C callbacks for it.

## Version 0.1

Command line options have been added, on systems with getopt.  The dependency on w3c's libwww has been removed.

## Version 0.0

This version adds the experimental feature of grabbing labels and their 3D coordinates from a well-known location on the server.



========================
Former life as mesh-view
========================

## Version 0.3

The mode of vertex normal computation can now be cycled through using the 'n' key.

## Version 0.2

This version has been made to work with MSVC 6.0 on Windows.

## Version 0.1

?

## Version 0

This has the bare minimum of functionality, displaying the surface and letting the user spin it around.

