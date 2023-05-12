cmake_minimum_required(VERSION 3.13)
project(FBPhysics2)

add_compile_definitions (_FB_STATIC_LIB_)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBPhysics2/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBPhysics2/*.cpp")

set(FB_HEADER_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBPhysics2/../../Include/FBPhysics2")
set(FB_SOURCE_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBPhysics2/../../Source/FBPhysics2")

foreach(_source IN ITEMS ${HEADER_FILES})
    get_filename_component(_source_file_path "${_source}" DIRECTORY)
    string(REPLACE "${FB_HEADER_REL_PATH}" "" _group_file_path "${_source_file_path}")
    string(REPLACE "/" "\\" _group_file_path "${_group_file_path}")
    source_group("Header Files\\${_group_file_path}" FILES "${_source}")
endforeach()

foreach(_source IN ITEMS ${SOURCE_FILES})
    get_filename_component(_source_file_path "${_source}" DIRECTORY)
    string(REPLACE "${FB_SOURCE_REL_PATH}" "" _group_file_path "${_source_file_path}")
    string(REPLACE "/" "\\" _group_file_path "${_group_file_path}")
    source_group("Source Files\\${_group_file_path}" FILES "${_source}")
endforeach()

if (UNIX)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

include_directories( ${FB_ROOT}/Dependencies/boost )
include_directories( ${FB_ROOT}/AdditionalLibraries/box2d/Include )
include_directories( ${FB_ROOT}/AdditionalLibraries/box2d/Source )

if (FB_USE_ONETBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/onetbb/include )
elseif (FB_USE_TBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tbb/include )
endif()

include_directories( ../../Include )
include_directories( ../../Include/FBPhysics2 )

add_library(FBPhysics2 STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBPhysics2 PROPERTIES FOLDER Engine/Cpp/Physics)