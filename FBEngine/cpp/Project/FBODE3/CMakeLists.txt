project(FBODE3)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBODE3/*.h")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBODE3/*.cpp")

if (UNIX)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

include_directories( ${FB_ROOT}/Dependencies/boost )

if (FB_USE_ONETBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/onetbb/include )
elseif (FB_USE_TBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tbb/include )
endif()

include_directories( ../../Include )

add_library(FBODE3 STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBODE3 PROPERTIES FOLDER Engine/Physics)
