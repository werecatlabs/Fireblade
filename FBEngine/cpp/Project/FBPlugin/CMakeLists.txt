project(FBPlugin)


# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBPlugin/*.h" "../../Include/FBPlugin/*.hpp" "../../Include/FBPlugin/*.inl")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBPlugin/*.cpp" "../../Source/FBPlugin/*.c")

set(FB_HEADER_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBPlugin/../../Include/FBPlugin")
set(FB_SOURCE_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBPlugin/../../Source/FBPlugin")

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
include_directories( ${FB_ROOT}/Dependencies/tinyxml/include )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/ois/includes )
include_directories( ${FB_ROOT}/AdditionalLibraries/zlib )
include_directories( ${FB_ROOT}/AdditionalLibraries/zlib/src )
include_directories( ${FB_ROOT}/AdditionalLibraries/zlib/src/contrib/minizip )
include_directories( ${FB_ROOT}/AdditionalLibraries/zziplib )
include_directories( ../../Include )
include_directories( ../../Include/extern )

if (FB_USE_PRECOMPILED_HEADERS)
    use_precompiled_header(FBPlugin "FBPlugin/FBPluginPCH.h" "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBPlugin/FBPluginPCH.cpp")
endif()

add_library(FBPlugin STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBPlugin PROPERTIES FOLDER Engine/Cpp/Core)

if (NOT _FB_STATIC_LIB_)
	target_compile_definitions(FBPlugin PUBLIC FB_EXPORTS)
endif()
