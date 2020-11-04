
add_library(FrameLogger SHARED 

AppActionErrLogger.cc
AppErrLog.cc
#AppErrLogModule.cc
)
target_link_libraries(FrameLogger Framework FrameUtil FrameJobReport ErrLogger CLHEP )

# set top-level directory as include root
target_include_directories(FrameLogger PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(FrameLogger PROPERTIES VERSION ${PROJECT_VERSION})
