#ifndef _CCameraController_H
#define _CCameraController_H

#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Math/AABB3.h>
#include <FBCore/Math/Ray3.h>

namespace fb
{
    namespace scene
    {
        /** A base camera controller class.
         */
        class FBCore_API CameraController : public Component
        {
        public:
            CameraController();
            ~CameraController() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            void setFlag( u32 flag, bool value );
            bool getFlag( u32 flag ) const;

            void setViewportId( u32 viewportId );
            u32 getViewportId() const;

            bool isInFrustum( const AABB3<real_Num> &box ) const;

            void handleSetActive( bool active );

            /** @copydoc IComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            virtual void focusSelection();

            FB_CLASS_REGISTER_DECL;

        protected:
            class EventListener : public IEventListener
            {
            public:
                EventListener() = default;
                ~EventListener() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object,
                                       SmartPtr<IEvent> event ) override;

                void setOwner( CameraController *owner );
                CameraController *getOwner() const;

            protected:
                CameraController *m_cameraController = nullptr;
            };

            u8 m_viewportId = 0;
            atomic_u32 m_flags = 0;
            SmartPtr<IEventListener> m_eventListener;
        };
    } // namespace scene
}     // namespace fb

#endif
