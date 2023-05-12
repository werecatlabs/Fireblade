cmake_minimum_required(VERSION 3.13)
project(FBFFMpeg)


# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBFFMpeg/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBFFMpeg/*.cpp")

include_directories( ${FB_ROOT}/Dependencies/boost )

if (FB_USE_ONETBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/onetbb/include )
elseif (FB_USE_TBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tbb/include )
endif()

include_directories( ${FB_ROOT}/Dependencies/tinybind )
include_directories( ${FB_ROOT}/Dependencies/tinybind/tinyxml )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/ffmpeg )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/OgreMain/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Components/Overlay/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Components/Terrain/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Components/Paging/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Components/Bites/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/particle_universe/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/PlugIns/CgProgramManager/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/RenderSystems/GL/include )
include_directories( ../../Include )
include_directories( ../../Include/FBFFMpeg )


add_library(FBFFMpeg STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBFFMpeg PROPERTIES FOLDER Engine/Cpp/Render)

if (NOT _FB_STATIC_LIB_)
	target_compile_definitions(FBFFMpeg PUBLIC FB_EXPORTS)
endif()

