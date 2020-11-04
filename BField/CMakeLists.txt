
add_library(BField SHARED 

BField.cc
BFieldDictionary.cc
BFieldFixed.cc
BFieldIntegrator.cc
)
target_link_libraries(BField CLHEP::Geometry CLHEP ErrLogger BaBar difAlgebra )

# set top-level directory as include root
target_include_directories(BField PRIVATE ${CLHEP_INCLUDE_DIRS}/g ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(BField PROPERTIES VERSION ${PROJECT_VERSION})
