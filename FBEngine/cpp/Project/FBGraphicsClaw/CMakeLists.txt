project(FBGraphicsClaw)
project(FBGraphicsClaw C CXX)

set(CMAKE_CXX_STANDARD 17)
add_compile_definitions (BX_CONFIG_DEBUG)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBGraphicsClaw/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBGraphicsClaw/*.cpp")

include_directories( ${FB_ROOT}/Dependencies/boost )
include_directories( ${FB_ROOT}/Dependencies/tbb/include )
include_directories( ${FB_ROOT}/Dependencies/tinybind )
include_directories( ${FB_ROOT}/Dependencies/tinybind/tinyxml )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/bgfx/bx/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/bgfx/bgfx/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/bgfx/bgfx/examples/common )
include_directories( ${FB_ROOT}/AdditionalLibraries/bgfx/bimg/include )

if (WIN32)
	include_directories( ${FB_ROOT}/AdditionalLibraries/bgfx/bx/include/compat/msvc )
endif()

include_directories( ${FB_ROOT}/AdditionalLibraries/imgui )
include_directories( ../../Include )
include_directories( ../../Include/FBGraphicsNull )

add_library(FBGraphicsClaw STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(FBGraphicsClaw PROPERTIES LINKER_LANGUAGE CXX)

if (NOT _FB_STATIC_LIB_)
	target_compile_definitions(FBGraphicsClaw PUBLIC FB_EXPORTS)
endif()

set_target_properties (FBGraphicsClaw PROPERTIES FOLDER Engine/Cpp/Render)
