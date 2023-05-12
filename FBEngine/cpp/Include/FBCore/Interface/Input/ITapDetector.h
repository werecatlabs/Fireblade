#ifndef ITapDetector_h__
#define ITapDetector_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /** Detects input taps.*/
    class ITapDetector : public ISharedObject
    {
    public:
        ~ITapDetector() override = default;
    };

}  // end namespace fb

#endif  // ITapDetector_h__
