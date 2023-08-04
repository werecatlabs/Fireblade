project(Dependencies)

include(ExternalProject)

if (WIN32)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${FB_ROOT}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${FB_ROOT}/Dependencies/lib/windows/${FB_TOOLSET}/${FB_ARCH})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Dependencies/bin/windows/${FB_TOOLSET}/${FB_ARCH})
elseif (APPLE)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${FB_ROOT}/Dependencies/lib/macOS)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${FB_ROOT}/Dependencies/lib/macOS)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Dependencies/bin/macOS)
elseif (UNIX)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${FB_ROOT}/Dependencies/lib/linux)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${FB_ROOT}/Dependencies/lib/linux)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/Dependencies/bin/linux)
endif (WIN32)

if (FB_ENABLE_BOOST)
	ExternalProject_Add(boost
		SOURCE_DIR ${FB_ROOT}/AdditionalLibraries/boost
		CONFIGURE_COMMAND ""
		INSTALL_COMMAND msbuild
		BUILD_COMMAND msbuild /p:PlatformToolset=${FB_TOOLSET} ${FB_ROOT}/AdditionalLibraries/physx/Source/compiler/vc14win32/PhysX.sln
	)
endif()
	
#add_subdirectory(tinybind)

