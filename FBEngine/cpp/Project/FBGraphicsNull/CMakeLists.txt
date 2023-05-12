project(FBGraphicsNull)
project(FBGraphicsNull C CXX)

set(CMAKE_CXX_STANDARD 17)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBGraphicsNull/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBGraphicsNull/*.cpp")

include_directories( ${FB_ROOT}/Dependencies/boost )
include_directories( ${FB_ROOT}/Dependencies/tbb/include )
include_directories( ${FB_ROOT}/Dependencies/tinybind )
include_directories( ${FB_ROOT}/Dependencies/tinybind/tinyxml )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ../../Include )
include_directories( ../../Include/FBGraphicsNull )

add_library(FBGraphicsNull STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(FBGraphicsNull PROPERTIES LINKER_LANGUAGE CXX)

if (NOT _FB_STATIC_LIB_)
	target_compile_definitions(FBGraphicsNull PUBLIC FB_EXPORTS)
endif()

set_target_properties (FBGraphicsNull PROPERTIES FOLDER Engine/Cpp/Render)