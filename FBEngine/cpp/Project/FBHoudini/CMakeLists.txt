cmake_minimum_required(VERSION 3.13)
project(FBHoudini)




# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBHoudini/*.h" "../../Include/FBHoudini/*.hpp" "../../Include/FBHoudini/*.inl")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBHoudini/*.cpp" "../../Source/FBHoudini/*.c")

set(FB_HEADER_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBHoudini/../../Include/FBHoudini")
set(FB_SOURCE_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBHoudini/../../Source/FBHoudini")

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

if (FB_USE_ONETBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/onetbb/include )
elseif (FB_USE_TBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tbb/include )
endif()

include_directories( ${FB_ROOT}/Dependencies/tinybind )
include_directories( ${FB_ROOT}/Dependencies/tinyxml/include )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/ois/includes )
include_directories( ${FB_ROOT}/AdditionalLibraries/zlib )
include_directories( ${FB_ROOT}/AdditionalLibraries/zlib/src )
include_directories( ${FB_ROOT}/AdditionalLibraries/zlib/src/contrib/minizip )
include_directories( ${FB_ROOT}/AdditionalLibraries/zziplib )
include_directories( ../../Include )
include_directories( ../../Include/extern )
include_directories( ../../Include/FBHoudini )
include_directories( ../../Source/FBHoudini )

if(WIN32)
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/win32 )
elseif(APPLE)
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/osx )
else()
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/linux )
endif()

include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/OgreMain/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Components/Overlay/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Components/Paging/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Components/Bites/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/particle_universe/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/PlugIns/CgProgramManager/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/RenderSystems/Direct3D9/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/RenderSystems/Direct3D11/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/RenderSystems/GL/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/directx/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/PlugIns/STBICodec/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Components/RTShaderSystem/include )

if (FB_USE_PRECOMPILED_HEADERS)
    use_precompiled_header(FBHoudini "FBHoudini/FBHoudiniPCH.h" "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBHoudini/FBHoudiniPCH.cpp")
endif()

add_library(FBHoudini STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBHoudini PROPERTIES FOLDER Engine/Cpp/Procedural)


if (NOT _FB_STATIC_LIB_)
	target_compile_definitions(FBHoudini PUBLIC FB_EXPORTS)
endif()


# Link against the Houdini libraries, and add required include directories and compile definitions.
#target_link_libraries( FBHoudini Houdini )
# Configure several common target properties, such as its output directory.
#houdini_configure_target( FBHoudini )