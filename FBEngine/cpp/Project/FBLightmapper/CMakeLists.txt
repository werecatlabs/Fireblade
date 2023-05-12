project(FBLightmapper)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBLightmapper/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBLightmapper/*.cpp")

include_directories( ${FB_ROOT}/Dependencies/boost )
include_directories( ${FB_ROOT}/Dependencies/tbb/include )
include_directories( ${FB_ROOT}/Dependencies/tinybind )
include_directories( ${FB_ROOT}/Dependencies/tinybind/tinyxml )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/cimg )
include_directories( ../../Include )
include_directories( ../../Include/FBLightmapper )


add_library(FBLightmapper STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBLightmapper PROPERTIES FOLDER Engine/Cpp/Render)

if (NOT _FB_STATIC_LIB_)
	target_compile_definitions(FBLightmapper PUBLIC FB_EXPORTS)
endif()
