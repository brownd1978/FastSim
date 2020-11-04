
add_library(AbsEnv SHARED 

AbsDetEnv.cc
AbsDetIndex.cc
AbsDetIndexFactory.cc
AbsDetStructure.cc
AbsDetStructureIterator.cc
AbsEnv.cc
AbsEnvDataHolder.cc
AbsEnvObj.cc
ThreeCoordIndex.cc
TwoCoordIndex.cc
)

# set top-level directory as include root
target_include_directories(AbsEnv PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(AbsEnv ErrLogger ProxyDict )
# set shared library version equal to project version
set_target_properties(AbsEnv PROPERTIES VERSION ${PROJECT_VERSION})
