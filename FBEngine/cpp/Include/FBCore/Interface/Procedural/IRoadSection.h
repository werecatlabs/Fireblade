#ifndef IRoadSection_h__
#define IRoadSection_h__

#include <FBCore/Interface/Procedural/IProceduralObject.h>

namespace fb
{
    namespace procedural
    {
        class IRoadSection : public IProceduralObject
        {
        public:
            ~IRoadSection() override = default;

            virtual void addRoadElement( SmartPtr<IRoadElement> roadElement ) = 0;
            virtual void removeRoadElement( SmartPtr<IRoadElement> roadElement ) = 0;
            virtual void clearRoadElements() = 0;

            virtual Array<SmartPtr<IRoadElement>> getElements() const = 0;
            virtual void setElements( const Array<SmartPtr<IRoadElement>> &elements ) = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IRoadSection_h__
