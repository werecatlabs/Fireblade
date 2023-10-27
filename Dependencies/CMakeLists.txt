set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(cpp_defines)
include(cpp_compiler_options)

if (WIN32)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${FB_ROOT}/libs/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/${CMAKE_BUILD_TYPE})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${FB_ROOT}/libs/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/${CMAKE_BUILD_TYPE})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/bin/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/${CMAKE_BUILD_TYPE})
elseif (APPLE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${FB_ROOT}/libs/macOS/${CMAKE_BUILD_TYPE})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${FB_ROOT}/libs/macOS/${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/bin/macOS/${CMAKE_BUILD_TYPE})
elseif (UNIX)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${FB_ROOT}/libs/linux/${CMAKE_BUILD_TYPE})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${FB_ROOT}/libs/linux/${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/bin/linux/${CMAKE_BUILD_TYPE})
endif (WIN32)

add_compile_definitions (PX_PHYSX_STATIC_LIB)
add_compile_definitions (PX_PHYSX_CORE_STATIC_LIB)
add_compile_definitions (PX_FOUNDATION_NO_EXPORTS)
add_compile_definitions (PX_COOKING)
#add_compile_definitions (PX_CALL_CONV)

if(FB_BUILD_PHYSICS_DEBUG)
	add_compile_definitions(
		$<$<CONFIG:Debug>:PX_SUPPORT_VISUAL_DEBUGGER>
	)
	
	add_compile_definitions(
		$<$<CONFIG:Debug>:PX_DEBUG>
	)
	
	add_compile_definitions(
		$<$<CONFIG:Debug>:PX_CHECKED>
	)
	
	add_compile_definitions(
		$<$<CONFIG:Debug>:PX_NVTX>
	)
endif()

# Set the BOOST_INSTALL_LAYOUT
set(BOOST_INSTALL_LAYOUT "SYSTEM" CACHE STRING "Boost install layout")
	
add_subdirectory(boost)
add_subdirectory(FreeImage)
add_subdirectory(freetype)
#add_subdirectory(freetype2)
add_subdirectory(imgui)
add_subdirectory(minizip)
add_subdirectory(nativefiledialog)
add_subdirectory(ois)
add_subdirectory(physx)
add_subdirectory(rttr)
add_subdirectory(simplefilewatcher)
add_subdirectory(tinyxml)
add_subdirectory(tinyxml2)
add_subdirectory(tbb)
add_subdirectory(yaml-cpp)
add_subdirectory(zlib)
add_subdirectory(zzip)

add_subdirectory( icu )
add_subdirectory( harfbuzz )
add_subdirectory( sds_library )

set_target_properties (boost_assert PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_atomic PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_chrono PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_container PROPERTIES FOLDER Dependencies/boost)

set_target_properties (boost_context PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_contract PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_core PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_coroutine PROPERTIES FOLDER Dependencies/boost)

set_target_properties (boost_date_time PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_exception PROPERTIES FOLDER Dependencies/boost)

set_target_properties (boost_fiber PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_fiber_numa PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_filesystem PROPERTIES FOLDER Dependencies/boost)

set_target_properties (boost_graph PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_iostreams PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_json PROPERTIES FOLDER Dependencies/boost)

set_target_properties (boost_locale PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_log PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_nowide PROPERTIES FOLDER Dependencies/boost)

set_target_properties (boost_prg_exec_monitor PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_program_options PROPERTIES FOLDER Dependencies/boost)

set_target_properties (boost_log_setup PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_random PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_serialization PROPERTIES FOLDER Dependencies/boost)

set_target_properties (boost_stacktrace_basic PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_stacktrace_noop PROPERTIES FOLDER Dependencies/boost)

if (WIN32)
	set_target_properties (boost_stacktrace_windbg PROPERTIES FOLDER Dependencies/boost)
	set_target_properties (boost_stacktrace_windbg_cached PROPERTIES FOLDER Dependencies/boost)
endif()

set_target_properties (boost_system PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_test_exec_monitor PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_thread PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_timer PROPERTIES FOLDER Dependencies/boost)

set_target_properties (boost_type_erasure PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_unit_test_framework PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_url PROPERTIES FOLDER Dependencies/boost)

set_target_properties (boost_variant2 PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_wave PROPERTIES FOLDER Dependencies/boost)
set_target_properties (boost_wserialization PROPERTIES FOLDER Dependencies/boost)

set_target_properties (FreeImage PROPERTIES FOLDER Dependencies/FreeImage)
set_target_properties (freetype PROPERTIES FOLDER Dependencies/freetype)
#set_target_properties (freetype2 PROPERTIES FOLDER Dependencies/freetype)

set_target_properties (minizip PROPERTIES FOLDER Dependencies/minizip)
set_target_properties (nativefiledialog PROPERTIES FOLDER Dependencies/nativefiledialog)
set_target_properties (OIS PROPERTIES FOLDER Dependencies/ois)
set_target_properties (FFConsoleApp PROPERTIES FOLDER Dependencies/ois)
set_target_properties (ConsoleApp PROPERTIES FOLDER Dependencies/ois)

set_target_properties (PhysX3 PROPERTIES FOLDER Dependencies/physx)
set_target_properties (PhysX3Extensions PROPERTIES FOLDER Dependencies/physx)
set_target_properties (PhysX3Common PROPERTIES FOLDER Dependencies/physx)
set_target_properties (PhysX3Cooking PROPERTIES FOLDER Dependencies/physx)
set_target_properties (PhysX3Vehicle PROPERTIES FOLDER Dependencies/physx)
set_target_properties (PhysXProfileSDK PROPERTIES FOLDER Dependencies/physx)
set_target_properties (PhysXVisualDebuggerSDK PROPERTIES FOLDER Dependencies/physx)
set_target_properties (PxTask PROPERTIES FOLDER Dependencies/physx)

set_target_properties (PhysX3Foundation PROPERTIES FOLDER Dependencies/physx)
set_target_properties (PhysX3LowLevel PROPERTIES FOLDER Dependencies/physx)
set_target_properties (PhysX3LowLevelCloth PROPERTIES FOLDER Dependencies/physx)
set_target_properties (PhysX3MetaData PROPERTIES FOLDER Dependencies/physx)
set_target_properties (PxSceneQuery PROPERTIES FOLDER Dependencies/physx)
set_target_properties (PxSimulationController PROPERTIES FOLDER Dependencies/physx)

set_target_properties (rttr PROPERTIES FOLDER Dependencies/rttr)
set_target_properties (tinyxml PROPERTIES FOLDER Dependencies/tinyxml)
set_target_properties (tinyxml2 PROPERTIES FOLDER Dependencies/tinyxml2)
set_target_properties( tbb tbb_def_files  tbbmalloc  tbbmalloc_proxy  PROPERTIES FOLDER Dependencies/tbb )
set_target_properties( tbb_static tbb_def_files tbbmalloc_static tbbmalloc_proxy_static PROPERTIES FOLDER Dependencies/tbb )
set_target_properties (yaml-cpp PROPERTIES FOLDER Dependencies/yaml-cpp)
set_target_properties (zlib PROPERTIES FOLDER Dependencies/zlib)
set_target_properties (zzip PROPERTIES FOLDER Dependencies/zzip)

set_target_properties (imgui PROPERTIES FOLDER Dependencies/imgui)
set_target_properties (simplefilewatcher PROPERTIES FOLDER Dependencies/simplefilewatcher)

set_target_properties (icucommon PROPERTIES FOLDER Dependencies/icu)
set_target_properties (sds_library PROPERTIES FOLDER Dependencies/sds_library)
set_target_properties (harfbuzz PROPERTIES FOLDER Dependencies/harfbuzz)
set_target_properties (harfbuzz-subset PROPERTIES FOLDER Dependencies/harfbuzz)