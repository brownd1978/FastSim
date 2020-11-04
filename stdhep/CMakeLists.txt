add_library(stdhep SHARED 
StdHepUtil.F
hepchg.F
hepcmp.F
heplst.F
hepnam.F
heprd.F
heprdm.F
heptree.F
hepwrt.F
hepzinit.F
hepzrd.F
hepzwrt.F
hptrlsth.F
hptrlsti.F
hptrlstj.F
hpzflb.F
hpzflc.F
hwghep.F
hwtran.F
isahep.F
isaprtlst.F
isflavr.F
islabl.F
istran.F
lutran.F
pdgprtb.F
pdgrdtb.F
pdgtran.F
stdchg.F
stdchgdsclst.F
stdcquarks.F
stddautr.F
stddautrlst.F
stddscndlst.F
stdexpname.F
stdflhwxsec.F
stdflisxsec.F
stdoldcopy.F
stdparent.F
stdparentlst.F
stdquarkcnt.F
stdquarklst.F
stdquarks.F
stdrbgn.F
stdrd.F
stdrdbuf.F
stdrend.F
stdrhwbg.F
stdrisbg.F
stdrisnd.F
stdrotboost.F
stdrstbg.F
stdrstnd.F
stdsort.F
stdspin.F
stdstdsclst.F
stdtcopy.F
stdtranslate.F
stdversn.F
stdwbgn.F
stdwend.F
stdwhwbg.F
stdwisbg.F
stdwisnd.F
stdwrt.F
stdwstbg.F
stdwstnd.F
stdzero.F
)
target_link_libraries(stdhep Jst74)

# set top-level directory as include root
target_include_directories(stdhep PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(stdhep PROPERTIES VERSION ${PROJECT_VERSION})
