project(FBGraphics)

add_compile_definitions (FB_OGRE_USE_PARTICLE_UNIVERSE=0)
add_compile_definitions (RAPIDJSON_HAS_STDSTRING=1)
add_compile_definitions (OGRE_STATIC_LIB)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBGraphics/*.h" "../../Include/FBGraphics/*.inl")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBGraphics/*.cpp")

set(FB_HEADER_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBGraphics/../../Include/FBGraphics")
set(FB_SOURCE_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBGraphics/../../Source/FBGraphics")

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

include_directories( ${FB_ROOT}/Dependencies/boost )

if (FB_USE_ONETBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/onetbb/include )
elseif (FB_USE_TBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tbb/include )
endif()

include_directories( ${FB_ROOT}/Dependencies/tinybind )
include_directories( ${FB_ROOT}/Dependencies/tinybind/tinyxml )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )

include_directories( ${FB_ROOT}/FBEngine/cpp/Include )
include_directories( ${FB_ROOT}/FBEngine/cpp/Include/FBGraphics )
include_directories( ${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include )

if (FB_GRAPHICS_SYSTEM_OGRENEXT)
    if(WIN32)
        include_directories( ../../Include/FBGraphicsOgreNext/Ogre/include/win32 )
    elseif(APPLE)
        include_directories( ../../Include/FBGraphicsOgreNext/Ogre/include/osx )
    else()
        include_directories( ../../Include/FBGraphicsOgreNext/Ogre/include/linux )
    endif()

    include_directories( ../../Include/FBGraphicsOgreNext/Ogre/include )
    include_directories( ../../Include/FBGraphicsOgreNext/Ogre/OgreMain/include )
elseif (FB_GRAPHICS_SYSTEM_OGRE)
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/OgreMain/include )

    include_directories( ${FB_ROOT}/AdditionalLibraries/particle_universe/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/PlugIns/CgProgramManager/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/RenderSystems/GL/include )

    if(WIN32)
        include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/win32 )
        include_directories( ../../Include/FBGraphicsOgre/BuildSettings/win32 )
    elseif(APPLE)
        include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/osx )
        include_directories( ../../Include/FBGraphicsOgre/BuildSettings/osx )
    else()
        include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/linux )
        include_directories( ../../Include/FBGraphicsOgre/BuildSettings/linux )
    endif()
endif()

add_library(FBGraphics STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBGraphics PROPERTIES FOLDER Engine/Cpp/Render)

if (NOT _FB_STATIC_LIB_)
	target_compile_definitions(FBGraphics PUBLIC FB_EXPORTS)
endif()

