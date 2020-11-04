
add_library(BbrStdUtils SHARED 
RegExp.cc
String.cc
Tokenize.cc
)

target_link_libraries(BbrStdUtils ${Boost_LIBRARIES} )
#target_link_libraries(BbrStdUtils Boost::program_options)
 # set top-level directory as include root
target_include_directories(BbrStdUtils PRIVATE ${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(BbrStdUtils PROPERTIES VERSION ${PROJECT_VERSION})
