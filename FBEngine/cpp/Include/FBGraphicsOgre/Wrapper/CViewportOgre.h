#ifndef __CViewportOgre_H
#define __CViewportOgre_H

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBGraphics/Wrapper/CViewport.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    namespace render
    {

        /**
         * Implementation of the IViewport interface using the Ogre3D rendering engine.
         */
        class CViewportOgre : public CViewport
        {
        public:
            /** Default constructor. */
            CViewportOgre();

            /** Default destructor. */
            ~CViewportOgre() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /**
             * Updates the viewport, rendering any changes made since the last update.
             */
            void update() override;

            void _getObject( void **ppObject ) const override;

            Ogre::Viewport *getViewport() const;

            void setViewport( Ogre::Viewport *viewport );

            FB_CLASS_REGISTER_DECL;

        protected:
            class ViewportStateListener : public CSharedObject<IStateListener>
            {
            public:
                ViewportStateListener() = default;
                ~ViewportStateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                CViewportOgre *getOwner() const;
                void setOwner( CViewportOgre *owner );

            protected:
                CViewportOgre *m_owner = nullptr;
            };

            void removeViewportFromRT();

            AtomicRawPtr<Ogre::Viewport> m_viewport;

            String m_backgroundTextureName;
            
            
            bool m_active = true;

            static u32 m_zOrderExt;
        };
    }  // end namespace render
}  // end namespace fb

#endif
