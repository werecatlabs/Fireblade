#ifndef _COverlayElementContainer_H
#define _COverlayElementContainer_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IOverlayElementContainer.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayElementOgre.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/State/States/BaseState.h>

namespace fb
{
    namespace render
    {
        class COverlayElementContainer : public COverlayElementOgre<IOverlayElementContainer>
        {
        public:
            COverlayElementContainer();
            ~COverlayElementContainer() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            bool isContainer() const override;

            // IOverlayElementContainer functions
            void addChild( SmartPtr<IOverlayElement> element ) override;
            void removeChild( SmartPtr<IOverlayElement> element ) override;

            SmartPtr<Properties> getProperties() const override;
            void setProperties( SmartPtr<Properties> properties ) override;

            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc IObject::isValid */
            bool isValid() const override;

            Ogre::v1::OverlayContainer *getContainerElement() const;
            void setContainerElement( Ogre::v1::OverlayContainer *container );

            FB_CLASS_REGISTER_DECL;

        protected:
            class StateListener : public StateListenerOgre
            {
            public:
                StateListener() = default;
                ~StateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;
            };

            class MaterialStateListener : public IStateListener
            {
            public:
                MaterialStateListener() = default;
                MaterialStateListener( COverlayElementContainer *owner );

                ~MaterialStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                SmartPtr<COverlayElementContainer> getOwner() const;
                void setOwner( SmartPtr<COverlayElementContainer> val );

            protected:
                AtomicSmartPtr<COverlayElementContainer> m_owner;
            };

            void setupMaterial( SmartPtr<IMaterial> material ) override;
            void createStateContext() override;

            void materialLoaded( SmartPtr<IMaterial> material );

            Ogre::v1::OverlayContainer *m_container = nullptr;

            SmartPtr<IMaterial> m_material;

            SmartPtr<IStateListener> m_materialStateListener;
        };
    }  // end namespace render
}  // end namespace fb

#endif
