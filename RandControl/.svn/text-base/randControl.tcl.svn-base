
# =================================================================
#	Define smart routine to insert new modules into path
# =================================================================

proc randInsertModule {path new args} {
    if { [llength $args] == 0 } {
	path insert $path $new
    } else {
	foreach mod $args {
	    if { ! [catch {path insert $path -before $mod $new}] } {
		return
	    }
	}
	error "Cannot insert module $new in the path $path. No modules: $args"
    }
}


# ========================================================================
#	Define a proc which will be called later with the path name
# ========================================================================

proc randControl { path } {

    # put the RandomControl module as a first module in the path.
    # this must work always, if $path is a good name.
    randInsertModule $path RandomControl

    # insert also "picker" modules each before corresponding stage. 
    # these may fail sometimes, because not all modules will be present.
    if [catch {randInsertModule $path RandomPickerPrimary GFISingleParticle GFIEvtGen} err] {
	puts stderr $err
    }
    if [catch {randInsertModule $path RandomPickerSimulation Bogus} err] {
	puts stderr $err
    }

    if [catch {randInsertModule $path RandomPickerDigis XxxYyyZzzModule} err] {
	puts stderr $err
    }

    if [catch {randInsertModule $path RandomPickerReco XxxYyyZzzModule} err] {
	puts stderr $err
    }

}

echo "randControl.tcl finished"
