project(FBMesh)

# define header and source files for the library
file(GLOB HEADER_FILES "../../Include/FBMesh/*.h")
file(GLOB SOURCE_FILES "../../Source/FBMesh/*.cpp")

set(FB_HEADER_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBMesh/../../Include/FBMesh")
set(FB_SOURCE_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBMesh/../../Source/FBMesh")

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
include_directories( ${FB_ROOT}/Dependencies/tinyxml2)
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/zlib )
include_directories( ${FB_ROOT}/AdditionalLibraries/zziplib )
include_directories( ${FB_ROOT}/AdditionalLibraries/ngplant )
include_directories( ../../Include )
include_directories( ${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include/FBMesh/ )
include_directories( ${CMAKE_SOURCE_DIR}/FBEngine/cpp/Include/FBMesh/FBX )

include_directories( ${FB_ROOT}/AdditionalLibraries/fbxsdk/2020.2/include )

if (FB_GRAPHICS_SYSTEM_OGRE)
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include )
    include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/OgreMain/include )

    if(WIN32)
        include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/win32 )
    elseif(APPLE)
        include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/osx )
    else()
        include_directories( ${FB_ROOT}/AdditionalLibraries/ogre/include/linux )
    endif()
endif()

if (FB_USE_PRECOMPILED_HEADERS)
    use_precompiled_header(FBMesh "FBMesh/FBMeshPCH.h" "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBMesh/FBMeshPCH.cpp")
endif()

if (NOT FB_STATIC_LIB)
    if (WIN32)
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/libs/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/${CMAKE_BUILD_TYPE})
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/tbb/${CMAKE_BUILD_TYPE})
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/Dependencies/lib/windows/boost/${CMAKE_BUILD_TYPE})
    elseif (APPLE)
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/libs/macOS/${CMAKE_BUILD_TYPE})
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS)

        link_directories(FBMesh ${FB_ROOT}/Dependencies/lib/macOS/boost)
        link_directories(FBMesh ${FB_ROOT}/Dependencies/lib/macOS/boost/${CMAKE_BUILD_TYPE})
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/Dependencies/lib/macOS/tbb/${CMAKE_BUILD_TYPE})
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS)
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/${CMAKE_BUILD_TYPE})

        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/assimp/${CMAKE_BUILD_TYPE})
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/physx/${CMAKE_BUILD_TYPE})
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/wildmagic/${CMAKE_BUILD_TYPE})
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/${CMAKE_BUILD_TYPE})

        if (FB_GRAPHICS_SYSTEM_OGRENEXT)
            link_directories(FBMesh ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogrenext/${CMAKE_BUILD_TYPE})
            link_directories(FBMesh ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogrenext/dependencies/lib/${CMAKE_BUILD_TYPE})
        elseif (FB_GRAPHICS_SYSTEM_OGRE)
            link_directories(FBMesh ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogre/)
            link_directories(FBMesh ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/macOS/ogre/dependencies/lib/)
            link_directories(FBMesh ${CMAKE_SOURCE_DIR}/AdditionalLibraries/directx/Lib/x64)
        endif ()
    elseif (UNIX)
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/libs/linux/${CMAKE_BUILD_TYPE})
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux)
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/boost)
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/boost/${CMAKE_BUILD_TYPE})
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/boost/${FB_BUILD_TYPE})
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/Dependencies/lib/linux/tbb)
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux)
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/physx/${CMAKE_BUILD_TYPE})
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/physx/${FB_BUILD_TYPE})
        link_directories(FBMesh ${CMAKE_SOURCE_DIR}/AdditionalLibraries/lib/linux/wildmagic)
    endif ()

    if (WIN32)
        #target_link_libraries(FBMesh FBMesh.lib )

    elseif (APPLE)
        target_link_libraries(FBMesh libFBAero.a)
        target_link_libraries(FBMesh libFBApplication.a)
        target_link_libraries(FBMesh libFBFileSystem.a)
        target_link_libraries(FBMesh libFBProcedural.a)  
        target_link_libraries(FBMesh libFBObjectTemplates.a)
        target_link_libraries(FBMesh libFBVehicle.a)
        target_link_libraries(FBMesh libFBAudio.a)
        target_link_libraries(FBMesh libFBTest.a )
        target_link_libraries(FBMesh libFBInput.a )

        target_link_libraries(FBMesh FBGraphics.a)
        target_link_libraries(FBMesh FBRenderUI.a)
        target_link_libraries(FBMesh FBPhysics3.a)

        if (FB_USE_ASSET_IMPORT)
            target_link_libraries(FBMesh FBAssimp.a)
        endif()

        if (FB_GRAPHICS_SYSTEM_OGRENEXT)
            target_link_libraries(FBMesh FBGraphicsOgreNext.a)
            target_link_libraries(FBMesh FreeImage.a)
            target_link_libraries(FBMesh freetype.a)
        elseif(FB_GRAPHICS_SYSTEM_OGRE)
            target_link_libraries(FBMesh FBGraphicsOgre.a)
        elseif(FB_GRAPHICS_SYSTEM_SOFTWARE)
            target_link_libraries(FBMesh libFBGraphicsSoftware.a)
        endif()

        if (FB_BUILD_PHYSX)
            target_link_libraries(FBMesh libFBPhysx.a)
            target_link_libraries(FBMesh libLowLevel.a libLowLevelCloth.a libPhysXProfileSDK.a libPhysX3Common.a libSceneQuery.a libSimulationController.a libPhysX3Cooking.a libPhysX3Extensions.a libPhysX3.a libPhysX3Vehicle.a )
        elseif (FB_BUILD_ODE)
            target_link_libraries(FBMesh libFBODE3.a)
        endif()

        target_link_libraries(FBMesh libFBSQLite.a)

        target_link_libraries(FBMesh libboost_thread.a)
        target_link_libraries(FBMesh libboost_date_time.a)
        target_link_libraries(FBMesh libboost_chrono.a)
        target_link_libraries(FBMesh libboost_timer.a)
        target_link_libraries(FBMesh libboost_filesystem.a)
        target_link_libraries(FBMesh libboost_unit_test_framework.a)
        target_link_libraries(FBMesh libboost_system.a)

        target_link_libraries(FBMesh libPxTask.a)
        target_link_libraries(FBMesh libPhysXVisualDebuggerSDK.a)
        target_link_libraries(FBMesh libPvdRuntime.a)

        target_link_libraries(FBMesh debug libtbb_debug.a optimized libtbb.a)
        target_link_libraries(FBMesh debug libtbbmalloc_debug.a optimized libtbbmalloc.a)

        if (FB_USE_ASSET_IMPORT)
            target_link_libraries(FBMesh FBAssimp.a)
            #target_link_libraries(FBMesh debug libassimpd.a optimized libassimp.a)
        endif()

        if (FB_BUILD_WILDMAGIC5)
            target_link_libraries(FBMesh libWm5Core.a)
            target_link_libraries(FBMesh libWm5Mathematics.a)
        endif()

        target_link_libraries(FBMesh libzlib.a)
        target_link_libraries(FBMesh libzziplib.a)

        target_link_libraries(FBMesh stdc++ "-framework Metal" "-framework OpenGL" "-framework Foundation" "-framework QuartzCore" "-framework CoreGraphics" "-framework CoreServices" "-framework Cocoa" "-framework Carbon" "-framework IOKit" objc)
        set_target_properties(FBMesh PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
    elseif (UNIX)
        if (FB_GRAPHICS_SYSTEM_OGRENEXT)
            target_link_libraries(FBMesh -Wl,--start-group FBMesh.a FBAero.a FBAssimp.a libFBTest.a
                FBApplication.a.a FBMath.a FBState.a FBVehicle.a FBData.a FBMesh.a FBGraphics.a
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

            #target_link_libraries(FBMesh -Wl,--start-group FBGraphicsOgreNext.a ${X11_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS} -Wl,--end-group )

            target_link_libraries(FBMesh freetype.a)
        elseif(FB_GRAPHICS_SYSTEM_SOFTWARE)
            target_link_libraries(FBMesh libFBGraphicsSoftware.a)
        else()
            target_link_libraries(FBMesh -Wl,--start-group FBMesh.a FBAero.a 
                FBApplication.a.a FBMath.a FBState.a FBVehicle.a FBData.a FBMesh.a FBGraphics.a
                libFBObjectTemplates.a libFBVehicle.a libFBProcedural.a libFBSQLite.a libFBPhysx.a FBPhysics3.a FBAudio.a FBRenderUI.a        
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
        endif()

        if (FB_BUILD_WILDMAGIC5)
            target_link_libraries(FBMesh libWm5Core.a)
            target_link_libraries(FBMesh libWm5Mathematics.a)
        endif()
    
        if (FB_USE_ASSET_IMPORT)
            target_link_libraries(FBMesh FBAssimp.a)
        endif()

        target_link_libraries(FBMesh png)
        target_link_libraries(FBMesh Xaw)
        target_link_libraries(FBMesh Xrandr)
        target_link_libraries(FBMesh Xt)
        target_link_libraries(FBMesh xcb)
        target_link_libraries(FBMesh ${X11_LIBRARIES})
        target_link_libraries(FBMesh ${CMAKE_THREAD_LIBS_INIT} )
        target_link_libraries(FBMesh ${CMAKE_DL_LIBS})
    endif ()
endif()

if (FB_STATIC_LIB)
    add_library(FBMesh STATIC ${HEADER_FILES} ${SOURCE_FILES})
else ()
    add_library(FBMesh SHARED ${HEADER_FILES} ${SOURCE_FILES})
endif()

set_target_properties (FBMesh PROPERTIES FOLDER Engine/Cpp/Core)
