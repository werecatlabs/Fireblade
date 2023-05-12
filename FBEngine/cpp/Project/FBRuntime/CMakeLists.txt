project(FBRuntime)

if (UNIX)
    if (NOT APPLE)
        find_package(X11 REQUIRED)
    endif()
endif()


# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBRuntime/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBRuntime/*.cpp")

include_directories( ${FB_ROOT}/Dependencies/boost )

if (FB_USE_ONETBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/onetbb/include )
elseif (FB_USE_TBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tbb/include )
endif()

include_directories( ${FB_ROOT}/Dependencies/tinyxml/include)
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ../../Include )
include_directories( ../../Include/FBRuntime )


if (WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Bin/windows/${FB_TOOLSET}/${FB_ARCH}/)
elseif (APPLE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Bin/macOS/${CMAKE_BUILD_TYPE})
elseif (UNIX)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Bin/linux/${CMAKE_BUILD_TYPE})
endif ()

if (WIN32)
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/libs/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/${CMAKE_BUILD_TYPE})
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/physx/)

    if (FB_USE_ONETBB)
        link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/oneTBB/)
    elseif (FB_USE_TBB)
        link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tbb/)
    endif()
    
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/boost/)
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tinybind/)
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tinyxml2/)
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/wildmagic/)
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ois/${CMAKE_BUILD_TYPE})
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/assimp/${CMAKE_BUILD_TYPE})
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/fbxsdk/2020.2/${CMAKE_BUILD_TYPE})

    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/python/)
    #link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/pypy/)
    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogrenext/)
        link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogrenext/dependencies/lib/)
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogre/)
        link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/ogre/dependencies/lib/)
        
        if (FB_PLATFORM_X64)
            link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x64)
        else ()
            link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x86)
        endif ()
    endif ()
elseif (APPLE)
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/libs/macOS/${CMAKE_BUILD_TYPE})
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS)
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/boost)
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/boost/${CMAKE_BUILD_TYPE})
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/tbb/${CMAKE_BUILD_TYPE})
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS)

    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ois/${CMAKE_BUILD_TYPE})
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/physx/${CMAKE_BUILD_TYPE})
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/wildmagic/${CMAKE_BUILD_TYPE})

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogrenext/${CMAKE_BUILD_TYPE})
        link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogrenext/dependencies/lib/${CMAKE_BUILD_TYPE})
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogre/)
        link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogre/dependencies/lib/)
        link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x64)
    endif ()
elseif (UNIX)
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/libs/linux/${CMAKE_BUILD_TYPE})
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux)
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/boost)
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/tbb)
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux)
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/physx/${CMAKE_BUILD_TYPE})
    link_directories(FBRuntime ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/wildmagic)
endif (WIN32)

add_executable(FBRuntime ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBRuntime PROPERTIES FOLDER Engine/Cpp)


if (WIN32)
    target_link_libraries(FBRuntime FBCore.lib)
    target_link_libraries(FBRuntime FBFileSystem.lib)
    target_link_libraries(FBRuntime FBMesh.lib)
    target_link_libraries(FBRuntime FBProcedural.lib)
    target_link_libraries(FBRuntime FBSQLite.lib)
    target_link_libraries(FBRuntime FBVehicle.lib)
    target_link_libraries(FBRuntime FBWxWidgets.lib)
    target_link_libraries(FBRuntime FBApplication.lib)
    target_link_libraries(FBRuntime FBOISInput.lib)
    target_link_libraries(FBRuntime FBGraphics.lib)
    target_link_libraries(FBRuntime FBCPlusPlus.lib)
    target_link_libraries(FBRuntime FBAssimp.lib)
    target_link_libraries(FBRuntime FBRenderUI.lib)

    if (FB_ENABLE_LUA)
        target_link_libraries(FBRuntime FBLua.lib)
        target_link_libraries(FBRuntime FBLuaBind.lib)
    elseif (FB_ENABLE_PYTHON)
        target_link_libraries(FBRuntime FBPython.lib)
        target_link_libraries(FBRuntime FBPythonBind.lib)
    endif()

    if (FB_BUILD_OISINPUT)
        target_link_libraries(FBRuntime debug OIS_d.lib optimized OIS.lib)
        target_link_libraries(FBRuntime dinput8.lib)
        target_link_libraries(FBRuntime dxguid.lib)
    endif()

    if (FB_USE_FBXSDK)
        #target_link_libraries(FBRuntime libfbxsdk.lib)
        target_link_libraries(FBRuntime libfbxsdk-mt.lib)
        target_link_libraries(FBRuntime libxml2-mt.lib)
        target_link_libraries(FBRuntime tinyxml2.lib)
    endif()

    if (FB_CODEC_FREEIMAGE)
        target_link_libraries(FBRuntime debug FreeImage_d.lib optimized FreeImage.lib)
    endif()

    if (FB_USE_FREETYPE)
        target_link_libraries(FBRuntime debug freetype_d.lib optimized freetype.lib)
    endif()

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        target_link_libraries(FBRuntime FBGraphicsOgreNext.lib)

        if (FB_BUILD_RENDERER_DX11)
            target_link_libraries(FBRuntime d3d11.lib)
            target_link_libraries(FBRuntime dxgi.lib)
            target_link_libraries(FBRuntime dxguid.lib)
        endif()

        if (FB_PLATFORM_X64)
            target_link_libraries(FBRuntime amd_ags_x64.lib)
        endif()

        if (FB_CODEC_FREEIMAGE)
            target_link_libraries(FBRuntime debug FreeImage_d.lib
                                optimized FreeImage.lib)
        endif ()
    elseif(FB_GRAPHICS_SYSTEM_OGRE)
        target_link_libraries(FBRuntime FBGraphicsOgre.lib)

        if (FB_GRAPHICS_SYSTEM_USE_STATIC_LIBS)
            target_link_libraries(FBRuntime debug OgreMainStatic_d.lib optimized OgreMainStatic.lib)
            target_link_libraries(FBRuntime debug OgreOverlayStatic_d.lib optimized OgreOverlayStatic.lib)

            target_link_libraries(FBRuntime debug RenderSystem_Direct3D9Static_d.lib
                                optimized RenderSystem_Direct3D9Static.lib)

            target_link_libraries(FBRuntime debug RenderSystem_Direct3D11Static_d.lib
                                optimized RenderSystem_Direct3D11Static.lib)

            target_link_libraries(FBRuntime debug RenderSystem_GL3PlusStatic_d.lib 
                                optimized RenderSystem_GL3PlusStatic.lib)

            target_link_libraries(FBRuntime debug RenderSystem_GLStatic_d.lib
                                optimized RenderSystem_GLStatic.lib)

            target_link_libraries(FBRuntime debug OgreGLSupportStatic_d.lib
                                optimized OgreGLSupportStatic.lib)

            target_link_libraries(FBRuntime debug OgreTerrainStatic_d.lib optimized OgreTerrainStatic.lib)

            target_link_libraries(FBRuntime debug OgreRTShaderSystemStatic_d.lib
                                       optimized OgreRTShaderSystemStatic.lib)

            target_link_libraries(FBRuntime debug Codec_STBIStatic_d.lib
                                       optimized Codec_STBIStatic.lib)

            target_link_libraries(FBRuntime debug OgreBitesStatic_d.lib
                                       optimized OgreBitesStatic.lib)

            target_link_libraries(FBRuntime freetype.lib)
        endif()

        if (FB_BUILD_RENDERER_OPENGL)
            target_link_libraries(FBRuntime opengl32.lib)
            target_link_libraries(FBRuntime glu32.lib)
        endif()

        if (FB_BUILD_RENDERER_DX9)
            target_link_libraries(FBRuntime d3d9.lib)
            target_link_libraries(FBRuntime d3dx9.lib)
            target_link_libraries(FBRuntime dxgi.lib)
            target_link_libraries(FBRuntime dxguid.lib)
        endif()

        if (FB_BUILD_RENDERER_DX11)
            target_link_libraries(FBRuntime d3d11.lib)
            target_link_libraries(FBRuntime dxgi.lib)
            target_link_libraries(FBRuntime dxguid.lib)
        endif()

        if (FB_PLATFORM_X64)
            target_link_libraries(FBRuntime amd_ags_x64.lib)
        endif()
    elseif(FB_BUILD_DX11)
        target_link_libraries(FBRuntime FBGraphicsD3D11.lib)
    endif()

    if (FB_BUILD_PHYSX)
        target_link_libraries(FBRuntime FBPhysx.lib)

        if (FB_PLATFORM_X64)
            target_link_libraries(FBRuntime 
                        debug PhysX3DEBUG_x64.lib
                        optimized PhysX3_x64.lib)
            target_link_libraries(FBRuntime 
                        debug PhysX3ExtensionsDEBUG.lib
                        optimized PhysX3Extensions.lib)
            target_link_libraries(FBRuntime 
                        debug PhysX3CommonDEBUG_x64.lib 
                        optimized PhysX3Common_x64.lib)
            target_link_libraries(FBRuntime 
                        debug PhysX3VehicleDEBUG.lib 
                        optimized PhysX3Vehicle.lib)
            target_link_libraries(FBRuntime 
                        debug PhysX3CookingDEBUG_x64.lib
                        optimized PhysX3Cooking_x64.lib)
            target_link_libraries(FBRuntime 
                        debug PhysXProfileSDKDEBUG.lib
                        optimized PhysXProfileSDK.lib)
            target_link_libraries(FBRuntime 
                        debug PxTaskDEBUG.lib
                        optimized PxTask.lib)
            target_link_libraries(FBRuntime 
                        debug PhysXVisualDebuggerSDKDEBUG.lib
                        optimized PhysXVisualDebuggerSDK.lib)
        else ()
            target_link_libraries(FBRuntime 
                        debug PhysX3DEBUG_x86.lib
                        optimized PhysX3_x86.lib)
            target_link_libraries(FBRuntime 
                        debug PhysX3ExtensionsDEBUG.lib
                        optimized PhysX3Extensions.lib)
            target_link_libraries(FBRuntime 
                        debug PhysX3CommonDEBUG_x86.lib 
                        optimized PhysX3Common_x86.lib)
            target_link_libraries(FBRuntime 
                        debug PhysX3VehicleDEBUG.lib 
                        optimized PhysX3Vehicle.lib)
            target_link_libraries(FBRuntime 
                        debug PhysX3CookingDEBUG_x86.lib
                        optimized PhysX3Cooking_x86.lib)
            target_link_libraries(FBRuntime 
                        debug PhysXProfileSDKDEBUG.lib
                        optimized PhysXProfileSDK.lib)
            target_link_libraries(FBRuntime 
                        debug PxTaskDEBUG.lib
                        optimized PxTask.lib)
            target_link_libraries(FBRuntime 
                        debug PhysXVisualDebuggerSDKDEBUG.lib
                        optimized PhysXVisualDebuggerSDK.lib)
        endif()
    elseif(FB_BUILD_ODE)
        target_link_libraries(FBRuntime FBODE3.lib)
    elseif(FB_BUILD_PHYSICS3)
        target_link_libraries(FBRuntime FBPhysics3.lib)
    endif()

    target_link_libraries(FBRuntime dxgi.lib)
    target_link_libraries(FBRuntime dxguid.lib)

    target_link_libraries(FBRuntime opengl32.lib)
    target_link_libraries(FBRuntime glu32.lib)
    target_link_libraries(FBRuntime d3d11.lib)
    target_link_libraries(FBRuntime dxgi.lib)
    target_link_libraries(FBRuntime dxguid.lib)
    #target_link_libraries(FBRuntime amd_ags_x64.lib)
    target_link_libraries(FBRuntime debug assimp-vc142-mtd.lib optimized assimp-vc142-mt.lib)
elseif (APPLE)
    # fireblade libraries
    target_link_libraries(FBRuntime libFBCore.a)
    target_link_libraries(FBRuntime libFBFileSystem.a)
    target_link_libraries(FBRuntime libFBMesh.a)
    target_link_libraries(FBRuntime libFBProcedural.a)
    target_link_libraries(FBRuntime libFBVehicle.a)
    target_link_libraries(FBRuntime libFBApplication.a)
    target_link_libraries(FBRuntime libFBSQLite.a)
    target_link_libraries(FBRuntime libFBCPlusPlus.a)
    target_link_libraries(FBRuntime libFBGraphics.a)
    target_link_libraries(FBRuntime FBAssimp.a)
    target_link_libraries(FBRuntime FBRenderUI.a)

    if (FB_BUILD_OISINPUT)
        target_link_libraries(FBRuntime libFBOISInput.a)
        target_link_libraries(FBRuntime debug libOIS_d.a optimized libOIS.a)
    endif()

    if (FB_ENABLE_LUA)
        target_link_libraries(FBRuntime FBLua.a)
        target_link_libraries(FBRuntime FBLuaBind.a)
    elseif (FB_ENABLE_PYTHON)
        target_link_libraries(FBRuntime FBPython.a)
        target_link_libraries(FBRuntime FBPythonBind.a)
    endif()

    if (WIN32)
        target_link_libraries(FBRuntime libboost_thread-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(FBRuntime libboost_date_time-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(FBRuntime libboost_chrono-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(FBRuntime libboost_timer-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(FBRuntime libboost_filesystem-gcc5-mt-s-x64-1_70.a)
    elseif (APPLE)
        target_link_libraries(FBRuntime libboost_thread.a)
        target_link_libraries(FBRuntime libboost_date_time.a)
        target_link_libraries(FBRuntime libboost_chrono.a)
        target_link_libraries(FBRuntime libboost_timer.a)
        target_link_libraries(FBRuntime libboost_filesystem.a)
    elseif (UNIX)
        target_link_libraries(FBRuntime libboost_thread-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(FBRuntime libboost_date_time-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(FBRuntime libboost_chrono-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(FBRuntime libboost_timer-gcc5-mt-s-x64-1_70.a)
        target_link_libraries(FBRuntime libboost_filesystem-gcc5-mt-s-x64-1_70.a)
    endif ()

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        target_link_libraries(FBRuntime FBGraphicsOgreNext.a)
        target_link_libraries(FBRuntime freetype.a)
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        target_link_libraries(FBRuntime FBGraphicsOgre.a)
    elseif(FB_GRAPHICS_SYSTEM_SOFTWARE)
        target_link_libraries(FBRuntime libFBGraphicsSoftware.a)
    endif()

    if (FB_BUILD_PHYSX)
        target_link_libraries(FBRuntime libFBPhysx.a)

        # physx libraries
        if (WIN32)
        elseif (APPLE)
            target_link_libraries(FBRuntime libLowLevel.a libLowLevelCloth.a libPhysXProfileSDK.a libPhysX3Common.a libSceneQuery.a libSimulationController.a libPhysX3Cooking.a libPhysX3Extensions.a libPhysX3.a libPhysX3Vehicle.a)
        elseif (UNIX)
            target_link_libraries(FBRuntime -Wl,--start-group libLowLevel.a libLowLevelCloth.a libPhysXProfileSDK.a libPhysX3Common.a libSceneQuery.a libSimulationController.a libPhysX3Cooking.a libPhysX3Extensions.a libPhysX3.a libPhysX3Vehicle.a -Wl,--end-group )
        endif (WIN32)
    elseif (FB_BUILD_ODE)
        target_link_libraries(FBRuntime libFBODE3.a)
    endif()


    target_link_libraries(FBRuntime libPxTask.a)
    target_link_libraries(FBRuntime libPhysXVisualDebuggerSDK.a)
    target_link_libraries(FBRuntime libPvdRuntime.a)

    target_link_libraries(FBRuntime libtbb.a)
    target_link_libraries(FBRuntime libtbbmalloc.a)

    if (FB_BUILD_WILDMAGIC5)
        target_link_libraries(FBRuntime libWm5Core.a)
        target_link_libraries(FBRuntime libWm5Mathematics.a)
    endif()

    target_link_libraries(FBRuntime stdc++ "-framework Metal" "-framework OpenGL" "-framework Foundation" "-framework QuartzCore" "-framework CoreGraphics" "-framework CoreServices" "-framework Cocoa" "-framework Carbon" "-framework IOKit" objc)
    set_target_properties(FBRuntime PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
else ()
    target_link_libraries(FBRuntime -Wl,--start-group FBCore.a FBAero.a 
        FBApplication.a FBFileSystem.a FBMath.a FBState.a FBVehicle.a FBData.a FBMesh.a FBGraphics.a
        libFBVehicle.a libFBProcedural.a libFBSQLite.a libFBPhysx.a FBAudio.a
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

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        target_link_libraries(FBRuntime FBGraphicsOgreNext.a)
        target_link_libraries(FBRuntime freetype.a)
    elseif(FB_GRAPHICS_SYSTEM_SOFTWARE)
        target_link_libraries(FBRuntime libFBGraphicsSoftware.a)
    endif()

    if(FB_BUILD_WILDMAGIC5)
        target_link_libraries(FBRuntime libWm5Core.a libWm5Mathematics.a)
    endif()
        
    target_link_libraries(FBRuntime png)
    target_link_libraries(FBRuntime Xaw)
    target_link_libraries(FBRuntime Xrandr)
    target_link_libraries(FBRuntime Xt)
    target_link_libraries(FBRuntime xcb)
    target_link_libraries(FBRuntime ${X11_LIBRARIES})
    target_link_libraries(FBRuntime ${CMAKE_THREAD_LIBS_INIT} )
    target_link_libraries(FBRuntime ${CMAKE_DL_LIBS})
endif ()

if (WIN32)
    set_property(TARGET FBRuntime PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${FB_ROOT}/Bin/windows/${FB_TOOLSET}/${FB_ARCH}/Debug)
endif()


add_dependencies(FBRuntime FBOISInput FBCPlusPlus FBAero FBAi FBApplication FBAudio FBCore FBFileSystem FBMesh FBProcedural FBSQLite FBVehicle)

if (FB_GRAPHICS_SYSTEM_OGRENEXT) 
    add_dependencies(FBRuntime FBGraphicsOgreNext)
elseif (FB_GRAPHICS_SYSTEM_OGRE)
    add_dependencies(FBRuntime FBGraphicsOgre)
endif()

if (FB_BUILD_ODE)
    add_dependencies(FBRuntime FBODE3)
elseif (FB_BUILD_PHYSX)
    add_dependencies(FBRuntime FBPhysx)
endif()
