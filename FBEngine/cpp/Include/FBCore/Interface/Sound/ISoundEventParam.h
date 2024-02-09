#ifndef ISoundEventParam_h__
#define ISoundEventParam_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /** Interface for a sound event parameter. */
    class ISoundEventParam : public ISharedObject
    {
    public:
        /** Destructor. */
        ~ISoundEventParam() override = default;

        /** Gets the parameter value. */
        virtual f32 getValue() const = 0;

        /** Sets the parameter value. */
        virtual void setValue( f32 value ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // ISoundEventParam_h__
