add_compile_definitions (FB_TYPE_MANAGER_NUM_TYPES=2048)
add_compile_definitions (FB_GC_MAX_SHARED_OBJECTS=32768)
add_compile_definitions (FB_GC_MAX_OBJECTS_REMOVED=1024)
add_compile_definitions (FB_UNITY_BUILD=1)
add_compile_definitions (FB_SIMD_ALIGNMENT=16)
add_compile_definitions (FB_MEDIA_PATH="${CMAKE_SOURCE_DIR}/bin/media")
add_compile_definitions (_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)

if(WIN32)
    add_compile_definitions (_WIN32_WINNT=0x0601)
endif()

if (FB_PLATFORM_X64)
    if (WIN32)
        add_compile_definitions (_AMD64_)
    endif() 
else ()
    if (WIN32)
        add_compile_definitions (_X86_)
    endif() 
endif()

if (FB_USE_HASH32)
    add_compile_definitions (FB_USE_HASH32=1)
else ()
    add_compile_definitions (FB_USE_HASH32=0)
endif()

if (FB_ENABLE_MULTITHREADED_INPUT)
    add_compile_definitions (FB_ENABLE_MULTITHREADED_INPUT=1)
else ()
    add_compile_definitions (FB_ENABLE_MULTITHREADED_INPUT=0)
endif()

if (FB_PROFILING)
    add_compile_definitions (FB_PROFILING=1)
else ()
    add_compile_definitions (FB_PROFILING=0)
endif()

if (FB_USE_C_MATH)
    add_compile_definitions (FB_USE_C_MATH=1)
else ()
    add_compile_definitions (FB_USE_C_MATH=0)
endif()


if (FB_USE_NEON_MATH)
    add_compile_definitions (FB_USE_NEON_MATH=1)
else ()
    add_compile_definitions (FB_USE_NEON_MATH=0)
endif()

if (FB_USE_SIMD)
    add_compile_definitions (FB_USE_SIMD=1)
else ()
    add_compile_definitions (FB_USE_SIMD=0)
endif()

if (FB_RESTRICT_ALIASING)
    add_compile_definitions (FB_RESTRICT_ALIASING=1)
else ()
    add_compile_definitions (FB_RESTRICT_ALIASING=0)
endif()

if (FB_BUILD_SDL2)
    add_compile_definitions (FB_BUILD_SDL2=1)
else ()
    add_compile_definitions (FB_BUILD_SDL2=0)
endif()


if (FB_STATIC_LINK_CRT)
    add_compile_definitions (FB_STATIC_LINK_CRT=1)
else ()
    add_compile_definitions (FB_STATIC_LINK_CRT=0)
endif()

if (FB_USE_LOCK_FREE_ATOMIC)
    add_compile_definitions (FB_USE_LOCK_FREE_ATOMIC=1)
else ()
    add_compile_definitions (FB_USE_LOCK_FREE_ATOMIC=0)
endif()

if (FB_STATIC_LIB)
    add_compile_definitions (_FB_STATIC_LIB_)
endif()

if (WIN32)
    #todo remove these
    add_compile_definitions (TBB_SUPPRESS_DEPRECATED_MESSAGES)
    add_compile_definitions (_CRT_SECURE_NO_WARNINGS)
    add_compile_definitions (_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)
    add_compile_definitions (_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
elseif (UNIX)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

if (FB_ENABLE_PTR_EXCEPTIONS)
    add_compile_definitions(
        $<$<CONFIG:Debug>:FB_ENABLE_PTR_EXCEPTIONS=1>
        $<$<CONFIG:Release>:FB_ENABLE_PTR_EXCEPTIONS=0>
        $<$<CONFIG:MinSizeRel>:FB_ENABLE_PTR_EXCEPTIONS=0>
        $<$<CONFIG:RelWithDebInfo>:FB_ENABLE_PTR_EXCEPTIONS=1>
        )
else()
    add_compile_definitions (FB_ENABLE_PTR_EXCEPTIONS=0)
endif()

if (FB_USE_HOUDINI)
    add_compile_definitions (FB_USE_HOUDINI=1)
else()
    add_compile_definitions (FB_USE_HOUDINI=0)
endif()

if (FB_USE_FORCE_INLINE)
    add_compile_definitions (FB_USE_FORCE_INLINE=1)
else()
    add_compile_definitions (FB_USE_FORCE_INLINE=0)
endif()

if (FB_BUILD_OISINPUT)
    add_compile_definitions (FB_BUILD_OISINPUT=1)
else()
    add_compile_definitions (FB_BUILD_OISINPUT=0)
endif()

if (FB_BUILD_CEGUI)
    add_compile_definitions (FB_BUILD_CEGUI=1)
else()
    add_compile_definitions (FB_BUILD_CEGUI=0)
endif()

if (FB_BUILD_IMGUI)
    add_compile_definitions (FB_BUILD_IMGUI=1)
else()
    add_compile_definitions (FB_BUILD_IMGUI=0)
endif()

if (FB_BUILD_WXWIDGETS)
    add_compile_definitions (FB_BUILD_WXWIDGETS=1)
else()
    add_compile_definitions (FB_BUILD_WXWIDGETS=0)
endif()

if (FB_USE_STD_ATOMIC)
    add_compile_definitions (FB_USE_STD_ATOMIC=1)
else()
    add_compile_definitions (FB_USE_STD_ATOMIC=0)
endif()

if (FB_USE_OPENSSL)
    add_compile_definitions (FB_USE_OPENSSL=1)
else()
    add_compile_definitions (FB_USE_OPENSSL=0)
endif()

if (FB_BUILD_SQLITE)
    add_compile_definitions (FB_BUILD_SQLITE=1)
else()
    add_compile_definitions (FB_BUILD_SQLITE=0)
endif()

if (FB_BUILD_WILDMAGIC5)
    add_compile_definitions (FB_BUILD_WILDMAGIC5=1)
else()
    add_compile_definitions (FB_BUILD_WILDMAGIC5=0)
endif()

if (FB_GRAPHICS_SYSTEM_OGRE)
    add_compile_definitions (FB_GRAPHICS_SYSTEM_OGRE=1)
else ()
    add_compile_definitions (FB_GRAPHICS_SYSTEM_OGRE=0)
endif()

if (FB_GRAPHICS_SYSTEM_CLAW)
    add_compile_definitions (FB_GRAPHICS_SYSTEM_CLAW=1)
else ()
    add_compile_definitions (FB_GRAPHICS_SYSTEM_CLAW=0)
endif()

if (FB_GRAPHICS_SYSTEM_OGRENEXT)
    add_compile_definitions (FB_GRAPHICS_SYSTEM_OGRENEXT=1)
else ()
    add_compile_definitions (FB_GRAPHICS_SYSTEM_OGRENEXT=0)
endif()

if (FB_GRAPHICS_SYSTEM_SOFTWARE)
    add_compile_definitions (FB_USE_SOFTWARE_RENDERER=1)
else()
    add_compile_definitions (FB_USE_SOFTWARE_RENDERER=0)
endif()

if (FB_BUILD_RENDERER_DX9)
    add_compile_definitions (FB_BUILD_RENDERER_DX9=1)
else()
    add_compile_definitions (FB_BUILD_RENDERER_DX9=0)
endif()

if (FB_BUILD_RENDERER_DX11)
    add_compile_definitions (FB_BUILD_RENDERER_DX11=1)
else()
    add_compile_definitions (FB_BUILD_RENDERER_DX11=0)
endif()

if (FB_BUILD_RENDERER_OPENGL)
    add_compile_definitions (FB_BUILD_RENDERER_OPENGL=1)
else()
    add_compile_definitions (FB_BUILD_RENDERER_OPENGL=0)
endif()

if (FB_BUILD_RENDERER_GL3PLUS)
    add_compile_definitions (FB_BUILD_RENDERER_GL3PLUS=1)
else()
    add_compile_definitions (FB_BUILD_RENDERER_GL3PLUS=0)
endif()

if (FB_BUILD_RENDERER_VULKAN)
    add_compile_definitions (FB_BUILD_RENDERER_VULKAN=1)
else()
    add_compile_definitions (FB_BUILD_RENDERER_VULKAN=0)
endif()

if (FB_BUILD_RENDERER_METAL)
    add_compile_definitions (FB_BUILD_RENDERER_METAL=1)
else()
    add_compile_definitions (FB_BUILD_RENDERER_METAL=0)
endif()

if (FB_CODEC_FREEIMAGE)
    add_compile_definitions (FB_CODEC_FREEIMAGE=1)
else()
    add_compile_definitions (FB_CODEC_FREEIMAGE=0)
endif()

if (FB_CODEC_STBI)
    add_compile_definitions (FB_CODEC_STBI=1)
else()
    add_compile_definitions (FB_CODEC_STBI=0)
endif()

if (FB_BUILD_BULLETPHYSICS)
    add_compile_definitions (FB_BUILD_BULLETPHYSICS=1)
else()
    add_compile_definitions (FB_BUILD_BULLETPHYSICS=0)
endif()

if (FB_BUILD_ODE)
    add_compile_definitions (FB_BUILD_ODE=1)
else()
    add_compile_definitions (FB_BUILD_ODE=0)
endif()

if (FB_BUILD_PHYSX)
    add_compile_definitions (FB_BUILD_PHYSX=1)
else()
    add_compile_definitions (FB_BUILD_PHYSX=0)
endif()

if (FB_BUILD_PHYSX_4)
    add_compile_definitions (FB_BUILD_PHYSX_4=1)
else()
    add_compile_definitions (FB_BUILD_PHYSX_4=0)
endif()

if (FB_BUILD_PHYSICS3)
    add_compile_definitions (FB_BUILD_PHYSICS3=1)
else()
    add_compile_definitions (FB_BUILD_PHYSICS3=0)
endif()

add_compile_definitions(
$<$<CONFIG:Debug>:_DEBUG>
$<$<CONFIG:Release>:NDEBUG>
$<$<CONFIG:MinSizeRel>:NDEBUG>
$<$<CONFIG:RelWithDebInfo>:NDEBUG>
)


if (FB_USE_FBXSDK)
    add_compile_definitions (FB_USE_FBXSDK=1)
else ()
    add_compile_definitions (FB_USE_FBXSDK=0)
endif()



if (FB_USE_ASSET_IMPORT)
    add_compile_definitions (FB_USE_ASSET_IMPORT=1)
else ()
    add_compile_definitions (FB_USE_ASSET_IMPORT=0)
endif()

if (FB_USE_CUSTOM_NEW_DELETE)
    add_compile_definitions (FB_USE_CUSTOM_NEW_DELETE=1)
else ()
    add_compile_definitions (FB_USE_CUSTOM_NEW_DELETE=0)
endif()

if (FB_OBJECT_ALIGNED_ALLOC)
    add_compile_definitions (FB_OBJECT_ALIGNED_ALLOC=1)
else ()
    add_compile_definitions (FB_OBJECT_ALIGNED_ALLOC=0)
endif()

add_compile_definitions (FB_ALIGNMENT=16)

if (FB_ENABLE_MEMORY_TRACKER)
    add_compile_definitions (FB_ENABLE_MEMORY_TRACKER=1)
else ()
    add_compile_definitions (FB_ENABLE_MEMORY_TRACKER=0)
endif()

if (FB_TRACK_REFERENCES)
    add_compile_definitions (FB_TRACK_REFERENCES=1)
else ()
    add_compile_definitions (FB_TRACK_REFERENCES=0)
endif()

if (FB_TRACK_WEAK_REFERENCES)
    add_compile_definitions (FB_TRACK_WEAK_REFERENCES=1)
else ()
    add_compile_definitions (FB_TRACK_WEAK_REFERENCES=0)
endif()

if (FB_USE_MEMORY_POOL)
    add_compile_definitions (FB_USE_MEMORY_POOL=1)
else ()
    add_compile_definitions (FB_USE_MEMORY_POOL=0)
endif()

if (FB_USE_ONETBB)
	add_compile_definitions (FB_USE_ONETBB=1)
else ()
    add_compile_definitions (FB_USE_ONETBB=0)
endif ()

if (FB_USE_TBB)
	add_compile_definitions (FB_USE_TBB=1)
else ()
    add_compile_definitions (FB_USE_TBB=0)
endif ()

if (FB_USE_FIREBLADE_CONTAINERS)
	add_compile_definitions (FB_USE_FIREBLADE_CONTAINERS=1)
else ()
    add_compile_definitions (FB_USE_FIREBLADE_CONTAINERS=0)
endif ()

if (FB_ENABLE_THREADING)
	add_compile_definitions (FB_ENABLE_THREADING=1)
else ()
    add_compile_definitions (FB_ENABLE_THREADING=0)
endif ()



if (FB_ENABLE_LUA)
	add_compile_definitions (FB_USE_LUA=1)
else ()
    add_compile_definitions (FB_USE_LUA=0)
endif ()

if(WIN32)
	add_compile_definitions (FB_USE_OPCODE_LIB=0)
	add_compile_definitions (FB_USE_ALLOCATED_BASE_OBJECT=1)
else()
	add_compile_definitions (FB_USE_OPCODE_LIB=0)
	add_compile_definitions (FB_USE_ALLOCATED_BASE_OBJECT=1)
endif()

if (FB_ENABLE_ZZIP)
    add_compile_definitions(FB_USE_ZZIP=1)
else()
    add_compile_definitions(FB_USE_ZZIP=0)
endif()



if (FB_ENABLE_BOOST)
    add_compile_definitions(FB_USE_BOOST=1)
else()
    add_compile_definitions(FB_USE_BOOST=0)
endif()


if (FB_USE_PRECOMPILED_HEADERS)
    add_compile_definitions(FB_USE_PRECOMPILED_HEADERS=1)
else()
    add_compile_definitions(FB_USE_PRECOMPILED_HEADERS=0)
endif()

if (FB_USE_PTHREADS)
    add_compile_definitions(FB_USE_PTHREADS=1)
else()
    add_compile_definitions(FB_USE_PTHREADS=0)
endif()



if (FB_AUTO_LINK)
    add_compile_definitions (FB_USE_AUTO_LINK=1)
else()
    add_compile_definitions (FB_USE_AUTO_LINK=0)
endif()

if (FB_EXCEPTIONS)
    add_compile_definitions(FB_EXCEPTIONS=1)
else()
    add_compile_definitions(FB_EXCEPTIONS=0)
endif()

if (FB_BUILD_FILEWATCHER)
    add_compile_definitions(FB_BUILD_FILEWATCHER=1)
else()
    add_compile_definitions(FB_BUILD_FILEWATCHER=0)
endif()

if (FB_ENABLE_PROFILER)
    add_compile_definitions (FB_PROFILING=1)
else()
	add_compile_definitions (FB_PROFILING=0)
endif()


if (FB_DOUBLE_PRECISION)
	add_compile_definitions (FB_DOUBLE_PRECISION=1)
else()
	add_compile_definitions (FB_DOUBLE_PRECISION=0)
endif()

if (FB_ENABLE_DEBUG_TRACE)
    add_compile_definitions (FB_ENABLE_DEBUG_TRACE=1)
else()
    add_compile_definitions (FB_ENABLE_DEBUG_TRACE=0)
endif()

if (FB_ENABLE_HEAP_DEBUG)
    add_compile_definitions (FB_ENABLE_HEAP_DEBUG=1)
else()
    add_compile_definitions (FB_ENABLE_HEAP_DEBUG=0)
endif()


if (FB_BUILD_EDITOR_PLUGIN)
    add_compile_definitions (FB_BUILD_EDITOR_PLUGIN=1)
else()
    add_compile_definitions (FB_BUILD_EDITOR_PLUGIN=0)
endif()

if (FB_BUILD_PHYSICS_DEBUG)
    add_compile_definitions (FB_PHYSX_DEBUG=1)
else()
    add_compile_definitions (FB_PHYSX_DEBUG=0)
endif()

if (FB_ENABLE_ACTIVETRANSFORMS)
    add_compile_definitions (FB_ENABLE_ACTIVETRANSFORMS=1)
else()
    add_compile_definitions (FB_ENABLE_ACTIVETRANSFORMS=0)
endif()



if (FB_PHYSICS_THREADED)
    add_compile_definitions (FLIGHT_USE_OWN_THREAD=1)
else()
    add_compile_definitions (FLIGHT_USE_OWN_THREAD=1)
endif()

if (FB_USE_TRANSFORM_THREAD)
    add_compile_definitions (FB_USE_TRANSFORM_THREAD=1)
else()
    add_compile_definitions (FB_USE_TRANSFORM_THREAD=0)
endif()

if (FB_ENABLE_LUA)
    add_compile_definitions (FB_ENABLE_LUA=1)
else()
    add_compile_definitions (FB_ENABLE_LUA=0)
endif()

if (FB_ENABLE_PYTHON)
    add_compile_definitions (FB_ENABLE_PYTHON=1)
else()
    add_compile_definitions (FB_ENABLE_PYTHON=0)
endif()

if (FB_BUILD_FINAL)
    add_compile_definitions (FB_CUSTOMER_FINAL=1)
    add_compile_definitions (FB_FINAL=1)
    add_compile_definitions (FB_END_USER_INSTALL=1)
else()
    add_compile_definitions (FB_CUSTOMER_FINAL=0)
    add_compile_definitions (FB_FINAL=0)
    add_compile_definitions (FB_END_USER_INSTALL=0)
endif()

add_compile_definitions (FB_USE_NGPLANT=0)

add_compile_definitions(
$<$<CONFIG:Debug>:FB_VERBOSE_LOG=1>
$<$<CONFIG:Release>:FB_VERBOSE_LOG=0>
$<$<CONFIG:MinSizeRel>:FB_VERBOSE_LOG=0>
$<$<CONFIG:RelWithDebInfo>:FB_VERBOSE_LOG=1>
)

add_compile_definitions(
$<$<CONFIG:Debug>:FB_ERROR_LOG=1>
$<$<CONFIG:Release>:FB_ERROR_LOG=1>
$<$<CONFIG:MinSizeRel>:FB_ERROR_LOG=1>
$<$<CONFIG:RelWithDebInfo>:FB_ERROR_LOG=1>
)

if (FB_ENABLE_TRACE)
    add_compile_definitions(
        $<$<CONFIG:Debug>:FB_ENABLE_TRACE=1>
        $<$<CONFIG:Release>:FB_ENABLE_TRACE=0>
        $<$<CONFIG:MinSizeRel>:FB_ENABLE_TRACE=0>
        $<$<CONFIG:RelWithDebInfo>:FB_ENABLE_TRACE=1>
        )

    if (WIN32)
        add_compile_definitions (BOOST_STACKTRACE_USE_WINDBG_CACHED)
    else()
        add_compile_definitions (BOOST_STACKTRACE_USE_ADDR2LINE)
        add_compile_definitions(_GNU_SOURCE)
    endif()
else()
    add_compile_definitions(FB_ENABLE_TRACE=0)
endif()

add_compile_definitions(
    $<$<CONFIG:Debug>:FB_LOCK_TRACKER=0>
    $<$<CONFIG:Release>:FB_LOCK_TRACKER=0>
    $<$<CONFIG:MinSizeRel>:FB_LOCK_TRACKER=0>
    $<$<CONFIG:RelWithDebInfo>:FB_LOCK_TRACKER=0>
)

add_compile_definitions(
    $<$<CONFIG:Debug>:FB_ENABLE_ASSERTS=1>
    $<$<CONFIG:Release>:FB_ENABLE_ASSERTS=0>
    $<$<CONFIG:MinSizeRel>:FB_ENABLE_ASSERTS=0>
    $<$<CONFIG:RelWithDebInfo>:FB_ENABLE_ASSERTS=1>
)

add_compile_definitions (FB_STREAM_TEMP_SIZE=2048)

add_compile_definitions (_ENABLE_ATOMIC_ALIGNMENT_FIX)