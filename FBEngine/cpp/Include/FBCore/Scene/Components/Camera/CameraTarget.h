#ifndef CameraTarget_h__
#define CameraTarget_h__

#include <FBCore/Scene/Components/Component.h>

namespace fb
{
    namespace scene
    {
        /** A camera target component.
         */
        class FBCore_API CameraTarget : public Component
        {
        public:
            /** Constructor. */
            CameraTarget();

            /** Destructor. */
            ~CameraTarget() override;

            /** @copydoc Component::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc Component::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            Vector3<real_Num> getOffsetPosition() const;

            void setOffsetPosition( const Vector3<real_Num> &offsetPosition );

            Quaternion<real_Num> getOffsetRotation() const;

            void setOffsetRotation( const Quaternion<real_Num> &offsetRotation );

            FB_CLASS_REGISTER_DECL;

        protected:
            Vector3<real_Num> m_offsetPosition;
            Quaternion<real_Num> m_offsetRotation;
        };
    }  // namespace scene
}  // namespace fb

#endif  // CameraTarget_h__
