# X11IRAF

This package contains X11/GUI development utilities and applications developed
by the IRAF Project, National Optical Astronomy Observatories, since 1994. It
provides the following programs:

 * **XGterm** provides a Tek 4012 compatible graphics terminal emulation plus,
   for clients in the know, a datastream driven widget server capability using
   the Object Manager to provide full access to the underlying toolkit and
   widget set.  The remote client application downloads a GUI file to the
   widget server (xgterm) which executes the GUI.  While the GUI is executing
   it exchanges messages with the remote client application at runtime via
   interprocess communication.  In the case of Xgterm, this currently uses a
   serial (tty based) protocol.

 * **XImtool** is an image display server.  This provides an image display
   capability to remote client applications using the standard imtool/iis
   image display protocol.  The image display server allows a number of image
   frame buffers to be created and displayed.  The client can read and write
   data in these frame buffers.  Any frame or combination of frames can be
   displayed.  Various display options are provided, e.g., zoom and pan, flip
   about either axis, frame blink, windowing of the display, and colortable
   enhancement.

Manual pages are included for all the above utilities.


## Installation

### System Requirements and Dependencies

Besides a normal development environment (C compiler, make, lex, yacc), the
installation requires the ncurses, X11 (xaw, xaw3d, xmu), TCL and XPM
libraries to be installed. To compile the helper for ximtool, also a working
IRAF installation with its development tools is required.

On Debian and its derivatives (Ubuntu, Mint, Devuan, Raspbian etc.):

    $ sudo apt install gcc make flex
    $ sudo apt install libncurses-dev libxaw7-dev libxmu-dev xaw3dg-dev tcl-dev libxpm-dev
    $ sudo apt install iraf-dev

On MacOS X, you need to have the XCode tools installed. If you
haven't, you can install them with:

    $ xcode-select --install

Click "Install" to download and install Xcode Command Line
Tools. Additionally, you need to install XQuartz from
https://www.xquartz.org/.

For the `ism_wcspix.e` helper of XImtool, you also need a working IRAF
installation. Make sure that the `xc` compiler works and can be found in the
`PATH`.


### Unpack the Source Tarball

The source distribution file is built as a tarball with the package
name and version as base directory. Thus, distribution files can be
unpacked with the command

    $ tar xzf /<path>/x11iraf-2.0-2020.06.15.tar.gz
    $ cd x11iraf-2.0-2020.06.15/


### Build from Sources

In the source directory, type `make`:

    $ make

This will automatically compile all sources and create the executables
`xgterm/xgterm` and `ximtool/ximtool`, and the helper
`ximtool/clients/ism_wcspix.e`. To copy them to `/usr/local/bin`, execute as
root

    # make install


## Other programs

Both XGterm and XImtool rely upon the IRAF Object Manager for the GUI.  The
**guidemo** package illustrates how to use GUIs in IRAF applications. The
**obmsh** package provides a simple shell for the object manager.

**XTapemon** is a conventional Xt/Athena application which allows the status
of an IRAF tape job to be monitored continuously while the tape is being
accessed.
