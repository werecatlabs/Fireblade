project(FBImGui)

if (APPLE)
    project(FBImGui C CXX OBJC)
else()
    project(FBImGui C CXX)
endif()

add_compile_definitions (_FB_STATIC_LIB_)
add_compile_definitions (IMGUI_IMPL_WIN32_DISABLE_GAMEPAD)


if (FB_GRAPHICS_SYSTEM_OGRENEXT)
    add_compile_definitions (FB_OGRE_USE_PARTICLE_UNIVERSE=0)
    add_compile_definitions (RAPIDJSON_HAS_STDSTRING=1)
    add_compile_definitions (OGRE_STATIC_LIB)
    add_compile_definitions (FREEIMAGE_LIB)
    add_compile_definitions (OGRE_DEPRECATED_2_2)
    #add_compile_definitions (IM_VEC2_CLASS_EXTRA)
endif()

# define header and source files for the library
if (WIN32)
    file(GLOB ROOT_HEADER_FILES "../../Include/FBImGui/*.h" "../../Include/FBImGui/*.inl")
    file(GLOB ROOT_SOURCE_FILES "../../Source/FBImGui/*.cpp")
elseif(APPLE)
    file(GLOB ROOT_HEADER_FILES "../../Include/FBImGui/*.h" "../../Include/FBImGui/*.inl")
    file(GLOB ROOT_SOURCE_FILES "../../Source/FBImGui/*.cpp" "../../Source/FBImGui/*.mm")
elseif (UNIX)
    file(GLOB ROOT_HEADER_FILES "../../Include/FBImGui/*.h" "../../Include/FBImGui/*.inl")
    file(GLOB ROOT_SOURCE_FILES "../../Source/FBImGui/*.cpp")
endif()

list(APPEND HEADER_FILES ${ROOT_HEADER_FILES})
list(APPEND SOURCE_FILES ${ROOT_SOURCE_FILES})

if (WIN32)
    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_demo.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_draw.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_impl_dx11.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_impl_dx9.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_impl_opengl3.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    if (FB_BUILD_SDL2)
        file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_impl_sdl.cpp")
        list(APPEND SOURCE_FILES ${EXTERN_FILE})

        file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_impl_sdlrenderer.cpp")
        list(APPEND SOURCE_FILES ${EXTERN_FILE})
    else()
        file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_impl_win32.cpp")
        list(APPEND SOURCE_FILES ${EXTERN_FILE})
    endif()

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_tables.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_widgets.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    if (FB_GRAPHICS_SYSTEM_OGRE AND NOT FB_GRAPHICS_SYSTEM_OGRENEXT)
        file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_freetype.cpp")
        list(APPEND SOURCE_FILES ${EXTERN_FILE})
    endif()
elseif(APPLE)
    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_demo.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_draw.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_impl_opengl3.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    if (FB_BUILD_SDL2)
        file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_impl_sdl.cpp")
        list(APPEND SOURCE_FILES ${EXTERN_FILE})

        file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_impl_sdlrenderer.cpp")
        list(APPEND SOURCE_FILES ${EXTERN_FILE})
    endif()

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_tables.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_widgets.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_impl_metal.mm")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_impl_osx.mm")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Apple/MainMenu.storyboard")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Apple/ImGuiApplicationOSX.mm")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})
elseif(UNIX)
    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_demo.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_draw.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_impl_opengl3.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    if (FB_BUILD_SDL2)
        file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_impl_sdl.cpp")
        list(APPEND SOURCE_FILES ${EXTERN_FILE})

        file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_impl_sdlrenderer.cpp")
        list(APPEND SOURCE_FILES ${EXTERN_FILE})
    endif()

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_tables.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "../../Source/FBImGui/Extern/imgui_widgets.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})
endif()

file(GLOB IMGUI_HEADER_FILES "../../Include/FBImGui/ImguiOgre/*.h")
file(GLOB IMGUI_SOURCE_FILES "../../Source/FBImGui/ImguiOgre/*.cpp")

list(APPEND HEADER_FILES ${IMGUI_HEADER_FILES})
list(APPEND SOURCE_FILES ${IMGUI_SOURCE_FILES})

file(GLOB IMGUI_HEADER_FILES "../../Include/FBImGui/ImGuizmo/*.h")
file(GLOB IMGUI_SOURCE_FILES "../../Source/FBImGui/ImGuizmo/*.cpp")

list(APPEND HEADER_FILES ${IMGUI_HEADER_FILES})
list(APPEND SOURCE_FILES ${IMGUI_SOURCE_FILES})

set(FB_HEADER_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBImGui/../../Include/FBImGui")
set(FB_SOURCE_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBImGui/../../Source/FBImGui")

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
include_directories( ${FB_ROOT}/Dependencies/tinyxml )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )

if (FB_BUILD_IMGUI)
    include_directories( ${FB_ROOT}/AdditionalLibraries/imgui )
    include_directories( ${FB_ROOT}/AdditionalLibraries/imgui/backends )
    include_directories( ${FB_ROOT}/AdditionalLibraries/imgui/misc/freetype )
endif()

if (FB_BUILD_SDL2)
    include_directories( ${FB_ROOT}/AdditionalLibraries/sdl2/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/sdl_image )
endif()

include_directories( ${FB_ROOT}/AdditionalLibraries/zlib )
include_directories( ${FB_ROOT}/AdditionalLibraries/zziplib )
include_directories( ../../Include )

include_directories( ${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include/FBImGui )
include_directories( ${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include/FBImGui/Extern )
include_directories( ${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include/FBImGui/ImguiOgre )
include_directories( ${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include/FBImGui/ImGuizmo )

if (FB_GRAPHICS_SYSTEM_OGRENEXT)
    if(WIN32)
        include_directories( ${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include/FBGraphicsOgreNext/Ogre/include/win32 )
    elseif(APPLE)
        include_directories( ${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include/FBGraphicsOgreNext/Ogre/include/osx )
    else()
        include_directories( ${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include/FBGraphicsOgreNext/Ogre/include/linux )
    endif()

    include_directories( ${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include/FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include )
    include_directories( ${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include/FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/GL )
    include_directories( ${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include/FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/GLSL )
    include_directories( ${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include/FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/Vao )
    include_directories( ${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include/FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/windowing )
    include_directories( ${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include/FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/WIN32 )

    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Dependencies/include/freetype2 )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Dependencies/include/freetype2/freetype )

    include_directories( ${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include/FBGraphicsOgreNext/Ogre/OgreMain/include )

    if (FB_BUILD_RENDERER_METAL)
        include_directories( ../../Include/FBGraphicsOgreNext/Ogre/RenderSystems/Metal/include )
        include_directories( ../../Include/FBGraphicsOgreNext/Ogre/RenderSystems/Metal/include/Windowing/OSX )
    endif()
elseif (FB_GRAPHICS_SYSTEM_OGRE)
    if(WIN32)
        include_directories( ../../Include/FBGraphicsOgre/BuildSettings/win32 )
    elseif(APPLE)
        include_directories( ../../Include/FBGraphicsOgre/BuildSettings/osx )
    else()
        include_directories( ../../Include/FBGraphicsOgre/BuildSettings/linux )
    endif()

    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Dependencies/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Dependencies/include/freetype2 )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Dependencies/include/freetype2/freetype )

    if (FB_GRAPHICS_SYSTEM_USE_STATIC_LIBS)
        include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include )
        include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/OgreMain/include )
        include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Components/Overlay/include )
    else ()
        include_directories( ${FB_ROOT}/FBEngine/cpp/Include/FBGraphicsOgre/OgreLibrary/OgreMain/include )
        include_directories( ${FB_ROOT}/FBEngine/cpp/Source/FBGraphicsOgre/OgreLibrary/OgreMain/src )

        include_directories( ${FB_ROOT}/FBEngine/cpp/Include/FBGraphicsOgre/OgreLibrary/Components/Overlay/include )
    endif ()
endif()

if (FB_USE_PRECOMPILED_HEADERS)
    use_precompiled_header(FBCore "FBImGui/FBImGuiPCH.h" "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBImGui/FBImGuiPCH.cpp")

    file(GLOB_RECURSE SOURCE_FILES_EXTERN "../../Source/FBImGui/Extern/*.cpp" "../../Source/FBImGui/Extern/*.c" )
    foreach(_source IN ITEMS ${SOURCE_FILES_EXTERN})
        set_source_files_properties(${_source}
            PPROPERTIES COMPILE_FLAGS /Y-
	    )
    endforeach()

    file(GLOB_RECURSE SOURCE_FILES_EXTERN "../../Source/FBImGui/ImguiOgre/*.cpp" "../../Source/FBImGui/ImguiOgre/*.c" )
    foreach(_source IN ITEMS ${SOURCE_FILES_EXTERN})
        set_source_files_properties(${_source}
            PPROPERTIES COMPILE_FLAGS /Y-
	    )
    endforeach()

    file(GLOB_RECURSE SOURCE_FILES_EXTERN "../../Source/FBImGui/ImGuizmo/*.cpp" "../../Source/FBImGui/ImGuizmo/*.c" )
    foreach(_source IN ITEMS ${SOURCE_FILES_EXTERN})
        set_source_files_properties(${_source}
            PPROPERTIES COMPILE_FLAGS /Y-
	    )
    endforeach()
endif()

add_library(FBImGui STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBImGui PROPERTIES FOLDER Engine/Cpp/Render)
