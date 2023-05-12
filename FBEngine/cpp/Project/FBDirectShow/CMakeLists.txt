cmake_minimum_required(VERSION 3.13)
project(FBDirectShow)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBDirectShow/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBDirectShow/*.cpp")

include_directories( ${FB_ROOT}/Dependencies/boost )
include_directories( ${FB_ROOT}/Dependencies/tbb/include )
include_directories( ${FB_ROOT}/Dependencies/tinybind )
include_directories( ${FB_ROOT}/Dependencies/tinybind/tinyxml )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
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
include_directories( ../../Include/FBDirectShow )


add_library(FBDirectShow STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBDirectShow PROPERTIES FOLDER Engine/Render)
