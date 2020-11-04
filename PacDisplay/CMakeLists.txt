
add_library(PacDisplay SHARED 

PacDisplay.cc
PacEvtDisplay.cc
)

# set top-level directory as include root
target_include_directories(PacDisplay PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(PacDisplay PROPERTIES VERSION ${PROJECT_VERSION})
