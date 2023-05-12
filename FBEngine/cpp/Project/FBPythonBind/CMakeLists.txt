project(FBPythonBind)

add_compile_definitions (_FB_STATIC_LIB_)
add_compile_definitions (BOOST_PYTHON_STATIC_LIB)
add_compile_definitions (__TBB_STATISTICS=0)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBPythonBind/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBPythonBind/*.cpp")

set(FB_HEADER_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBPythonBind/../../Include/FBPythonBind")
set(FB_SOURCE_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBPythonBind/../../Source/FBPythonBind")

foreach(_source IN ITEMS ${HEADER_FILES})
    get_filename_component(_source_file_path "${_source}" DIRECTORY)
    string(REPLACE "${FB_HEADER_REL_PATH}" "" _group_file_path "${_source_file_path}")
    string(REPLACE "\\" "/" _group_file_path "${_group_file_path}")
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
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/python/PC )
include_directories( ${FB_ROOT}/AdditionalLibraries/python/Include )
#include_directories( ${FB_ROOT}/AdditionalLibraries/pypy/pypy39/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/wildmagic/SDK/Include )
include_directories( ../../Include )

#if (FB_USE_PRECOMPILED_HEADERS)
#    use_precompiled_header(FBPythonBind "FBPythonBind/FBPythonBindPCH.h" "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBPythonBind/FBPythonBindPCH.cpp")
#endif()

add_library(FBPythonBind STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBPythonBind PROPERTIES FOLDER Engine/Cpp/Scripting)

if (NOT _FB_STATIC_LIB_)
	target_compile_definitions(FBPythonBind PUBLIC FB_EXPORTS)
endif()
