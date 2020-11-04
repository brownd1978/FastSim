
add_library(MsgLogger SHARED 

MsgFormatter.cc
MsgHandler.cc
MsgHandlerStdStreams.cc
MsgLogLevel.cc
MsgLogStream.cc
MsgLogger.cc
MsgLoggerImpl.cc
)
target_link_libraries(MsgLogger ${Boost_LIBRARIES} ErrLogger )

# set top-level directory as include root
target_include_directories(MsgLogger PRIVATE ${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(MsgLogger PROPERTIES VERSION ${PROJECT_VERSION})
