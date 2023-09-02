file(GLOB_RECURSE HEADER_FILES "./*.h")
#file(GLOB_RECURSE SOURCE_FILES "./*.c" "./*.cpp")

if (WIN32)
    file(GLOB EXTERN_FILE "imgui.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    #file(GLOB EXTERN_FILE "imgui_demo.cpp")
    #list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "imgui_draw.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "backends/imgui_impl_dx11.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "backends/imgui_impl_dx9.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "backends/imgui_impl_opengl3.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    if (FB_BUILD_SDL2)
        file(GLOB EXTERN_FILE "backends/imgui_impl_sdl.cpp")
        list(APPEND SOURCE_FILES ${EXTERN_FILE})

        file(GLOB EXTERN_FILE "backends/imgui_impl_sdlrenderer.cpp")
        list(APPEND SOURCE_FILES ${EXTERN_FILE})
    else()
        file(GLOB EXTERN_FILE "backends/imgui_impl_win32.cpp")
        list(APPEND SOURCE_FILES ${EXTERN_FILE})
    endif()

    file(GLOB EXTERN_FILE "imgui_tables.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    file(GLOB EXTERN_FILE "imgui_widgets.cpp")
    list(APPEND SOURCE_FILES ${EXTERN_FILE})

    if (FB_USE_FREETYPE)
        file(GLOB EXTERN_FILE "misc/freetype/imgui_freetype.cpp")
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

include_directories( ./ )

add_library(imgui ${HEADER_FILES} ${SOURCE_FILES})
