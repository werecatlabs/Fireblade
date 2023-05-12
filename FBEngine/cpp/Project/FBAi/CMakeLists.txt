project(FBAi)

add_compile_definitions (_FB_STATIC_LIB_)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBAi/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBAi/*.cpp")

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
include_directories( ../../Include )
include_directories( ../../Include/FBAi/Extern )

add_library(FBAi STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBAi PROPERTIES FOLDER Engine/Cpp/Core)
