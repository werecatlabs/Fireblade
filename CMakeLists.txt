cmake_minimum_required(VERSION 3.13)
#set(CMAKE_USER_MAKE_RULES_OVERRIDE "CMakeRules.cmake")

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.11" CACHE STRING "Minimum OS X deployment version")
endif()

project(FB C CXX)
find_package (Threads REQUIRED)

set(FB_ROOT "${CMAKE_SOURCE_DIR}")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(PrecompiledHeader)
include(Util)

if(UNIX)
    #include(FindXCB)
endif()

# determine if we are compiling for a 32bit or 64bit system
include(CheckTypeSize)
CHECK_TYPE_SIZE("void*" FB_PTR_SIZE BUILTIN_TYPES_ONLY)
if (FB_PTR_SIZE EQUAL 8)
  set(FB_PLATFORM_X64 TRUE)
  set(FB_ARCH "x64")
else ()
  set(FB_PLATFORM_X64 FALSE)
  set(FB_ARCH "x86")
endif ()

# determine system endianess
if (MSVC OR ANDROID OR EMSCRIPTEN OR APPLE_IOS)
  # This doesn't work on VS 2010
  # MSVC only builds for intel anyway
  # all IOS devices are little endian
  set(FB_TEST_BIG_ENDIAN FALSE)
else()
  include(TestBigEndian)
  test_big_endian(FB_TEST_BIG_ENDIAN)
endif()

if (FB_TEST_BIG_ENDIAN)
  set(FB_CONFIG_BIG_ENDIAN 1)
else ()
  set(FB_CONFIG_LITTLE_ENDIAN 1)
endif ()


if(WIN32)
	if (MSVC_TOOLSET_VERSION EQUAL "141")
		set(FB_TOOLSET "v141")
	elseif(MSVC_TOOLSET_VERSION EQUAL "142")
		set(FB_TOOLSET "v142")
    elseif(MSVC_TOOLSET_VERSION EQUAL "143")
		set(FB_TOOLSET "v143")
	endif()
elseif(APPLE)
    set(FB_TOOLSET "")
else()
    set(FB_TOOLSET "")
endif()

string( TOLOWER "${CMAKE_BUILD_TYPE}" FB_BUILD_TYPE )

set(FB_UNITY3D_VERSION "2021.3.7f1")

option(FB_STATIC_LINK_CRT "Build with the crt statically linked" ON)
option(FB_STATIC_LIB "Build the engine as shared libs" ON)

if (WIN32)
	if (CMAKE_GENERATOR MATCHES "Visual Studio")
		option(FB_USE_PRECOMPILED_HEADERS "Build with precompiled headers" ON)
    else ()
        option(FB_USE_PRECOMPILED_HEADERS "Build with precompiled headers" OFF)
    endif()
else()
    option(FB_USE_PRECOMPILED_HEADERS "Build with precompiled headers" OFF)
endif()

option(FB_USE_CPP20 "Build with c++20" OFF)
option(FB_USE_FORCE_INLINE "Build with force inline" ON)
option(FB_AUTO_LINK "Build with force inline" OFF)
option(FB_BUILD_DEPENDENCIES "Build dependencies" OFF)
option(FB_BUILD_ADDITIONALLIBRARIES "Build additional libraries" OFF)
option(FB_BUILD_ENGINE "Build engine" ON)
option(FB_BUILD_PLUGINS "Build plugins" ON)
option(FB_BUILD_TESTS "Build tests" ON)
option(FB_BUILD_TOOLS "Build tools" ON)
option(FB_REPLACE_MACROS "Builds with macros replaced with code" OFF)
option(FB_EXCEPTIONS "Builds with exceptions" ON)
option(FB_DOUBLE_PRECISION "Build DOUBLE_PRECISION" OFF)
option(FB_USE_PTHREADS "Build with pthreads" OFF)
option(FB_ENABLE_TRACE "Build with stack tracing" OFF)
option(FB_TRACK_REFERENCES "track references" OFF)
option(FB_TRACK_STRONG_REFERENCES "track strong references" OFF)
option(FB_TRACK_WEAK_REFERENCES "track weak references" OFF)
option(FB_USE_MEMORY_POOL "Use memory pool" ON)
option(FB_USE_HASH32 "Use 32 bit hash (64 bit default)" OFF)


if (WIN32 OR APPLE)
    option(FB_USE_LOCK_FREE_ATOMIC  "Build with lock free atomics" ON)
else ()
    option(FB_USE_LOCK_FREE_ATOMIC  "Build with lock free atomics" OFF)
endif()

if (WIN32)
    option(FB_USE_HOUDINI "Build with houdini" ON)
else()
    option(FB_USE_HOUDINI "Build with houdini" OFF)
endif()

if (APPLE)
    option(FB_BUILD_APPLE_IOS "Build Apple iOS" OFF)
endif()

option(FB_ENABLE_MEMORY_TRACKER "Enable memory tracking" OFF)
option(FB_ENABLE_PROFILER "Enable profiler" OFF)
option(FB_USE_FIREBLADE_CONTAINERS "Use fireblade replacement containers" OFF)
option(FB_USE_TBB "Enable tbb" ON)
option(FB_USE_ONETBB "Enable one tbb" OFF)
option(FB_ENABLE_BOOST "Enable boost" ON)
option(FB_ENABLE_LUA "Enable lua" ON)
option(FB_ENABLE_PYPY "Enable pypy" OFF)
option(FB_ENABLE_PYTHON "Enable python" OFF)
option(FB_ENABLE_ZZIP "Enable zzip" ON)
option(FB_USE_OPENSSL "Enable open ssl" OFF)
option(FB_USE_STD_ATOMIC "Use std atomic" OFF)
option(FB_USE_FREETYPE "Enable freetype" OFF)

option(FB_BUILD_FINAL "Build final" OFF)
option(FB_BUILD_EDITOR_PLUGIN "Editor library" OFF)
option(FB_BUILD_PHYSICS_DEBUG "Build with physics debug code" OFF)
option(FB_BUILD_MEMORY_DEBUG "Build version 3" OFF)
option(FB_ENABLE_ACTIVETRANSFORMS "ENABLE_ACTIVETRANSFORMS" ON)
option(FB_USE_TRANSFORM_THREAD "USE_TRANSFORM_THREAD" ON)
option(FB_PHYSICS_THREADED "Physics Threaded" ON)
option(FB_USE_SQL_CODEC "Use sql codec" ON)
option(FB_ENABLE_MULTITHREADED_INPUT "Enable multithreaded input" OFF)

option(FB_USE_FBXSDK "Use fbx sdk" OFF)

if (WIN32)
    option(FB_USE_ASSET_IMPORT "Use asset import" ON)
elseif(APPLE)
    option(FB_USE_ASSET_IMPORT "Use asset import" ON)
else()
    option(FB_USE_ASSET_IMPORT "Use asset import" OFF)
endif()

option(FB_ENABLE_DEBUG_TRACE "Enable debug check" OFF)
option(FB_ENABLE_HEAP_DEBUG "Enable memory debug" OFF)
option(FB_ENABLE_PTR_EXCEPTIONS "Enable pointer exceptions" ON)
option(FB_ENABLE_THREADING "Build with threading" ON)
option(FB_PROFILING "Build with profiling enabled" OFF)

option(FB_EDITOR_TESTS "Build editor with unit tests" ON)

option(FB_BUILD_UNITTESTS "build unit tests" ON)
option(FB_BUILD_TESTS "build unit tests" ON)
option(FB_USE_CUSTOM_NEW_DELETE "custom new delete" ON)
option(FB_OBJECT_ALIGNED_ALLOC "use aligned allocation" ON)

option(FB_CODEC_FREEIMAGE "use free image codec" OFF)
option(FB_CODEC_STBI "use stbi codec" ON)

if (FB_USE_CPP20)
    set(CMAKE_CXX_STANDARD 20)
else()
    set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (FB_PLATFORM_X64)
    if (WIN32)
        option(FB_GRAPHICS_SYSTEM_CLAW "Build claw" OFF)
        option(FB_GRAPHICS_SYSTEM_OGRE "Build ogre" ON)
        option(FB_GRAPHICS_SYSTEM_OGRENEXT "Build ogre next" OFF)
        option(FB_GRAPHICS_SYSTEM_SOFTWARE "Build software renderer" OFF)
        option(FB_BUILD_RENDERER_NULL "Build null" OFF)

        option(FB_GRAPHICS_SYSTEM_USE_STATIC_LIBS "Use static libraries" OFF)

        if (FB_GRAPHICS_SYSTEM_OGRENEXT)
            option(FB_BUILD_RENDERER_DX9 "Build dx9" OFF)
            option(FB_BUILD_RENDERER_DX11 "Build dx11" ON)
            option(FB_BUILD_RENDERER_OPENGL "Build open gl" OFF)
            option(FB_BUILD_RENDERER_GL3PLUS "Build open gl 3 plus" OFF)
            option(FB_BUILD_RENDERER_GLES2 "Build open gles2" OFF)
            option(FB_BUILD_RENDERER_VULKAN "Build vulkan" OFF)
            option(FB_BUILD_RENDERER_METAL "Build metal" OFF)
		    
        elseif (FB_GRAPHICS_SYSTEM_OGRE)
            option(FB_BUILD_RENDERER_DX9 "Build dx9" OFF)
            option(FB_BUILD_RENDERER_DX11 "Build dx11" OFF)
            option(FB_BUILD_RENDERER_OPENGL "Build open gl" ON)
            option(FB_BUILD_RENDERER_GL3PLUS "Build open gl 3 plus" OFF)
            option(FB_BUILD_RENDERER_GLES2 "Build open gles2" OFF)
            option(FB_BUILD_RENDERER_VULKAN "Build vulkan" OFF)
            option(FB_BUILD_RENDERER_METAL "Build metal" OFF)
        endif()
    elseif (APPLE)
        option(FB_GRAPHICS_SYSTEM_CLAW "Build claw" OFF)
        option(FB_GRAPHICS_SYSTEM_OGRE "Build ogre" ON)
        option(FB_GRAPHICS_SYSTEM_OGRENEXT "Build ogre next" OFF)
        option(FB_GRAPHICS_SYSTEM_SOFTWARE "Build software renderer" OFF)
        option(FB_BUILD_RENDERER_NULL "Build null" OFF)

        option(FB_GRAPHICS_SYSTEM_USE_STATIC_LIBS "Use static libraries" OFF)

        if (FB_GRAPHICS_SYSTEM_OGRENEXT)
            option(FB_BUILD_RENDERER_DX9 "Build dx9" OFF)
            option(FB_BUILD_RENDERER_DX11 "Build dx11" OFF)
            option(FB_BUILD_RENDERER_OPENGL "Build open gl" OFF)
            option(FB_BUILD_RENDERER_GL3PLUS "Build open gl 3 plus" OFF)
            option(FB_BUILD_RENDERER_VULKAN "Build vulkan" OFF)
            option(FB_BUILD_RENDERER_METAL "Build metal" ON)
        elseif (FB_GRAPHICS_SYSTEM_OGRE)
            option(FB_BUILD_RENDERER_DX9 "Build dx9" OFF)
            option(FB_BUILD_RENDERER_DX11 "Build dx11" OFF)
            option(FB_BUILD_RENDERER_OPENGL "Build open gl" ON)
            option(FB_BUILD_RENDERER_GL3PLUS "Build open gl 3 plus" OFF)
            option(FB_BUILD_RENDERER_VULKAN "Build vulkan" OFF)
            option(FB_BUILD_RENDERER_METAL "Build metal" OFF)
        endif()
    elseif (UNIX)
        option(FB_GRAPHICS_SYSTEM_CLAW "Build claw" OFF)
        option(FB_GRAPHICS_SYSTEM_OGRE "Build ogre" ON)
        option(FB_GRAPHICS_SYSTEM_OGRENEXT "Build ogre next" OFF)
        option(FB_GRAPHICS_SYSTEM_SOFTWARE "Build software renderer" OFF)
        option(FB_BUILD_RENDERER_NULL "Build null" OFF)

        option(FB_GRAPHICS_SYSTEM_USE_STATIC_LIBS "Use static libraries" OFF)

        option(FB_BUILD_RENDERER_DX9 "Build dx9" OFF)
        option(FB_BUILD_RENDERER_DX11 "Build dx11" OFF)
        option(FB_BUILD_RENDERER_OPENGL "Build open gl" ON)
        option(FB_BUILD_RENDERER_GL3PLUS "Build open gl 3 plus" OFF)
        option(FB_BUILD_RENDERER_VULKAN "Build vulkan" OFF)
        option(FB_BUILD_RENDERER_METAL "Build metal" OFF)
    else ()
        option(FB_GRAPHICS_SYSTEM_CLAW "Build claw" OFF)
        option(FB_GRAPHICS_SYSTEM_OGRE "Build ogre" ON)
        option(FB_GRAPHICS_SYSTEM_OGRENEXT "Build ogre next" OFF)
        option(FB_GRAPHICS_SYSTEM_SOFTWARE "Build software renderer" OFF)
        option(FB_BUILD_RENDERER_NULL "Build null" OFF)

        option(FB_GRAPHICS_SYSTEM_USE_STATIC_LIBS "Use static libraries" OFF)

        option(FB_BUILD_RENDERER_DX9 "Build dx9" OFF)
        option(FB_BUILD_RENDERER_DX11 "Build dx11" OFF)
        option(FB_BUILD_RENDERER_OPENGL "Build open gl" OFF)
        option(FB_BUILD_RENDERER_GL3PLUS "Build open gl 3 plus" OFF)
        option(FB_BUILD_RENDERER_VULKAN "Build vulkan" OFF)
        option(FB_BUILD_RENDERER_METAL "Build metal" OFF)
    endif ()
else () 
    if (WIN32)
        option(FB_GRAPHICS_SYSTEM_CLAW "Build claw" OFF)
        option(FB_GRAPHICS_SYSTEM_OGRE "Build ogre" OFF)
        option(FB_GRAPHICS_SYSTEM_OGRENEXT "Build ogre next" ON)
        option(FB_GRAPHICS_SYSTEM_SOFTWARE "Build software renderer" OFF)
        option(FB_BUILD_RENDERER_NULL "Build null" OFF)

        option(FB_GRAPHICS_SYSTEM_USE_STATIC_LIBS "Use static libraries" OFF)

        option(FB_BUILD_RENDERER_DX9 "Build dx9" OFF)
        option(FB_BUILD_RENDERER_DX11 "Build dx11" ON)
        option(FB_BUILD_RENDERER_OPENGL "Build open gl" ON)
        option(FB_BUILD_RENDERER_VULKAN "Build vulkan" OFF)
        option(FB_BUILD_RENDERER_METAL "Build metal" ON)
    elseif (APPLE)
        option(FB_GRAPHICS_SYSTEM_CLAW "Build claw" OFF)
        option(FB_GRAPHICS_SYSTEM_OGRE "Build ogre" OFF)
        option(FB_GRAPHICS_SYSTEM_OGRENEXT "Build ogre next" ON)
        option(FB_GRAPHICS_SYSTEM_SOFTWARE "Build software renderer" OFF)
        option(FB_BUILD_RENDERER_NULL "Build null" OFF)

        option(FB_GRAPHICS_SYSTEM_USE_STATIC_LIBS "Use static libraries" OFF)

        option(FB_BUILD_RENDERER_DX9 "Build dx9" OFF)
        option(FB_BUILD_RENDERER_DX11 "Build dx11" OFF)
        option(FB_BUILD_RENDERER_OPENGL "Build open gl" OFF)
        option(FB_BUILD_RENDERER_VULKAN "Build vulkan" OFF)
        option(FB_BUILD_RENDERER_METAL "Build metal" ON)
    elseif (UNIX)
        option(FB_GRAPHICS_SYSTEM_CLAW "Build claw" OFF)
        option(FB_GRAPHICS_SYSTEM_OGRE "Build ogre" ON)
        option(FB_GRAPHICS_SYSTEM_OGRENEXT "Build ogre next" OFF)
        option(FB_GRAPHICS_SYSTEM_SOFTWARE "Build software renderer" OFF)
        option(FB_BUILD_RENDERER_NULL "Build null" OFF)

        option(FB_GRAPHICS_SYSTEM_USE_STATIC_LIBS "Use static libraries" OFF)

        option(FB_BUILD_RENDERER_DX9 "Build dx9" OFF)
        option(FB_BUILD_RENDERER_DX11 "Build dx11" OFF)
        option(FB_BUILD_RENDERER_OPENGL "Build open gl" OFF)
        option(FB_BUILD_RENDERER_VULKAN "Build vulkan" OFF)
        option(FB_BUILD_RENDERER_METAL "Build metal" OFF)
    else ()
        option(FB_GRAPHICS_SYSTEM_CLAW "Build claw" OFF)
        option(FB_GRAPHICS_SYSTEM_OGRE "Build ogre" ON)
        option(FB_GRAPHICS_SYSTEM_OGRENEXT "Build ogre next" OFF)
        option(FB_GRAPHICS_SYSTEM_SOFTWARE "Build software renderer" OFF)
        option(FB_BUILD_RENDERER_NULL "Build null" OFF)

        option(FB_GRAPHICS_SYSTEM_USE_STATIC_LIBS "Use static libraries" OFF)

        option(FB_BUILD_RENDERER_DX9 "Build dx9" OFF)
        option(FB_BUILD_RENDERER_DX11 "Build dx11" OFF)
        option(FB_BUILD_RENDERER_OPENGL "Build open gl" OFF)
        option(FB_BUILD_RENDERER_VULKAN "Build vulkan" OFF)
        option(FB_BUILD_RENDERER_METAL "Build metal" OFF)
    endif ()
endif()

if (WIN32)
    option(FB_BUILD_OGRENEXT_DEPRECATED "Build ogre next deprecated" OFF)
elseif (APPLE)
    option(FB_BUILD_OGRENEXT_DEPRECATED "Build ogre next deprecated" OFF)
elseif (UNIX)
    option(FB_BUILD_OGRENEXT_DEPRECATED "Build ogre next deprecated" OFF)
endif()

option(FB_BUILD_SDL2 "Build sdl2" OFF)
option(FB_BUILD_CEGUI "Build ce gui" OFF)
option(FB_BUILD_IMGUI "Build imgui" ON)

if (WIN32)
    option(FB_BUILD_WXWIDGETS "Build wxwidgets" ON)
elseif (APPLE)
    option(FB_BUILD_WXWIDGETS "Build wxwidgets" OFF)
elseif (UNIX)
    option(FB_BUILD_WXWIDGETS "Build wxwidgets" OFF)
endif()

option(FB_BUILD_OISINPUT "Build ois" ON)
option(FB_BUILD_DIRECTSHOW "Build direct show" OFF)
option(FB_BUILD_FMOD "Build fmod" ON)
option(FB_BUILD_FILEWATCHER "build file watcher" ON)

if (WIN32)
    option(FB_BUILD_EDITOR "Build editor" ON)
elseif (APPLE)
    option(FB_BUILD_EDITOR "Build editor" ON)
elseif (UNIX)
    option(FB_BUILD_EDITOR "Build editor" ON)
else ()
    option(FB_BUILD_EDITOR "Build editor" OFF)
endif ()

option(FB_BUILD_PLUGINS "Build plugins" ON)
option(FB_BUILD_PLUGIN_AUDIO "Build audio plugin" OFF)
option(FB_BUILD_PLUGIN_UNITY "Build engine plugin" ON)
option(FB_BUILD_PLUGIN_UNREAL "Build engine plugin" ON)

option(FB_BUILD_BULLETPHYSICS "Build bullet physics" OFF)
option(FB_BUILD_ODE "Build ODE physics" OFF)
option(FB_BUILD_PHYSICS3 "Build physics" ON)
option(FB_BUILD_PHYSX "Build physx" ON)
option(FB_BUILD_PHYSX_4 "Build physx 4" OFF)

option(FB_BUILD_SQLITE "Build sqlite" ON)

if(WIN32)
    option(FB_BUILD_WILDMAGIC5 "Build wild magic 5" ON)
elseif (APPLE)
    option(FB_BUILD_WILDMAGIC5 "Build wild magic 5" ON)
elseif(UNIX)
    option(FB_BUILD_WILDMAGIC5 "Build wild magic 5" OFF)
else()
    option(FB_BUILD_WILDMAGIC5 "Build wild magic 5" OFF)
endif()

option(FB_USE_C_MATH "use c math" ON)
option(FB_USE_NEON_MATH "use neon math" OFF)
option(FB_USE_SIMD "use simd" OFF)
option(FB_RESTRICT_ALIASING "restrict aliasing" ON)

if(WIN32)
    if (FB_STATIC_LINK_CRT)
        set(FB_STATIC_LINK_CRT_TYPE "MT")
    else()
        set(FB_STATIC_LINK_CRT_TYPE "MD")
    endif()
else()
    set(FB_STATIC_LINK_CRT_TYPE "")
endif()

if (FB_USE_HOUDINI)
    list( APPEND CMAKE_PREFIX_PATH "C:/Program Files/Side Effects Software/Houdini 19.0.498/toolkit/cmake" )
	find_package( Houdini REQUIRED )
endif()

# Set up iOS overrides.
if (FB_BUILD_APPLE_IOS)
    # Set build variables
    set(XCODE_ATTRIBUTE_SDKROOT iphoneos)
    if(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
        execute_process(COMMAND xcodebuild -version -sdk "${XCODE_ATTRIBUTE_SDKROOT}" Path | head -n 1 OUTPUT_VARIABLE CMAKE_OSX_SYSROOT)
        string(REGEX REPLACE "(\r?\n)+$" "" CMAKE_OSX_SYSROOT "${CMAKE_OSX_SYSROOT}")
    else()
        set(CMAKE_OSX_SYSROOT iphoneos)
    endif()

    set(CMAKE_EXE_LINKER_FLAGS "-framework Foundation -framework CoreGraphics -framework QuartzCore -framework UIKit")
    set( CMAKE_XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES )

    set(XCODE_ATTRIBUTE_GCC_UNROLL_LOOPS "YES")
    set(XCODE_ATTRIBUTE_LLVM_VECTORIZE_LOOPS "YES")
    set(XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
    set(XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES")
    #set(OGRE_BUILD_RENDERSYSTEM_GLES2 TRUE CACHE BOOL "Forcing OpenGL ES 2 RenderSystem for iOS" FORCE)
    set(OGRE_STATIC TRUE CACHE BOOL "Forcing static build for iOS" FORCE)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourcompany.\${PRODUCT_NAME:rfc1034identifier}")
    set( CMAKE_XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2" )

    set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_INCLUDING_64_BIT)")
    string (REPLACE "-msse2" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string (REPLACE "-msse" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
elseif(WIN32)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast")
   #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Qvec-report:2")
   
   if (NOT FB_PLATFORM_X64)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
   endif()

   if (FB_STATIC_LINK_CRT)
        if(FB_REPLACE_MACROS)
            set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /GF /Ob0 /Od /EP /P /C /W3")
        else()
            set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MP /MTd /Zi /GF /Ob0 /Od /W3")
        endif()

        set(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /MP /Zi /O2 /Ob2 /D NDEBUG /W3")
        set(CMAKE_CXX_FLAGS_RELEASE "/MT /MP /O2 /Ob2 /Zi /GF /D NDEBUG /W3")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /MP /Zi /GF /Ob0 /Od /D NDEBUG /W3")
   else()
        if(FB_REPLACE_MACROS)
            set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MDd /Zi /Ob0 /Od /EP /P /C /W3")
        else()
            set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MP /MDd /Zi /Ob0 /Od /W3")
        endif()

        set(CMAKE_CXX_FLAGS_MINSIZEREL "/MD /MP /Zi /O2 /Ob2 /D NDEBUG /W3")
        set(CMAKE_CXX_FLAGS_RELEASE "/MD /MP /O2 /Ob2 /Zi /D NDEBUG /W3")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MD /MP /Zi /Ob0 /Ob0 /D NDEBUG /W3")
   endif()
elseif (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse2 -msse3")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -w -allow-multiple-definition")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-O3 -finline-functions -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -finline-functions -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -w -finline-functions -DNDEBUG -allow-multiple-definition")
else()
    #set(CMAKE_CXX_FLAGS_INIT "-w")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse2 -msse3 -w")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse2 -msse3 -Wno-everything")    
    set(CMAKE_CXX_FLAGS_DEBUG "-g -w")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-O3 -finline-functions -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -finline-functions -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -finline-functions -DNDEBUG")
endif()

if(MSVC)
  # Force to always compile with W4
  #if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
  #  string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  #else()
  #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  #endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsyntax-only")
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

if (WIN32)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${FB_ROOT}/libs/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/${CMAKE_BUILD_TYPE})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${FB_ROOT}/libs/windows/${FB_TOOLSET}/${FB_ARCH}/${FB_STATIC_LINK_CRT_TYPE}/${CMAKE_BUILD_TYPE})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/bin/windows/${FB_TOOLSET}/${FB_ARCH}/${CMAKE_BUILD_TYPE})
elseif (APPLE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${FB_ROOT}/libs/macOS/${CMAKE_BUILD_TYPE})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${FB_ROOT}/libs/macOS/${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/bin/macOS/${CMAKE_BUILD_TYPE})
elseif (UNIX)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${FB_ROOT}/libs/linux/${CMAKE_BUILD_TYPE})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${FB_ROOT}/libs/linux/${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FB_ROOT}/bin/linux/${CMAKE_BUILD_TYPE})
endif ()

if (FB_BUILD_DEPENDENCIES)
    add_subdirectory(Dependencies)
endif()

if (FB_BUILD_ADDITIONALLIBRARIES)
    add_subdirectory(AdditionalLibraries)
endif()

if (FB_BUILD_ENGINE)
    source_group(./FBEngine FBEngine)
    add_subdirectory(FBEngine)
endif()

if (FB_BUILD_PLUGINS)
    #add_subdirectory(Plugins)
endif()

#add_subdirectory(Samples)
 
if (FB_BUILD_TESTS)
    #add_subdirectory(Tests)
endif()

if (FB_BUILD_TOOLS)
    add_subdirectory(Tools)
endif()
