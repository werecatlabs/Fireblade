#ifndef CameraFollow_h__
#define CameraFollow_h__

#include <FBCore/Scene/Components/Component.h>

namespace fb
{
    namespace scene
    {
        /** A component to follow an object.
         */
        class FBCore_API CameraFollow : public Component
        {
        public:
            /** Constructor. */
            CameraFollow();

            /** Destructor. */
            ~CameraFollow() override;

            /** @copydoc Component::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc Component::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            SmartPtr<CameraTarget> getTarget() const;

            void setTarget( SmartPtr<CameraTarget> target );

            SmartPtr<IActor> getFollowObject() const;

            void setFollowObject( SmartPtr<IActor> followObject );

            FB_CLASS_REGISTER_DECL;

        protected:
            SmartPtr<CameraTarget> m_target;
            SmartPtr<IActor> m_followObject;
        };
    }  // namespace scene
}  // namespace fb

#endif  // CameraFollow_h__
