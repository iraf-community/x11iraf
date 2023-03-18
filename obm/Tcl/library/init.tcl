# init.tcl --
#
# Default system startup file for Tcl-based applications.  Defines
# "unknown" procedure and auto-load facilities.
#
# $Header: /user6/ouster/tcl/library/RCS/init.tcl,v 1.28 93/10/08 09:11:21 ouster Exp $ SPRITE (Berkeley)
#
# Copyright (c) 1991-1993 The Regents of the University of California.
# All rights reserved.
#
# Permission is hereby granted, without written agreement and without
# license or royalty fees, to use, copy, modify, and distribute this
# software and its documentation for any purpose, provided that the
# above copyright notice and the following two paragraphs appear in
# all copies of this software.
#
# IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
# DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
# OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
# CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
# AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
# ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
# PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
#

set auto_path [info library]

# unknown:
# Invoked when a Tcl command is invoked that doesn't exist in the
# interpreter:
#
#	1. See if the autoload facility can locate the command in a
#	   Tcl script file.  If so, load it and execute it.
#	2. See if the command exists as an executable UNIX program.
#	   If so, "exec" the command.
#	3. If the command was invoked at top-level:
#	    (a) see if the command requests csh-like history substitution
#		in one of the common forms !!, !<number>, or ^old^new.  If
#		so, emulate csh's history substitution.
#	    (b) see if the command is a unique abbreviation for another
#		command.  If so, invoke the command.

proc unknown args {
    global auto_noexec auto_noload env unknown_pending tcl_interactive;

    set name [lindex $args 0]
    if ![info exists auto_noload] {
	#
	# Make sure we're not trying to load the same proc twice.
	#
	if [info exists unknown_pending($name)] {
	    unset unknown_pending($name)
	    if {[array size unknown_pending] == 0} {
		unset unknown_pending
	    }
	    return -code error "self-referential recursion in \"unknown\" for command \"$name\"";
	}
	set unknown_pending($name) pending;
	set ret [catch {auto_load $name} msg]
	unset unknown_pending($name);
	if {$ret != 0} {
	    return -code $ret "error while autoloading \"$name\": $msg"
	}
	if ![array size unknown_pending] {
	    unset unknown_pending
	}
	if $msg {
	    return [uplevel $args]
	}
    }
    if {([info level] == 1) && ([info script] == "") && $tcl_interactive} {
	if ![info exists auto_noexec] {
	    if [auto_execok $name] {
		return [uplevel exec >&@stdout <@stdin $args]
	    }
	}
	if {$name == "!!"} {
	    return [uplevel {history redo}]
	}
	if [regexp {^!(.+)$} $name dummy event] {
	    return [uplevel [list history redo $event]]
	}
	if [regexp {^\^([^^]*)\^([^^]*)\^?$} $name dummy old new] {
	    return [uplevel [list history substitute $old $new]]
	}
	set cmds [info commands $name*]
	if {[llength $cmds] == 1} {
	    return [uplevel [lreplace $args 0 0 $cmds]]
	}
	if {[llength $cmds] != 0} {
	    if {$name == ""} {
		return -code error "empty command name \"\""
	    } else {
		return -code error \
			"ambiguous command name \"$name\": [lsort $cmds]"
	    }
	}
    }
    return -code error "invalid command name \"$name\""
}

# auto_load:
# Checks a collection of library directories to see if a procedure
# is defined in one of them.  If so, it sources the appropriate
# library file to create the procedure.  Returns 1 if it successfully
# loaded the procedure, 0 otherwise.

proc auto_load cmd {
    global auto_index auto_oldpath auto_path env errorInfo errorCode

    if [info exists auto_index($cmd)] {
	uplevel #0 $auto_index($cmd)
	return 1
    }
    if [catch {set path $auto_path}] {
	if [catch {set path $env(TCLLIBPATH)}] {
	    if [catch {set path [info library]}] {
		return 0
	    }
	}
    }
    if [info exists auto_oldpath] {
	if {$auto_oldpath == $path} {
	    return 0
	}
    }
    set auto_oldpath $path
    catch {unset auto_index}
    for {set i [expr [llength $path] - 1]} {$i >= 0} {incr i -1} {
	set dir [lindex $path $i]
	set f ""
	if [catch {set f [open $dir/tclIndex]}] {
	    continue
	}
	set error [catch {
	    set id [gets $f]
	    if {$id == "# Tcl autoload index file, version 2.0"} {
		eval [read $f]
	    } elseif {$id == "# Tcl autoload index file: each line identifies a Tcl"} {
		while {[gets $f line] >= 0} {
		    if {([string index $line 0] == "#")
			    || ([llength $line] != 2)} {
			continue
		    }
		    set name [lindex $line 0]
		    set auto_index($name) "source $dir/[lindex $line 1]"
		}
	    } else {
		error "$dir/tclIndex isn't a proper Tcl index file"
	    }
	} msg]
	if {$f != ""} {
	    close $f
	}
	if $error {
	    error $msg $errorInfo $errorCode
	}
    }
    if [info exists auto_index($cmd)] {
	uplevel #0 $auto_index($cmd)
	if {[info commands $cmd] != ""} {
	    return 1
	}
    }
    return 0
}

# auto_execok:
# Returns 1 if there's an executable in the current path for the
# given name, 0 otherwise.  Builds an associative array auto_execs
# that caches information about previous checks, for speed.

proc auto_execok name {
    global auto_execs env

    if [info exists auto_execs($name)] {
	return $auto_execs($name)
    }
    set auto_execs($name) 0
    if {[string first / $name] >= 0} {
	if {[file executable $name] && ![file isdirectory $name]} {
	    set auto_execs($name) 1
	}
	return $auto_execs($name)
    }
    foreach dir [split $env(PATH) :] {
	if {[file executable $dir/$name] && ![file isdirectory $dir/$name]} {
	    set auto_execs($name) 1
	    return 1
	}
    }
    return 0
}

# auto_reset:
# Destroy all cached information for auto-loading and auto-execution,
# so that the information gets recomputed the next time it's needed.
# Also delete any procedures that are listed in the auto-load index
# except those related to auto-loading.

proc auto_reset {} {
    global auto_execs auto_index auto_oldpath
    foreach p [info procs] {
	if {[info exists auto_index($p)] && ($p != "unknown")
		&& ![string match auto_* $p]} {
	    rename $p {}
	}
    }
    catch {unset auto_execs}
    catch {unset auto_index}
    catch {unset auto_oldpath}
}

# auto_mkindex:
# Regenerate a tclIndex file from Tcl source files.  Takes as argument
# the name of the directory in which the tclIndex file is to be placed,
# floowed by any number of glob patterns to use in that directory to
# locate all of the relevant files.

proc auto_mkindex {dir args} {
    global errorCode errorInfo
    set oldDir [pwd]
    cd $dir
    set dir [pwd]
    append index "# Tcl autoload index file, version 2.0\n"
    append index "# This file is generated by the \"auto_mkindex\" command\n"
    append index "# and sourced to set up indexing information for one or\n"
    append index "# more commands.  Typically each line is a command that\n"
    append index "# sets an element in the auto_index array, where the\n"
    append index "# element name is the name of a command and the value is\n"
    append index "# a script that loads the command.\n\n"
    foreach file [eval glob $args] {
	set f ""
	set error [catch {
	    set f [open $file]
	    while {[gets $f line] >= 0} {
		if [regexp {^proc[ 	]+([^ 	]*)} $line match procName] {
		    append index "set [list auto_index($procName)]"
		    append index " \"source \$dir/$file\"\n"
		}
	    }
	    close $f
	} msg]
	if $error {
	    set code $errorCode
	    set info $errorInfo
	    catch {close $f}
	    cd $oldDir
	    error $msg $info $code
	}
    }
    set f [open tclIndex w]
    puts $f $index nonewline
    close $f
    cd $oldDir
}
