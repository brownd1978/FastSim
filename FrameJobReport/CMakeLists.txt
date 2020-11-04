
add_library(FrameJobReport SHARED 

FjrAbsJobReport.cc
FjrFwkJobReport.cc
FjrJobReportFile.cc
)

# set top-level directory as include root
target_link_libraries(FrameJobReport ${Boost_LIBRARIES} ErrLogger BaBar BbrStdUtils )

target_include_directories(FrameJobReport PRIVATE ${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(FrameJobReport PROPERTIES VERSION ${PROJECT_VERSION})
