project(FBWxWidgets)


add_compile_definitions (_UNICODE)

if (APPLE)
    add_compile_definitions(__WXMAC__)
    add_compile_definitions(__WXOSX__)
    add_compile_definitions(__WXOSX_COCOA__)
elseif (UNIX)
    add_compile_definitions(__WXUNIX__)
    add_compile_definitions(__WXGTK__)
endif()

# define header and source files for the library
file(GLOB_RECURSE HEADER_FILES "../../Include/FBWxWidgets/*.h" "../../Include/FBWxWidgets/*.inl")
file(GLOB_RECURSE SOURCE_FILES "../../Source/FBWxWidgets/*.cpp")


include_directories( ${FB_ROOT}/Dependencies/boost )

if (FB_USE_ONETBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/onetbb/include )
elseif (FB_USE_TBB)
    include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tbb/include )
endif()

include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tinybind )
include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/tinyxml )
include_directories( ${CMAKE_SOURCE_DIR}/Dependencies/utf8/source )
include_directories( ${CMAKE_SOURCE_DIR}/AdditionalLibraries/imgui  )
include_directories( ${CMAKE_SOURCE_DIR}/AdditionalLibraries/wxWidgets  )
include_directories( ${CMAKE_SOURCE_DIR}/AdditionalLibraries/wxWidgets/include )

if (WIN32)
    include_directories( ${CMAKE_SOURCE_DIR}/AdditionalLibraries/wxWidgets/include/msvc )
    include_directories( ${CMAKE_SOURCE_DIR}/AdditionalLibraries/wxWidgets/include/msvc/wx )
    include_directories( ${CMAKE_SOURCE_DIR}/AdditionalLibraries/wxWidgets/lib/vc_x64_lib/mswud  )
elseif (APPLE)
    include_directories( ${CMAKE_SOURCE_DIR}/AdditionalLibraries/wxWidgets/lib/osx/wx/include/osx_cocoa-unicode-static-3.1  )
elseif (UNIX)
    include_directories( ${CMAKE_SOURCE_DIR}/AdditionalLibraries/wxWidgets/include/osx_cocoa-unicode-static-3.1 )
    include_directories( ${CMAKE_SOURCE_DIR}/AdditionalLibraries/wxWidgets/include/osx_cocoa-unicode-static-3.1/wx )
endif ()

include_directories( ../../Include )

if (FB_USE_PRECOMPILED_HEADERS)
    use_precompiled_header(FBWxWidgets "FBWxWidgets/FBWxWidgetsPCH.h" "${FB_ROOT}/FBEngine/cpp/Source/FBWxWidgets/FBWxWidgetsPCH.cpp")
endif()

add_library(FBWxWidgets STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties (FBWxWidgets PROPERTIES FOLDER Engine/Cpp/Render)
