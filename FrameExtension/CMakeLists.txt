
add_library(FrameExtension SHARED 

FweAbsInputModDecorator.cc
FweJobReportCommand.cc
FweJobReportModule.cc
)
target_link_libraries(FrameExtension  ErrLogger Framework FrameJobReport)

# set top-level directory as include root
target_include_directories(FrameExtension PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(FrameExtension PROPERTIES VERSION ${PROJECT_VERSION})
