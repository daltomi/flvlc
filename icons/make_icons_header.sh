#!/bin/wish
###########################################################
# Concatenate all files *.xpm in to icons.h
# Make list static Fl_Pixmap image_file_name(file_name_xpm) end of file.
#
# Use XPM format by Gimp 2.8
# FIXME: Unfortunately Gimp declare xpm: "static char*"; 
#		 should by "static const char*"
# 
# Author: daltomi <daltomi@disroot.org> - daltomi@aol.com 
#
###########################################################


set file_name {../icons.h}

proc join_xpm_files { } {

  global file_name
  set out [open $file_name w]
  fconfigure $out -translation binary
  puts $out "// Generate by 'make_icons_header.tcl'"
  puts $out "// Author: daltomi'"
  puts $out " "
  puts $out "#include <FL/Fl_Pixmap.H>"
  puts $out " "
  puts $out " "
  foreach fname [glob -nocomplain -type f "*.xpm"] {
      set in [open $fname]
      fconfigure $in -translation binary
      fcopy $in $out
      puts $out " "
      close $in
  }
  close $out
}

proc make_fl_pixmaps { } {

  global file_name
  set exp {static char( \* )(\w+)_xpm}
  set fp [open $file_name r]
  set file_data [read $fp]
  close $fp

  set data [split $file_data "\n"]

  foreach line $data {
  	if { [regexp $exp $line /1 /2 name_var] > 0 } {
		set name_var [concat static Fl_Pixmap image_$name_var\(${name_var}_xpm\)\;]
		lappend list_names $name_var
  	}
  }

  set fp [open $file_name {a+}]

  foreach item $list_names {
	puts $fp $item
  }
  close $fp
}


proc message { } {
  set text {
  "-------------------------------------------------
  
  Note: Bug XPM format by Gimp 2.8
  
  Unfortunately Gimp declare xpm:
  "static char*"  should by "static const char*"

  GCC prints many warnings messages.
  
  Fix: Add -Wno-write-strings to flags.

  ------------------------------------------------- "
  }

  label .l -text $text -fg red -bd 80 
  pack .l
  tk_messageBox -message "Done!" -type ok
}

###########################################################

join_xpm_files

make_fl_pixmaps

message

