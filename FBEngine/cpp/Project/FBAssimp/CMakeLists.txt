project(FBAssimp)

add_compile_definitions (_FB_STATIC_LIB_)
add_compile_definitions (ASSIMP_BUILD_NO_OWN_ZLIB)
add_compile_definitions (ASSIMP_USE_HUNTER)
add_compile_definitions (OPENDDL_STATIC_LIBARY)

add_compile_definitions (ASSIMP_BUILD_DRACO=0)

add_compile_definitions (ASSIMP_BUILD_NO_IFC_IMPORTER)
add_compile_definitions (ASSIMP_BUILD_NO_IFC_EXPORTER)
add_compile_definitions (ASSIMP_BUILD_NO_MD3_IMPORTER)
add_compile_definitions (ASSIMP_BUILD_NO_MD3_EXPORTER)
add_compile_definitions (ASSIMP_BUILD_NO_M3D_IMPORTER)
add_compile_definitions (ASSIMP_BUILD_NO_M3D_EXPORTER)
add_compile_definitions (WIN32_LEAN_AND_MEAN)
add_compile_definitions (UNICODE)
add_compile_definitions (_UNICODE)
add_compile_definitions (ASSIMP_BUILD_NO_C4D_IMPORTER)

add_compile_definitions (MINIZ_USE_UNALIGNED_LOADS_AND_STORES=0)
add_compile_definitions (ASSIMP_IMPORTER_GLTF_USE_OPEN3DGC=1)
add_compile_definitions (RAPIDJSON_HAS_STDSTRING=1)
add_compile_definitions (RAPIDJSON_NOMEMBERITERATORCLASS)
add_compile_definitions (ASSIMP_BUILD_DLL_EXPORT)
add_compile_definitions (_SCL_SECURE_NO_WARNINGS)
add_compile_definitions (_CRT_SECURE_NO_WARNINGS)
add_compile_definitions (OPENDDLPARSER_BUILD)

add_compile_definitions (ASSIMP_BUILD_NO_GLTF_IMPORTER)
add_compile_definitions (ASSIMP_BUILD_NO_GLTF2_IMPORTER)
add_compile_definitions (ASSIMP_BUILD_NO_GLTF_EXPORTER)
add_compile_definitions (ASSIMP_BUILD_NO_GLTF2_EXPORTER)

# define header and source files for the library
file(GLOB HEADER_FILES "../../Include/FBAssimp/*.h")
file(GLOB SOURCE_FILES "../../Source/FBAssimp/*.h" "../../Source/FBAssimp/*.cpp" "../../Source/FBAssimp/*.c" "../../Source/FBAssimp/*.cc" "../../Source/FBAssimp/*.mm")

set(FB_HEADER_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBAssimp/../../Include/FBAssimp")
set(FB_SOURCE_REL_PATH "${FB_ROOT}/FBEngine/cpp/Project/FBAssimp/../../Source/FBAssimp")

file(GLOB HEADER_FILES_SUBFOLDER "../../Include/FBAssimp/Extern/*.h")
file(GLOB SOURCE_FILES_SUBFOLDER "../../Source/FBAssimp/Extern/*.cpp")
list(APPEND HEADER_FILES ${HEADER_FILES_SUBFOLDER})
list(APPEND SOURCE_FILES ${SOURCE_FILES_SUBFOLDER})

file(GLOB_RECURSE HEADER_FILES_SUBFOLDER "../../Include/FBAssimp/Extern/assimp/*.h")
file(GLOB_RECURSE SOURCE_FILES_SUBFOLDER "../../Source/FBAssimp/Extern/assimp/*.h" "../../Source/FBAssimp/Extern/assimp/*.cpp")
list(APPEND HEADER_FILES ${HEADER_FILES_SUBFOLDER})
list(APPEND SOURCE_FILES ${SOURCE_FILES_SUBFOLDER})

file(GLOB_RECURSE HEADER_FILES_SUBFOLDER "../../Include/FBAssimp/Extern/contrib/Open3DGC/*.h")
file(GLOB_RECURSE SOURCE_FILES_SUBFOLDER "../../Source/FBAssimp/Extern/contrib/Open3DGC/*.h" "../../Source/FBAssimp/Extern/contrib/Open3DGC/*.cpp")
list(APPEND HEADER_FILES ${HEADER_FILES_SUBFOLDER})
list(APPEND SOURCE_FILES ${SOURCE_FILES_SUBFOLDER})

file(GLOB_RECURSE HEADER_FILES_SUBFOLDER "../../Include/FBAssimp/Extern/contrib/openddlparser/*.h")
file(GLOB_RECURSE SOURCE_FILES_SUBFOLDER "../../Source/FBAssimp/Extern/contrib/openddlparser/*.h" "../../Source/FBAssimp/Extern/contrib/openddlparser/*.cpp")
list(APPEND HEADER_FILES ${HEADER_FILES_SUBFOLDER})
list(APPEND SOURCE_FILES ${SOURCE_FILES_SUBFOLDER})

file(GLOB_RECURSE HEADER_FILES_SUBFOLDER "../../Include/FBAssimp/Extern/contrib/poly2tri/*.h")
file(GLOB_RECURSE SOURCE_FILES_SUBFOLDER "../../Source/FBAssimp/Extern/contrib/poly2tri/*.h" "../../Source/FBAssimp/Extern/contrib/poly2tri/*.cpp")
list(APPEND HEADER_FILES ${HEADER_FILES_SUBFOLDER})
list(APPEND SOURCE_FILES ${SOURCE_FILES_SUBFOLDER})

file(GLOB_RECURSE HEADER_FILES_SUBFOLDER "../../Include/FBAssimp/Extern/contrib/polyclipping/*.h")
file(GLOB_RECURSE SOURCE_FILES_SUBFOLDER "../../Source/FBAssimp/Extern/contrib/polyclipping/*.h" "../../Source/FBAssimp/Extern/contrib/polyclipping/*.cpp")
list(APPEND HEADER_FILES ${HEADER_FILES_SUBFOLDER})
list(APPEND SOURCE_FILES ${SOURCE_FILES_SUBFOLDER})

file(GLOB_RECURSE HEADER_FILES_SUBFOLDER "../../Include/FBAssimp/Extern/contrib/pugixml/*.h")
file(GLOB_RECURSE SOURCE_FILES_SUBFOLDER "../../Source/FBAssimp/Extern/contrib/pugixml/*.h" "../../Source/FBAssimp/Extern/contrib/pugixml/*.cpp")
list(APPEND HEADER_FILES ${HEADER_FILES_SUBFOLDER})
list(APPEND SOURCE_FILES ${SOURCE_FILES_SUBFOLDER})

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

if (FB_USE_ASSET_IMPORT)
    #include_directories( ${FB_ROOT}/AdditionalLibraries/assimp/include )
    #include_directories( ${FB_ROOT}/AdditionalLibraries/assimp/include/vc142 )
endif()

include_directories( ${FB_ROOT}/Dependencies/tinybind )
include_directories( ${FB_ROOT}/Dependencies/tinyxml )
include_directories( ${FB_ROOT}/Dependencies/utf8/source )
include_directories( ${FB_ROOT}/AdditionalLibraries/zlib )
include_directories( ${FB_ROOT}/AdditionalLibraries/zziplib )
include_directories( ../../Include )
include_directories( "${FB_ROOT}/FBEngine/cpp/Include/FBAssimp/Extern/assimp" )
include_directories( "${FB_ROOT}/FBEngine/cpp/Include/FBAssimp/Extern/assimp/assimp" )
include_directories( "${FB_ROOT}/FBEngine/cpp/Source/FBAssimp/Extern/assimp" )


include_directories( "${FB_ROOT}/FBEngine/cpp/Source/FBAssimp/Extern/contrib" )
include_directories( "${FB_ROOT}/FBEngine/cpp/Source/FBAssimp/Extern/contrib/draco/src" )
include_directories( "${FB_ROOT}/FBEngine/cpp/Source/FBAssimp/Extern/contrib/pugixml/src" )
include_directories( "${FB_ROOT}/FBEngine/cpp/Source/FBAssimp/Extern/contrib/pugixml/src" )
include_directories( "${FB_ROOT}/FBEngine/cpp/Source/FBAssimp/Extern/contrib/poly2tri" )
include_directories( "${FB_ROOT}/FBEngine/cpp/Source/FBAssimp/Extern/contrib/zlib/contrib" )
include_directories( "${FB_ROOT}/FBEngine/cpp/Source/FBAssimp/Extern/contrib/polyclipping" )
include_directories( "${FB_ROOT}/FBEngine/cpp/Source/FBAssimp/Extern/contrib/zip" )
include_directories( "${FB_ROOT}/FBEngine/cpp/Source/FBAssimp/Extern/contrib/zip/src" )
include_directories( "${FB_ROOT}/FBEngine/cpp/Source/FBAssimp/Extern/contrib/openddlparser/include" )
include_directories( "${FB_ROOT}/FBEngine/cpp/Source/FBAssimp/Extern/contrib/zlib" )
include_directories( "${FB_ROOT}/FBEngine/cpp/Source/FBAssimp/Extern" )
include_directories( "${FB_ROOT}/AdditionalLibraries/rapidjson/include" )
include_directories( "${FB_ROOT}/AdditionalLibraries/rapidjson/include/rapidjson" )


if (WIN32)
    include_directories( "${FB_ROOT}/FBEngine/cpp/Include/FBAssimp/Extern/assimp/vc142" )
    include_directories( "${FB_ROOT}/FBEngine/cpp/Include/FBAssimp/Extern/assimp/vc142/assimp" )
elseif (APPLE)
    include_directories( "${FB_ROOT}/FBEngine/cpp/Include/FBAssimp/Extern/assimp/xcode" )
    include_directories( "${FB_ROOT}/FBEngine/cpp/Include/FBAssimp/Extern/assimp/xcode/assimp" )
elseif (UNIX)
    include_directories( "${FB_ROOT}/FBEngine/cpp/Include/FBAssimp/Extern/assimp/linux" )
    include_directories( "${FB_ROOT}/FBEngine/cpp/Include/FBAssimp/Extern/assimp/linux/assimp" )
endif()

if (FB_USE_PRECOMPILED_HEADERS)
    use_precompiled_header(FBAssimp "FBAssimp/FBAssimpPCH.h" "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBAssimp/FBAssimpPCH.cpp")

    file(GLOB_RECURSE OGRE_SOURCE_FILES "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBAssimp/Extern/*.cpp")
    set_source_files_properties(${OGRE_SOURCE_FILES} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

    file(GLOB_RECURSE OGRE_SOURCE_FILES "${CMAKE_SOURCE_DIR}/FBEngine/cpp/Source/FBAssimp/Extern/*.c")
    set_source_files_properties(${OGRE_SOURCE_FILES} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endif()

add_library(FBAssimp STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBAssimp PROPERTIES FOLDER Engine/Cpp/Render)
