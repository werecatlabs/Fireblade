cmake_minimum_required(VERSION 3.13)
project(FBBulletPhysics)

add_compile_definitions (_FB_STATIC_LIB_)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBBulletPhysics/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBBulletPhysics/*.cpp")

if (UNIX)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

include_directories( ${FB_ROOT}/Dependencies/boost )
include_directories( ${FB_ROOT}/Dependencies/tbb/include )
include_directories( ${FB_ROOT}/Dependencies/tinybind )
include_directories( ${FB_ROOT}/Dependencies/tinyxml )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/bullet/src )
include_directories( ${FB_ROOT}/AdditionalLibraries/zlib )
include_directories( ${FB_ROOT}/AdditionalLibraries/zziplib )
include_directories( ../../Include )

add_library(FBBulletPhysics STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBBulletPhysics PROPERTIES FOLDER Engine/Physics)