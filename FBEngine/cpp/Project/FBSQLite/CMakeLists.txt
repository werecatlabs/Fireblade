project(FBSQLite)

option(FB_BUILD_SQLITECODEC "Build sqlite codec" ON)

if (FB_BUILD_SQLITECODEC)
	add_compile_definitions (SQLITE_HAS_CODEC)
endif()

if (UNIX)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBSQLite/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBSQLite/extern/*.c" "../../Source/FBSQLite/*.c" "../../Source/FBSQLite/*.cpp")

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

include_directories( ${FB_ROOT}/Dependencies/boost )

if (FB_USE_ONETBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/onetbb/include )
elseif (FB_USE_TBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tbb/include )
endif()

include_directories( ${FB_ROOT}/Dependencies/tinyxml )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/zlib )
include_directories( ${FB_ROOT}/AdditionalLibraries/zziplib )
#include_directories( ${FB_ROOT}/AdditionalLibraries/cppsqlite/include )
include_directories( ../../Include )
include_directories(../../Include/FBSQLite/extern)

add_library(FBSQLite STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBSQLite PROPERTIES FOLDER Engine/Cpp/Core)
