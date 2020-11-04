
add_library(ProxyDict SHARED 

AbsArg.cc
IfdCompositeKey.cc
IfdCounter.cc
IfdDataProxyIFace.cc
IfdDictKey.cc
IfdIntKey.cc
IfdKey.cc
IfdKeyHash.cc
IfdProxyDict.cc
IfdProxyDictFrontEnd.cc
IfdSimpleProxyDict.cc
IfdSimpleProxyDictEntry.cc
IfdStrKey.cc
IfdTypeKeyIFace.cc
)

# set top-level directory as include root
target_include_directories(ProxyDict PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(ProxyDict PROPERTIES VERSION ${PROJECT_VERSION})
