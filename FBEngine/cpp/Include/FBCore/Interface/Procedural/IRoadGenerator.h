#ifndef IRoadGenerator_h__
#define IRoadGenerator_h__

#include <FBCore/Interface/Procedural/IProceduralGenerator.h>

namespace fb
{
    namespace procedural
    {
        class IRoadGenerator : public IProceduralGenerator
        {
        public:
            ~IRoadGenerator() override = default;

            virtual String getPatternData() const = 0;
            virtual void setPatternData( const String &val ) = 0;

            virtual SmartPtr<IProceduralCity> getCity() const = 0;
            virtual void setCity( SmartPtr<IProceduralCity> val ) = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IRoadGenerator_h__
