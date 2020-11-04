
add_library(MemUtil SHARED 

MemAllocator.cc
MemPool.cc
MemUnsafePool.cc
)

# set top-level directory as include root
target_include_directories(MemUtil PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(MemUtil PROPERTIES VERSION ${PROJECT_VERSION})
