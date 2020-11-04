
add_library(OdfCommon SHARED 

odfDamageTranslator.cc
odfDuration.cc
odfQuadWord.cc
odfTime.cc
odfTimeIO.cc
odfTransitionTranslator.cc
)

# set top-level directory as include root
target_include_directories(OdfCommon PRIVATE ${PROJECT_SOURCE_DIR})
#target_link_libraries(OdfCommon ErrLogger ProxyDict )
# set shared library version equal to project version
set_target_properties(OdfCommon PROPERTIES VERSION ${PROJECT_VERSION})
