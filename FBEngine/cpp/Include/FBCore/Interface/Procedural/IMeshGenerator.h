#ifndef IMeshGenerator_h__
#define IMeshGenerator_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace procedural
    {
        class IMeshGenerator : public ISharedObject
        {
        public:
            ~IMeshGenerator() override = default;

            virtual SmartPtr<IMesh> generate( SmartPtr<IRoadNetwork> roadNetwork ) = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IMeshGenerator_h__
