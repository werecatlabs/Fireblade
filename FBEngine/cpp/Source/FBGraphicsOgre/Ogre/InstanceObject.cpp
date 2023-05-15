#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include "FBGraphicsOgre/Ogre/InstanceObject.h"
#include <FBCore/FBCore.h>
#include <Ogre.h>

namespace fb
{
    namespace render
    {

        CInstanceObjectOld::CInstanceObjectOld() :
            m_position( Ogre::Vector3::ZERO ),
            m_lastDistance( 1e10 ),
            m_nextUpdate( 0 ),
            m_isVisible( true )
        {
            auto engine = core::IApplicationManager::instance();

            // m_stateObject = platformMgr->createStateObject();

            m_stateListener = SmartPtr<IStateListener>( new InstanceObjectStateListener( this ) );
            m_stateObject->addStateListener( m_stateListener );
        }

        CInstanceObjectOld::~CInstanceObjectOld()
        {
        }

        void CInstanceObjectOld::add( Ogre::InstancedEntity *entity )
        {
            m_instanceEntities.push_back( entity );
        }

        bool CInstanceObjectOld::isVisible() const
        {
            return m_isVisible;
        }

        void CInstanceObjectOld::setVisible( bool visible )
        {
            auto currentTaskId = Thread::getCurrentTask();

            if( currentTaskId == Thread::Task::Render )
            {
                if( m_isVisible != visible )
                {
                    for( u32 i = 0; i < m_instanceEntities.size(); ++i )
                    {
                        Ogre::InstancedEntity *instancedEntity = m_instanceEntities[i];
                        FB_ASSERT( instancedEntity );
                        instancedEntity->setVisible( visible );
                    }

                    m_isVisible = visible;
                }
            }
            else
            {
                if( m_isVisible != visible )
                {
                    auto engine = core::IApplicationManager::instance();
                    // auto& pool = engine->getPool();

                    SmartPtr<StateMessageVisible> visibleMessage( new StateMessageVisible );
                    visibleMessage->setVisible( visible );
                    m_stateObject->addMessage( Thread::Task::Render, visibleMessage );
                }
            }
        }

        const Ogre::Vector3 &CInstanceObjectOld::getPosition() const
        {
            return m_position;
        }

        void CInstanceObjectOld::setPosition( const Ogre::Vector3 &val )
        {
            m_position = val;
        }

        Ogre::InstancedEntity *CInstanceObjectOld::getBillboard() const
        {
            return m_billboard;
        }

        void CInstanceObjectOld::setBillboard( Ogre::InstancedEntity *val )
        {
            m_billboard = val;
        }

        fb::SmartPtr<fb::IStateContext> CInstanceObjectOld::getStateObject() const
        {
            return m_stateObject;
        }

        void CInstanceObjectOld::setStateObject( SmartPtr<IStateContext> val )
        {
            m_stateObject = val;
        }

        CInstanceObjectOld::InstanceObjectStateListener::InstanceObjectStateListener(
            CInstanceObjectOld *instanceObject ) :
            m_instanceObject( instanceObject )
        {
        }

        CInstanceObjectOld::InstanceObjectStateListener::~InstanceObjectStateListener()
        {
        }

        void CInstanceObjectOld::InstanceObjectStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            // if(message->isExactly(StateMessageVisible::TYPE_INFO))
            //{
            //	StateMessageVisiblePtr visibleMessage = message;
            //	m_instanceObject->setVisible(visibleMessage->isVisible());
            // }
        }

        void CInstanceObjectOld::InstanceObjectStateListener::handleStateChanged(
            SmartPtr<IState> &state )
        {
        }

        void CInstanceObjectOld::InstanceObjectStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

    }  // namespace render
}  // end namespace fb
