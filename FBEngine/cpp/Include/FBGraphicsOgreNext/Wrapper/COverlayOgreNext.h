#ifndef _COverlay_H
#define _COverlay_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IOverlay.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Graphics/CSharedGraphicsObject.h>

namespace fb
{
    namespace render
    {

        class COverlayOgreNext : public CSharedGraphicsObject<IOverlay>
        {
        public:
            COverlayOgreNext();
            ~COverlayOgreNext() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void update() override;

            /** @copydoc IOverlay::getName */
            String getName() const override;

            /** @copydoc IOverlay::setName */
            void setName( const String &name ) override;

            void addElement( SmartPtr<IOverlayElement> element ) override;
            bool removeElement( SmartPtr<IOverlayElement> element ) override;

            void addSceneNode( SmartPtr<ISceneNode> sceneNode ) override;
            bool removeSceneNode( SmartPtr<ISceneNode> sceneNode ) override;

            void setVisible( bool isVisible ) override;
            bool isVisible() const override;

            void setZOrder( u32 zorder ) override;
            u32 getZOrder() const override;

            void updateZOrder();

            void _getObject( void **ppObject ) const override;

            Array<SmartPtr<IOverlayElement>> getElements() const override;

            Vector2I getAbsoluteResolution() const override;
            void setAbsoluteResolution( const Vector2I &val ) override;

            /** @copydoc ISharedObject::isValid */
            bool isValid() const override;

            /** @copydoc IComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            class OverlayStateListener : public IStateListener
            {
            public:
                OverlayStateListener() = default;
                ~OverlayStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                COverlayOgreNext *getOwner() const;
                void setOwner( COverlayOgreNext *val );

            protected:
                COverlayOgreNext *m_owner = nullptr;
            };

            ///
            Ogre::v1::Overlay *m_overlay = nullptr;

            Ogre::v1::OverlayContainer *m_container = nullptr;

            ///
            Array<SmartPtr<IOverlayElement>> m_elements;

            ///
            SmartPtr<OverlayState> m_state;
        };
    }  // end namespace render
}  // end namespace fb

#endif
