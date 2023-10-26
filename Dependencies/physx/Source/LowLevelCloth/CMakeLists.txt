file(GLOB_RECURSE HEADER_FILES "./*.h")

if (WIN32)
	file(GLOB SOURCE_FILES "./*.c" "./src/*.cpp" "./src/avx/*.cpp" "./src/sse2/*.cpp")
else()
	file(GLOB SOURCE_FILES "./*.c" "./src/*.cpp" "./src/sse2/*.cpp")
endif()

#add_compile_definitions (PX_PHYSX_CORE_EXPORTS)
add_compile_definitions (DELAYIMP_INSECURE_WRITABLE_HOOKS)

include_directories( ../../Include/foundation )
include_directories( ../foundation/include )
include_directories( ../../Include/physxprofilesdk )
include_directories( ../../Include/physxvisualdebuggersdk )
include_directories( ../../Include )
include_directories( ../../Include/pxtask )
include_directories( ../../Include/common )
include_directories( ../Common/src )
include_directories( ../LowLevelCloth/include )
include_directories( ../LowLevelCloth/src )

add_library(PhysX3LowLevelCloth ${HEADER_FILES} ${SOURCE_FILES})