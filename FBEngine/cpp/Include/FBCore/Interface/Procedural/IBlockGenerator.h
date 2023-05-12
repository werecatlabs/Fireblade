#ifndef IBlockGenerator_h__
#define IBlockGenerator_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace procedural
    {
        class IBlockGenerator : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~IBlockGenerator() override = default;

            virtual void generate() = 0;

            virtual SmartPtr<IProceduralCity> getCity() const = 0;
            virtual void setCity( SmartPtr<IProceduralCity> city ) = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IBlockGenerator_h__
