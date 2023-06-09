set(CMAKE_USE_RELATIVE_PATHS true)
set(CMAKE_SUPPRESS_REGENERATION true)

project(SampleApplication)

if (UNIX)
    if (NOT APPLE)
        find_package(X11 REQUIRED)
    endif()
endif()

add_compile_definitions (_FB_STATIC_LIB_)
add_compile_definitions (_UNICODE)
add_compile_definitions (wxUSE_UNICODE=1)
#ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

include_directories( ${FB_ROOT}/Dependencies/boost)

if (FB_USE_ONETBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/onetbb/include )
elseif (FB_USE_TBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tbb/include )
endif()

include_directories( ${FB_ROOT}/AdditionalLibraries/cppsqlite/include)
include_directories( ${FB_ROOT}/AdditionalLibraries/sqlite/include)
include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets  )
include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets/include )
include_directories( ${FB_ROOT}/FBEngine/cpp/Include )
include_directories(Source)

if (WIN32)
    include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets/include/msvc )
    include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets/include/msvc/wx )
    include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets/lib/vc_x64_lib/mswud  )
elseif (APPLE)
    include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets/include/gtk3-unicode-3.1 )
    include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets/include/gtk3-unicode-3.1/wx )
    include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets/lib/osx/wx/include/osx_cocoa-unicode-static-3.1/wx  )
elseif (UNIX)
    include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets/include/osx_cocoa-unicode-static-3.1 )
    include_directories( ${FB_ROOT}/AdditionalLibraries/wxWidgets/include/osx_cocoa-unicode-static-3.1/wx )
endif ()

if(WIN32)
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/win32 )
elseif(APPLE)
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/osx )
else()
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/linux )
endif()

include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/OgreMain/include )

if (WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Bin/windows/${FB_TOOLSET}/${FB_ARCH}/)
elseif (APPLE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Bin/macOS/${CMAKE_BUILD_TYPE})
elseif (UNIX)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Bin/linux/${CMAKE_BUILD_TYPE})
endif (WIN32)

if (WIN32)
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/libs/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/${CMAKE_BUILD_TYPE})
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/physx/)
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tbb/)
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/boost/)
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tinybind/)
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/wildmagic/)
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/assimp/${CMAKE_BUILD_TYPE})
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ois/)
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/sdl2/${CMAKE_BUILD_TYPE})

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogrenext/)
        link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogrenext/dependencies/lib/)
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogre/)
        link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogre/dependencies/lib/)
        
        if (FB_PLATFORM_X64)
            link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x64)
        else ()
            link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x86)
        endif()
    endif ()
elseif (APPLE)
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/libs/macOS/${CMAKE_BUILD_TYPE})
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS)
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/boost)
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/boost/${CMAKE_BUILD_TYPE})
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS)
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/physx/${CMAKE_BUILD_TYPE})
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/wildmagic/${CMAKE_BUILD_TYPE})
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/tbb/${CMAKE_BUILD_TYPE})

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogrenext/${CMAKE_BUILD_TYPE})
        link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogrenext/dependencies/lib/${CMAKE_BUILD_TYPE})
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogre/)
        link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogre/dependencies/lib/)
        link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x64)
    endif ()
elseif (UNIX)
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/libs/linux/${CMAKE_BUILD_TYPE})
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux)
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/boost)
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/tbb)
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux)
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/physx)
    link_directories(SampleApplication ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/wildmagic)
endif ()

file(GLOB_RECURSE HEADER_FILES "./*.h")
file(GLOB_RECURSE SOURCE_FILES "./*.c" "./*.cpp")

add_executable(SampleApplication ${HEADER_FILES} ${SOURCE_FILES})

if (WIN32)
    target_link_libraries(SampleApplication FBCore.lib)
    target_link_libraries(SampleApplication FBFileSystem.lib)
    target_link_libraries(SampleApplication FBMesh.lib)
    target_link_libraries(SampleApplication FBProcedural.lib)
    target_link_libraries(SampleApplication FBPhysx.lib)
    target_link_libraries(SampleApplication FBSQLite.lib)
    target_link_libraries(SampleApplication FBVehicle.lib)
    target_link_libraries(SampleApplication FBWxWidgets.lib)
    target_link_libraries(SampleApplication FBApplication.lib)
    target_link_libraries(SampleApplication FBGraphics.lib)

    if (FB_BUILD_OISINPUT)
        target_link_libraries(SampleApplication FBOISInput.lib)
        #target_link_libraries(SampleApplication debug OIS_d.lib optimized OIS.lib)
        target_link_libraries(SampleApplication dinput8.lib)
        target_link_libraries(SampleApplication dxguid.lib)
    endif ()

    if (FB_USE_FBXSDK)
        #target_link_libraries(SampleApplication libfbxsdk.lib)
        target_link_libraries(SampleApplication libfbxsdk-mt.lib)
        target_link_libraries(SampleApplication libxml2-mt.lib)
        target_link_libraries(SampleApplication tinyxml2.lib)
    endif()

    if (FB_BUILD_SDL2)
        target_link_libraries(SampleApplication debug SDL2d.lib optimized SDL2.lib)
    endif()

    if (FB_BUILD_IMGUI)        
        target_link_libraries(SampleApplication FBImGui.lib)
    endif()

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        target_link_libraries(SampleApplication FBGraphicsOgreNext.lib)
        target_link_libraries(SampleApplication debug FreeImage_d.lib optimized FreeImage.lib)
        target_link_libraries(SampleApplication debug freetype_d.lib optimized freetype.lib)

        if (FB_BUILD_RENDERER_DX11)
            target_link_libraries(SampleApplication d3d11.lib)
            target_link_libraries(SampleApplication dxgi.lib)
            target_link_libraries(SampleApplication dxguid.lib)
        endif()

        if (FB_BUILD_RENDERER_GL3PLUS)
            target_link_libraries(SampleApplication opengl32.lib)
            target_link_libraries(SampleApplication glu32.lib)
        endif ()

        if (FB_PLATFORM_X64)
            target_link_libraries(SampleApplication amd_ags_x64.lib)
        endif()
    elseif(FB_GRAPHICS_SYSTEM_OGRE)
        target_link_libraries(SampleApplication FBGraphicsOgre.lib)

        if (FB_GRAPHICS_SYSTEM_USE_STATIC_LIBS)
            target_link_libraries(SampleApplication debug OgreMainStatic_d.lib optimized OgreMainStatic.lib)
            target_link_libraries(SampleApplication debug OgreOverlayStatic_d.lib optimized OgreOverlayStatic.lib)

            target_link_libraries(SampleApplication debug RenderSystem_Direct3D9Static_d.lib
                                optimized RenderSystem_Direct3D9Static.lib)

            target_link_libraries(SampleApplication debug RenderSystem_Direct3D11Static_d.lib
                                optimized RenderSystem_Direct3D11Static.lib)

            target_link_libraries(SampleApplication debug RenderSystem_GL3PlusStatic_d.lib 
                                optimized RenderSystem_GL3PlusStatic.lib)

            target_link_libraries(SampleApplication debug RenderSystem_GLStatic_d.lib
                                optimized RenderSystem_GLStatic.lib)

            target_link_libraries(SampleApplication debug OgreGLSupportStatic_d.lib
                                optimized OgreGLSupportStatic.lib)

            target_link_libraries(SampleApplication debug OgreTerrainStatic_d.lib optimized OgreTerrainStatic.lib)

            target_link_libraries(SampleApplication debug OgreRTShaderSystemStatic_d.lib
                                       optimized OgreRTShaderSystemStatic.lib)

            target_link_libraries(SampleApplication debug Codec_STBIStatic_d.lib
                                       optimized Codec_STBIStatic.lib)

            target_link_libraries(SampleApplication debug OgreBitesStatic_d.lib
                                       optimized OgreBitesStatic.lib)
        endif()

        if (FB_BUILD_RENDERER_DX9)
            target_link_libraries(SampleApplication d3d9.lib)
            target_link_libraries(SampleApplication d3dx9.lib)
        endif ()

        if (FB_BUILD_RENDERER_DX11)
            target_link_libraries(SampleApplication d3d11.lib)
            target_link_libraries(SampleApplication dxgi.lib)
            target_link_libraries(SampleApplication dxguid.lib)
        endif()

        if (FB_BUILD_RENDERER_OPENGL)
            target_link_libraries(SampleApplication opengl32.lib)
            target_link_libraries(SampleApplication glu32.lib)
            
            if (FB_PLATFORM_X64)
                target_link_libraries(SampleApplication amd_ags_x64.lib)
            endif ()
        endif ()

        if (FB_BUILD_RENDERER_GL3PLUS)
            target_link_libraries(SampleApplication opengl32.lib)
            target_link_libraries(SampleApplication glu32.lib)

            if (FB_PLATFORM_X64)
                target_link_libraries(SampleApplication amd_ags_x64.lib)
            endif ()
        endif ()
    elseif(FB_BUILD_DX11)
        target_link_libraries(SampleApplication FBGraphicsD3D11.lib)
    endif()

    if (FB_BUILD_PHYSX)
        target_link_libraries(SampleApplication FBPhysx.lib)

        if (FB_PLATFORM_X64)
            target_link_libraries(SampleApplication 
                        debug PhysX3DEBUG_x64.lib
                        optimized PhysX3_x64.lib)
            target_link_libraries(SampleApplication 
                        debug PhysX3ExtensionsDEBUG.lib
                        optimized PhysX3Extensions.lib)
            target_link_libraries(SampleApplication 
                        debug PhysX3CommonDEBUG_x64.lib 
                        optimized PhysX3Common_x64.lib)
            target_link_libraries(SampleApplication 
                        debug PhysX3VehicleDEBUG.lib 
                        optimized PhysX3Vehicle.lib)
            target_link_libraries(SampleApplication 
                        debug PhysX3CookingDEBUG_x64.lib
                        optimized PhysX3Cooking_x64.lib)
            target_link_libraries(SampleApplication 
                        debug PhysXProfileSDKDEBUG.lib
                        optimized PhysXProfileSDK.lib)
            target_link_libraries(SampleApplication 
                        debug PxTaskDEBUG.lib
                        optimized PxTask.lib)
            target_link_libraries(SampleApplication 
                        debug PhysXVisualDebuggerSDKDEBUG.lib
                        optimized PhysXVisualDebuggerSDK.lib)
        else ()
            target_link_libraries(SampleApplication 
                        debug PhysX3DEBUG_x86.lib
                        optimized PhysX3_x86.lib)
            target_link_libraries(SampleApplication 
                        debug PhysX3ExtensionsDEBUG.lib
                        optimized PhysX3Extensions.lib)
            target_link_libraries(SampleApplication 
                        debug PhysX3CommonDEBUG_x86.lib 
                        optimized PhysX3Common_x86.lib)
            target_link_libraries(SampleApplication 
                        debug PhysX3VehicleDEBUG.lib 
                        optimized PhysX3Vehicle.lib)
            target_link_libraries(SampleApplication 
                        debug PhysX3CookingDEBUG_x86.lib
                        optimized PhysX3Cooking_x86.lib)
            target_link_libraries(SampleApplication 
                        debug PhysXProfileSDKDEBUG.lib
                        optimized PhysXProfileSDK.lib)
            target_link_libraries(SampleApplication 
                        debug PxTaskDEBUG.lib
                        optimized PxTask.lib)
            target_link_libraries(SampleApplication 
                        debug PhysXVisualDebuggerSDKDEBUG.lib
                        optimized PhysXVisualDebuggerSDK.lib)
        endif()
    endif()
elseif (APPLE)
    # fireblade libraries
    target_link_libraries(SampleApplication libFBCore.a)
    target_link_libraries(SampleApplication libFBFileSystem.a)
    target_link_libraries(SampleApplication libFBGraphics.a)
    target_link_libraries(SampleApplication libFBMesh.a)
    target_link_libraries(SampleApplication libFBProcedural.a)
    target_link_libraries(SampleApplication libFBPhysx.a)
    target_link_libraries(SampleApplication libFBVehicle.a)
    target_link_libraries(SampleApplication libFBApplication.a)
    target_link_libraries(SampleApplication libFBSQLite.a)

    target_link_libraries(SampleApplication libFBAssimp.a)

    if (FB_BUILD_OISINPUT)
        target_link_libraries(SampleApplication libFBOISInput.a)
    endif()

    if (WIN32)
        target_link_libraries(SampleApplication libboost_thread-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleApplication libboost_date_time-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleApplication libboost_chrono-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleApplication libboost_timer-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleApplication libboost_filesystem-gcc5-mt-s-x64-1_70.a)
    elseif (APPLE)
        target_link_libraries(SampleApplication libboost_thread.a)
        target_link_libraries(SampleApplication libboost_date_time.a)
        target_link_libraries(SampleApplication libboost_chrono.a)
        target_link_libraries(SampleApplication libboost_timer.a)
        target_link_libraries(SampleApplication libboost_filesystem.a)
    elseif (UNIX)
        target_link_libraries(SampleApplication libboost_thread-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleApplication libboost_date_time-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleApplication libboost_chrono-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleApplication libboost_timer-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleApplication libboost_filesystem-gcc5-mt-s-x64-1_70.a)
    endif ()

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        target_link_libraries(SampleApplication FBGraphicsOgreNext.a)
        target_link_libraries(SampleApplication FreeImage.a)
        target_link_libraries(SampleApplication freetype.a)
    elseif(FB_GRAPHICS_SYSTEM_OGRE)
        target_link_libraries(SampleApplication FBGraphicsOgre.a)
    elseif(FB_GRAPHICS_SYSTEM_SOFTWARE)
        target_link_libraries(SampleApplication libFBGraphicsSoftware.a)
    endif()

    if (FB_BUILD_IMGUI)
        target_link_libraries(SampleApplication FBImGui.a)
    endif()

    # physx libraries
    if (WIN32)
    elseif (APPLE)
        target_link_libraries(SampleApplication libLowLevel.a libLowLevelCloth.a libPhysXProfileSDK.a libPhysX3Common.a libSceneQuery.a libSimulationController.a libPhysX3Cooking.a libPhysX3Extensions.a libPhysX3.a libPhysX3Vehicle.a)
    elseif (UNIX)
        target_link_libraries(SampleApplication -Wl,--start-group libLowLevel.a libLowLevelCloth.a libPhysXProfileSDK.a libPhysX3Common.a libSceneQuery.a libSimulationController.a libPhysX3Cooking.a libPhysX3Extensions.a libPhysX3.a libPhysX3Vehicle.a -Wl,--end-group )
    endif (WIN32)

    target_link_libraries(SampleApplication libPxTask.a)
    target_link_libraries(SampleApplication libPhysXVisualDebuggerSDK.a)
    target_link_libraries(SampleApplication libPvdRuntime.a)

    target_link_libraries(SampleApplication libtbb.a)
    target_link_libraries(SampleApplication libtbbmalloc.a)

    if (FB_BUILD_WILDMAGIC5)
        target_link_libraries(SampleApplication libWm5Core.a)
        target_link_libraries(SampleApplication libWm5Mathematics.a)
    endif ()

    #target_link_libraries(SampleApplication stdc++ "-framework CoreGraphics" objc)
    #target_link_libraries(SampleApplication ${AUDIO_UNIT})
    target_link_libraries(SampleApplication stdc++ "-framework Metal" "-framework OpenGL" "-framework Foundation" "-framework QuartzCore" "-framework CoreGraphics" "-framework CoreServices" "-framework Cocoa" "-framework Carbon" "-framework IOKit" "-framework GameController" objc)
    set_target_properties(SampleApplication PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
else ()
    target_link_libraries(SampleApplication -Wl,--start-group FBCore.a FBAero.a FBGraphics.a FBGraphicsOgreNext.a
        FBApplication.a FBFileSystem.a FBMath.a FBState.a FBVehicle.a FBData.a FBImGui.a 
        libFBObjectTemplates.a libFBVehicle.a FBProcedural.a FBMesh.a       
        libboost_thread-gcc5-mt-s-x64-1_70.a
        libboost_date_time-gcc5-mt-s-x64-1_70.a
        libboost_chrono-gcc5-mt-s-x64-1_70.a
        libboost_timer-gcc5-mt-s-x64-1_70.a
        libboost_filesystem-gcc5-mt-s-x64-1_70.a
        freetype.a
        debug libtbb_debug.a optimized libtbb.a
        debug libtbbmalloc_debug.a optimized libtbbmalloc.a
        libWm5Core.a libWm5Mathematics.a
        -Wl,--end-group )

    target_link_libraries(SampleApplication png)
    target_link_libraries(SampleApplication Xaw)
    target_link_libraries(SampleApplication Xrandr)
    target_link_libraries(SampleApplication Xt)
    target_link_libraries(SampleApplication xcb)
    target_link_libraries(SampleApplication GL)
    target_link_libraries(SampleApplication ${X11_LIBRARIES})
    target_link_libraries(SampleApplication ${CMAKE_THREAD_LIBS_INIT} )
    target_link_libraries(SampleApplication ${CMAKE_DL_LIBS})
endif ()






if (WIN32)
    set_property(TARGET SampleApplication PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${FB_ROOT}/Bin/windows/${FB_TOOLSET}/${FB_ARCH}/Debug)
endif()

add_dependencies(SampleApplication FBAero FBAi FBAudio FBApplication FBCore FBFileSystem FBMesh FBProcedural FBSQLite FBVehicle)

if (FB_GRAPHICS_SYSTEM_OGRENEXT) 
    add_dependencies(SampleApplication FBGraphicsOgreNext)
elseif (FB_GRAPHICS_SYSTEM_OGRE)
    add_dependencies(SampleApplication FBGraphicsOgre)
endif()

if (FB_BUILD_ODE)
    add_dependencies(SampleApplication FBODE3)
elseif (FB_BUILD_PHYSX)
    add_dependencies(SampleApplication FBPhysx)
endif()

set_target_properties (SampleApplication PROPERTIES FOLDER Samples)
