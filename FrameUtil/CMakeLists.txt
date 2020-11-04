

add_library(FrameUtil SHARED 

AbsCommand.cc
AbsInterp.cc
ftclCmdLine.c
ftclAlias.c
ftclParse.c
ftclHelp.c
FtclInterp.cc
FtclInterp_handler.cc
TclInterp.cc
TclInterp_handler.cc
)

# set top-level directory as include root
target_include_directories(FrameUtil PRIVATE ${PROJECT_SOURCE_DIR})

message( "TCL :" ${TCL_LIBRARY})
target_link_libraries(FrameUtil ${TCL_LIBRARY})

# set shared library version equal to project version
set_target_properties(FrameUtil PROPERTIES VERSION ${PROJECT_VERSION})
