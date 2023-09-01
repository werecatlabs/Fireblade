file(GLOB_RECURSE HEADER_FILES "./include/*.h")
file(GLOB_RECURSE SOURCE_FILES "./src/*.c" "./src/*.cpp")

add_compile_definitions (YAML_CPP_STATIC_DEFINE)

include_directories( include )

add_library(yaml-cpp ${HEADER_FILES} ${SOURCE_FILES})
