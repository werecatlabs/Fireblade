cmake_minimum_required(VERSION 3.13)
project(FBSQLite)


option(FB_BUILD_SQLITECODEC "Build sqlite codec" ON)

add_compile_definitions (_FB_STATIC_LIB_)

if (FB_BUILD_SQLITECODEC)
	add_compile_definitions (SQLITE_HAS_CODEC)
endif()

if (UNIX)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBSQLite/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBSQLite/extern/*.c" "../../Source/FBSQLite/*.c" "../../Source/FBSQLite/*.cpp")

include_directories( ${FB_ROOT}/Dependencies/boost )

if (FB_USE_ONETBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/onetbb/include )
elseif (FB_USE_TBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tbb/include )
endif()

include_directories( ${FB_ROOT}/Dependencies/tinybind )
include_directories( ${FB_ROOT}/Dependencies/tinyxml/include )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/zlib )
include_directories( ${FB_ROOT}/AdditionalLibraries/zziplib )
#include_directories( ${FB_ROOT}/AdditionalLibraries/cppsqlite/include )
include_directories( ../../Include )
include_directories(../../Include/FBSQLite/extern)

add_library(FBSQLite STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBSQLite PROPERTIES FOLDER Engine/Cpp/Core)
