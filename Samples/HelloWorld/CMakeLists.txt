set(CMAKE_USE_RELATIVE_PATHS true)
set(CMAKE_SUPPRESS_REGENERATION true)

project(SampleHelloWorld)

if (UNIX)
    if (NOT APPLE)
        find_package(X11 REQUIRED)
    endif()
endif()

add_compile_definitions (_FB_STATIC_LIB_)
#ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

include_directories( ${FB_ROOT}/Dependencies/boost)

if (FB_USE_ONETBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/onetbb/include )
elseif (FB_USE_TBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tbb/include )
endif()

include_directories( ${FB_ROOT}/AdditionalLibraries/cppsqlite/include)
include_directories( ${FB_ROOT}/AdditionalLibraries/sqlite/include)
include_directories( ${FB_ROOT}/FBEngine/cpp/Include )
include_directories(Source)


if (UNIX)
    if (NOT APPLE)
        include_directories(${X11_INCLUDE_DIR})
    endif()
endif()


if (WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Bin/windows/${FB_TOOLSET}/${FB_ARCH}/)
elseif (APPLE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Bin/macOS/${CMAKE_BUILD_TYPE})
elseif (UNIX)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Bin/linux/${CMAKE_BUILD_TYPE})
endif ()

if (WIN32)
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/libs/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/${CMAKE_BUILD_TYPE})
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/physx/)
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogre/)    
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogre/Dependencies/lib/)
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tbb/)
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/boost/)
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tinybind/)
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/wildmagic/)
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/assimp/${CMAKE_BUILD_TYPE})
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ois/)

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogrenext/)
        link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogrenext/dependencies/lib/)
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogre/)
        link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogre/dependencies/lib/)
        
        if (FB_PLATFORM_X64)
            link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x64)
        else ()
            link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x86)
        endif()
    endif ()
elseif (APPLE)
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/libs/macOS/${CMAKE_BUILD_TYPE})
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS)
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/boost)
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/tbb/${CMAKE_BUILD_TYPE})
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS)
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/${CMAKE_BUILD_TYPE})
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/physx/${CMAKE_BUILD_TYPE})
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/wildmagic/${CMAKE_BUILD_TYPE})

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogrenext/${CMAKE_BUILD_TYPE})
        link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogrenext/dependencies/lib/${CMAKE_BUILD_TYPE})
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogre/)
        link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogre/dependencies/lib/)
        
        if (FB_PLATFORM_X64)
            link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x64)
        else ()
            link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x86)
        endif()
    endif ()
elseif (UNIX)
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/libs/linux/${CMAKE_BUILD_TYPE})
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux)
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/boost)
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/tbb)
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux)
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/physx)
    link_directories(SampleHelloWorld ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/wildmagic)
endif ()

file(GLOB_RECURSE HEADER_FILES "./*.h")
file(GLOB_RECURSE SOURCE_FILES "./*.c" "./*.cpp")

add_executable(SampleHelloWorld ${HEADER_FILES} ${SOURCE_FILES})

if (WIN32)
    target_link_libraries(SampleHelloWorld FBCore.lib)
    target_link_libraries(SampleHelloWorld FBFileSystem.lib)
    target_link_libraries(SampleHelloWorld FBMesh.lib)
    target_link_libraries(SampleHelloWorld FBProcedural.lib)
    target_link_libraries(SampleHelloWorld FBPhysx.lib)
    target_link_libraries(SampleHelloWorld FBSQLite.lib)
    target_link_libraries(SampleHelloWorld FBVehicle.lib)
    target_link_libraries(SampleHelloWorld FBApplication.lib)
    target_link_libraries(SampleHelloWorld FBOISInput.lib)
    target_link_libraries(SampleHelloWorld FBGraphics.lib)
    target_link_libraries(SampleHelloWorld FBRenderUI.lib)

    if (FB_BUILD_OISINPUT)
        target_link_libraries(SampleHelloWorld FBOISInput.lib)

        #target_link_libraries(SampleHelloWorld debug OIS_d.lib optimized OIS.lib)
        target_link_libraries(SampleHelloWorld dinput8.lib)
        target_link_libraries(SampleHelloWorld dxguid.lib)
    endif()

    if (FB_USE_FBXSDK)
        #target_link_libraries(SampleHelloWorld libfbxsdk.lib)
        target_link_libraries(SampleHelloWorld libfbxsdk-mt.lib)
        target_link_libraries(SampleHelloWorld libxml2-mt.lib)
        target_link_libraries(SampleHelloWorld tinyxml2.lib)
    endif()

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        target_link_libraries(SampleHelloWorld FBGraphicsOgreNext.lib)
        target_link_libraries(SampleHelloWorld debug FreeImage_d.lib optimized FreeImage.lib)
        target_link_libraries(SampleHelloWorld debug freetype_d.lib optimized freetype.lib)

        if (FB_BUILD_RENDERER_DX11)
            target_link_libraries(SampleHelloWorld d3d11.lib)
            target_link_libraries(SampleHelloWorld dxgi.lib)
            target_link_libraries(SampleHelloWorld dxguid.lib)
        endif()

        if (FB_BUILD_RENDERER_GL3PLUS)
            target_link_libraries(SampleHelloWorld opengl32.lib)
            target_link_libraries(SampleHelloWorld glu32.lib)
        endif ()

        if (FB_PLATFORM_X64)
            target_link_libraries(SampleHelloWorld amd_ags_x64.lib)
        endif()
    elseif(FB_GRAPHICS_SYSTEM_OGRE)
        target_link_libraries(SampleHelloWorld FBGraphicsOgre.lib)

        if (FB_GRAPHICS_SYSTEM_USE_STATIC_LIBS)
            target_link_libraries(SampleHelloWorld debug OgreMainStatic_d.lib optimized OgreMainStatic.lib)
            target_link_libraries(SampleHelloWorld debug OgreOverlayStatic_d.lib optimized OgreOverlayStatic.lib)

            target_link_libraries(SampleHelloWorld debug RenderSystem_Direct3D9Static_d.lib
                                optimized RenderSystem_Direct3D9Static.lib)

            target_link_libraries(SampleHelloWorld debug RenderSystem_Direct3D11Static_d.lib
                                optimized RenderSystem_Direct3D11Static.lib)

            target_link_libraries(SampleHelloWorld debug RenderSystem_GL3PlusStatic_d.lib 
                                optimized RenderSystem_GL3PlusStatic.lib)

            target_link_libraries(SampleHelloWorld debug RenderSystem_GLStatic_d.lib
                                optimized RenderSystem_GLStatic.lib)

            target_link_libraries(SampleHelloWorld debug OgreGLSupportStatic_d.lib
                                optimized OgreGLSupportStatic.lib)

            target_link_libraries(SampleHelloWorld debug OgreTerrainStatic_d.lib optimized OgreTerrainStatic.lib)

            target_link_libraries(SampleHelloWorld debug OgreRTShaderSystemStatic_d.lib
                                       optimized OgreRTShaderSystemStatic.lib)

            target_link_libraries(SampleHelloWorld debug Codec_STBIStatic_d.lib
                                       optimized Codec_STBIStatic.lib)

            target_link_libraries(SampleHelloWorld debug OgreBitesStatic_d.lib
                                       optimized OgreBitesStatic.lib)
        endif()

        if (FB_BUILD_RENDERER_DX9)
            target_link_libraries(SampleHelloWorld d3d9.lib)
            target_link_libraries(SampleHelloWorld d3dx9.lib)
        endif ()

        if (FB_BUILD_RENDERER_DX11)
            target_link_libraries(SampleHelloWorld d3d11.lib)
            target_link_libraries(SampleHelloWorld dxgi.lib)
            target_link_libraries(SampleHelloWorld dxguid.lib)
        endif()

        if (FB_BUILD_RENDERER_OPENGL)
            target_link_libraries(SampleHelloWorld opengl32.lib)
            target_link_libraries(SampleHelloWorld glu32.lib)
            
            if (FB_PLATFORM_X64)
                target_link_libraries(SampleHelloWorld amd_ags_x64.lib)
            endif ()
        endif ()

        if (FB_BUILD_RENDERER_GL3PLUS)
            target_link_libraries(SampleHelloWorld opengl32.lib)
            target_link_libraries(SampleHelloWorld glu32.lib)

            if (FB_PLATFORM_X64)
                target_link_libraries(SampleHelloWorld amd_ags_x64.lib)
            endif ()
        endif ()

        if (FB_USE_FREETYPE)
            target_link_libraries(SampleHelloWorld freetype.lib)
        endif ()
    elseif(FB_BUILD_DX11)
        target_link_libraries(SampleHelloWorld FBGraphicsD3D11.lib)
    endif()

    if (FB_BUILD_PHYSX)
        target_link_libraries(SampleHelloWorld FBPhysx.lib)

        if (FB_PLATFORM_X64)
            target_link_libraries(SampleHelloWorld 
                        debug PhysX3DEBUG_x64.lib
                        optimized PhysX3_x64.lib)
            target_link_libraries(SampleHelloWorld 
                        debug PhysX3ExtensionsDEBUG.lib
                        optimized PhysX3Extensions.lib)
            target_link_libraries(SampleHelloWorld 
                        debug PhysX3CommonDEBUG_x64.lib 
                        optimized PhysX3Common_x64.lib)
            target_link_libraries(SampleHelloWorld 
                        debug PhysX3VehicleDEBUG.lib 
                        optimized PhysX3Vehicle.lib)
            target_link_libraries(SampleHelloWorld 
                        debug PhysX3CookingDEBUG_x64.lib
                        optimized PhysX3Cooking_x64.lib)
            target_link_libraries(SampleHelloWorld 
                        debug PhysXProfileSDKDEBUG.lib
                        optimized PhysXProfileSDK.lib)
            target_link_libraries(SampleHelloWorld 
                        debug PxTaskDEBUG.lib
                        optimized PxTask.lib)
            target_link_libraries(SampleHelloWorld 
                        debug PhysXVisualDebuggerSDKDEBUG.lib
                        optimized PhysXVisualDebuggerSDK.lib)
        else ()
            target_link_libraries(SampleHelloWorld 
                        debug PhysX3DEBUG_x86.lib
                        optimized PhysX3_x86.lib)
            target_link_libraries(SampleHelloWorld 
                        debug PhysX3ExtensionsDEBUG.lib
                        optimized PhysX3Extensions.lib)
            target_link_libraries(SampleHelloWorld 
                        debug PhysX3CommonDEBUG_x86.lib 
                        optimized PhysX3Common_x86.lib)
            target_link_libraries(SampleHelloWorld 
                        debug PhysX3VehicleDEBUG.lib 
                        optimized PhysX3Vehicle.lib)
            target_link_libraries(SampleHelloWorld 
                        debug PhysX3CookingDEBUG_x86.lib
                        optimized PhysX3Cooking_x86.lib)
            target_link_libraries(SampleHelloWorld 
                        debug PhysXProfileSDKDEBUG.lib
                        optimized PhysXProfileSDK.lib)
            target_link_libraries(SampleHelloWorld 
                        debug PxTaskDEBUG.lib
                        optimized PxTask.lib)
            target_link_libraries(SampleHelloWorld 
                        debug PhysXVisualDebuggerSDKDEBUG.lib
                        optimized PhysXVisualDebuggerSDK.lib)
        endif()
    endif()
elseif (APPLE)
    # fireblade libraries
    target_link_libraries(SampleHelloWorld libFBCore.a)
    target_link_libraries(SampleHelloWorld libFBFileSystem.a)
    target_link_libraries(SampleHelloWorld libFBGraphics.a)
    target_link_libraries(SampleHelloWorld libFBMesh.a)
    target_link_libraries(SampleHelloWorld libFBProcedural.a)
    target_link_libraries(SampleHelloWorld libFBPhysx.a)
    target_link_libraries(SampleHelloWorld libFBVehicle.a)
    target_link_libraries(SampleHelloWorld libFBApplication.a)
    target_link_libraries(SampleHelloWorld FBRenderUI.a)

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        target_link_libraries(SampleHelloWorld FBGraphicsOgreNext.a)

        if (FB_USE_FREETYPE)
            target_link_libraries(SampleHelloWorld freetype.a)
        endif()
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        target_link_libraries(SampleHelloWorld FBGraphicsOgre.a)

        if (FB_USE_FREETYPE)
            target_link_libraries(SampleHelloWorld libfreetype.a)
        endif()
    elseif(FB_GRAPHICS_SYSTEM_SOFTWARE)
        target_link_libraries(SampleHelloWorld libFBGraphicsSoftware.a)
    endif()

    target_link_libraries(SampleHelloWorld libFBSQLite.a)

    if (WIN32)
        target_link_libraries(SampleHelloWorld libboost_thread-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleHelloWorld libboost_date_time-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleHelloWorld libboost_chrono-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleHelloWorld libboost_timer-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleHelloWorld libboost_filesystem-gcc5-mt-s-x64-1_70.a)
    elseif (APPLE)
        target_link_libraries(SampleHelloWorld libboost_thread.a)
        target_link_libraries(SampleHelloWorld libboost_date_time.a)
        target_link_libraries(SampleHelloWorld libboost_chrono.a)
        target_link_libraries(SampleHelloWorld libboost_timer.a)
        target_link_libraries(SampleHelloWorld libboost_filesystem.a)
    elseif (UNIX)
        target_link_libraries(SampleHelloWorld libboost_thread-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleHelloWorld libboost_date_time-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleHelloWorld libboost_chrono-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleHelloWorld libboost_timer-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleHelloWorld libboost_filesystem-gcc5-mt-s-x64-1_70.a)
    endif ()

    # physx libraries
    if (WIN32)
    elseif (APPLE)
        target_link_libraries(SampleHelloWorld libLowLevel.a libLowLevelCloth.a libPhysXProfileSDK.a libPhysX3Common.a libSceneQuery.a libSimulationController.a libPhysX3Cooking.a libPhysX3Extensions.a libPhysX3.a libPhysX3Vehicle.a)
    elseif (UNIX)
        target_link_libraries(SampleHelloWorld -Wl,--start-group libLowLevel.a libLowLevelCloth.a libPhysXProfileSDK.a libPhysX3Common.a libSceneQuery.a libSimulationController.a libPhysX3Cooking.a libPhysX3Extensions.a libPhysX3.a libPhysX3Vehicle.a -Wl,--end-group )
    endif (WIN32)

    target_link_libraries(SampleHelloWorld libPxTask.a)
    target_link_libraries(SampleHelloWorld libPhysXVisualDebuggerSDK.a)
    target_link_libraries(SampleHelloWorld libPvdRuntime.a)

    target_link_libraries(SampleHelloWorld libtbb.a)
    target_link_libraries(SampleHelloWorld libtbbmalloc.a)

    if (FB_BUILD_WILDMAGIC5)
        target_link_libraries(SampleHelloWorld libWm5Core.a)
        target_link_libraries(SampleHelloWorld libWm5Mathematics.a)
    endif ()

    target_link_libraries(SampleHelloWorld stdc++ "-framework Metal" "-framework OpenGL" "-framework Foundation" "-framework QuartzCore" "-framework CoreGraphics" "-framework CoreServices" "-framework Cocoa" "-framework Carbon" "-framework IOKit" objc)
    set_target_properties(SampleHelloWorld PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
else ()
    target_link_libraries(SampleHelloWorld -Wl,--start-group FBCore.a FBAero.a 
        FBApplication.a FBFileSystem.a FBMath.a FBMesh.a FBVehicle.a FBData.a FBState.a 
        libFBObjectTemplates.a libFBVehicle.a FBGraphics.a
        FBGraphicsOgreNext.a ${X11_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS}
        png Xaw Xrandr Xt xcb
        libboost_thread-gcc5-mt-s-x64-1_70.a
        libboost_date_time-gcc5-mt-s-x64-1_70.a
        libboost_chrono-gcc5-mt-s-x64-1_70.a
        libboost_timer-gcc5-mt-s-x64-1_70.a
        libboost_filesystem-gcc5-mt-s-x64-1_70.a
        debug libtbb_debug.a optimized libtbb.a
        debug libtbbmalloc_debug.a optimized libtbbmalloc.a
        libWm5Core.a libWm5Mathematics.a
        -Wl,--end-group )

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        #target_link_libraries(SampleHelloWorld FBGraphicsOgreNext.a)
        target_link_libraries(SampleHelloWorld freetype.a)
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        target_link_libraries(SampleHelloWorld FBGraphicsOgre.a)
    elseif(FB_GRAPHICS_SYSTEM_SOFTWARE)
        target_link_libraries(SampleHelloWorld libFBGraphicsSoftware.a)
    endif()

    target_link_libraries(SampleHelloWorld png)
    target_link_libraries(SampleHelloWorld Xaw)
    target_link_libraries(SampleHelloWorld Xrandr)
    target_link_libraries(SampleHelloWorld Xt)
    target_link_libraries(SampleHelloWorld xcb)
    target_link_libraries(SampleHelloWorld GL)
    target_link_libraries(SampleHelloWorld ${X11_LIBRARIES})
    target_link_libraries(SampleHelloWorld ${CMAKE_THREAD_LIBS_INIT} )
    target_link_libraries(SampleHelloWorld ${CMAKE_DL_LIBS})
endif ()






if (WIN32)
    set_property(TARGET SampleHelloWorld PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${FB_ROOT}/Bin/windows/${FB_TOOLSET}/${FB_ARCH}/Debug)
endif()

add_dependencies(SampleHelloWorld FBAero FBAi FBAudio FBApplication FBCore FBFileSystem FBMesh FBProcedural FBSQLite FBVehicle)

if (FB_GRAPHICS_SYSTEM_OGRENEXT) 
    add_dependencies(SampleHelloWorld FBGraphicsOgreNext)
elseif (FB_GRAPHICS_SYSTEM_OGRE)
    add_dependencies(SampleHelloWorld FBGraphicsOgre)
endif()

if (FB_BUILD_ODE)
    add_dependencies(SampleHelloWorld FBODE3)
elseif (FB_BUILD_PHYSX)
    add_dependencies(SampleHelloWorld FBPhysx)
endif()

set_target_properties (SampleHelloWorld PROPERTIES FOLDER Samples)
