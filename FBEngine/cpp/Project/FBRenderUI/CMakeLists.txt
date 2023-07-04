project(FBRenderUI)

#add_compile_definitions (MYGUI_STATIC)
#add_compile_definitions (MYGUI_USE_FREETYPE)
add_compile_definitions (FB_USE_MYGUI=0)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBRenderUI/*.h" "../../Include/FBRenderUI/*.inl")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBRenderUI/*.cpp")

set(FB_HEADER_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBRenderUI/../../Include/FBRenderUI")
set(FB_SOURCE_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBRenderUI/../../Source/FBRenderUI")

foreach(_source IN ITEMS ${HEADER_FILES})
    get_filename_component(_source_file_path "${_source}" DIRECTORY)
    string(REPLACE "${FB_HEADER_REL_PATH}" "" _group_file_path "${_source_file_path}")
    string(REPLACE "\\" "/" _group_file_path "${_group_file_path}")
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

include_directories( ${FB_ROOT}/Dependencies/tinyxml/include )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ../../Include )
include_directories( ../../Include/FBRenderUI )
#include_directories( ../../Include/FBRenderUI/Extern/MyGUI/MyGUIEngine/include )
#include_directories( ../../Include/FBRenderUI/Extern/MyGUI/Platforms/Ogre/OgrePlatform/include )

if(WIN32)
    #include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/win32 )
    include_directories( ../../Include/FBGraphicsOgre/BuildSettings/win32 )
elseif(APPLE)
    #include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/osx )
    include_directories( ../../Include/FBGraphicsOgre/BuildSettings/osx )
else()
    #include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/linux )
    include_directories( ../../Include/FBGraphicsOgre/BuildSettings/linux )
endif()

#include_directories( ${FB_ROOT}/FBEngine/cpp/Include/FBGraphicsOgre/Ogre/include )
#include_directories( ${FB_ROOT}/FBEngine/cpp/Include/FBGraphicsOgre/Ogre/OgreMain/include )

#include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Dependencies/include )
#include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Dependencies/include/freetype )
#include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Dependencies/include/freetype/config )

if (FB_USE_PRECOMPILED_HEADERS)
    use_precompiled_header(FBWxWidgets "FBRenderUI/FBRenderUIPCH.h" "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBRenderUI/FBRenderUIPCH.cpp")

    file(GLOB_RECURSE OGRE_SOURCE_FILES "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBRenderUI/Extern/MyGUI/MyGUIEngine/src/*.cpp")
    set_source_files_properties(${OGRE_SOURCE_FILES} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

    file(GLOB_RECURSE OGRE_SOURCE_FILES "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBRenderUI/Extern/MyGUI/Platforms/Ogre/OgrePlatform/src/*.cpp")
    set_source_files_properties(${OGRE_SOURCE_FILES} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endif()

add_library(FBRenderUI STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBRenderUI PROPERTIES FOLDER Engine/Cpp/Render)

if (NOT _FB_STATIC_LIB_)
	target_compile_definitions(FBRenderUI PUBLIC FB_EXPORTS)
endif()
