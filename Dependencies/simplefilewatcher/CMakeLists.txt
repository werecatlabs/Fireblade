file(GLOB_RECURSE HEADER_FILES "./includes/*.h")
file(GLOB_RECURSE SOURCE_FILES "./source/*.c" "./source/*.cpp")

include_directories( includes )

add_library(simplefilewatcher ${HEADER_FILES} ${SOURCE_FILES})
