#ifndef CGraphicsObject_h__
#define CGraphicsObject_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Graphics/CGraphicsObject.h>
#include <FBCore/Interface/Graphics/IGraphicsObject.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/Handle.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/System/IEvent.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/State/States/GraphicsObjectState.h>
#include <OgreMovableObject.h>

namespace fb
{
    namespace render
    {

        template <class T>
        class CGraphicsObjectOgreNext : public CGraphicsObject<T>
        {
        public:
            CGraphicsObjectOgreNext() = default;
            ~CGraphicsObjectOgreNext() override = default;

            Ogre::MovableObject *getMovableObject() const;

            void setMovableObject( Ogre::MovableObject *movableObject );

        protected:
            class CStateListener : public IStateListener
            {
            public:
                CStateListener() = default;
                ~CStateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override
                {
                
                }

                void handleStateChanged( SmartPtr<IState> &state ) override
                {
                    auto graphicsMeshState = fb::static_pointer_cast<GraphicsObjectState>( state );
                    auto visible = graphicsMeshState->isVisible();

                    if( auto owner = getOwner() )
                    {
                        if( owner->isLoaded() )
                        {
                            if( auto movableObject = owner->getMovableObject() )
                            {
                                movableObject->setVisible( visible );
                            }
                        }
                    }
                }

                void handleQuery( SmartPtr<IStateQuery> &query ) override
                {
                }

                CGraphicsObjectOgreNext<T>* getOwner() const
                {
                    return m_owner;
                }

                void setOwner( CGraphicsObjectOgreNext<T>* *owner )
                {
                    m_owner = owner;
                }

            protected:
                CGraphicsObjectOgreNext<T>* m_owner = nullptr;
            };

            Ogre::MovableObject *m_movableObject = nullptr;
        };

        template <class T>
        Ogre::MovableObject *CGraphicsObjectOgreNext<T>::getMovableObject() const
        {
            return m_movableObject;
        }

        template <class T>
        void CGraphicsObjectOgreNext<T>::setMovableObject( Ogre::MovableObject *movableObject )
        {
            m_movableObject = movableObject;
        }

    }  // end namespace render
}  // end namespace fb

#endif  // CGraphicsObject_h__
