#ifndef IDebugText_h__
#define IDebugText_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    namespace render
    {

        /** Interface for a debug line. */
        class IDebugText : public ISharedObject
        {
        public:
            /** Destructor. */
            ~IDebugText() override = default;

            /** Returns the text string. */
            virtual String getText() const = 0;

            /** Sets the text string. */
            virtual void setText( const String &text ) = 0;

            /** Returns the underlying overlay text element. */
            virtual SmartPtr<IOverlayElementText> getTextElement() const = 0;

            /** Sets the underlying overlay text element. */
            virtual void setTextElement( SmartPtr<IOverlayElementText> textElement ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // IDebugText_h__
