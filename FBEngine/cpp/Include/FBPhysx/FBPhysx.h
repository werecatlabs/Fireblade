#ifndef __FBPhysx_h__
#define __FBPhysx_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Memory/CSharedObject.h>

#if FB_USE_AUTO_LINK
#    ifdef _DEBUG
#        if FB_ARCH_TYPE == FB_ARCHITECTURE_32
#            pragma comment( lib, "FBPhysx.lib" )
#            pragma comment( lib, "PhysX3DEBUG_x86.lib" )
//#pragma comment(lib, "FoundationCHECKED.lib")
#            pragma comment( lib, "PhysX3CommonDEBUG_x86.lib" )

//#pragma comment(lib, "PhysX3CommonCHECKED.lib")
#            pragma comment( lib, "PhysX3CommonCHECKED_x86.lib" )

#            pragma comment( lib, "PhysX3CookingDEBUG_x86.lib" )
#            pragma comment( lib, "PhysX3ExtensionsDEBUG.lib" )
#            pragma comment( lib, "PhysX3CharacterKinematicDEBUG_x86.lib" )
#            pragma comment( lib, "PhysX3VehicleDEBUG.lib" )
#            pragma comment( lib, "PhysXVisualDebuggerSDKDEBUG.lib" )
#        elif FB_ARCH_TYPE == FB_ARCHITECTURE_64
#            pragma comment( lib, "FBPhysx.lib" )
#            pragma comment( lib, "PhysX3DEBUG_x64.lib" )
//#pragma comment(lib, "FoundationCHECKED.lib")
#            pragma comment( lib, "PhysX3CommonDEBUG_x64.lib" )

//#pragma comment(lib, "PhysX3CommonCHECKED.lib")
#            pragma comment( lib, "PhysX3CommonCHECKED_x64.lib" )

#            pragma comment( lib, "PhysX3CookingDEBUG_x64.lib" )
#            pragma comment( lib, "PhysX3ExtensionsDEBUG.lib" )
#            pragma comment( lib, "PhysX3CharacterKinematicDEBUG_x64.lib" )
#            pragma comment( lib, "PhysX3VehicleDEBUG.lib" )
#            pragma comment( lib, "PhysXVisualDebuggerSDKDEBUG.lib" )
#        endif
#    elif NDEBUG
#        if FB_ARCH_TYPE == FB_ARCHITECTURE_32
#            pragma comment( lib, "PhysX3_x86.lib" )
#            pragma comment( lib, "PhysX3Extensions.lib" )
#            pragma comment( lib, "PhysX3Common_x86.lib" )
#            pragma comment( lib, "PhysX3Vehicle.lib" )
#            pragma comment( lib, "PhysX3Cooking_x86.lib" )
#            pragma comment( lib, "PhysXProfileSDK.lib" )
#            pragma comment( lib, "PxTask.lib" )
#        else
#            pragma comment( lib, "PhysX3_x64.lib" )
#            pragma comment( lib, "PhysX3Extensions.lib" )
#            pragma comment( lib, "PhysX3Common_x64.lib" )
#            pragma comment( lib, "PhysX3Vehicle.lib" )
#            pragma comment( lib, "PhysX3Cooking_x64.lib" )
#            pragma comment( lib, "PhysXProfileSDK.lib" )
#            pragma comment( lib, "PxTask.lib" )
#        endif
#    else
#        if FB_ARCH_TYPE == FB_ARCHITECTURE_32
#            pragma comment( lib, "FBPhysx.lib" )
#            pragma comment( lib, "PhysX3_x86.lib" )
#            pragma comment( lib, "PhysX3Common_x86.lib" )
#            pragma comment( lib, "PhysX3Cooking_x86.lib" )
#            pragma comment( lib, "PhysX3Extensions.lib" )
#            pragma comment( lib, "PhysX3CharacterKinematic_x86.lib" )
#            pragma comment( lib, "PhysX3Vehicle.lib" )
#        else
#            pragma comment( lib, "FBPhysx.lib" )
#            pragma comment( lib, "PhysX3_x64.lib" )
#            pragma comment( lib, "PhysX3Extensions.lib" )
#            pragma comment( lib, "PhysX3Common_x64.lib" )
#            pragma comment( lib, "PhysX3Vehicle.lib" )
#            pragma comment( lib, "PhysX3Cooking_x64.lib" )
#            pragma comment( lib, "PhysXProfileSDK.lib" )
#            pragma comment( lib, "PxTask.lib" )
#        endif
#    endif
#endif

#ifdef FB_PLATFORM_WIN32
#    ifndef _FB_STATIC_LIB_
#        define FB_PHYSX_API __declspec( dllexport )
#    else
#        define FB_PHYSX_API
#    endif
#else
#    define FB_PHYSX_API
#endif

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/SmartPtr.h>

namespace fb
{
    namespace physics
    {
        class FBPhysx : public CSharedObject<ISharedObject>
        {
        public:
            FBPhysx() = default;
            ~FBPhysx() = default;

            void load( SmartPtr<ISharedObject> data );
            void unload( SmartPtr<ISharedObject> data );
        };

        SmartPtr<IPhysicsManager> createPhysxManager( const SmartPtr<Properties> &properties );

    }  // end namespace physics
}  // end namespace fb

#endif  // FBPhysx_h__
