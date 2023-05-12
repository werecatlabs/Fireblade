cmake_minimum_required(VERSION 3.13)
project(FBFMODSound)

set(CMAKE_CXX_STANDARD 17)
add_compile_definitions (FB_OGRE_USE_PARTICLE_UNIVERSE=0)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBFMODSound/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBFMODSound/*.cpp")

include_directories( ${FB_ROOT}/Dependencies/boost )
include_directories( ${FB_ROOT}/Dependencies/tbb/include )
include_directories( ${FB_ROOT}/Dependencies/tinybind )
include_directories( ${FB_ROOT}/Dependencies/tinybind/tinyxml )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/fmod/studio/api/studio/inc )
include_directories( ${FB_ROOT}/AdditionalLibraries/fmod/studio/api/lowlevel/inc )
include_directories( ../../Include )
include_directories( ../../Include/FBFMODSound )


add_library(FBFMODSound STATIC ${HEADER_FILES} ${SOURCE_FILES})
