VirtualT v1.7
=============

> Linux 64-bit binary build

New features
============
  - Added TDock Video emulation support (and TDock I/O general processing).
  - Added support for Steve Adolph's QUAD Model 100 128K Banked RAM module.
  - Added right-click menu support to easily change QUAD bank when enabled.
  - Started adding documentation for the IDE / Assembler / Linker.
  - Added support in Linker Script for segment addresses based on known symbol
    values (e.g. from an absolute address in an ASEG) and also using simple
    equations involving basic math (+ - / *).
  - Added linker script PRELINK and POSTLINK processing to allow defining symbols
    based on #if / #else / #endif C preprocessor style statements.  Also added
    ability to display link messages using "echo" for displaying things like
    program sizes and addresses, etc.
  - Added support for NUM lock key so T200 Calculator will work.

Updates / bug fixes
===================
  - Fixed bug with deleting a .CO file where the deletion length wasn't including
    the 6-byte CO header, causing memory corruption.
  - Fixed the IDE Editor's undo function.
  - Made the IDE's undo local to each file vs. being a global entity.  This allows
    undo to be specific to the file being edited.
  - Fixed a bug in the linker to detect link addresses that are out of range.
  - Updated the GNUMakefile to include libjpeg and libpng.
  - Updated linker so it back annotates listing files with actual addresses
    determined during the link step.
  - Made updates to the assembler to push unresolved equations into the object
    file and added an equation evaluation function to the linker.  This allows
    the linker to resolve addresses that consist of a relocatable address
    location (i.e. label) embedded in an equation, such as Label+12h.
  - Modified the IDE's Find Dialog so that when it doesn't find the requested
    text, it displays the error message in the Find Dialog instead of using
    the FLTK fl_alert routine.  Using the fl_alert causes the main VirtualT
    emulation window to come into focus instead of the IDE window, plus showing
    the error in the Find Dialog box provides opportunity to modify the search.
  - Modified the non-T200 LCD emulation routines when in 2.4Mhz emulaiton mode
    to more closely reflect the actual delay introduced by the LCD hardware.
  - Updated IDE Tab control to deal with more tabs than can fit on the window.
    Tabs are now clipped and a "More Files" icon is displayed to allow selection
    of off-screen open file windows.

Release Date
----
April 7th, 2015

License
----
BSD

