add_library(EvtGenModels SHARED 
begevtgengetx.F
begevtgenstorex.F
#EvtModelReg.cc
EvtPHOTOS.cc
)
target_link_libraries(EvtGenModels EvtGenBase photos stdhep)

# set top-level directory as include root
target_include_directories(EvtGenModels PRIVATE ${CLHEP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(EvtGenModels PROPERTIES VERSION ${PROJECT_VERSION})
