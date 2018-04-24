The **Plugin Patterns Library** provides classes for common tasks with the XPLM SDK for the X-Plane flight simulator. It is provided under BSD license.

Questions, support and pull request are handled through http://forums.x-plane.org/index.php?showtopic=49958

Usage examples can be found [here](https://github.com/JDeeth/PPL-Demo).

---

Library contents
================

Grouped by category


Sim interface
-------------

**dataref.h**
Connects with existing datarefs

**owneddata.h**
Creates new datarefs

**command.h**
Observes and/or replaces existing commands or creates new ones

**processor.h**
Simple wrapper for flight loop callback (i.e. simple 'update' functions

**logichandler.h**
Abstract class for XP > FLCB > custom dataref processing

**sharedobject.h**
Passes large objects as binary data to/from datarefs


User interface
--------------

**messagewindow.h**
Displays pop-up message

**onscreendisplay.h**
Draws a window on the screen

**menuitem.h**
Adds item to plugin menu

**menuaction.h**
Abstract base for menu items


Sound
-----

**alsoundbuffer.h**
Stores a sound file

**alcontextmanager.h**
Manages the playing of sound buffers

**alcontextchanger.h**
Changes AL context using RAII


OpenGL
------

**overlaygauge.h**
Creates context for you to draw on the 3D panel and/or a 2D popup/VR window.
This class contains some XP11-only features which have been crudely wrapped in
#ifdef statements for XP10 compatibility.

**texture.h**
Loads .bmp/.tga image for OpenGL drawing

**fontmgr.h**
Loads .ttf/.otf font file for OpenGL drawing

**vertexbuffer.hpp**
Wraps OpenGL Vertex Buffer Objects


Utility
-------

**pluginpath.h**
Platform-independent paths to various folders

**logwriter.h**
Writes log message

**log.h**
Set up logging

**settings.h**
Load and save settings in a .ini file

**basics.h**
Maths utility functions

**smoothed.h**
Superficial smoothing - returns average of the last n assigned values

**namespaces.h**
Prevents name collision on 32-bit systems

---

Why this fork?
--------------

Key differences to Philipp's PPL are:

 - OverlayGauge reworked a bit, replacing static functions with lambdas for
   callbacks, and making XP11-only elements optional so it works with XP10.
 - OwnedData allows you to specify read and write callback functions, which
   are stored as std::function objects rather than function pointers.
 - Add Command class (also gratuitously overusing std::function)
 - Minor fixes prompted by compiler warnings, adding static_cast and nullptr
 - Configured to use clang-format on new files only


 Using this library
 ------------------

 Include it in a plugin project as a `git submodule`. Using Qt Creator/qmake,
 you can set up a `subdirs` project containing your plugin, PPL, and any other
 libraries you'd like to use. This is demonstrated with [PPL-Demo]
 (https://github.com/JDeeth/PPL-Demo).
