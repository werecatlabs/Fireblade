project(FBLua)


add_compile_definitions (FB_PROFILE_LUA_CALLS=0)
add_compile_definitions (BOOST_BIND_GLOBAL_PLACEHOLDERS)

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBLua/*.h" "../../Include/FBLua/*.hpp")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBLua/*.cpp" "../../Source/FBLua/*.c")

set(FB_HEADER_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBLua/../../Include/FBLua")
set(FB_SOURCE_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBLua/../../Source/FBLua")

foreach(_source IN ITEMS ${HEADER_FILES})
    get_filename_component(_source_file_path "${_source}" DIRECTORY)
    string(REPLACE "${FB_HEADER_REL_PATH}" "" _group_file_path "${_source_file_path}")
    string(REPLACE "\\" "/" _group_file_path "${_group_file_path}")
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
include_directories( ${FB_ROOT}/Dependencies/tinyxml )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/wildmagic/SDK/Include )
include_directories( ../../Include )
include_directories( ../../Include/FBLua )
include_directories( ../../Include/FBLua/extern )
include_directories( ../../Include/FBLua/extern/lua/inc )
include_directories( ../../Include/FBLua/extern/luabind/inc )
include_directories( ../../Include/FBLua/extern/lua_cjson )

#if (FB_USE_PRECOMPILED_HEADERS)
#    use_precompiled_header(FBProcedural "FBLua/FBLuaPCH.h" "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBLua/FBLuaPCH.cpp")
#endif()

add_library(FBLua STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBLua PROPERTIES FOLDER Engine/Cpp/Scripting)

if (NOT _FB_STATIC_LIB_)
	target_compile_definitions(FBLua PUBLIC FB_EXPORTS)
endif()
