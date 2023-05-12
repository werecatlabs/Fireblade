project(FBRakNet C CXX)

add_compile_definitions (_FB_STATIC_LIB_)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBRakNet/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBRakNet/*.cpp")

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
include_directories( ${FB_ROOT}/AdditionalLibraries/raknet/Source )
include_directories( ../../Include )
include_directories( ../../Include/FBRakNet/Extern )

add_library(FBRakNet STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBRakNet PROPERTIES FOLDER Engine/Cpp/Network)
