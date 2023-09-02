cmake_minimum_required(VERSION 3.13)
project(FBTest)



# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBTest/*.h" "../../Include/FBTest/*.hpp" "../../Include/FBTest/*.inl")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBTest/*.cpp")

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
include_directories( ${FB_ROOT}/AdditionalLibraries/wildmagic/SDK/Include )
include_directories( ../../Include )

if (FB_USE_PTHREADS)
	include_directories( ${FB_ROOT}/AdditionalLibraries/pthreads/Include )
endif()

set(FB_HEADER_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBTest/../../Include/FBTest")
set(FB_SOURCE_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBTest/../../Source/FBTest")

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

if (FB_USE_PRECOMPILED_HEADERS)
    use_precompiled_header(FBTest "FBTest/FBTestPCH.h" "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBTest/FBTestPCH.cpp")
endif()

add_library(FBTest STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBTest PROPERTIES FOLDER Engine/Cpp/Core)
