#ifndef __CViewportOgreNext_H
#define __CViewportOgreNext_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Graphics/Viewport.h>

namespace fb
{
    namespace render
    {

        class CViewportOgreNext : public Viewport
        {
        public:
            CViewportOgreNext();
            ~CViewportOgreNext() override;

            void initialise( Ogre::Viewport *viewport );

            void update() override;

            void _getObject( void **ppObject ) const override;

        protected:
            class ViewportStateListener : public IStateListener
            {
            public:
                ViewportStateListener() = default;
                ~ViewportStateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                CViewportOgreNext *getOwner() const;
                void setOwner( CViewportOgreNext *owner );

            protected:
                CViewportOgreNext *m_owner = nullptr;
            };

            Ogre::Viewport *m_viewport = nullptr;
        };
    }  // end namespace render
}  // end namespace fb

#endif
