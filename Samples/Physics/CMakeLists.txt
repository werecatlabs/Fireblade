set(CMAKE_USE_RELATIVE_PATHS true)
set(CMAKE_SUPPRESS_REGENERATION true)

project(SamplePhysics)

if (UNIX)
    if (NOT APPLE)
        find_package(X11 REQUIRED)
    endif()
endif()

add_compile_definitions (_FB_STATIC_LIB_)
#ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

include_directories( ${FB_ROOT}/Dependencies/boost)
include_directories( ${FB_ROOT}/Dependencies/tinyxml/include)

if (FB_USE_ONETBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/onetbb/include )
elseif (FB_USE_TBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tbb/include )
endif()

include_directories( ${FB_ROOT}/AdditionalLibraries/cppsqlite/include)
include_directories( ${FB_ROOT}/AdditionalLibraries/sqlite/include)
include_directories( ${FB_ROOT}/FBEngine/cpp/Include )
include_directories(Source)



if (WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Bin/windows/${FB_TOOLSET}/${FB_ARCH}/)
elseif (APPLE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Bin/macOS/${CMAKE_BUILD_TYPE})
elseif (UNIX)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Bin/linux/${CMAKE_BUILD_TYPE})
endif ()

if (WIN32)
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/libs/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/${CMAKE_BUILD_TYPE})
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/physx/)
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tbb/)
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/boost/)
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tinybind/)
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tinyxml2/${CMAKE_BUILD_TYPE})
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/wildmagic/)
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ois/)
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/assimp/${CMAKE_BUILD_TYPE})
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/fbxsdk/2020.2/${CMAKE_BUILD_TYPE})

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogrenext/)
        link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogrenext/dependencies/lib/)
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogre/)
        link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogre/dependencies/lib/)
        
        if (FB_PLATFORM_X64)
            link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x64)
        else ()
            link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x86)
        endif()
    endif ()
elseif (APPLE)
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/libs/macOS/${CMAKE_BUILD_TYPE})
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS)
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/boost)
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/boost/${CMAKE_BUILD_TYPE})
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/tbb/${CMAKE_BUILD_TYPE})
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS)
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/assimp/${CMAKE_BUILD_TYPE})
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/physx/${CMAKE_BUILD_TYPE})
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/wildmagic/${CMAKE_BUILD_TYPE})

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogrenext/${CMAKE_BUILD_TYPE})
        link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogrenext/dependencies/lib/${CMAKE_BUILD_TYPE})
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogre/)
        link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogre/dependencies/lib/)
        link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x64)
    endif ()
elseif (UNIX)
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/libs/linux/${CMAKE_BUILD_TYPE})
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux)
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/boost)
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/tbb)
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux)
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/physx)
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/physx/${CMAKE_BUILD_TYPE})
    link_directories(SamplePhysics ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/wildmagic)
endif ()

file(GLOB_RECURSE HEADER_FILES "./*.h")
file(GLOB_RECURSE SOURCE_FILES "./*.c" "./*.cpp")

add_executable(SamplePhysics ${HEADER_FILES} ${SOURCE_FILES})


# fireblade libraries
if (WIN32)
    target_link_libraries(SamplePhysics FBCore.lib)
    target_link_libraries(SamplePhysics FBFileSystem.lib)
    target_link_libraries(SamplePhysics FBMesh.lib)
    target_link_libraries(SamplePhysics FBProcedural.lib)
    target_link_libraries(SamplePhysics FBPhysx.lib)
    target_link_libraries(SamplePhysics FBSQLite.lib)
    target_link_libraries(SamplePhysics FBWxWidgets.lib)
    target_link_libraries(SamplePhysics FBGraphics.lib)
    target_link_libraries(SamplePhysics FBApplication.lib)
    target_link_libraries(SamplePhysics FBVehicle.lib)
    target_link_libraries(SamplePhysics FBRenderUI.lib)

    if (FB_BUILD_OISINPUT)
        target_link_libraries(SamplePhysics FBOISInput.lib)
        #target_link_libraries(SamplePhysics debug OIS_d.lib optimized OIS.lib)
        target_link_libraries(SamplePhysics dinput8.lib)
        target_link_libraries(SamplePhysics dxguid.lib)
    endif ()

    if (FB_USE_FBXSDK)
        #target_link_libraries(SamplePhysics libfbxsdk.lib)
        target_link_libraries(SamplePhysics libfbxsdk-mt.lib)
        target_link_libraries(SamplePhysics libxml2-mt.lib)
        target_link_libraries(SamplePhysics tinyxml2.lib)
    endif()

    target_link_libraries(SamplePhysics 
                        debug Wm5CoreD.lib
                        optimized Wm5Core.lib)
    target_link_libraries(SamplePhysics 
                        debug Wm5MathematicsD.lib
                        optimized Wm5Mathematics.lib)
    target_link_libraries(SamplePhysics 
                        debug Wm5PhysicsD.lib
                        optimized Wm5Physics.lib)

    if (FB_BUILD_PHYSX)
        target_link_libraries(SamplePhysics FBPhysx.lib)

        if (FB_PLATFORM_X64)
            target_link_libraries(SamplePhysics 
                        debug PhysX3DEBUG_x64.lib
                        optimized PhysX3_x64.lib)
            target_link_libraries(SamplePhysics 
                        debug PhysX3ExtensionsDEBUG.lib
                        optimized PhysX3Extensions.lib)
            target_link_libraries(SamplePhysics 
                        debug PhysX3CommonDEBUG_x64.lib 
                        optimized PhysX3Common_x64.lib)
            target_link_libraries(SamplePhysics 
                        debug PhysX3VehicleDEBUG.lib 
                        optimized PhysX3Vehicle.lib)
            target_link_libraries(SamplePhysics 
                        debug PhysX3CookingDEBUG_x64.lib
                        optimized PhysX3Cooking_x64.lib)
            target_link_libraries(SamplePhysics 
                        debug PhysXProfileSDKDEBUG.lib
                        optimized PhysXProfileSDK.lib)
            target_link_libraries(SamplePhysics 
                        debug PxTaskDEBUG.lib
                        optimized PxTask.lib)
            target_link_libraries(SamplePhysics 
                        debug PhysXVisualDebuggerSDKDEBUG.lib
                        optimized PhysXVisualDebuggerSDK.lib)
        else ()
            target_link_libraries(SamplePhysics 
                        debug PhysX3DEBUG_x86.lib
                        optimized PhysX3_x86.lib)
            target_link_libraries(SamplePhysics 
                        debug PhysX3ExtensionsDEBUG.lib
                        optimized PhysX3Extensions.lib)
            target_link_libraries(SamplePhysics 
                        debug PhysX3CommonDEBUG_x86.lib 
                        optimized PhysX3Common_x86.lib)
            target_link_libraries(SamplePhysics 
                        debug PhysX3VehicleDEBUG.lib 
                        optimized PhysX3Vehicle.lib)
            target_link_libraries(SamplePhysics 
                        debug PhysX3CookingDEBUG_x86.lib
                        optimized PhysX3Cooking_x86.lib)
            target_link_libraries(SamplePhysics 
                        debug PhysXProfileSDKDEBUG.lib
                        optimized PhysXProfileSDK.lib)
            target_link_libraries(SamplePhysics 
                        debug PxTaskDEBUG.lib
                        optimized PxTask.lib)
            target_link_libraries(SamplePhysics 
                        debug PhysXVisualDebuggerSDKDEBUG.lib
                        optimized PhysXVisualDebuggerSDK.lib)
        endif()
    endif()

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        target_link_libraries(SamplePhysics FBGraphicsOgreNext.lib)
        target_link_libraries(SamplePhysics debug FreeImage_d.lib optimized FreeImage.lib)
        target_link_libraries(SamplePhysics debug freetype_d.lib optimized freetype.lib)
    
        if (FB_BUILD_RENDERER_DX11)
            target_link_libraries(SamplePhysics d3d11.lib)
            target_link_libraries(SamplePhysics dxgi.lib)
            target_link_libraries(SamplePhysics dxguid.lib)
        endif()

        if (FB_BUILD_RENDERER_GL3PLUS)
            target_link_libraries(SamplePhysics opengl32.lib)
            target_link_libraries(SamplePhysics glu32.lib)
        endif ()

        if (FB_PLATFORM_X64)
            target_link_libraries(SamplePhysics amd_ags_x64.lib)
        endif()
    elseif(FB_GRAPHICS_SYSTEM_OGRE)
        target_link_libraries(SamplePhysics FBGraphicsOgre.lib)

        if (FB_GRAPHICS_SYSTEM_USE_STATIC_LIBS)
            target_link_libraries(SamplePhysics debug OgreMainStatic_d.lib optimized OgreMainStatic.lib)
            target_link_libraries(SamplePhysics debug OgreOverlayStatic_d.lib optimized OgreOverlayStatic.lib)

            target_link_libraries(SamplePhysics debug RenderSystem_Direct3D9Static_d.lib
                                optimized RenderSystem_Direct3D9Static.lib)

            target_link_libraries(SamplePhysics debug RenderSystem_Direct3D11Static_d.lib
                                optimized RenderSystem_Direct3D11Static.lib)

            target_link_libraries(SamplePhysics debug RenderSystem_GL3PlusStatic_d.lib 
                                optimized RenderSystem_GL3PlusStatic.lib)

            target_link_libraries(SamplePhysics debug RenderSystem_GLStatic_d.lib 
                                optimized RenderSystem_GLStatic.lib)

                            
            target_link_libraries(SamplePhysics debug OgreGLSupportStatic_d.lib
                                optimized OgreGLSupportStatic.lib)

            target_link_libraries(SamplePhysics debug OgreTerrainStatic_d.lib optimized OgreTerrainStatic.lib)

            target_link_libraries(SamplePhysics debug OgreRTShaderSystemStatic_d.lib
                                       optimized OgreRTShaderSystemStatic.lib)

            target_link_libraries(SamplePhysics debug Codec_STBIStatic_d.lib
                                       optimized Codec_STBIStatic.lib)

            target_link_libraries(SamplePhysics debug OgreBitesStatic_d.lib
                                       optimized OgreBitesStatic.lib)
        endif()
    
        if (FB_CODEC_FREEIMAGE)
            target_link_libraries(SamplePhysics debug FreeImage_d.lib
                                optimized FreeImage.lib)
        endif ()

        #target_link_libraries(SamplePhysics freetype.lib)   
        
        if (FB_BUILD_RENDERER_DX9)
            target_link_libraries(SamplePhysics d3d9.lib)
            target_link_libraries(SamplePhysics d3dx9.lib)
        endif ()

        if (FB_BUILD_RENDERER_DX11)
            target_link_libraries(SamplePhysics d3d11.lib)
            target_link_libraries(SamplePhysics dxgi.lib)
            target_link_libraries(SamplePhysics dxguid.lib)
        endif()

        if (FB_BUILD_RENDERER_OPENGL)
            target_link_libraries(SamplePhysics opengl32.lib)
            target_link_libraries(SamplePhysics glu32.lib)
            
            if (FB_PLATFORM_X64)
                target_link_libraries(SamplePhysics amd_ags_x64.lib)
            endif ()
        endif ()

        if (FB_BUILD_RENDERER_GL3PLUS)
            target_link_libraries(SamplePhysics opengl32.lib)
            target_link_libraries(SamplePhysics glu32.lib)

            if (FB_PLATFORM_X64)
                target_link_libraries(SamplePhysics amd_ags_x64.lib)
            endif ()
        endif ()
    elseif(FB_BUILD_DX11)
        target_link_libraries(SamplePhysics FBGraphicsD3D11.lib)
    endif()
elseif (APPLE)
    target_link_libraries(SamplePhysics libFBCore.a)
    target_link_libraries(SamplePhysics libFBFileSystem.a)
    target_link_libraries(SamplePhysics libFBGraphics.a)
    target_link_libraries(SamplePhysics libFBMesh.a)
    target_link_libraries(SamplePhysics libFBProcedural.a)
    target_link_libraries(SamplePhysics libFBPhysx.a)
    target_link_libraries(SamplePhysics libFBAero.a)
    target_link_libraries(SamplePhysics libFBVehicle.a)
    target_link_libraries(SamplePhysics libFBAudio.a)
    target_link_libraries(SamplePhysics libFBApplication.a)
    target_link_libraries(SamplePhysics libFBRenderUI.a)

    if (FB_BUILD_OISINPUT)
        target_link_libraries(SamplePhysics FBOISInput.a)
    endif()

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        target_link_libraries(SamplePhysics FBGraphicsOgreNext.a)
        target_link_libraries(SamplePhysics freetype.a)
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        target_link_libraries(SamplePhysics FBGraphicsOgre.a)
    elseif(FB_GRAPHICS_SYSTEM_SOFTWARE)
        target_link_libraries(SamplePhysics libFBGraphicsSoftware.a)
    endif()

    if (FB_BUILD_PHYSX)
        target_link_libraries(SamplePhysics libFBPhysx.a)
    elseif (FB_BUILD_ODE)
        target_link_libraries(SamplePhysics libFBODE3.a)
    endif()

    target_link_libraries(SamplePhysics libFBSQLite.a)

    target_link_libraries(SamplePhysics libboost_thread.a)
    target_link_libraries(SamplePhysics libboost_date_time.a)
    target_link_libraries(SamplePhysics libboost_chrono.a)
    target_link_libraries(SamplePhysics libboost_timer.a)
    target_link_libraries(SamplePhysics libboost_filesystem.a)
    target_link_libraries(SamplePhysics libboost_unit_test_framework.a)
    target_link_libraries(SamplePhysics libboost_system.a)

    target_link_libraries(SamplePhysics libPxTask.a)
    target_link_libraries(SamplePhysics libPhysXVisualDebuggerSDK.a)
    target_link_libraries(SamplePhysics libPvdRuntime.a)

    target_link_libraries(SamplePhysics libtbb.a)
    target_link_libraries(SamplePhysics libtbbmalloc.a)

    if (FB_BUILD_WILDMAGIC5)
        target_link_libraries(SamplePhysics libWm5Core.a)
        target_link_libraries(SamplePhysics libWm5Mathematics.a)
    endif()

    target_link_libraries(SamplePhysics debug libassimpd.a optimized libassimp.a)

    # physx libraries
    target_link_libraries(SamplePhysics libLowLevel.a libLowLevelCloth.a libPhysXProfileSDK.a libPhysX3Common.a libSceneQuery.a libSimulationController.a libPhysX3Cooking.a libPhysX3Extensions.a libPhysX3.a libPhysX3Vehicle.a )

    #target_link_libraries(OverlayTest stdc++ "-framework CoreGraphics" objc)
    #target_link_libraries(OverlayTest ${AUDIO_UNIT})
    target_link_libraries(SamplePhysics stdc++ "-framework Metal" "-framework OpenGL" "-framework Foundation" "-framework QuartzCore" "-framework CoreGraphics" "-framework CoreServices" "-framework Cocoa" "-framework Carbon" "-framework IOKit" objc)
    set_target_properties(SamplePhysics PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
else (UNIX)
    target_link_libraries(SamplePhysics -Wl,--start-group FBCore.a FBAero.a
        FBApplication.a FBFileSystem.a FBMath.a FBState.a FBVehicle.a FBData.a FBMesh.a FBGraphics.a
        libFBObjectTemplates.a libFBVehicle.a libFBProcedural.a libFBSQLite.a libFBPhysx.a FBPhysics3.a FBAudio.a FBRenderUI.a  
        FBGraphicsOgreNext.a ${X11_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS}
        png Xaw Xrandr Xt xcb GL
        libboost_thread-gcc5-mt-s-x64-1_70.a
        libboost_date_time-gcc5-mt-s-x64-1_70.a
        libboost_chrono-gcc5-mt-s-x64-1_70.a
        libboost_timer-gcc5-mt-s-x64-1_70.a
        libboost_filesystem-gcc5-mt-s-x64-1_70.a
        libboost_unit_test_framework-gcc5-mt-s-x64-1_70.a
        debug libtbb_debug.a optimized libtbb.a
        debug libtbbmalloc_debug.a optimized libtbbmalloc.a
        libLowLevel.a libLowLevelCloth.a libPhysXProfileSDK.a libPhysX3Common.a libSceneQuery.a 
        libSimulationController.a libPhysX3Cooking.a libPhysX3Extensions.a libPhysX3.a libPhysX3Vehicle.a libPxTask.a
        -Wl,--end-group )

    #target_link_libraries(SamplePhysics -Wl,--start-group FBGraphicsOgreNext.a ${X11_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS} -Wl,--end-group )

    target_link_libraries(SamplePhysics freetype.a)

    target_link_libraries(SamplePhysics png)
    target_link_libraries(SamplePhysics Xaw)
    target_link_libraries(SamplePhysics Xrandr)
    target_link_libraries(SamplePhysics Xt)
    target_link_libraries(SamplePhysics xcb)
    target_link_libraries(SamplePhysics ${X11_LIBRARIES})
    target_link_libraries(SamplePhysics ${CMAKE_THREAD_LIBS_INIT} )
    target_link_libraries(SamplePhysics ${CMAKE_DL_LIBS})
endif ()

if (WIN32)
    set_property(TARGET SamplePhysics PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${FB_ROOT}/Bin/windows/${FB_TOOLSET}/${FB_ARCH}/Debug)
endif()


add_dependencies(SamplePhysics FBAero FBApplication FBAi FBAudio FBCore FBFileSystem FBMesh FBProcedural FBSQLite FBVehicle)

if (FB_GRAPHICS_SYSTEM_OGRENEXT) 
    add_dependencies(SamplePhysics FBGraphicsOgreNext)
elseif (FB_GRAPHICS_SYSTEM_OGRE)
    add_dependencies(SamplePhysics FBGraphicsOgre)
endif()

if (FB_BUILD_ODE)
    add_dependencies(SamplePhysics FBODE3)
elseif (FB_BUILD_PHYSX)
    add_dependencies(SamplePhysics FBPhysx)
endif()

set_target_properties (SamplePhysics PROPERTIES FOLDER Samples)
