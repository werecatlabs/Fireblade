set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(cpp_defines)
include(cpp_compiler_options)

if (FB_BUILD_EDITOR)
	add_subdirectory(./Editor)
endif()



