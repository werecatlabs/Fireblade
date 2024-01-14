#ifndef WheelController_h__
#define WheelController_h__

#include <FBCore/Scene/Components/Component.h>

namespace fb
{
    namespace scene
    {

        /** WheelController component.
         */
        class WheelController : public Component
        {
        public:
            WheelController();
            ~WheelController() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc BaseComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc BaseComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            SmartPtr<IWheelController> getWheelController() const;

            void setWheelController( SmartPtr<IWheelController> wheelController );

            f32 getMassFraction() const;
            void setMassFraction( f32 val );

            f32 getRadius() const;
            void setRadius( f32 val );

            f32 getWheelDamping() const;
            void setWheelDamping( f32 val );

            f32 getSuspensionDistance() const;
            void setSuspensionDistance( f32 val );

            void reset();

            FB_CLASS_REGISTER_DECL;

        protected:
            SmartPtr<IWheelController> m_wheelController;

            f32 m_massFraction = 0.05f;
            f32 m_radius = 0.5f;
            f32 m_wheelDamping = 1.0f;
            f32 m_suspensionDistance = 0.3f;
            f32 m_springRate = 1.0f;
            f32 m_suspensionDamper = 1.0f;
            f32 m_targetPosition = 0.0f;
            f32 m_forwardExtremumSlip = 10.0f;
            f32 m_forwardExtrememValue = 10.0f;
            f32 m_forwardAsymptoteSlip = 10.0f;
            f32 m_forwardAsymptoteValue = 10.0f;
            f32 m_forwardStiffness = 10.0f;
            f32 m_sidewaysExtremumSlip = 1.0f;
            f32 m_sidewaysExtrememValue = 1.0f;
            f32 m_sidewaysAsymptoteSlip = 1.0f;
            f32 m_sidewaysAsymptoteValue = 1.0f;
            f32 m_sidewaysStiffness = 1.0f;
            f32 m_steeringAngle = 1.0f;
            bool m_isSteeringWheel = false;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // WheelController_h__
