if (APPLE)
    set(DEVELOPMENT_PROJECT_NAME "project")                     # <== Set to your project name, e.g. project.xcodeproj
    set(DEVELOPMENT_TEAM_ID "AAAAAAAAAA")                       # <== Set to your team ID from Apple
    set(APP_NAME "YOURAPP")                                     # <== Set To your app's name
    set(APP_BUNDLE_IDENTIFIER "com.company.app")                # <== Set to your app's bundle identifier
    set(FRAMEWORK_NAME "FooBar")                                # <== Set to your framework's name
    set(FRAMEWORK_BUNDLE_IDENTIFIER "com.company.framework")    # <== Set to your framework's bundle identifier (cannot be the same as app bundle identifier)
    set(TEST_NAME "Tests")                                      # <== Set to your test's name
    set(TEST_BUNDLE_IDENTIFIER "com.company.tests")             # <== Set to your tests's bundle ID
    set(CODE_SIGN_IDENTITY "iPhone Developer")                  # <== Set to your preferred code sign identity, to see list:
    # /usr/bin/env xcrun security find-identity -v -p codesigning
    set(DEPLOYMENT_TARGET 8.0)                                  # <== Set your deployment target version of iOS
    set(DEVICE_FAMILY "1")                                      # <== Set to "1" to target iPhone, set to "2" to target iPad, set to "1,2" to target both
    set(LOGIC_ONLY_TESTS 0)                                     # <== Set to 1 if you do not want tests to be hosted by the application, speeds up pure logic tests but you can not run them on real devices
    project(Editor)


    message(STATUS XCTestFound:${XCTest_FOUND})

    set(PRODUCT_NAME ${APP_NAME})
    set(EXECUTABLE_NAME ${APP_NAME})
    set(MACOSX_BUNDLE_EXECUTABLE_NAME ${APP_NAME})
    set(MACOSX_BUNDLE_INFO_STRING ${APP_BUNDLE_IDENTIFIER})
    set(MACOSX_BUNDLE_GUI_IDENTIFIER ${APP_BUNDLE_IDENTIFIER})
    set(MACOSX_BUNDLE_BUNDLE_NAME ${APP_BUNDLE_IDENTIFIER})
    set(MACOSX_BUNDLE_ICON_FILE "")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "1.0")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "1.0")
    set(MACOSX_BUNDLE_COPYRIGHT "Copyright YOU")
    set(MACOSX_DEPLOYMENT_TARGET ${DEPLOYMENT_TARGET})
else ()
    project(Editor)
endif()

if (UNIX)
    if (NOT APPLE)
        find_package(X11 REQUIRED)
    endif()
endif()

if (FB_EDITOR_TESTS)
    add_compile_definitions(
        $<$<CONFIG:Debug>:FB_EDITOR_TESTS=1>
        $<$<CONFIG:Release>:FB_EDITOR_TESTS=0>
        $<$<CONFIG:MinSizeRel>:FB_EDITOR_TESTS=0>
        $<$<CONFIG:RelWithDebInfo>:FB_EDITOR_TESTS=0>
        )
else ()
    add_compile_definitions (FB_EDITOR_TESTS=0)
endif()

add_compile_definitions( BOOST_ALL_NO_LIB )
add_compile_definitions( __TBB_NO_IMPLICIT_LINKAGE=1 )
add_compile_definitions( __TBBMALLOC_NO_IMPLICIT_LINKAGE=1 )
add_compile_definitions (_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
add_compile_definitions (_UNICODE)

if (FB_BUILD_WXWIDGETS)
    if (APPLE)
        add_compile_definitions(__WXMAC__)
        add_compile_definitions(__WXOSX__)
        add_compile_definitions(__WXOSX_COCOA__)
    elseif (UNIX)
        add_compile_definitions(__WXUNIX__)
        add_compile_definitions(__WXGTK__)
    endif()
endif()

# define header and source files for the library
file(GLOB HEADER_FILES "src/*.h")
file(GLOB SOURCE_FILES "src/*.cpp")
set(FB_HEADER_REL_PATH "${FB_ROOT}/Tools/cpp/Editor/src")
set(FB_SOURCE_REL_PATH "${FB_ROOT}/Tools/cpp/Editor/src")

file(GLOB_RECURSE HEADER_FILES_EDITOR "src/ai/*.h")
file(GLOB_RECURSE SOURCE_FILES_EDITOR "src/ai/*.cpp")
list(APPEND HEADER_FILES ${HEADER_FILES_EDITOR})
list(APPEND SOURCE_FILES ${SOURCE_FILES_EDITOR})

file(GLOB_RECURSE HEADER_FILES_EDITOR "src/commands/*.h")
file(GLOB_RECURSE SOURCE_FILES_EDITOR "src/commands/*.cpp")
list(APPEND HEADER_FILES ${HEADER_FILES_EDITOR})
list(APPEND SOURCE_FILES ${SOURCE_FILES_EDITOR})

file(GLOB_RECURSE HEADER_FILES_EDITOR "src/core/*.h")
file(GLOB_RECURSE SOURCE_FILES_EDITOR "src/core/*.cpp")
list(APPEND HEADER_FILES ${HEADER_FILES_EDITOR})
list(APPEND SOURCE_FILES ${SOURCE_FILES_EDITOR})

file(GLOB_RECURSE HEADER_FILES_EDITOR "src/editor/*.h")
file(GLOB_RECURSE SOURCE_FILES_EDITOR "src/editor/*.cpp")
list(APPEND HEADER_FILES ${HEADER_FILES_EDITOR})
list(APPEND SOURCE_FILES ${SOURCE_FILES_EDITOR})

file(GLOB_RECURSE HEADER_FILES_EDITOR "src/jobs/*.h")
file(GLOB_RECURSE SOURCE_FILES_EDITOR "src/jobs/*.cpp")
list(APPEND HEADER_FILES ${HEADER_FILES_EDITOR})
list(APPEND SOURCE_FILES ${SOURCE_FILES_EDITOR})

file(GLOB_RECURSE HEADER_FILES_EDITOR "src/messages/*.h")
file(GLOB_RECURSE SOURCE_FILES_EDITOR "src/messages/*.cpp")
list(APPEND HEADER_FILES ${HEADER_FILES_EDITOR})
list(APPEND SOURCE_FILES ${SOURCE_FILES_EDITOR})

file(GLOB_RECURSE HEADER_FILES_EDITOR "src/script/*.h")
file(GLOB_RECURSE SOURCE_FILES_EDITOR "src/script/*.cpp")
list(APPEND HEADER_FILES ${HEADER_FILES_EDITOR})
list(APPEND SOURCE_FILES ${SOURCE_FILES_EDITOR})

file(GLOB_RECURSE HEADER_FILES_EDITOR "src/tests/*.h")
file(GLOB_RECURSE SOURCE_FILES_EDITOR "src/tests/*.cpp")
list(APPEND HEADER_FILES ${HEADER_FILES_EDITOR})
list(APPEND SOURCE_FILES ${SOURCE_FILES_EDITOR})

file(GLOB_RECURSE HEADER_FILES_EDITOR "src/ui/*.h")
file(GLOB_RECURSE SOURCE_FILES_EDITOR "src/ui/*.cpp")
list(APPEND HEADER_FILES ${HEADER_FILES_EDITOR})
list(APPEND SOURCE_FILES ${SOURCE_FILES_EDITOR})

if (WIN32)
    file(GLOB HEADER_FILES_INCLUDE_WIN32 "${FB_ROOT}/Tools/cpp/Editor/src/application/win32/*.h")
    file(GLOB SOURCE_FILES_INCLUDE_WIN32 "${FB_ROOT}/Tools/cpp/Editor/src/application/win32/*.cpp")
    list(APPEND HEADER_FILES ${HEADER_FILES_INCLUDE_WIN32})
    list(APPEND SOURCE_FILES ${SOURCE_FILES_INCLUDE_WIN32})
elseif(APPLE)
    set(CMAKE_MACOSX_RPATH 1)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(FOUNDATION_LIBRARY Foundation)
    find_library(IOKIT_LIBRARY IOKit)
    find_path(COCOA_INCLUDE_DIR Cocoa/Cocoa.h)
    find_library(CARBON_LIBRARY Carbon)

    #add_definitions(-x objective-c++)

    file(GLOB HEADER_FILES_INCLUDE_OSX "${FB_ROOT}/Tools/cpp/Editor/src/application/apple/*.h")
    file(GLOB SOURCE_FILES_INCLUDE_OSX "${FB_ROOT}/Tools/cpp/Editor/src/application/apple/*.cpp" "${FB_ROOT}/Tools/cpp/Editor/src/application/apple/*.mm")
    list(APPEND HEADER_FILES ${HEADER_FILES_INCLUDE_OSX})
    list(APPEND SOURCE_FILES ${SOURCE_FILES_INCLUDE_OSX})
elseif(UNIX)
    file(GLOB HEADER_FILES_OGRE_INCLUDE_UNIX "../../Include/FBGraphicsOgreNext/Window/OIS/*.h")
    file(GLOB SOURCE_FILES_OGRE_INCLUDE_UNIX "../../Source/FBGraphicsOgreNext/Window/OIS/*.cpp")
    list(APPEND HEADER_FILES ${HEADER_FILES_OGRE_INCLUDE_UNIX})
    list(APPEND SOURCE_FILES ${SOURCE_FILES_OGRE_INCLUDE_UNIX})
endif()

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

include_directories( ${FB_ROOT}/Dependencies/boost)
include_directories( ${FB_ROOT}/Dependencies/tinyxml/include)

if (FB_USE_ONETBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/onetbb/include )
elseif (FB_USE_TBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tbb/include )
endif()

include_directories( ${FB_ROOT}/AdditionalLibraries/cppsqlite/include)
include_directories( ${FB_ROOT}/AdditionalLibraries/sqlite/include)

if (FB_BUILD_WXWIDGETS)
    #include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets  )
    #include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets/include )

    if (WIN32)
        #include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets/include/msvc )
        #include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets/include/msvc/wx )
        #include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets/lib/vc_x64_lib/mswud  )
    elseif (APPLE)
        #include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets/lib/osx/wx/include/osx_cocoa-unicode-static-3.1  )
    elseif (UNIX)
        #include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets/include/gtk3-unicode-3.1 )
        #include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets/include/gtk3-unicode-3.1/wx )
    endif ()
endif()

include_directories(${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include)
include_directories(${CMAKE_SOURCE_DIR}/Tools/cpp/Editor/src)
include_directories( ${FB_ROOT}/AdditionalLibraries/entt/src )
include_directories( ${FB_ROOT}/AdditionalLibraries/imgui )
include_directories( ${FB_ROOT}/AdditionalLibraries/imgui/backends )

if (WIN32)
    link_directories(Editor ${CMAKE_SOURCE_DIR}/libs/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/${CMAKE_BUILD_TYPE})

    if (FB_BUILD_PHYSX)
        link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/physx/${CMAKE_BUILD_TYPE})
    endif()

    if (FB_USE_ONETBB)
        link_directories(Editor ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/oneTBB/)
    elseif (FB_USE_TBB)
        link_directories(Editor ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tbb/)
    endif()

    link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/python/)


    link_directories(Editor ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/boost/${CMAKE_BUILD_TYPE})
    link_directories(Editor ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tinybind/${CMAKE_BUILD_TYPE})
    link_directories(Editor ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tinyxml2/${CMAKE_BUILD_TYPE})
    
    link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ois/)

    if (FB_BUILD_WILDMAGIC5)
        link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/wildmagic/${CMAKE_BUILD_TYPE})   
    endif()

    if (FB_USE_FBXSDK)
        link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/fbxsdk/2020.2/${CMAKE_BUILD_TYPE})
    endif()

    if (FB_BUILD_SDL2)
        link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/sdl2/${CMAKE_BUILD_TYPE})
        link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/sdl_image/${CMAKE_BUILD_TYPE})
    endif()

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogrenext/${CMAKE_BUILD_TYPE})
        link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogrenext/dependencies/lib)
    elseif(FB_GRAPHICS_SYSTEM_OGRE)
        link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogre/${CMAKE_BUILD_TYPE})
        link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogre/dependencies/lib)
        link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogre/dependencies/lib/${CMAKE_BUILD_TYPE}/ )
        
        if (FB_PLATFORM_X64)
            link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x64)
        else ()
            link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x86)
        endif()
    endif ()

    if (FB_BUILD_WXWIDGETS)
        link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/wxWidgets/vc_x64_lib)
    endif()

    if (FB_USE_ASSET_IMPORT)
        link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/assimp/${CMAKE_BUILD_TYPE})
    endif()
elseif (APPLE)
    link_directories(Editor ${CMAKE_SOURCE_DIR}/libs/macOS/${CMAKE_BUILD_TYPE})

    link_directories(Editor ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/boost)
    link_directories(Editor ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/boost/${CMAKE_BUILD_TYPE})
    link_directories(Editor ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/tbb/${CMAKE_BUILD_TYPE})

    link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS)
    link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/${CMAKE_BUILD_TYPE})
    link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/assimp/${CMAKE_BUILD_TYPE})
    link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/physx/${CMAKE_BUILD_TYPE})
    link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/wildmagic/${CMAKE_BUILD_TYPE})
    link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/wxwidgets/${CMAKE_BUILD_TYPE})
    link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/sdl2/${CMAKE_BUILD_TYPE})
    link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ois/${CMAKE_BUILD_TYPE})

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogrenext/${CMAKE_BUILD_TYPE})
        link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogrenext/dependencies/lib/${CMAKE_BUILD_TYPE})
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogre/)
        link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogre/dependencies/lib/)
        link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x64)
    endif ()
elseif (UNIX)
    link_directories(Editor ${CMAKE_SOURCE_DIR}/libs/linux/${CMAKE_BUILD_TYPE})
    link_directories(Editor ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux)
    link_directories(Editor ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/boost)
    link_directories(Editor ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/tbb)
    link_directories(Editor ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/tinybind)
    link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux)
    link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/physx/${CMAKE_BUILD_TYPE})
    link_directories(Editor ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/wildmagic)
endif (WIN32)

if (FB_USE_PRECOMPILED_HEADERS)
    use_precompiled_header(Editor "GameEditorPCH.h" "${CMAKE_SOURCE_DIR}/Tools/cpp/Editor/src/GameEditorPCH.cpp")
endif()

set(RESOURCES
        "${FB_ROOT}/Tools/cpp/Editor/src/application/apple/macOS/MainMenu.storyboard")


set(STORYBOARD
        "${FB_ROOT}/Tools/cpp/Editor/src/application/apple/macOS/MainMenu.storyboard")

if (WIN32)
    add_executable(Editor ${HEADER_FILES} ${SOURCE_FILES})
    #add_executable(Editor WIN32 ${HEADER_FILES} ${SOURCE_FILES})
elseif(APPLE)
    #add_executable(Editor ${HEADER_FILES} ${SOURCE_FILES} ${RESOURCES})
    add_executable(Editor MACOSX_BUNDLE ${HEADER_FILES} ${SOURCE_FILES} ${RESOURCES})
else()
    add_executable(Editor ${HEADER_FILES} ${SOURCE_FILES})
endif()

if (WIN32)
    target_link_libraries(Editor FBAssimp.lib)
    target_link_libraries(Editor FBCore.lib)
    target_link_libraries(Editor FBGraphics.lib)
    target_link_libraries(Editor FBMesh.lib)
    target_link_libraries(Editor FBRenderUI.lib)
    target_link_libraries(Editor FBSQLite.lib)

    if (FB_ENABLE_LUA)
        target_link_libraries(Editor FBLua.lib)
        target_link_libraries(Editor FBLuaBind.lib)
    elseif (FB_ENABLE_PYTHON)
        target_link_libraries(Editor FBPython.lib)
        target_link_libraries(Editor FBPythonBind.lib)
    endif()

    if (FB_USE_ONETBB)
        target_link_libraries(Editor debug tbb12_debug.lib optimized tbb12.lib)
        target_link_libraries(Editor debug tbbmalloc_debug.lib optimized tbbmalloc.lib)
        target_link_libraries(Editor debug tbbmalloc_proxy_debug.lib optimized tbbmalloc_proxy.lib)
    elseif (FB_USE_TBB)
        target_link_libraries(Editor tbb_static.lib)
        target_link_libraries(Editor tbbmalloc_static.lib)
    endif()

    if (FB_BUILD_OISINPUT)
        target_link_libraries(Editor FBOISInput.lib)
        #target_link_libraries(Editor debug OIS_d.lib optimized OIS.lib)
        target_link_libraries(Editor dinput8.lib)
        target_link_libraries(Editor dxguid.lib)
    endif()

    if (FB_BUILD_WXWIDGETS)
        #target_link_libraries(Editor FBWxWidgets.lib)
    endif()

    if (FB_BUILD_SDL2)
        target_link_libraries(Editor SDL2main.lib)
        target_link_libraries(Editor SDL2_image.lib)
        target_link_libraries(Editor debug SDL2d.lib optimized SDL2.lib)
    endif()

    if (FB_BUILD_IMGUI)
        target_link_libraries(Editor FBImGui.lib)
        target_link_libraries(Editor imgui.lib)
    endif()

    if (FB_USE_FBXSDK)
        #target_link_libraries(Editor libfbxsdk.lib)
        target_link_libraries(Editor libfbxsdk-mt.lib)
        target_link_libraries(Editor libxml2-mt.lib)
        target_link_libraries(Editor tinyxml2.lib)
    endif()

    if (FB_CODEC_FREEIMAGE)
        target_link_libraries(Editor debug FreeImage_d.lib optimized FreeImage.lib)
    endif()

    if (FB_USE_FREETYPE)
        target_link_libraries(Editor debug freetype_d.lib optimized freetype.lib)
    endif()

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        target_link_libraries(Editor FBGraphicsOgreNext.lib)
        target_link_libraries(Editor harfbuzz.lib)
        target_link_libraries(Editor harfbuzz-subset.lib)
        target_link_libraries(Editor icucommon.lib)
        target_link_libraries(Editor sds_library.lib)

        if (FB_BUILD_RENDERER_DX11)
            target_link_libraries(Editor d3d11.lib)
            target_link_libraries(Editor dxgi.lib)
            target_link_libraries(Editor dxguid.lib)
        endif()

        if (FB_BUILD_RENDERER_GL3PLUS)
            target_link_libraries(Editor opengl32.lib)
            target_link_libraries(Editor glu32.lib)
        endif ()

        if (FB_PLATFORM_X64)
            #target_link_libraries(Editor amd_ags_x64.lib)
        endif()
    elseif(FB_GRAPHICS_SYSTEM_OGRE)
        target_link_libraries(Editor FBGraphicsOgre.lib)

        if (FB_BUILD_RENDERER_DX9)
            target_link_libraries(Editor d3d9.lib)
            target_link_libraries(Editor d3dx9.lib)
        endif ()

        if (FB_BUILD_RENDERER_DX11)
            target_link_libraries(Editor d3d11.lib)
            target_link_libraries(Editor dxgi.lib)
            target_link_libraries(Editor dxguid.lib)
        endif()

        if (FB_BUILD_RENDERER_OPENGL)
            target_link_libraries(Editor opengl32.lib)
            target_link_libraries(Editor glu32.lib)

            if (FB_PLATFORM_X64)
                #target_link_libraries(Editor amd_ags_x64.lib)
            endif()
        endif ()

        if (FB_BUILD_RENDERER_GL3PLUS)
            target_link_libraries(Editor opengl32.lib)
            target_link_libraries(Editor glu32.lib)
            
            if (FB_PLATFORM_X64)
                #target_link_libraries(Editor amd_ags_x64.lib)
            endif()
        endif ()

        if (FB_USE_FREETYPE)
            target_link_libraries(Editor freetype.lib)
        endif ()
    elseif(FB_BUILD_DX11)
        target_link_libraries(Editor FBGraphicsD3D11.lib)
    endif()

    if (FB_BUILD_PHYSX)
        target_link_libraries(Editor FBPhysx.lib)

        target_link_libraries(Editor PhysX3.lib)
        target_link_libraries(Editor PhysX3Extensions.lib)
        target_link_libraries(Editor PhysX3Common.lib)
        target_link_libraries(Editor PhysX3Vehicle.lib)
        target_link_libraries(Editor PhysX3Cooking.lib)
        target_link_libraries(Editor PxTask.lib)
        target_link_libraries(Editor PhysX3MetaData.lib)
        target_link_libraries(Editor PhysX3Foundation.lib)
        target_link_libraries(Editor PxSceneQuery.lib)
        target_link_libraries(Editor PxSimulationController.lib)
        target_link_libraries(Editor PhysX3LowLevel.lib)
        target_link_libraries(Editor PhysX3LowLevelCloth.lib)
        target_link_libraries(Editor PhysXProfileSDK.lib)

        if(FB_BUILD_PHYSICS_DEBUG)
            target_link_libraries(Editor PhysXVisualDebuggerSDK.lib)
            target_link_libraries(Editor PvdRuntime.lib)
        endif()
    elseif(FB_BUILD_ODE)
        target_link_libraries(Editor FBODE3.lib)
    elseif(FB_BUILD_PHYSICS3)
        target_link_libraries(Editor FBPhysics3.lib)
    endif()

    if (FB_USE_ASSET_IMPORT)
        #target_link_libraries(Editor debug assimp-vc142-mtd.lib optimized assimp-vc142-mt.lib)
    endif()

    #if (FB_BUILD_WILDMAGIC5)
     #   target_link_libraries(Editor debug Wm5CoreD.lib optimized Wm5Core.lib)
     #   target_link_libraries(Editor debug Wm5MathematicsD.lib optimized Wm5Mathematics.lib)
    #    target_link_libraries(Editor debug Wm5PhysicsD.lib optimized Wm5Physics.lib)
    #endif()

    target_link_libraries(Editor bcrypt.lib)
    target_link_libraries(Editor libboost_thread.lib)
    target_link_libraries(Editor libboost_date_time.lib)
    target_link_libraries(Editor libboost_chrono.lib)
    target_link_libraries(Editor libboost_timer.lib)
    target_link_libraries(Editor libboost_filesystem.lib)
    target_link_libraries(Editor libboost_json.lib)
    target_link_libraries(Editor minizip.lib)
    target_link_libraries(Editor nativefiledialog.lib)
    target_link_libraries(Editor rttr.lib)
    target_link_libraries(Editor tinyxml.lib)   
    target_link_libraries(Editor yaml-cpp.lib)
    target_link_libraries(Editor zlib.lib)
    target_link_libraries(Editor zzip.lib)
elseif (APPLE)
    target_link_libraries(Editor libFBCore.a)
    target_link_libraries(Editor libFBMesh.a)
    target_link_libraries(Editor libFBProcedural.a)
    target_link_libraries(Editor libFBAssimp.a)

    if (FB_ENABLE_LUA)
        target_link_libraries(Editor FBLua.a)
        target_link_libraries(Editor FBLuaBind.a)
    elseif (FB_ENABLE_PYTHON)
        target_link_libraries(Editor FBPython.a)
        target_link_libraries(Editor FBPythonBind.a)
    endif()

    if (FB_BUILD_SDL2)
        target_link_libraries(Editor SDL2main.a)
        target_link_libraries(Editor SDL2_image.a)
        target_link_libraries(Editor debug SDL2d.a optimized SDL2.a)
    endif()

    if (FB_BUILD_IMGUI)
        target_link_libraries(Editor libFBImGui.a)
        target_link_libraries(Editor imgui.a)
    endif()

    if (FB_BUILD_PHYSX)
        target_link_libraries(Editor libFBPhysx.a)
    elseif (FB_BUILD_ODE)
        target_link_libraries(Editor libFBODE3.a)
    endif()

    if (FB_BUILD_OISINPUT)
        target_link_libraries(Editor libFBOISInput.a)
        #target_link_libraries(Editor debug libOIS_d.a optimized libOIS.a)
    endif()

    target_link_libraries(Editor libFBVehicle.a)

    if (FB_BUILD_WXWIDGETS)
        #target_link_libraries(Editor libFBWxWidgets.a)

        target_link_libraries(Editor libwx_osx_cocoau_core-3.1.a)
        target_link_libraries(Editor libwx_baseu-3.1.a)
        target_link_libraries(Editor libwx_osx_cocoau_xrc-3.1.a)
        target_link_libraries(Editor libwx_osx_cocoau_propgrid-3.1.a)
        target_link_libraries(Editor libwx_osx_cocoau_richtext-3.1.a)
        target_link_libraries(Editor libwxexpat-3.1.a)
        target_link_libraries(Editor libwx_baseu_xml-3.1.a)
        target_link_libraries(Editor libwx_baseu_net-3.1.a)
        target_link_libraries(Editor libwx_osx_cocoau_webview-3.1.a)
        target_link_libraries(Editor libwx_osx_cocoau_xrc-3.1.a)
        target_link_libraries(Editor libwx_osx_cocoau_adv-3.1.a)
        target_link_libraries(Editor libwx_osx_cocoau_aui-3.1.a)
        target_link_libraries(Editor libwx_osx_cocoau_html-3.1.a)
        target_link_libraries(Editor libwxtiff-3.1.a)
        target_link_libraries(Editor libwxjpeg-3.1.a)
        target_link_libraries(Editor libwxpng-3.1.a)

        target_link_libraries(Editor libiconv.a)
    endif()

    target_link_libraries(Editor libFBRenderUI.a)
    target_link_libraries(Editor libFBSQLite.a)
    target_link_libraries(Editor libFBCPlusPlus.a)
    target_link_libraries(Editor libFBGraphics.a)

    target_link_libraries(Editor libboost_thread.a)
    target_link_libraries(Editor libboost_date_time.a)
    target_link_libraries(Editor libboost_chrono.a)
    target_link_libraries(Editor libboost_timer.a)
    target_link_libraries(Editor libboost_filesystem.a)
    target_link_libraries(Editor libboost_json.a)

    if (FB_BUILD_PHYSX)
        target_link_libraries(Editor FBPhysx.a)

        target_link_libraries(Editor PhysX3.a)
        target_link_libraries(Editor PhysX3Extensions.a)
        target_link_libraries(Editor PhysX3Common.a)
        target_link_libraries(Editor PhysX3Vehicle.a)
        target_link_libraries(Editor PhysX3Cooking.a)
        target_link_libraries(Editor PhysXProfileSDK.a)
        target_link_libraries(Editor PxTask.a)
        target_link_libraries(Editor PhysXVisualDebuggerSDK.a)
        target_link_libraries(Editor PhysX3MetaData.a)
        target_link_libraries(Editor PhysX3Foundation.a)
        target_link_libraries(Editor PxSceneQuery.a)
        target_link_libraries(Editor PxSimulationController.a)
        target_link_libraries(Editor PhysX3LowLevel.a)
        target_link_libraries(Editor PhysX3LowLevelCloth.a)
    endif()

    if (FB_USE_TBB)
        target_link_libraries(Editor libtbb.a)
        target_link_libraries(Editor libtbbmalloc.a)
    endif()

    target_link_libraries(Editor minizip.a)
    target_link_libraries(Editor nativefiledialog.a)
    target_link_libraries(Editor rttr.a)
    target_link_libraries(Editor tinyxml.a)
    target_link_libraries(Editor yaml-cpp.a)
    target_link_libraries(Editor zlib.a)
    target_link_libraries(Editor zzip.a)

    if (FB_BUILD_WILDMAGIC5)
        target_link_libraries(Editor libWm5Core.a)
        target_link_libraries(Editor libWm5Mathematics.a)
    endif()

    if (FB_USE_ASSET_IMPORT)
        #target_link_libraries(Editor debug libassimpd.a optimized libassimp.a)
    endif()

    if (FB_CODEC_FREEIMAGE)
        target_link_libraries(Editor FreeImage.a)
    endif()

    if (FB_USE_FREETYPE)
        target_link_libraries(Editor freetype.a)
    endif()

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        target_link_libraries(Editor FBGraphicsOgreNext.a)
        target_link_libraries(Editor harfbuzz.a)
        target_link_libraries(Editor harfbuzz-subset.a)
        target_link_libraries(Editor icucommon.a)
        target_link_libraries(Editor sds_library.a)

        if (FB_USE_FREETYPE)
            target_link_libraries(Editor freetype.a)
        endif()
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        target_link_libraries(Editor FBGraphicsOgre.a)

        if (FB_USE_FREETYPE)
            target_link_libraries(Editor freetype.a)
        endif()
    endif()

    #target_link_libraries(OgreOverlayTest stdc++ "-framework CoreGraphics" objc)
    #target_link_libraries(OgreOverlayTest ${AUDIO_UNIT})
    target_link_libraries(Editor stdc++ "-framework AppKit" "-framework Metal" "-framework MetalKit" "-framework OpenGL" "-framework Foundation" "-framework QuartzCore" "-framework CoreGraphics" "-framework CoreServices" "-framework Cocoa" "-framework Carbon" "-framework IOKit" "-framework GameController" objc)
    set_target_properties(Editor PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
elseif (UNIX)
    target_link_libraries(Editor -Wl,--start-group FBCore.a FBAero.a libFBProcedural.a FBImGui.a
        FBMath.a FBMesh.a FBOISInput.a FBVehicle.a FBData.a FBState.a libFBPhysx.a libFBSQLite.a
        libFBVehicle.a FBGraphics.a FBGraphicsOgreNext.a FBAssimp.a FBRenderUI.a libFBCPlusPlus.a
        libboost_thread-gcc5-mt-s-x64-1_70.a
        libboost_date_time-gcc5-mt-s-x64-1_70.a
        libboost_chrono-gcc5-mt-s-x64-1_70.a
        libboost_timer-gcc5-mt-s-x64-1_70.a
        libboost_filesystem-gcc5-mt-s-x64-1_70.a
        debug libtbb_debug.a optimized libtbb.a
        debug libtbbmalloc_debug.a optimized libtbbmalloc.a
        libWm5Core.a libWm5Mathematics.a
        libLowLevel.a libLowLevelCloth.a libPhysXProfileSDK.a libPhysX3Common.a libSceneQuery.a 
        libSimulationController.a libPhysX3Cooking.a libPhysX3Extensions.a libPhysX3.a libPhysX3Vehicle.a libPxTask.a
        -Wl,--end-group )

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        target_link_libraries(Editor FBGraphicsOgreNext.a)
        target_link_libraries(Editor freetype.a)
    elseif(FB_GRAPHICS_SYSTEM_SOFTWARE)
        target_link_libraries(Editor libFBGraphicsSoftware.a)
    endif()

    target_link_libraries(Editor png)
    target_link_libraries(Editor Xaw)
    target_link_libraries(Editor Xrandr)
    target_link_libraries(Editor Xt)
    target_link_libraries(Editor xcb)
    target_link_libraries(Editor GL)
    target_link_libraries(Editor ${X11_LIBRARIES})
    target_link_libraries(Editor ${CMAKE_THREAD_LIBS_INIT} )
    target_link_libraries(Editor ${CMAKE_DL_LIBS})
endif ()

# Create the app target
set_target_properties(Editor PROPERTIES
        RESOURCE "${RESOURCES}"
        MACOSX_BUNDLE_INFO_PLIST ${FB_ROOT}/Tools/cpp/Editor/src/application/apple/macOS/Info-macOS.plist

        )

add_dependencies(Editor FBAssimp)
add_dependencies(Editor FBCore)
add_dependencies(Editor FBGraphics)
add_dependencies(Editor FBMesh)
add_dependencies(Editor FBRenderUI)
add_dependencies(Editor FBSQLite)

if (FB_USE_TBB)
    add_dependencies(Editor tbb_static)
    add_dependencies(Editor tbbmalloc_static)
endif()

if (FB_ENABLE_LUA)
    add_dependencies(Editor FBLua)
    add_dependencies(Editor FBLuaBind)
elseif (FB_ENABLE_PYTHON)
    add_dependencies(Editor FBPython)
    add_dependencies(Editor FBPythonBind)
endif()

if (FB_BUILD_OISINPUT)
    add_dependencies(Editor FBOISInput)
endif()

if (FB_BUILD_IMGUI)
    add_dependencies(Editor FBImGui)
    add_dependencies(Editor imgui)
endif()

if (FB_BUILD_WXWIDGETS)
    #add_dependencies(Editor FBWxWidgets)
endif()

if (FB_BUILD_PHYSX)
    add_dependencies(Editor FBPhysx)

    add_dependencies(Editor PhysX3)
    add_dependencies(Editor PhysX3Extensions)
    add_dependencies(Editor PhysX3Common)
    add_dependencies(Editor PhysX3Vehicle)
    add_dependencies(Editor PhysX3Cooking)
    add_dependencies(Editor PxTask)    
    add_dependencies(Editor PhysX3MetaData)
    add_dependencies(Editor PhysX3Foundation)
    add_dependencies(Editor PxSceneQuery)
    add_dependencies(Editor PxSimulationController)
    add_dependencies(Editor PhysX3LowLevel)
    add_dependencies(Editor PhysX3LowLevelCloth)
    add_dependencies(Editor PhysXProfileSDK)

    if(FB_BUILD_PHYSICS_DEBUG)
        add_dependencies(Editor PhysXVisualDebuggerSDK)        
        add_dependencies(Editor PvdRuntime)
    endif()
elseif(FB_BUILD_ODE)
    add_dependencies(Editor FBODE3)
elseif(FB_BUILD_PHYSICS3)
    add_dependencies(Editor FBPhysics3)
endif()

if (FB_GRAPHICS_SYSTEM_OGRENEXT) 
    add_dependencies(Editor FBGraphicsOgreNext)
    add_dependencies(Editor harfbuzz)
    add_dependencies(Editor harfbuzz-subset)
    add_dependencies(Editor icucommon)
    add_dependencies(Editor sds_library)
elseif (FB_GRAPHICS_SYSTEM_OGRE)
    add_dependencies(Editor FBGraphicsOgre)
endif()

add_dependencies(Editor boost_thread)
add_dependencies(Editor boost_date_time)
add_dependencies(Editor boost_chrono)
add_dependencies(Editor boost_timer)
add_dependencies(Editor boost_filesystem)
add_dependencies(Editor boost_json)
add_dependencies(Editor minizip)
add_dependencies(Editor nativefiledialog)
add_dependencies(Editor rttr)
add_dependencies(Editor tinyxml) 
add_dependencies(Editor yaml-cpp)
add_dependencies(Editor zlib)
add_dependencies(Editor zzip)

if (WIN32)
    #set_property(TARGET Editor PROPERTY VS_DEBUGGER_COMMAND ${CMAKE_SOURCE_DIR}/Bin/windows/v142/x64/Debug/Editor.exe)
    #set_property(TARGET Editor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Bin/windows/v142/x64/Debug)

    set_target_properties(Editor PROPERTIES 
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Bin/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/$(Configuration)")

    set_target_properties (Editor PROPERTIES FOLDER Tools/Cpp)
endif()