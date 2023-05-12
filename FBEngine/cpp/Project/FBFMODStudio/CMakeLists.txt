project(FBFMODStudio)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBFMODStudio/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBFMODStudio/*.cpp")

include_directories( ${FB_ROOT}/Dependencies/boost )
include_directories( ${FB_ROOT}/Dependencies/tbb/include )
include_directories( ${FB_ROOT}/Dependencies/tinybind )
include_directories( ${FB_ROOT}/Dependencies/tinybind/tinyxml )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/fmod/studio/api/studio/inc )
include_directories( ${FB_ROOT}/AdditionalLibraries/fmod/studio/api/lowlevel/inc )
include_directories( ../../Include )
include_directories( ../../Include/FBFMODStudio )

add_library(FBFMODStudio STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBFMODStudio PROPERTIES FOLDER Engine/Cpp/Sound)
