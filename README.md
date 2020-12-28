[![GitHub release](https://img.shields.io/github/release/iraf-community/x11iraf.svg)](https://github.com/iraf-community/x11iraf/releases/latest)
[![X11IRAF's Travis CI Status](https://travis-ci.com/iraf-community/x11iraf.svg)](https://travis-ci.com/iraf-community/x11iraf)

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

The source code if x11iraf is mostly Open Source conform to the Open Source
Definition by the [Open Source Initiative](https://opensource.org/osd). 
The list of files that are not conform is documented in the file
[`LICENSES-nonfree`](https://github.com/iraf-community/x11iraf/blob/master/LICENSES-nonfree)
of the source package. Note that x11iraf can be built without using these
files.


## Contributing

The x11iraf package is maintained on
[Github](https://github.com/iraf-community/x11iraf). The preferred way
to report a bug is to create a new issue on the [x11iraf GitHub
issue](https://github.com/iraf-community/x11iraf/issues) page.  To
contribute patches, we suggest to create a [pull request on
GitHub](https://github.com/iraf-community/x11iraf/pulls).


## Installation

### Distribution Files

The X11IRAF snapshots are available from github at

[https://github.com/iraf-community/x11iraf/releases/latest/](https://github.com/iraf-community/x11iraf/releases/latest/)

The snapshot has the release date as a suffix in the version number
and in the file name.


### System Requirements and Dependencies

Besides a normal development environment (C compiler, make, lex, yacc), the
installation requires the ncurses, X11 (xaw, xaw3d, xmu), TCL and XPM
libraries to be installed. To compile the helper for ximtool, also a working
IRAF installation with its development tools is required.

On Debian and its derivatives (Ubuntu, Mint, Devuan, Raspbian etc.):

    $ sudo apt install gcc make bison flex libncurses-dev tcl-dev
    $ sudo apt install libxaw7-dev libxmu-dev xaw3dg-dev libxpm-dev
    $ sudo apt install iraf-dev

On MacOS X, you need to have the XCode tools installed. If you
haven't, you can install them with:

    $ xcode-select --install

Click "Install" to download and install Xcode Command Line
Tools. Additionally, you need to install XQuartz from
[www.xquartz.org](https://www.xquartz.org/).

For the `ism_wcspix.e` helper of XImtool, you also need a working IRAF
installation. Make sure that the `xc` compiler works and can be found in the
`PATH`.


### Unpack the Source Tarball

The source distribution file is built as a tarball with the package
name and version as base directory. Thus, distribution files can be
unpacked with the command

    $ tar xzf /<path>/x11iraf-2.0+2020.06.15.tar.gz
    $ cd x11iraf-2.0+2020.06.15/


### Build from Sources

In the source directory, type `make`:

    $ make

This will automatically compile all sources and create the executables
`xgterm/xgterm` and `ximtool/ximtool`, and the helper
`ximtool/clients/ism_wcspix.e`. The compilation can be tuned by
setting `CFLAGS` and `LDFLAGS`. If the environment variable
`OSI_COMPLIANT` is set to `YES`, the build uses only source files that
are Open Source.

To copy the executables to `/usr/local/bin`, execute as root

    # make install

This will also install the manpages to `/usr/local/man/` und the
required terminfo file for xgterm to `/usr/share/terminfo/`. If you
don't want to invoke `make install`, you should make sure to run
`tic xgterm/xgterm.terminfo` to compile and install the terminfo file.


## Other included programs

The sources include a few more programs that are out of interest for
the normal user:

 * The **guidemo** package illustrates how to use GUIs in IRAF applications.

 * The **obmsh** package provides a simple shell for the IRAF Object Manager.

 * **XTapemon** is a conventional Xt/Athena application which allows the status
   of an IRAF tape job to be monitored continuously while the tape is being
   accessed.

These programs are not built by default.
