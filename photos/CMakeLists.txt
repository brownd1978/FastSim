add_library(photos SHARED 
phoran.F
photos.F
)
target_link_libraries(photos )

# set top-level directory as include root
target_include_directories(photos PRIVATE ${CLHEP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(photos PROPERTIES VERSION ${PROJECT_VERSION})
