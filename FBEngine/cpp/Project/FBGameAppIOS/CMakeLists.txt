cmake_minimum_required(VERSION 3.13)
project(FBGameAppIOS)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBGameAppIOS/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBGameAppIOS/*.cpp")

include_directories( ${FB_ROOT}/Dependencies/boost )
include_directories( ${FB_ROOT}/Dependencies/tbb/include )
include_directories( ${FB_ROOT}/Dependencies/tinybind )
include_directories( ${FB_ROOT}/Dependencies/tinybind/tinyxml )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ../../Include )
include_directories( ../../Include/FBGameAppIOS )

add_library(FBGameAppIOS STATIC ${HEADER_FILES} ${SOURCE_FILES})
