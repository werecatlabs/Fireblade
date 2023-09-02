if (APPLE)
    project(FBFileSystem C CXX OBJC)
else()
    project(FBFileSystem C CXX)
endif()

if(WIN32)
    add_compile_definitions (ZZIP_HAVE_BYTESWAP_H=0)
elseif(APPLE)
    add_compile_definitions (ZZIP_HAVE_BYTESWAP_H=0)
else()
    add_compile_definitions (ZZIP_HAVE_BYTESWAP_H=1)
endif()


# define header and source files for the library
file(GLOB HEADER_FILES "../../Include/FBFileSystem/*.h" "../../Include/FBFileSystem/*.hpp" "../../Include/FBFileSystem/*.inl")
file(GLOB SOURCE_FILES "../../Source/FBFileSystem/*.cpp" "../../Source/FBFileSystem/*.c")

set(FB_HEADER_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBFileSystem/../../Include/FBFileSystem")
set(FB_SOURCE_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBFileSystem/../../Source/FBFileSystem")

if (WIN32)
    file(GLOB_RECURSE HEADER_FILES_SUBFOLDER "../../Include/FBFileSystem/extern/nativefiledialog/*.h")
    file(GLOB_RECURSE SOURCE_FILES_SUBFOLDER "../../Source/FBFileSystem/extern/nativefiledialog/*.cpp" "../../Source/FBFileSystem/extern/nativefiledialog/*.c")
    list(APPEND HEADER_FILES ${HEADER_FILES_SUBFOLDER})
    list(APPEND SOURCE_FILES ${SOURCE_FILES_SUBFOLDER})
elseif (APPLE)
    file(GLOB_RECURSE HEADER_FILES_SUBFOLDER "../../Include/FBFileSystem/extern/nativefiledialog/*.h")
    file(GLOB_RECURSE SOURCE_FILES_SUBFOLDER "../../Source/FBFileSystem/extern/nativefiledialog/*.m" "../../Source/FBFileSystem/extern/nativefiledialog/*.c")
    list(APPEND HEADER_FILES ${HEADER_FILES_SUBFOLDER})
    list(APPEND SOURCE_FILES ${SOURCE_FILES_SUBFOLDER})
endif()

file(GLOB_RECURSE HEADER_FILES_SUBFOLDER "../../Include/FBFileSystem/extern/minizip/*.h")
file(GLOB_RECURSE SOURCE_FILES_SUBFOLDER "../../Source/FBFileSystem/extern/minizip/*.cpp" "../../Source/FBFileSystem/extern/minizip/*.c")
list(APPEND HEADER_FILES ${HEADER_FILES_SUBFOLDER})
list(APPEND SOURCE_FILES ${SOURCE_FILES_SUBFOLDER})

file(GLOB_RECURSE HEADER_FILES_SUBFOLDER "../../Include/FBFileSystem/extern/zlib/*.h")
file(GLOB_RECURSE SOURCE_FILES_SUBFOLDER "../../Source/FBFileSystem/extern/zlib/*.cpp" "../../Source/FBFileSystem/extern/zlib/*.c")
list(APPEND HEADER_FILES ${HEADER_FILES_SUBFOLDER})
list(APPEND SOURCE_FILES ${SOURCE_FILES_SUBFOLDER})

file(GLOB_RECURSE HEADER_FILES_SUBFOLDER "../../Include/FBFileSystem/extern/zzip/*.h")
file(GLOB_RECURSE SOURCE_FILES_SUBFOLDER "../../Source/FBFileSystem/extern/zzip/*.cpp" "../../Source/FBFileSystem/extern/zzip/*.c")
list(APPEND HEADER_FILES ${HEADER_FILES_SUBFOLDER})
list(APPEND SOURCE_FILES ${SOURCE_FILES_SUBFOLDER})

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

if (FB_BUILD_FILEWATCHER)
    include_directories( ${FB_ROOT}/AdditionalLibraries/simplefilewatcher/includes )
endif()

include_directories( ../../Include )
include_directories( ../../Include/extern )
include_directories( ${FB_ROOT}/FBEngine/cpp/Include/FBFileSystem/extern/nativefiledialog )

if (FB_USE_PRECOMPILED_HEADERS)
    use_precompiled_header(FBCore "FBFileSystem/FBFileSystemPCH.h" "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBFileSystem/FBFileSystemPCH.cpp")

    file(GLOB_RECURSE SOURCE_FILES_EXTERN "../../Source/FBFileSystem/extern/*.cpp" "../../Source/FBFileSystem/extern/*.c" )
    foreach(_source IN ITEMS ${SOURCE_FILES_EXTERN})
        set_source_files_properties(${_source}
            PPROPERTIES COMPILE_FLAGS /Y-
	    )
    endforeach()
endif()

if (FB_STATIC_LIB)
    add_library(FBFileSystem STATIC ${HEADER_FILES} ${SOURCE_FILES})
else ()
    add_library(FBFileSystem SHARED ${HEADER_FILES} ${SOURCE_FILES})
endif()

if (NOT FB_STATIC_LIB)
if (WIN32)
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/libs/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/${CMAKE_BUILD_TYPE})
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/physx/${CMAKE_BUILD_TYPE})
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tbb/${CMAKE_BUILD_TYPE})
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/boost/)
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tinybind/${CMAKE_BUILD_TYPE})
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/wildmagic/${CMAKE_BUILD_TYPE})
elseif (APPLE)
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/libs/macOS/${CMAKE_BUILD_TYPE})
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS)
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/boost)
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/tbb)
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS)
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/physx)
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/wildmagic)
elseif (UNIX)
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/libs/linux/${CMAKE_BUILD_TYPE})
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux)
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/boost)
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/tbb)
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux)
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/physx/${CMAKE_BUILD_TYPE})
    link_directories(CarPhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/wildmagic)
endif (WIN32)

if (WIN32)
    target_link_libraries(FBFileSystem FBCore.lib)

    if (FB_USE_ONETBB)
        target_link_libraries(FBFileSystem debug tbb12_debug.lib optimized tbb12.lib)
        target_link_libraries(FBFileSystem debug tbbmalloc_debug.lib optimized tbbmalloc.lib)
        target_link_libraries(FBFileSystem debug tbbmalloc_proxy_debug.lib optimized tbbmalloc_proxy.lib)
    elseif (FB_USE_TBB)
        target_link_libraries(FBFileSystem debug tbb_debug.lib optimized tbb.lib)
    endif()
elseif (APPLE)
    # fireblade libraries
    target_link_libraries(FBFileSystem libFBApplication.a)
    target_link_libraries(FBFileSystem libFBCore.a)
    target_link_libraries(FBFileSystem libFBFileSystem.a)
    target_link_libraries(FBFileSystem libFBMesh.a)
    target_link_libraries(FBFileSystem libFBProcedural.a)
    target_link_libraries(FBFileSystem libFBAssimp.a)

    if (FB_ENABLE_LUA)
        target_link_libraries(FBFileSystem FBLua.a)
        target_link_libraries(FBFileSystem FBLuaBind.a)
    elseif (FB_ENABLE_PYTHON)
        target_link_libraries(FBFileSystem FBPython.a)
        target_link_libraries(FBFileSystem FBPythonBind.a)
    endif()

    if (FB_BUILD_SDL2)
        target_link_libraries(FBFileSystem SDL2main.a)
        target_link_libraries(FBFileSystem SDL2_image.a)
        target_link_libraries(FBFileSystem debug SDL2d.a optimized SDL2.a)
    endif()

    if (FB_BUILD_IMGUI)
        target_link_libraries(FBFileSystem libFBImGui.a)
    endif()

    if (FB_BUILD_PHYSX)
        target_link_libraries(FBFileSystem libFBPhysx.a)
    elseif (FB_BUILD_ODE)
        target_link_libraries(FBFileSystem libFBODE3.a)
    endif()

    if (FB_BUILD_OISINPUT)
        target_link_libraries(FBFileSystem libFBOISInput.a)
        #target_link_libraries(FBFileSystem debug libOIS_d.a optimized libOIS.a)
    endif()

    target_link_libraries(FBFileSystem libFBVehicle.a)

    if (FB_BUILD_WXWIDGETS)
        #target_link_libraries(FBFileSystem libFBWxWidgets.a)
    endif()

    target_link_libraries(FBFileSystem libFBRenderUI.a)
    target_link_libraries(FBFileSystem libFBSQLite.a)
    target_link_libraries(FBFileSystem libFBCPlusPlus.a)
    target_link_libraries(FBFileSystem libFBGraphics.a)

    target_link_libraries(FBFileSystem libboost_thread.a)
    target_link_libraries(FBFileSystem libboost_date_time.a)
    target_link_libraries(FBFileSystem libboost_chrono.a)
    target_link_libraries(FBFileSystem libboost_timer.a)
    target_link_libraries(FBFileSystem libboost_filesystem.a)
    target_link_libraries(FBFileSystem libboost_json.a)

    target_link_libraries(FBFileSystem libwx_osx_cocoau_core-3.1.a)
    target_link_libraries(FBFileSystem libwx_baseu-3.1.a)
    target_link_libraries(FBFileSystem libwx_osx_cocoau_xrc-3.1.a)
    target_link_libraries(FBFileSystem libwx_osx_cocoau_propgrid-3.1.a)
    target_link_libraries(FBFileSystem libwx_osx_cocoau_richtext-3.1.a)
    target_link_libraries(FBFileSystem libwxexpat-3.1.a)
    target_link_libraries(FBFileSystem libwx_baseu_xml-3.1.a)
    target_link_libraries(FBFileSystem libwx_baseu_net-3.1.a)
    target_link_libraries(FBFileSystem libwx_osx_cocoau_webview-3.1.a)
    target_link_libraries(FBFileSystem libwx_osx_cocoau_xrc-3.1.a)
    target_link_libraries(FBFileSystem libwx_osx_cocoau_adv-3.1.a)
    target_link_libraries(FBFileSystem libwx_osx_cocoau_aui-3.1.a)
    target_link_libraries(FBFileSystem libwx_osx_cocoau_html-3.1.a)
    target_link_libraries(FBFileSystem libwxtiff-3.1.a)
    target_link_libraries(FBFileSystem libwxjpeg-3.1.a)
    target_link_libraries(FBFileSystem libwxpng-3.1.a)

    target_link_libraries(FBFileSystem libiconv.a)

    if (FB_BUILD_PHYSX)
        target_link_libraries(FBFileSystem libLowLevel.a libLowLevelCloth.a libPhysXProfileSDK.a libPhysX3Common.a libSceneQuery.a libSimulationController.a libPhysX3Cooking.a libPhysX3Extensions.a libPhysX3.a libPhysX3Vehicle.a)

        target_link_libraries(FBFileSystem libPxTask.a)
        target_link_libraries(FBFileSystem libPhysXVisualDebuggerSDK.a)
        target_link_libraries(FBFileSystem libPvdRuntime.a)
    endif()

    target_link_libraries(FBFileSystem libtbb.a)
    target_link_libraries(FBFileSystem libtbbmalloc.a)

    target_link_libraries(FBFileSystem libWm5Core.a)
    target_link_libraries(FBFileSystem libWm5Mathematics.a)

    if (FB_USE_ASSET_IMPORT)
        #target_link_libraries(FBFileSystem debug libassimpd.a optimized libassimp.a)
    endif()

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        target_link_libraries(FBFileSystem FBGraphicsOgreNext.a)
        
        if (FB_USE_FREETYPE)
            target_link_libraries(FBFileSystem freetype.a)
        endif()
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        target_link_libraries(FBFileSystem FBGraphicsOgre.a)

        if (FB_USE_FREETYPE)
            target_link_libraries(FBFileSystem freetype.a)
        endif()
    endif()

    #target_link_libraries(OgreOverlayTest stdc++ "-framework CoreGraphics" objc)
    #target_link_libraries(OgreOverlayTest ${AUDIO_UNIT})
    target_link_libraries(FBFileSystem stdc++ "-framework AppKit" "-framework Metal" "-framework MetalKit" "-framework OpenGL" "-framework Foundation" "-framework QuartzCore" "-framework CoreGraphics" "-framework CoreServices" "-framework Cocoa" "-framework Carbon" "-framework IOKit" "-framework GameController" objc)
    set_target_properties(FBFileSystem PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
elseif (UNIX)
    target_link_libraries(FBFileSystem -Wl,--start-group FBCore.a FBAero.a libFBProcedural.a FBImGui.a
        FBFileSystem.a FBMath.a FBMesh.a FBOISInput.a FBVehicle.a FBData.a FBState.a libFBPhysx.a libFBSQLite.a
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
        target_link_libraries(FBFileSystem FBGraphicsOgreNext.a)
        target_link_libraries(FBFileSystem freetype.a)
    elseif(FB_GRAPHICS_SYSTEM_SOFTWARE)
        target_link_libraries(FBFileSystem libFBGraphicsSoftware.a)
    endif()

    target_link_libraries(FBFileSystem png)
    target_link_libraries(FBFileSystem Xaw)
    target_link_libraries(FBFileSystem Xrandr)
    target_link_libraries(FBFileSystem Xt)
    target_link_libraries(FBFileSystem xcb)
    target_link_libraries(FBFileSystem GL)
    target_link_libraries(FBFileSystem ${X11_LIBRARIES})
    target_link_libraries(FBFileSystem ${CMAKE_THREAD_LIBS_INIT} )
    target_link_libraries(FBFileSystem ${CMAKE_DL_LIBS})
endif ()
endif()

set_target_properties (FBFileSystem PROPERTIES FOLDER Engine/Cpp/FileSystem)
