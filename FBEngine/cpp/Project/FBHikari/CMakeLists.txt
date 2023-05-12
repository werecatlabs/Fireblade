cmake_minimum_required(VERSION 3.13)
project(FBHikari)


# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBHikari/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBHikari/*.cpp")

include_directories( ${FB_ROOT}/Dependencies/boost )
include_directories( ${FB_ROOT}/Dependencies/tbb/include )
include_directories( ${FB_ROOT}/Dependencies/tinybind )
include_directories( ${FB_ROOT}/Dependencies/tinybind/tinyxml )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/hikari/include )
include_directories( ../../Include )
include_directories( ../../Include/FBHikari )

add_library(FBHikari STATIC ${HEADER_FILES} ${SOURCE_FILES})
