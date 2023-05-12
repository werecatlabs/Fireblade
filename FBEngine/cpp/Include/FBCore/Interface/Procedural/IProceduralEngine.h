#ifndef IProceduralEngine_h__
#define IProceduralEngine_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace procedural
    {
        class IProceduralEngine : public ISharedObject
        {
        public:
            ~IProceduralEngine() override = default;

            virtual SmartPtr<scene::IActor> import( const String &fileName ) = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IProceduralEngine_h__
