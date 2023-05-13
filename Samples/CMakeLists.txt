set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(cpp_defines)

if (FB_BUILD_TESTS)
	add_subdirectory(./Application)
	add_subdirectory(./Game)
	add_subdirectory(./HelloWorld)
	add_subdirectory(./Physics)
	add_subdirectory(./Vehicle)
endif()



