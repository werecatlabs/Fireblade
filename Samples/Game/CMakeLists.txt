set(CMAKE_USE_RELATIVE_PATHS true)
set(CMAKE_SUPPRESS_REGENERATION true)

project(SampleGame)

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

if (FB_GRAPHICS_SYSTEM_OGRENEXT)
    if(WIN32)
        include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/include/win32 )
    elseif(APPLE)
        include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/include/osx )
    else()
        include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/include/linux )
    endif()

    include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/OgreMain/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/OgreMain/include/Deprecated )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/Components/Overlay/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/Components/Paging/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/Components/Bites/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/Components/Hlms/Common/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/Components/Hlms/Pbs/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/Components/Hlms/Unlit/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/PlugIns/CgProgramManager/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/RenderSystems/Direct3D11/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/RenderSystems/GL3Plus/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/RenderSystems/GL3Plus/include/GLSL )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/RenderSystems/GLES2/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/RenderSystems/Metal/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogrenext/RenderSystems/NULL/include )
elseif (FB_GRAPHICS_SYSTEM_OGRE)
    if(WIN32)
        include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/win32 )
    elseif(APPLE)
        include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/osx )
    else()
        include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/linux )
    endif()

    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/OgreMain/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/Components/Overlay/include )
endif()

include_directories( ${FB_ROOT}/FBEngine/cpp/Include )
include_directories(Source)



if (WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Bin/windows/${FB_TOOLSET}/${FB_ARCH}/)
elseif (APPLE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Bin/macOS/${CMAKE_BUILD_TYPE})
elseif (UNIX)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Bin/linux/${CMAKE_BUILD_TYPE})
endif (WIN32)

if (WIN32)
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/libs/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/${CMAKE_BUILD_TYPE})
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/physx/)
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tbb/)
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/boost/)
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tinybind/)
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tinyxml2/)
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/wildmagic/)
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ois/)
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/assimp/${CMAKE_BUILD_TYPE})
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/fbxsdk/2020.2/${CMAKE_BUILD_TYPE})

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogrenext/)
        link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogrenext/dependencies/lib/)
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogre/)
        link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogre/dependencies/lib/)
        
        if (FB_PLATFORM_X64)
            link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x64)
        else ()
            link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x86)
        endif()
    endif ()
elseif (APPLE)
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/libs/macOS/${CMAKE_BUILD_TYPE})
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS)
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/boost)
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/boost/${CMAKE_BUILD_TYPE})
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/tbb/${CMAKE_BUILD_TYPE})
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS)
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/assimp/${CMAKE_BUILD_TYPE})
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ois/${CMAKE_BUILD_TYPE})
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/physx/${CMAKE_BUILD_TYPE})
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/wildmagic/${CMAKE_BUILD_TYPE})

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogrenext/${CMAKE_BUILD_TYPE})
        link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogrenext/dependencies/lib/${CMAKE_BUILD_TYPE})
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogre/)
        link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogre/dependencies/lib/)
        
        if (FB_PLATFORM_X64)
            link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x64)
        else ()
            link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x86)
        endif()
    endif ()
elseif (UNIX)
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/libs/linux/${CMAKE_BUILD_TYPE})
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux)
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/boost)
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/tbb)
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux)
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/physx/${CMAKE_BUILD_TYPE})
    link_directories(SampleGame ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/wildmagic)
endif (WIN32)

file(GLOB_RECURSE HEADER_FILES "./*.h")
file(GLOB_RECURSE SOURCE_FILES "./*.c" "./*.cpp")

add_executable(SampleGame ${HEADER_FILES} ${SOURCE_FILES})

if (WIN32)
    target_link_libraries(SampleGame FBCore.lib)
    target_link_libraries(SampleGame FBFileSystem.lib)
    target_link_libraries(SampleGame FBMesh.lib)
    target_link_libraries(SampleGame FBProcedural.lib)
    target_link_libraries(SampleGame FBPhysx.lib)
    target_link_libraries(SampleGame FBSQLite.lib)
    target_link_libraries(SampleGame FBVehicle.lib)
    target_link_libraries(SampleGame FBWxWidgets.lib)
    target_link_libraries(SampleGame FBApplication.lib)
    target_link_libraries(SampleGame FBGraphics.lib)

    if (FB_BUILD_OISINPUT)
        target_link_libraries(SampleGame FBOISInput.lib)
        #target_link_libraries(SampleGame debug OIS_d.lib optimized OIS.lib)
        target_link_libraries(SampleGame dinput8.lib)
        target_link_libraries(SampleGame dxguid.lib)
    endif ()

    if (FB_USE_FBXSDK)
        #target_link_libraries(SampleGame libfbxsdk.lib)
        target_link_libraries(SampleGame libfbxsdk-mt.lib)
        target_link_libraries(SampleGame libxml2-mt.lib)
        target_link_libraries(SampleGame tinyxml2.lib)
    endif()

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        target_link_libraries(SampleGame FBGraphicsOgreNext.lib)
        target_link_libraries(SampleGame debug FreeImage_d.lib optimized FreeImage.lib)
        target_link_libraries(SampleGame debug freetype_d.lib optimized freetype.lib)

        if (FB_BUILD_RENDERER_DX11)
            target_link_libraries(SampleGame d3d11.lib)
            target_link_libraries(SampleGame dxgi.lib)
            target_link_libraries(SampleGame dxguid.lib)
        endif()

        if (FB_BUILD_RENDERER_GL3PLUS)
            target_link_libraries(SampleGame opengl32.lib)
            target_link_libraries(SampleGame glu32.lib)
        endif ()

        if (FB_PLATFORM_X64)
            target_link_libraries(SampleGame amd_ags_x64.lib)
        endif()
    elseif(FB_GRAPHICS_SYSTEM_OGRE)
        target_link_libraries(SampleGame FBGraphicsOgre.lib)

        if (FB_GRAPHICS_SYSTEM_USE_STATIC_LIBS)
            target_link_libraries(SampleGame debug OgreMainStatic_d.lib optimized OgreMainStatic.lib)
            target_link_libraries(SampleGame debug OgreOverlayStatic_d.lib optimized OgreOverlayStatic.lib)

            target_link_libraries(SampleGame debug RenderSystem_Direct3D9Static_d.lib
                                optimized RenderSystem_Direct3D9Static.lib)

            target_link_libraries(SampleGame debug RenderSystem_Direct3D11Static_d.lib
                                optimized RenderSystem_Direct3D11Static.lib)

            target_link_libraries(SampleGame debug RenderSystem_GL3PlusStatic_d.lib 
                                optimized RenderSystem_GL3PlusStatic.lib)

            target_link_libraries(SampleGame debug RenderSystem_GLStatic_d.lib
                                optimized RenderSystem_GLStatic.lib)

            target_link_libraries(SampleGame debug OgreGLSupportStatic_d.lib
                                optimized OgreGLSupportStatic.lib)

            target_link_libraries(SampleGame debug OgreTerrainStatic_d.lib optimized OgreTerrainStatic.lib)

            target_link_libraries(SampleGame debug OgreRTShaderSystemStatic_d.lib
                                       optimized OgreRTShaderSystemStatic.lib)

            target_link_libraries(SampleGame debug Codec_STBIStatic_d.lib
                                       optimized Codec_STBIStatic.lib)

            target_link_libraries(SampleGame debug OgreBitesStatic_d.lib
                                       optimized OgreBitesStatic.lib)
        endif()

        if (FB_BUILD_RENDERER_DX9)
            target_link_libraries(SampleGame d3d9.lib)
            target_link_libraries(SampleGame d3dx9.lib)
        endif ()

        if (FB_BUILD_RENDERER_DX11)
            target_link_libraries(SampleGame d3d11.lib)
            target_link_libraries(SampleGame dxgi.lib)
            target_link_libraries(SampleGame dxguid.lib)
        endif()

        if (FB_BUILD_RENDERER_OPENGL)
            target_link_libraries(SampleGame opengl32.lib)
            target_link_libraries(SampleGame glu32.lib)
            
            if (FB_PLATFORM_X64)
                target_link_libraries(SampleGame amd_ags_x64.lib)
            endif ()
        endif ()

        if (FB_BUILD_RENDERER_GL3PLUS)
            target_link_libraries(SampleGame opengl32.lib)
            target_link_libraries(SampleGame glu32.lib)

            if (FB_PLATFORM_X64)
                target_link_libraries(SampleGame amd_ags_x64.lib)
            endif ()
        endif ()
    elseif(FB_BUILD_DX11)
        target_link_libraries(SampleGame FBGraphicsD3D11.lib)
    endif()

    if (FB_BUILD_PHYSX)
        target_link_libraries(SampleGame FBPhysx.lib)

        if (FB_PLATFORM_X64)
            target_link_libraries(SampleGame 
                        debug PhysX3DEBUG_x64.lib
                        optimized PhysX3_x64.lib)
            target_link_libraries(SampleGame 
                        debug PhysX3ExtensionsDEBUG.lib
                        optimized PhysX3Extensions.lib)
            target_link_libraries(SampleGame 
                        debug PhysX3CommonDEBUG_x64.lib 
                        optimized PhysX3Common_x64.lib)
            target_link_libraries(SampleGame 
                        debug PhysX3VehicleDEBUG.lib 
                        optimized PhysX3Vehicle.lib)
            target_link_libraries(SampleGame 
                        debug PhysX3CookingDEBUG_x64.lib
                        optimized PhysX3Cooking_x64.lib)
            target_link_libraries(SampleGame 
                        debug PhysXProfileSDKDEBUG.lib
                        optimized PhysXProfileSDK.lib)
            target_link_libraries(SampleGame 
                        debug PxTaskDEBUG.lib
                        optimized PxTask.lib)
            target_link_libraries(SampleGame 
                        debug PhysXVisualDebuggerSDKDEBUG.lib
                        optimized PhysXVisualDebuggerSDK.lib)
        else ()
            target_link_libraries(SampleGame 
                        debug PhysX3DEBUG_x86.lib
                        optimized PhysX3_x86.lib)
            target_link_libraries(SampleGame 
                        debug PhysX3ExtensionsDEBUG.lib
                        optimized PhysX3Extensions.lib)
            target_link_libraries(SampleGame 
                        debug PhysX3CommonDEBUG_x86.lib 
                        optimized PhysX3Common_x86.lib)
            target_link_libraries(SampleGame 
                        debug PhysX3VehicleDEBUG.lib 
                        optimized PhysX3Vehicle.lib)
            target_link_libraries(SampleGame 
                        debug PhysX3CookingDEBUG_x86.lib
                        optimized PhysX3Cooking_x86.lib)
            target_link_libraries(SampleGame 
                        debug PhysXProfileSDKDEBUG.lib
                        optimized PhysXProfileSDK.lib)
            target_link_libraries(SampleGame 
                        debug PxTaskDEBUG.lib
                        optimized PxTask.lib)
            target_link_libraries(SampleGame 
                        debug PhysXVisualDebuggerSDKDEBUG.lib
                        optimized PhysXVisualDebuggerSDK.lib)
        endif()
    endif()
elseif (APPLE)
    # fireblade libraries
    target_link_libraries(SampleGame libFBCore.a)
    target_link_libraries(SampleGame libFBFileSystem.a)
    target_link_libraries(SampleGame libFBGraphics.a)
    target_link_libraries(SampleGame libFBMesh.a)
    target_link_libraries(SampleGame libFBProcedural.a)
    target_link_libraries(SampleGame libFBPhysx.a)
    target_link_libraries(SampleGame libFBVehicle.a)
    target_link_libraries(SampleGame libFBApplication.a)
    target_link_libraries(SampleGame libFBSQLite.a)
    target_link_libraries(SampleGame libFBOISInput.a)

    if (WIN32)
        target_link_libraries(SampleGame libboost_thread-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleGame libboost_date_time-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleGame libboost_chrono-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleGame libboost_timer-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleGame libboost_filesystem-gcc5-mt-s-x64-1_70.a)
    elseif (APPLE)
        target_link_libraries(SampleGame libboost_thread.a)
        target_link_libraries(SampleGame libboost_date_time.a)
        target_link_libraries(SampleGame libboost_chrono.a)
        target_link_libraries(SampleGame libboost_timer.a)
        target_link_libraries(SampleGame libboost_filesystem.a)
    elseif (UNIX)
        target_link_libraries(SampleGame libboost_thread-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleGame libboost_date_time-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleGame libboost_chrono-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleGame libboost_timer-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(SampleGame libboost_filesystem-gcc5-mt-s-x64-1_70.a)
    endif ()

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        target_link_libraries(SampleGame FBGraphicsOgreNext.a)
        target_link_libraries(SampleGame freetype.a)
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        target_link_libraries(SampleGame FBGraphicsOgre.a)
    elseif(FB_GRAPHICS_SYSTEM_SOFTWARE)
        target_link_libraries(SampleGame libFBGraphicsSoftware.a)
    endif()

    target_link_libraries(SampleGame debug libassimpd.a optimized libassimp.a)

    if (FB_BUILD_PHYSX)
        target_link_libraries(SampleGame libFBPhysx.a)
    elseif (FB_BUILD_ODE)
        target_link_libraries(SampleGame libFBODE3.a)
    endif()

    # physx libraries
    if (WIN32)
    elseif (APPLE)
        target_link_libraries(SampleGame libLowLevel.a libLowLevelCloth.a libPhysXProfileSDK.a libPhysX3Common.a libSceneQuery.a libSimulationController.a libPhysX3Cooking.a libPhysX3Extensions.a libPhysX3.a libPhysX3Vehicle.a)
    elseif (UNIX)
        target_link_libraries(SampleGame -Wl,--start-group libLowLevel.a libLowLevelCloth.a libPhysXProfileSDK.a libPhysX3Common.a libSceneQuery.a libSimulationController.a libPhysX3Cooking.a libPhysX3Extensions.a libPhysX3.a libPhysX3Vehicle.a -Wl,--end-group )
    endif (WIN32)

    target_link_libraries(SampleGame libPxTask.a)
    target_link_libraries(SampleGame libPhysXVisualDebuggerSDK.a)
    target_link_libraries(SampleGame libPvdRuntime.a)

    target_link_libraries(SampleGame libtbb.a)
    target_link_libraries(SampleGame libtbbmalloc.a)
    target_link_libraries(SampleGame debug libOIS_d.a optimized libOIS.a)

    if (FB_BUILD_WILDMAGIC5)
        target_link_libraries(SampleGame libWm5Core.a)
        target_link_libraries(SampleGame libWm5Mathematics.a)
    endif()

    #target_link_libraries(SampleGame stdc++ "-framework CoreGraphics" objc)
    #target_link_libraries(SampleGame ${AUDIO_UNIT})
    target_link_libraries(SampleGame stdc++ "-framework Metal" "-framework OpenGL" "-framework Foundation" "-framework QuartzCore" "-framework CoreGraphics" "-framework CoreServices" "-framework Cocoa" "-framework Carbon" "-framework IOKit" objc)
    set_target_properties(SampleGame PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
else ()
    target_link_libraries(SampleGame -Wl,--start-group FBCore.a FBSampleGame.a 
        FBApplication.a FBFileSystem.a FBMath.a FBVehicle.a FBData.a FBMesh.a 
        libFBObjectTemplates.a libFBVehicle.a libFBProcedural.a libFBSQLite.a libFBPhysx.a FBAudio.a         
        libboost_thread-gcc5-mt-s-x64-1_70.a
        libboost_date_time-gcc5-mt-s-x64-1_70.a
        libboost_chrono-gcc5-mt-s-x64-1_70.a
        libboost_timer-gcc5-mt-s-x64-1_70.a
        libboost_filesystem-gcc5-mt-s-x64-1_70.a
        debug libtbb_debug.a optimized libtbb.a
        debug libtbbmalloc_debug.a optimized libtbbmalloc.a
        libLowLevel.a libLowLevelCloth.a libPhysXProfileSDK.a libPhysX3Common.a libSceneQuery.a 
        libSimulationController.a libPhysX3Cooking.a libPhysX3Extensions.a libPhysX3.a libPhysX3Vehicle.a libPxTask.a
        libWm5Core.a libWm5Mathematics.a
        -Wl,--end-group )
        
    target_link_libraries(SampleGame ${CMAKE_THREAD_LIBS_INIT} )
    target_link_libraries(SampleGame ${CMAKE_DL_LIBS})
endif ()

if (WIN32)
    set_property(TARGET SampleGame PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${FB_ROOT}/Bin/windows/${FB_TOOLSET}/${FB_ARCH}/Debug)
endif()


add_dependencies(SampleGame FBAero FBAi FBApplication FBAudio FBCore FBFileSystem FBMesh FBProcedural FBSQLite FBVehicle)


if (FB_GRAPHICS_SYSTEM_OGRENEXT) 
    add_dependencies(SampleGame FBGraphicsOgreNext)
elseif (FB_GRAPHICS_SYSTEM_OGRE)
    add_dependencies(SampleGame FBGraphicsOgre)
endif()

if (FB_BUILD_ODE)
    add_dependencies(SampleGame FBODE3)
elseif (FB_BUILD_PHYSX)
    add_dependencies(SampleGame FBPhysx)
endif()

set_target_properties (SampleGame PROPERTIES FOLDER Samples)
