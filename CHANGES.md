# X11IRAF important changes

## Version 2.2 (2025-03-28)

 * Bugfix release

 * Add metadata required for Linux desktops

## Version 2.1 (2021-07-17)

 * __IRAF community maintainance__

 * __64 bit compatibility__ (#2)

 * __External libraries removed__

   External libraries that were included as convenience copies were
   removed. The code is updated to use the current versions of these
   libraries, which are included in macOS as well as in the Linux
   distributions:
  
   - Tcl (#14),
   - xpm (#16),
   - xaw3d (#16)

   The unused CDL (#11) and vximtool (#18) subdirectories were also
   removed. They are maintained in a separate repository.

 * __Imake build system replaced__
 
   The Imake build system (xmkmf) was replaced by plain Makefiles
   (#19). Re-introduce support for macOS (#17).

 * __Several bugfixes and spellfixes__ (#12, #13, #21, #26, #27, #28). 
   Consequently use stdarg.h (#3)

 * __Clarify licenses and add licensing information__ (#20).
   Make it easy to remove non-free code.

 * __Remove unused code files__,
   and fix a number of compiler warnings. (#21, #34)

 * __Reorganize README and installation instructions__ (#22, #30, #36).

## Selected NOAO versions

This list is compiled from the available release information in the
original packages.

### Version 2.0BETA (2008-11-17)

 * 24-bit display Support for XImtool

 * 24-bit display Support for XGterm applications using imaging

 * General platform support improvements

### Version 1.5dev

### Version 1.3.1

### Version 1.3 (2002-05-06)

 *  real-time cursor image WCS and actual pixel readout

 *  new cursor centroiding function

 *  ability to "auto-register" frames

 *  integrated/enhanced control panel

 *  support for up to 16 display frames

 *  improved support for image tiling

 *  a "compass" indicator for the display orientation

 *  pixel table readout
 
 *  image header display (keyword selection, WCS display)
 
 *  horizontal/vertical cut-graphs of the display
 
 *  ruler markers on the display
 
 *  control port for external process communications

### Version 1.2 (2000-04-28)

### Version 1.1 (1997-09-08)

### Version 1.0 patch 1 (1997-04-05)

### Version 1.0 (1997-03-06)

 * initial version
