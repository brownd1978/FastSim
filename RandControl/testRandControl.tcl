#list of modules which need to be installed
set modlist {
    GenBuildEnv
    RacEidMaker
    RandomControl
    RandomPickerPrimary
    RandomPickerSimulation
    RandomPickerDigis
    RandomPickerReco
    RandomPickerAnalysis
}

# create path and append all
path create all
foreach mod $modlist {
    path append all $mod
}

# produce a lot of noise
foreach mod $modlist {
    catch { mod talk $mod ; verbose set t ; production set f ; exit }
}

# set start-of-event seeds calculation mode
mod talk RandomControl 
    computeEventSeeds set t 
exit

puts "testRandControl.tcl finished..."
