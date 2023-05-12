cmake_minimum_required(VERSION 3.13)
project(FBCPlusPlus)


add_compile_definitions (WINDOWS_IGNORE_PACKING_MISMATCH)
add_compile_definitions (dpNoLib)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBCPlusPlus/*.h" "../../Include/FBCPlusPlus/*.inl")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBCPlusPlus/*.cpp" "../../Source/FBCPlusPlus/*.c")

set(FB_HEADER_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBCPlusPlus/../../Include/FBCPlusPlus")
set(FB_SOURCE_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBCPlusPlus/../../Source/FBCPlusPlus")

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

include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/boost )

if (FB_USE_ONETBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/onetbb/include )
elseif (FB_USE_TBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tbb/include )
endif()

include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tinybind )
include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tinybind/tinyxml )
include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/utf8/source )
include_directories( ../../Include )
include_directories( ../../Include/FBCPlusPlus )
include_directories( ../../Include/FBCPlusPlus/Extern )
include_directories( ../../Include/FBCPlusPlus/Extern/disasm-lib )
include_directories( ../../Include/FBCPlusPlus/Extern/DynamicPatcher )

add_library(FBCPlusPlus STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBCPlusPlus PROPERTIES FOLDER Engine/Cpp/Scripting)

if (NOT _FB_STATIC_LIB_)
	target_compile_definitions(FBCPlusPlus PUBLIC FB_EXPORTS)
endif()
