file(GLOB_RECURSE HEADER_FILES "./include/*.h")
file(GLOB_RECURSE SOURCE_FILES "./source/*.c" "./source/*.cpp")

include_directories( include )

add_library(rttr ${HEADER_FILES} ${SOURCE_FILES})
