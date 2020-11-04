
add_library(CLHEP SHARED 
Alist/AIteratorBase.cc
Alist/AListBase.cc
Combination/Lock.cc
Combination/Lockable.cc
Geometry/AngleSets.cc
Geometry/HepPoint.cc
Geometry/Transformation.cc
Geometry/Translation.cc
String/CmdArg.cc
String/CmdLine.cc
String/Strings.cc
)

target_link_libraries(CLHEP CLHEP::Geometry)

# set top-level directory as include root
target_include_directories(CLHEP PRIVATE ${CLHEP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(CLHEP PROPERTIES VERSION ${PROJECT_VERSION})
