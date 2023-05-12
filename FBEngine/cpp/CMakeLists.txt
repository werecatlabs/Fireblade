set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(cpp_defines)
include(cpp_compiler_options)

add_subdirectory(Project/FBAero)
add_subdirectory(Project/FBAi)
add_subdirectory(Project/FBAnimation)
add_subdirectory(Project/FBApplication)
add_subdirectory(Project/FBAudio)
add_subdirectory(Project/FBAssimp)

if (FB_BUILD_BULLETPHYSICS)
	add_subdirectory(Project/FBBulletPhysics)
endif ()

if (FB_BUILD_CEGUI)
	add_subdirectory(Project/FBCEGui)
endif ()

if (FB_BUILD_IMGUI)
	add_subdirectory(Project/FBImGui)
endif ()

add_subdirectory(Project/FBCore)
add_subdirectory(Project/FBCPlusPlus)

if (FB_BUILD_DIRECTSHOW)
	add_subdirectory(Project/FBDirectShow)
endif ()

add_subdirectory(Project/FBFFMpeg)

if (FB_BUILD_FMOD)
	add_subdirectory(Project/FBFMODStudio)
endif ()

add_subdirectory(Project/FBRuntime)

add_subdirectory(Project/FBGraphics)

if (FB_GRAPHICS_SYSTEM_CLAW)
	add_subdirectory(Project/FBGraphicsClaw)
endif()

if (FB_GRAPHICS_SYSTEM_OGRE)
	add_subdirectory(Project/FBGraphicsOgre)
endif()

if (FB_GRAPHICS_SYSTEM_OGRENEXT)
	add_subdirectory(Project/FBGraphicsOgreNext)
endif()

if (FB_GRAPHICS_SYSTEM_SOFTWARE)
	add_subdirectory(Project/FBGraphicsSoftware)
endif()

#add_subdirectory(Project/FBGStreamer)
add_subdirectory(Project/FBRenderUI)

add_subdirectory(Project/FBFileSystem)

add_subdirectory(Project/FBLightmapper)
add_subdirectory(Project/FBMesh)

if (FB_ENABLE_LUA)
	add_subdirectory(Project/FBLua)
	add_subdirectory(Project/FBLuaBind)
endif()

if (FB_ENABLE_PYPY)
	add_subdirectory(Project/FBPyPy)
	add_subdirectory(Project/FBPyPyBind)
endif()

if (FB_ENABLE_PYTHON)
	add_subdirectory(Project/FBPython)
	add_subdirectory(Project/FBPythonBind)
endif()

#add_subdirectory(Project/FBState)
add_subdirectory(Project/FBParticleSystem)
add_subdirectory(Project/FBProcedural)
add_subdirectory(Project/FBVehicle)
add_subdirectory(Project/FBInput)
add_subdirectory(Project/FBPlugin)

if (FB_BUILD_PHYSX)
	add_subdirectory(Project/FBPhysx)
endif()

add_subdirectory(Project/FBPhysics2)
add_subdirectory(Project/FBPhysics3)

if (FB_BUILD_SQLITE)
	add_subdirectory(Project/FBSQLite)
endif()

if (FB_BUILD_ODE)
	add_subdirectory(Project/FBODE3)
endif()
	
add_subdirectory(Project/FBRakNet)
add_subdirectory(Project/FBOISInput)
add_subdirectory(Project/FBTest)

if (FB_USE_HOUDINI)
	add_subdirectory(Project/FBHoudini)
endif()

if (FB_BUILD_WXWIDGETS)
	add_subdirectory(Project/FBWxWidgets)
endif ()