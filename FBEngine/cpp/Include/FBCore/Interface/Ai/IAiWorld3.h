#ifndef IAiWorld3_h__
#define IAiWorld3_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    //-------------------------------------------------
    class IAiWorld3 : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IAiWorld3() override = default;
    };
}  // end namespace fb

#endif  // IAiWorld3_h__
