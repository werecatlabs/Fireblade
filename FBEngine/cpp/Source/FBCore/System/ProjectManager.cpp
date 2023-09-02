#include <FBCore/FBCorePCH.h>
#include <FBCore/System/ProjectManager.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/IO/IFileSystem.h>

namespace fb
{
    void ProjectManager::generateCMakeProject()
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto fileSystem = applicationManager->getFileSystem();

        Array<String> defines;
        Array<String> includes;
        Array<String> libraryPaths;
        Array<String> libs;

        defines.push_back( "_FB_STATIC_LIB_" );

#if FB_USE_ONETBB
        defines.push_back( "FB_USE_ONETBB=1" );
#elif FB_USE_TBB
        defines.push_back( "FB_USE_TBB=1" );
#endif

        auto enginePath = String( "E:/dev/fireblade" );

        includes.push_back( enginePath + "/Dependencies/boost" );

#if FB_USE_ONETBB
        includes.push_back( enginePath + "/Dependencies/onetbb/include" );
#elif FB_USE_TBB
        includes.push_back( enginePath + "/Dependencies/tbb/include" );
#endif

        includes.push_back( enginePath + "/FBEngine/cpp/Include/FBCPlusPlus" );
        includes.push_back( enginePath + "/AdditionalLibraries/cppsqlite/include" );
        includes.push_back( enginePath + "/AdditionalLibraries/sqlite/include" );
        // includes.push_back(enginePath + "/AdditionalLibraries/aurora");
        includes.push_back( enginePath + "/FBEngine/cpp/Include" );
        includes.push_back( enginePath + "/FBEngine/cpp/Include/FBCPlusPlus" );

        libs.push_back( "FBCore.lib" );
        libs.push_back( "FBFileSystem.lib" );
        libs.push_back( "FBMath.lib" );
        libs.push_back( "FBMesh.lib" );
        libs.push_back( "FBInterface.lib" );
        libs.push_back( "FBProcedural.lib" );
        libs.push_back( "FBSystem.lib" );
        libs.push_back( "FBPhysx.lib" );
        libs.push_back( "FBData.lib" );
        libs.push_back( "FBSQLite.lib" );
        libs.push_back( "FBObjectTemplates.lib" );
        libs.push_back( "FBVehicle.lib" );
        libs.push_back( "FBWxWidgets.lib" );
        libs.push_back( "FBObjectTemplates.lib" );
        libs.push_back( "FBApplication.lib" );
        libs.push_back( "FBState.lib" );
        libs.push_back( "FBOISInput.lib" );
        libs.push_back( "FBGraphics.lib" );
        // libs.push_back("glfw.lib");
        // libs.push_back("RocketControls_d.lib");
        // libs.push_back("RocketCore_d.lib");
        // libs.push_back("RocketDebugger_d.lib");
        // libs.push_back("RuntimeCompiler_VS2010.lib");
        // libs.push_back("RuntimeObjectSystem.lib");
        // libs.push_back("Systems.lib");

        auto toolset = String( "v142" );
        auto arch = String( "x64" );

#if FB_STATIC_LINK_CRT
        auto crtType = String( "MT" );
#else
        auto crtType = String( "MD" );
#endif

        libraryPaths.push_back( enginePath + "/libs/windows/" + toolset + "/" + arch + "/" + crtType +
                                "/${CMAKE_BUILD_TYPE}" );
        libraryPaths.push_back( enginePath + "/AdditionalLibraries/lib/windows/" + toolset + "/" + arch +
                                "/" + crtType + "/physx/" );
        libraryPaths.push_back( enginePath + "/AdditionalLibraries/lib/windows/" + toolset + "/" + arch +
                                "/" + crtType + "/ogre/" );
        libraryPaths.push_back( enginePath + "/Dependencies/lib/windows/" + toolset + "/" + arch + "/" +
                                crtType + "/tbb/" );
        libraryPaths.push_back( enginePath + "/Dependencies/lib/windows/" + toolset + "/" + arch + "/" +
                                crtType + "/boost/" );
        libraryPaths.push_back( enginePath + "/Dependencies/lib/windows/" + toolset + "/" + arch + "/" +
                                crtType + "/tinybind/" );
        libraryPaths.push_back( enginePath + "/AdditionalLibraries/lib/windows/" + toolset + "/" + arch +
                                "/" + crtType + "/wildmagic/" );
        libraryPaths.push_back( enginePath + "/AdditionalLibraries/lib/windows/" + toolset + "/" + arch +
                                "/" + crtType + "/assimp/${CMAKE_BUILD_TYPE}" );
        libraryPaths.push_back( enginePath + "/AdditionalLibraries/lib/windows/" + toolset + "/" + arch +
                                "/" + crtType + "/ois/" );
        // libraryPaths.push_back(enginePath + "/AdditionalLibraries/aurora/x64/${CMAKE_BUILD_TYPE}");

        auto newLineStr = "\n";
        auto cmakeRequired = String( "cmake_minimum_required(VERSION 3.13)\n" );
        auto projectName = String( "Vehicle" );
        auto projectDeclare = "project(" + projectName + ")\n";

        auto compileOptions =
            String( "if (WIN32)" ) + newLineStr +
            String( "	set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} /fp:fast\")" ) + newLineStr +
            String( "	set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} /Qvec-report:2\")" ) + newLineStr +
            String( "" ) + newLineStr + String( "	if (NOT FB_PLATFORM_X64)" ) + newLineStr +
            String( "		set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} /arch:SSE2\")" ) + newLineStr +
            String( "	endif()" ) + newLineStr + String( "" ) + newLineStr +

#if FB_STATIC_LINK_CRT
            String( "	set(CMAKE_CXX_FLAGS_DEBUG \"/D_DEBUG /MP /MTd /Zi /Ob0 /Od\")" ) + newLineStr +
            String( "	set(CMAKE_CXX_FLAGS_MINSIZEREL \"/MT /MP /Zi /O2 /Ob2 /D NDEBUG\")" ) +
            newLineStr + String( "	set(CMAKE_CXX_FLAGS_RELEASE \"/MT /MP /O2 /Ob2 /Zi /D NDEBUG\")" ) +
            newLineStr +
            String( "	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO \"/MT /MP /Zi /Od /Ob0 /D NDEBUG\")" ) +
            newLineStr +
#else
            String( "	set(CMAKE_CXX_FLAGS_DEBUG \"/D_DEBUG /MP /MDd /Zi /Ob0 /Od\")" ) + newLineStr +
            String( "	set(CMAKE_CXX_FLAGS_MINSIZEREL \"/MD /MP /Zi /O2 /Ob2 /D NDEBUG\")" ) +
            newLineStr + String( "	set(CMAKE_CXX_FLAGS_RELEASE \"/MD /MP /O2 /Ob2 /Zi /D NDEBUG\")" ) +
            newLineStr +
            String( "	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO \"/MD /MP /Zi /Od /Ob0 /D NDEBUG\")" ) +
            newLineStr +
#endif
            String( "elseif(APPLE)" ) + newLineStr +
            String( "	set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -msse -msse2 -msse3\")" ) + newLineStr +
            String( "	set(CMAKE_CXX_FLAGS_DEBUG \"-g\")" ) + newLineStr +
            String( "	set(CMAKE_CXX_FLAGS_MINSIZEREL \"-O3 -finline-functions -DNDEBUG\")" ) +
            newLineStr +
            String( "	set(CMAKE_CXX_FLAGS_RELEASE \"-O3 -finline-functions -DNDEBUG\")" ) +
            newLineStr +
            String( "	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO \"-O3 -g -finline-functions -DNDEBUG\")" ) +
            newLineStr + String( "else()" ) + newLineStr +
            String( "	set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -msse -msse2 -msse3\")" ) + newLineStr +
            String(
                "	#set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -msse -msse2 -msse3 -Wno-everything\")" ) +
            newLineStr + String( "	set(CMAKE_CXX_FLAGS_DEBUG \"-g\")" ) + newLineStr +
            String( "	set(CMAKE_CXX_FLAGS_MINSIZEREL \"-O3 -finline-functions -DNDEBUG\")" ) +
            newLineStr +
            String( "	set(CMAKE_CXX_FLAGS_RELEASE \"-O3 -finline-functions -DNDEBUG\")" ) +
            newLineStr +
            String( "	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO \"-O3 -g -finline-functions -DNDEBUG\")" ) +
            newLineStr + String( "" ) + newLineStr + String( "	add_compile_definitions(NDEBUG)" ) +
            newLineStr + String( "endif()" );

        auto headerFilesStr = String( "file(GLOB_RECURSE HEADER_FILES \"Assets/*.h\")\n" );
        auto sourceFilesStr = String( "file(GLOB_RECURSE SOURCE_FILES \"Assets/*.cpp\")\n" );

        auto defineOpen = String( "add_compile_definitions(" );
        auto defineClose = String( ")" );

        auto includeDirOpen = String( "include_directories(" );
        auto includeDirClose = String( ")" );

        auto libraryDirOpen = String( "link_directories(" );
        auto libraryDirClose = String( ")" );

        auto libraryOpen = String( "target_link_libraries(" );
        auto libraryClose = String( ")" );

        auto cmakeStr = cmakeRequired + projectDeclare;
        cmakeStr += newLineStr;
        cmakeStr += "set(CMAKE_CXX_STANDARD 17)";
        cmakeStr += newLineStr;
        cmakeStr += newLineStr;

        cmakeStr += compileOptions + newLineStr;

        cmakeStr += newLineStr;

        for( auto define : defines )
        {
            cmakeStr += defineOpen + define + defineClose + newLineStr;
        }

        cmakeStr += newLineStr + headerFilesStr + sourceFilesStr + newLineStr;

        for( auto includeDir : includes )
        {
            cmakeStr += includeDirOpen + includeDir + includeDirClose + newLineStr;
        }

        cmakeStr += newLineStr;

        for( auto libraryPath : libraryPaths )
        {
            cmakeStr += libraryDirOpen + projectName + " " + libraryPath + libraryDirClose + newLineStr;
        }

        cmakeStr += newLineStr;

        auto libraryType = String( "" );

        if( isSharedLibrary() )
        {
            libraryType = String( "SHARED" );
        }

        auto libraryDeclare = String( "add_library(" + projectName + " " + libraryType +
                                      " ${HEADER_FILES} ${SOURCE_FILES})\n" );
        cmakeStr += libraryDeclare + newLineStr;

        for( auto lib : libs )
        {
            cmakeStr += libraryOpen + projectName + " " + lib + libraryClose + newLineStr;
        }

        cmakeStr += newLineStr;

        auto projectPath = applicationManager->getProjectPath();

        fileSystem->writeAllText( projectPath + "/CMakeLists.txt", cmakeStr );
    }

    bool ProjectManager::isSharedLibrary() const
    {
        return m_isSharedLibrary;
    }

    void ProjectManager::setSharedLibrary( bool val )
    {
        m_isSharedLibrary = val;
    }
}  // end namespace fb
