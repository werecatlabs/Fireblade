cmake_minimum_required(VERSION 3.13)
project(FBAnimation)

add_compile_definitions (_FB_STATIC_LIB_)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBAnimation/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBAnimation/*.cpp")

if (UNIX)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

include_directories( ${FB_ROOT}/Dependencies/boost )

if (FB_USE_ONETBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/onetbb/include )
elseif (FB_USE_TBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tbb/include )
endif()

include_directories( ${FB_ROOT}/Dependencies/tinybind )
include_directories( ${FB_ROOT}/Dependencies/tinyxml )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/zlib )
include_directories( ${FB_ROOT}/AdditionalLibraries/zziplib )
include_directories( ../../Include )

add_library(FBAnimation STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBAnimation PROPERTIES FOLDER Engine/Cpp/Render)
