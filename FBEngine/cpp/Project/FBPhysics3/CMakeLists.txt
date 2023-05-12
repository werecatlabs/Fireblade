cmake_minimum_required(VERSION 3.13)
project(FBPhysics3 C CXX)

add_compile_definitions (_FB_STATIC_LIB_)

#add_compile_definitions (GRIDMESH)
#add_compile_definitions (OCTREEMESH)
add_compile_definitions (OPCODEMESH)
#add_compile_definitions (MAGICMESH)
add_compile_definitions (MAX_RIGID_BODIES=1e10)
#add_compile_definitions (USING_RAYCAST_VEHICLE)
add_compile_definitions(FB_USE_OPCODE_COLLISION=0)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBPhysics3/*.h" "../../Include/FBPhysics3/*.inl")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBPhysics3/*.cpp" "../../Source/FBPhysics3/*.c" "../../Source/FBPhysics3/*.mm")

set(FB_HEADER_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBPhysics3/../../Include/FBPhysics3")
set(FB_SOURCE_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBPhysics3/../../Source/FBPhysics3")

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

include_directories( ../../Include )
include_directories( ../../Include/FBPhysics3 )
include_directories( ../../Include/FBPhysics3/Collision )
include_directories( ../../Include/FBPhysics3/Collision/Bullet )
include_directories( ../../Include/FBPhysics3/Collision/Bullet/BroadphaseCollision)
include_directories( ../../Include/FBPhysics3/Collision/Bullet/CollisionDispatch)
include_directories( ../../Include/FBPhysics3/Collision/Bullet/CollisionShapes)
include_directories( ../../Include/FBPhysics3/Collision/Bullet/LinearMath)
include_directories( ../../Include/FBPhysics3/Collision/Bullet/NarrowPhaseCollision)

if (FB_BUILD_WILDMAGIC5)
    include_directories( ${FB_ROOT}/AdditionalLibraries/wildmagic/SDK/Include )
endif()

add_library(FBPhysics3 STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBPhysics3 PROPERTIES FOLDER Engine/Cpp/Physics)