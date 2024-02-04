set(CMAKE_USE_RELATIVE_PATHS true)
set(CMAKE_SUPPRESS_REGENERATION true)

project(SampleGame)

if (UNIX)
    if (NOT APPLE)
        find_package(X11 REQUIRED)
    endif()
endif()


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
    target_link_libraries(SampleGame FBMesh.lib)
    target_link_libraries(SampleGame FBPhysx.lib)
    target_link_libraries(SampleGame FBSQLite.lib)
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

    if (FB_CODEC_FREEIMAGE)
        target_link_libraries(SampleGame FreeImage.a)
    endif()

    if (FB_USE_FREETYPE)
        target_link_libraries(SampleGame freetype.a)
    endif()

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        target_link_libraries(SampleGame FBGraphicsOgreNext.lib)

        target_link_libraries(SampleGame harfbuzz.lib)
        target_link_libraries(SampleGame harfbuzz-subset.lib)
        target_link_libraries(SampleGame icucommon.lib)
        target_link_libraries(SampleGame sds_library.lib)

        if (FB_BUILD_RENDERER_DX11)
            target_link_libraries(SampleGame d3d11.lib)
            target_link_libraries(SampleGame dxgi.lib)
            target_link_libraries(SampleGame dxguid.lib)
        endif()

        if (FB_BUILD_RENDERER_GL3PLUS)
            target_link_libraries(SampleGame opengl32.lib)
            target_link_libraries(SampleGame glu32.lib)
        endif ()
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

        target_link_libraries(SampleGame PhysX3.lib)
        target_link_libraries(SampleGame PhysX3Extensions.lib)
        target_link_libraries(SampleGame PhysX3Common.lib)
        target_link_libraries(SampleGame PhysX3Vehicle.lib)
        target_link_libraries(SampleGame PhysX3Cooking.lib)
        target_link_libraries(SampleGame PhysXProfileSDK.lib)
        target_link_libraries(SampleGame PxTask.lib)
        target_link_libraries(SampleGame PhysXVisualDebuggerSDK.lib)
        target_link_libraries(SampleGame PhysX3MetaData.lib)
        target_link_libraries(SampleGame PhysX3Foundation.lib)
        target_link_libraries(SampleGame PxSceneQuery.lib)
        target_link_libraries(SampleGame PxSimulationController.lib)
        target_link_libraries(SampleGame PhysX3LowLevel.lib)
        target_link_libraries(SampleGame PhysX3LowLevelCloth.lib)
    endif()

    target_link_libraries(SampleGame bcrypt.lib)
    target_link_libraries(SampleGame libboost_thread.lib)
    target_link_libraries(SampleGame libboost_date_time.lib)
    target_link_libraries(SampleGame libboost_chrono.lib)
    target_link_libraries(SampleGame libboost_timer.lib)
    target_link_libraries(SampleGame libboost_filesystem.lib)
    target_link_libraries(SampleGame libboost_unit_test_framework.lib)
    target_link_libraries(SampleGame libboost_json.lib)

    target_link_libraries(SampleGame minizip.lib)
    target_link_libraries(SampleGame nativefiledialog.lib)
    target_link_libraries(SampleGame rttr.lib)
    target_link_libraries(SampleGame tinyxml.lib)
    target_link_libraries(SampleGame yaml-cpp.lib)
    target_link_libraries(SampleGame zlib.lib)
    target_link_libraries(SampleGame zzip.lib)

    if (FB_USE_TBB)
        target_link_libraries(SampleGame tbb_static.lib)
        target_link_libraries(SampleGame tbbmalloc_static.lib)
    endif()
elseif (APPLE)
    # fireblade libraries
    target_link_libraries(SampleGame libFBCore.a)
    target_link_libraries(SampleGame libFBGraphics.a)
    target_link_libraries(SampleGame libFBMesh.a)
    target_link_libraries(SampleGame libFBProcedural.a)
    target_link_libraries(SampleGame libFBPhysx.a)
    target_link_libraries(SampleGame libFBVehicle.a)
    target_link_libraries(SampleGame libFBSQLite.a)
    target_link_libraries(SampleGame libFBOISInput.a)

    if (FB_USE_FREETYPE)
        target_link_libraries(SampleGame freetype.a)
    endif()

    if (FB_GRAPHICS_SYSTEM_OGRENEXT)
        target_link_libraries(SampleGame FBGraphicsOgreNext.a)

        target_link_libraries(SampleGame harfbuzz.a)
        target_link_libraries(SampleGame harfbuzz-subset.a)
        target_link_libraries(SampleGame icucommon.a)
        target_link_libraries(SampleGame sds_library.a)
    elseif (FB_GRAPHICS_SYSTEM_OGRE)
        target_link_libraries(SampleGame FBGraphicsOgre.a)
    endif()

    target_link_libraries(SampleGame debug libassimpd.a optimized libassimp.a)

    if (FB_BUILD_PHYSX)
        target_link_libraries(SampleGame FBPhysx.a)

        target_link_libraries(SampleGame PhysX3.a)
        target_link_libraries(SampleGame PhysX3Extensions.a)
        target_link_libraries(SampleGame PhysX3Common.a)
        target_link_libraries(SampleGame PhysX3Vehicle.a)
        target_link_libraries(SampleGame PhysX3Cooking.a)
        target_link_libraries(SampleGame PhysXProfileSDK.a)
        target_link_libraries(SampleGame PxTask.a)
        target_link_libraries(SampleGame PhysXVisualDebuggerSDK.a)
        target_link_libraries(SampleGame PhysX3MetaData.a)
        target_link_libraries(SampleGame PhysX3Foundation.a)
        target_link_libraries(SampleGame PxSceneQuery.a)
        target_link_libraries(SampleGame PxSimulationController.a)
        target_link_libraries(SampleGame PhysX3LowLevel.a)
        target_link_libraries(SampleGame PhysX3LowLevelCloth.a)
    endif()

    target_link_libraries(SampleGame libboost_thread.a)
    target_link_libraries(SampleGame libboost_date_time.a)
    target_link_libraries(SampleGame libboost_chrono.a)
    target_link_libraries(SampleGame libboost_timer.a)
    target_link_libraries(SampleGame libboost_filesystem.a)
    target_link_libraries(SampleGame libboost_json.a)

    target_link_libraries(SampleGame minizip.a)
    target_link_libraries(SampleGame nativefiledialog.a)
    target_link_libraries(SampleGame rttr.a)
    target_link_libraries(SampleGame tinyxml.a)
    target_link_libraries(SampleGame yaml-cpp.a)
    target_link_libraries(SampleGame zlib.a)
    target_link_libraries(SampleGame zzip.a)

    if (FB_USE_TBB)
        target_link_libraries(SampleGame tbb_static.a)
        target_link_libraries(SampleGame tbbmalloc_static.a)
    endif ()

    #target_link_libraries(SampleGame stdc++ "-framework CoreGraphics" objc)
    #target_link_libraries(SampleGame ${AUDIO_UNIT})
    target_link_libraries(SampleGame stdc++ "-framework Metal" "-framework OpenGL" "-framework Foundation" "-framework QuartzCore" "-framework CoreGraphics" "-framework CoreServices" "-framework Cocoa" "-framework Carbon" "-framework IOKit" objc)
    set_target_properties(SampleGame PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
else ()
    target_link_libraries(SampleGame -Wl,--start-group FBCore.a FBSampleGame.a 
        FBApplication.a.a FBMath.a FBVehicle.a FBData.a FBMesh.a 
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
    set_property(TARGET SampleGame PROPERTY VS_DEBUGGER_WORKING_DIRECTORY 
        "${CMAKE_SOURCE_DIR}/Bin/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/$(Configuration)")
endif()

add_dependencies(SampleGame FBAudio FBCore FBMesh FBSQLite)

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
