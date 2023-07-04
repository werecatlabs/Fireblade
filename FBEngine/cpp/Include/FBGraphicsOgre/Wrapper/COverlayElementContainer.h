#ifndef _COverlayElementContainer_H
#define _COverlayElementContainer_H

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IOverlayElementContainer.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/State/States/BaseState.h>
#include <FBGraphicsOgre/Wrapper/COverlayElementOgre.h>

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

            void _getObject( void **ppObject ) const override;

            // IOverlayElementContainer functions
            void addChild( SmartPtr<IOverlayElement> element ) override;
            void removeChild( SmartPtr<IOverlayElement> element ) override;

            SmartPtr<Properties> getProperties() const override;
            void setProperties( SmartPtr<Properties> properties ) override;

            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc IObject::isValid */
            bool isValid() const override;

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

            class MaterialStateListener : public SharedObject<IStateListener>
            {
            public:
                MaterialStateListener() = default;
                MaterialStateListener( COverlayElementContainer *owner );

                ~MaterialStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                COverlayElementContainer *getOwner() const;
                void setOwner( COverlayElementContainer *val );

            protected:
                COverlayElementContainer *m_owner = nullptr;
            };

            void setupMaterial( SmartPtr<IMaterial> material ) override;
            void createStateContext() override;

            void materialLoaded( SmartPtr<IMaterial> material );

            Ogre::OverlayContainer *m_container = nullptr;

            SmartPtr<IMaterial> m_material;

            SmartPtr<IStateListener> m_materialStateListener;
        };
    } // end namespace render
}     // end namespace fb

#endif
