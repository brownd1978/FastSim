cmake_minimum_required(VERSION 3.9.0)

# Check + Enforce build type
set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui                                                                                                                                                           
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release")
endif()

if (CMAKE_BUILD_TYPE MATCHES "^((p|P)(rofile|rof))|(release)$")
    message(STATUS "Setting build type to 'Release' instead of '${CMAKE_BUILD_TYPE}'.")
    set(CMAKE_BUILD_TYPE "Release" CACHE
        STRING "Choose the type of build." FORCE)
endif()

if (CMAKE_BUILD_TYPE MATCHES "^debug$")
    message(STATUS "Setting build type to 'Debug' instead of '${CMAKE_BUILD_TYPE}'.")
    set(CMAKE_BUILD_TYPE "Debug" CACHE
        STRING "Choose the type of build." FORCE)
endif()

if (NOT CMAKE_BUILD_TYPE MATCHES "^(Debug|Release)$")
    message(FATAL_ERROR "'${CMAKE_BUILD_TYPE}' is not a valid build type. Please choose either 'Release' or 'Debug'.")
endif()

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}" )


# enable c++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_COMPILER "/usr/bin/clang")
set(CMAKE_C_COMPILER "/usr/bin/clang")
# make sure CMake is able to find ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# versioning

set (_build_ver_maj 0)
set (_build_ver_min 0)
set (_build_ver_patch 0)
set (_build_ver_commits_since_tag 0)
set (_build_ver_commit_short 0)

# find git information
find_package(Git)

if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} describe
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE _build_version
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  message( STATUS "git describes this revision as: ${_build_version}")

  if (_build_version MATCHES "^.*([0-9]+)[._]([0-9]+)[._]([0-9]+)\\-([0-9]+)\\-([a-z0-9]+)$")
    set (_build_ver_maj ${CMAKE_MATCH_1})
    set (_build_ver_min ${CMAKE_MATCH_2})
    set (_build_ver_patch ${CMAKE_MATCH_3})
    set (_build_ver_commits_since_tag ${CMAKE_MATCH_4})
    set (_build_ver_commit_short ${CMAKE_MATCH_5})
    message( STATUS "Version string correctly matched.")
  endif()
else()
  message( STATUS "Git not found!")
endif()

#fortran
set(CMAKE_FC_COMPILER /usr/local/bin/gfortran)

# Set project name and version
project (FastSim
    LANGUAGES C CXX Fortran
    VERSION ${_build_ver_maj}.${_build_ver_min}.${_build_ver_patch}
    DESCRIPTION "Kinematic Kalman filter track fit code package"
)
message ( STATUS "Fotran Flags: ${CMAKE_FORTRAN_FLAGS}" )

message( STATUS "Project Version: ${CMAKE_PROJECT_VERSION}")
message( STATUS "Commits made since last tag: ${_build_ver_commits_since_tag}")
message( STATUS "Commit short hash: ${_build_ver_commit_short}")


# find the ROOT dependency
find_package(ROOT REQUIRED COMPONENTS 
        Core RIO Net Hist GenVector MLP Graf Graf3d Gpad Tree 
        Rint Postscript Matrix Physics MathCore Thread Gui)

find_package(CLHEP REQUIRED COMPONENTS Geometry Vector Matrix Random)
find_package(Boost REQUIRED COMPONENTS RegEx )
find_package(Tclsh REQUIRED  )

# include "useful" ROOT cmake functions
# which are useful for building dictionaries
include(${ROOT_USE_FILE})

# set RPATH options
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# output shared libraries and executables in lib/ and bin/
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# enable tests
enable_testing()

# compiler flags
set(CMAKE_CXX_FLAGS "-Wall -Wno-unused-parameter -Wno-unused-local-typedefs -gdwarf-2 -Werror=return-type -Winit-self -Woverloaded-virtual -ftrapping-math -O0 -lstdc++")
set(CMAKE_C_FLAGS "")
#
    # debug flags
#    "$<$<CONFIG:DEBUG>:-Og;-g>"
    # release flags
#    "$<$<CONFIG:RELEASE>:-O3;-DNDEBUG;-fno-omit-frame-pointer>"
#)

message( STATUS "C++ compiler flags: ${CMAKE_CXX_FLAGS}")

# install rules
include(GNUInstallDirs)


# clang tidy

if(ENABLE_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY clang-tidy) 
endif()



# add shared library targets
add_subdirectory(CrnLib)
add_subdirectory(ProxyDict)
add_subdirectory(ErrLogger)
add_subdirectory(AbsEnv)
#add_subdirectory(AbsCalo)
add_subdirectory(AbsEvent)
add_subdirectory(AbsEventTag)
add_subdirectory(CLHEP)
add_subdirectory(difAlgebra)
add_subdirectory(OdfCommon)
add_subdirectory(BbrStdUtils)
add_subdirectory(BdbTime)
add_subdirectory(EidData)
add_subdirectory(GenEnv)
add_subdirectory(BField)
add_subdirectory(BaBar)
add_subdirectory(BbrGeom)
add_subdirectory(Framework)
add_subdirectory(G3Data)
add_subdirectory(EvtGenBase)
add_subdirectory(EvtGen)
add_subdirectory(EvtGenModels)
add_subdirectory(photos)
add_subdirectory(stdhep)
add_subdirectory(Jst74)
add_subdirectory(AbsParm)
add_subdirectory(CommonUtils)
add_subdirectory(DetectorModel)
add_subdirectory(FrameExtension)
add_subdirectory(FrameJobReport)
add_subdirectory(FrameLogger)
add_subdirectory(FrameUtil)
add_subdirectory(KalmanTrack)
add_subdirectory(MatEnv)
add_subdirectory(MemUtil)
add_subdirectory(MsgLogger)
add_subdirectory(Pdt)
add_subdirectory(PacDetector)
add_subdirectory(PacDisplay)
add_subdirectory(PacEmc)
add_subdirectory(PacEnv)
add_subdirectory(PacGeom)
add_subdirectory(PacSim)
#add_subdirectory(PacTest)
add_subdirectory(PacTrk)
add_subdirectory(ProbTools)
add_subdirectory(RandControl)
#add_subdirectory(StdHepData)
add_subdirectory(TrajGeom)
add_subdirectory(RecoUtils)
add_subdirectory(TrkBase)
add_subdirectory(TrkEnv)
add_subdirectory(TrkFitter)
add_subdirectory(TrkGeom)


install(TARGETS AbsEnv AbsEvent AbsEventTag AbsParm BField BaBar BbrGeom BbrStdUtils BdbTime CLHEP CommonUtils CrnLib DetectorModel ErrLogger FrameExtension FrameJobReport FrameLogger FrameUtil Framework GenEnv KalmanTrack MatEnv MemUtil MsgLogger Pdt PacDetector PacDisplay PacEnv PacGeom PacTrk ProbTools ProxyDict RandControl TrajGeom TrkEnv TrkBase TrkFitter TrkGeom difAlgebra RecoUtils
PacEmc PacSim G3Data EvtGenBase EvtGen EvtGenModels photos stdhep Jst74
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install headers
# Globbing here is fine because it does not influence build behaviour
install(DIRECTORY "${CMAKE_SOURCE_DIR}/"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/KinKal
        FILES_MATCHING PATTERN "*.hh" 
        PATTERN ".git*" EXCLUDE
)


message (STATUS "Writing setup.sh...")

file(WRITE ${PROJECT_BINARY_DIR}/setup.sh "
#
# Do not hand edit this file. It was generated by CMake.

(return 0 2>/dev/null) && sourced=1 || sourced=0
if [ \"\$sourced\" = \"0\" ];then
    echo \"You should be sourcing this file, not executing it.\"
    exit 1
fi
export PACKAGE_SOURCE=${CMAKE_SOURCE_DIR}
export DEBUG_LEVEL=${CMAKE_BUILD_TYPE}

# Linux: 
export LD_LIBRARY_PATH=\${PWD}/lib:\${LD_LIBRARY_PATH}

# MacOS:
export DYLD_FALLBACK_LIBRARY_PATH=\${PWD}/lib:\${ROOT_LIB}:\${DYLD_FALLBACK_LIBRARY_PATH}

")


