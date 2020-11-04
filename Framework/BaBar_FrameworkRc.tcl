########################################################################
#       $Id: BaBar_FrameworkRc.tcl,v 1.2 2004/08/24 05:05:12 bartoldu Exp $

# Redefine 'proc' to prevent redefinition of commands

rename proc proc_internal

proc_internal proc {name arglist body} {
  if [llength [info commands $name]] {
     variable whatscript [info script]
     echo "Error : Redefining command $name in a tcl script file $whatscript."
     exitTcl
  }
  proc_internal $name $arglist $body
}

#######################################################################

# Startup BaBar related scripts

sourceFoundFile FrameScripts/BaBarScripts.tcl

#######################################################################



