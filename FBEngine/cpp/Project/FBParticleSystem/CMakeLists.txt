cmake_minimum_required(VERSION 3.13)
project(FBParticleSystem)

add_compile_definitions (_FB_STATIC_LIB_)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBParticleSystem/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBParticleSystem/*.cpp")

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

if (FB_USE_PRECOMPILED_HEADERS)
    use_precompiled_header(FBParticleSystem "FBParticleSystem/FBParticleSystemPCH.h" "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBParticleSystem/FBParticleSystemPCH.cpp")
endif()

add_library(FBParticleSystem STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBParticleSystem PROPERTIES FOLDER Engine/Cpp/Render)
