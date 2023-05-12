cmake_minimum_required(VERSION 3.13)
project(FBFlashPlayer)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBFlashPlayer/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBFlashPlayer/*.cpp")

set(FB_HEADER_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBFlashPlayer/../../Include/FBFlashPlayer")
set(FB_SOURCE_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBFlashPlayer/../../Source/FBFlashPlayer")

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

include_directories( ${FB_ROOT}/Dependencies/boost )
include_directories( ${FB_ROOT}/Dependencies/tbb/include )
include_directories( ${FB_ROOT}/Dependencies/tinybind )
include_directories( ${FB_ROOT}/Dependencies/tinybind/tinyxml )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/hikari/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/zlib )
include_directories( ${FB_ROOT}/AdditionalLibraries/zlib/src )
include_directories( ${FB_ROOT}/AdditionalLibraries/zlib/src/contrib/minizip )
include_directories( ${FB_ROOT}/AdditionalLibraries/zziplib )
include_directories( ../../Include )
include_directories( ../../Include/FBFlashPlayer )


add_library(FBFlashPlayer STATIC ${HEADER_FILES} ${SOURCE_FILES})
