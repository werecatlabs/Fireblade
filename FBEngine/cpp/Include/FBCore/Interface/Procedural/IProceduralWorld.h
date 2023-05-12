#ifndef IProceduralWorld_h__
#define IProceduralWorld_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>

namespace fb
{
    namespace procedural
    {
        class IProceduralWorld : public ISharedObject
        {
        public:
            ~IProceduralWorld() override = default;

            virtual void addScene( SmartPtr<IProceduralScene> scene ) = 0;
            virtual void removeScene( SmartPtr<IProceduralScene> scene ) = 0;
            virtual Array<SmartPtr<IProceduralScene>> getScenes() const = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IProceduralWorld_h__
