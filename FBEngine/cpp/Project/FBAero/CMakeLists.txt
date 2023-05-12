cmake_minimum_required(VERSION 3.13)
project(FBAero)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBAero/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBAero/*.cpp")

include_directories( ${FB_ROOT}/Dependencies/boost )

if (FB_USE_ONETBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/onetbb/include )
elseif (FB_USE_TBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tbb/include )
endif()

include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/Dependencies/tinyxml/include )
include_directories( ../../Include )

if (FB_USE_PRECOMPILED_HEADERS)
    use_precompiled_header(FBAero "FBAero/FBAeroPCH.h" "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBAero/FBAeroPCH.cpp")
endif()

add_library(FBAero STATIC ${HEADER_FILES} ${SOURCE_FILES})


add_dependencies(FBAero FBCore)
set_target_properties (FBAero PROPERTIES FOLDER Engine/Cpp/Physics)

if (NOT _FB_STATIC_LIB_)
	target_compile_definitions(FBAero PUBLIC FB_EXPORTS)
endif()
