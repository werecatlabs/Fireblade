if (APPLE)
    project(FBFileSystem C CXX OBJC)
else()
    project(FBFileSystem C CXX)
endif()

add_compile_definitions (_FB_STATIC_LIB_)

if(WIN32)
    add_compile_definitions (ZZIP_HAVE_BYTESWAP_H=0)
elseif(APPLE)
    add_compile_definitions (ZZIP_HAVE_BYTESWAP_H=0)
else()
    add_compile_definitions (ZZIP_HAVE_BYTESWAP_H=1)
endif()


# define header and source files for the library
file(GLOB HEADER_FILES "../../Include/FBFileSystem/*.h" "../../Include/FBFileSystem/*.hpp" "../../Include/FBFileSystem/*.inl")
file(GLOB SOURCE_FILES "../../Source/FBFileSystem/*.cpp" "../../Source/FBFileSystem/*.c")

set(FB_HEADER_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBFileSystem/../../Include/FBFileSystem")
set(FB_SOURCE_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBFileSystem/../../Source/FBFileSystem")

if (WIN32)
    file(GLOB_RECURSE HEADER_FILES_SUBFOLDER "../../Include/FBFileSystem/extern/nativefiledialog/*.h")
    file(GLOB_RECURSE SOURCE_FILES_SUBFOLDER "../../Source/FBFileSystem/extern/nativefiledialog/*.cpp" "../../Source/FBFileSystem/extern/nativefiledialog/*.c")
    list(APPEND HEADER_FILES ${HEADER_FILES_SUBFOLDER})
    list(APPEND SOURCE_FILES ${SOURCE_FILES_SUBFOLDER})
elseif (APPLE)
    file(GLOB_RECURSE HEADER_FILES_SUBFOLDER "../../Include/FBFileSystem/extern/nativefiledialog/*.h")
    file(GLOB_RECURSE SOURCE_FILES_SUBFOLDER "../../Source/FBFileSystem/extern/nativefiledialog/*.m" "../../Source/FBFileSystem/extern/nativefiledialog/*.c")
    list(APPEND HEADER_FILES ${HEADER_FILES_SUBFOLDER})
    list(APPEND SOURCE_FILES ${SOURCE_FILES_SUBFOLDER})
endif()

file(GLOB_RECURSE HEADER_FILES_SUBFOLDER "../../Include/FBFileSystem/extern/minizip/*.h")
file(GLOB_RECURSE SOURCE_FILES_SUBFOLDER "../../Source/FBFileSystem/extern/minizip/*.cpp" "../../Source/FBFileSystem/extern/minizip/*.c")
list(APPEND HEADER_FILES ${HEADER_FILES_SUBFOLDER})
list(APPEND SOURCE_FILES ${SOURCE_FILES_SUBFOLDER})

file(GLOB_RECURSE HEADER_FILES_SUBFOLDER "../../Include/FBFileSystem/extern/zlib/*.h")
file(GLOB_RECURSE SOURCE_FILES_SUBFOLDER "../../Source/FBFileSystem/extern/zlib/*.cpp" "../../Source/FBFileSystem/extern/zlib/*.c")
list(APPEND HEADER_FILES ${HEADER_FILES_SUBFOLDER})
list(APPEND SOURCE_FILES ${SOURCE_FILES_SUBFOLDER})

file(GLOB_RECURSE HEADER_FILES_SUBFOLDER "../../Include/FBFileSystem/extern/zzip/*.h")
file(GLOB_RECURSE SOURCE_FILES_SUBFOLDER "../../Source/FBFileSystem/extern/zzip/*.cpp" "../../Source/FBFileSystem/extern/zzip/*.c")
list(APPEND HEADER_FILES ${HEADER_FILES_SUBFOLDER})
list(APPEND SOURCE_FILES ${SOURCE_FILES_SUBFOLDER})

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

if (FB_BUILD_FILEWATCHER)
    include_directories( ${FB_ROOT}/AdditionalLibraries/simplefilewatcher/includes )
endif()

include_directories( ../../Include )
include_directories( ../../Include/extern )
include_directories( ${FB_ROOT}/FBEngine/cpp/Include/FBFileSystem/extern/nativefiledialog )

if (FB_USE_PRECOMPILED_HEADERS)
    use_precompiled_header(FBCore "FBFileSystem/FBFileSystemPCH.h" "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBFileSystem/FBFileSystemPCH.cpp")

    file(GLOB_RECURSE SOURCE_FILES_EXTERN "../../Source/FBFileSystem/extern/*.cpp" "../../Source/FBFileSystem/extern/*.c" )
    foreach(_source IN ITEMS ${SOURCE_FILES_EXTERN})
        set_source_files_properties(${_source}
            PPROPERTIES COMPILE_FLAGS /Y-
	    )
    endforeach()
endif()

add_library(FBFileSystem STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBFileSystem PROPERTIES FOLDER Engine/Cpp/FileSystem)
