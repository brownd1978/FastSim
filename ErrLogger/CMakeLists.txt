
add_library(ErrLogger SHARED 

ErrLineLog.cc
ErrLog.cc
ErrOptParser.cc
ErrStdLog.cc
ErrStream.cc
ErrStreamBuffer.cc
ErrStreamWrapper.cc
)

# set top-level directory as include root
target_include_directories(ErrLogger PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(ErrLogger PROPERTIES VERSION ${PROJECT_VERSION})
