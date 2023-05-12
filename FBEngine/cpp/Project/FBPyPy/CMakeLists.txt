project(FBPyPy)

add_compile_definitions (_FB_STATIC_LIB_)


# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBPyPy/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBPyPy/*.cpp")

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
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/wildmagic/SDK/Include )
include_directories( ../../Include )

#if (FB_USE_PRECOMPILED_HEADERS)
#    use_precompiled_header(FBProcedural "FBPyPy/FBPyPyPCH.h" "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBPyPy/FBPyPyPCH.cpp")
#endif()

add_library(FBPyPy STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBPyPy PROPERTIES FOLDER Engine/Cpp/Scripting)

if (NOT _FB_STATIC_LIB_)
	target_compile_definitions(FBPyPy PUBLIC FB_EXPORTS)
endif()
