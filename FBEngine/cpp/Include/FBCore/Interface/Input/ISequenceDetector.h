#ifndef ISequenceDetector_h__
#define ISequenceDetector_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    /** Detects an input sequence.*/
    class ISequenceDetector : public ISharedObject
    {
    public:
        ~ISequenceDetector() override = default;
    };
}  // end namespace fb

#endif  // ISequenceDetector_h__
