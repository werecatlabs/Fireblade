project(FBPhysx)

add_compile_definitions (FB_TRACK_LOCKS=0)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBPhysx/*.h" "../../Include/FBPhysx/*.inl")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBPhysx/*.cpp")

if (UNIX)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

include_directories( ${FB_ROOT}/Dependencies/boost )

if (FB_USE_ONETBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/onetbb/include )
elseif (FB_USE_TBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tbb/include )
endif()

include_directories( ${FB_ROOT}/AdditionalLibraries/physx )
include_directories( ${FB_ROOT}/AdditionalLibraries/physx/Include )
include_directories( ${FB_ROOT}/AdditionalLibraries/physx/Include/common )
include_directories( ${FB_ROOT}/AdditionalLibraries/physx/Include/extensions )
include_directories( ${FB_ROOT}/AdditionalLibraries/physx/Include/foundation )
include_directories( ${FB_ROOT}/AdditionalLibraries/physx/Include/foundation/include )
include_directories( ${FB_ROOT}/AdditionalLibraries/physx/Include/physxvisualdebuggersdk )
include_directories( ${FB_ROOT}/AdditionalLibraries/wildmagic/SDK/Include )
include_directories( ../../Include )

if (FB_USE_PRECOMPILED_HEADERS)
    use_precompiled_header(FBPhysx "FBPhysx/FBPhysxPCH.h" "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBPhysx/FBPhysxPCH.cpp")
endif()

add_library(FBPhysx STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBPhysx PROPERTIES FOLDER Engine/Cpp/Physics)
