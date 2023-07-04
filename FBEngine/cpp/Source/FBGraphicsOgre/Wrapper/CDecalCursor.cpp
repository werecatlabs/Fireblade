#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CDecalCursor.h>
#include <FBGraphicsOgre/Ogre/DecalCursor.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CDecalCursor, SharedObject<IDecalCursor> );

        CDecalCursor::CDecalCursor() : m_decalCursor( nullptr )
        {
            auto engine = core::IApplicationManager::instance();

            // m_stateObject = platformMgr->createStateObject();

            m_stateListener = SmartPtr<IStateListener>( new CDecalCursorStateListener( this ) );
            m_stateObject->addStateListener( m_stateListener );
        }

        CDecalCursor::~CDecalCursor()
        {
        }

        void CDecalCursor::initialise( SmartPtr<IGraphicsScene> sceneMgr, const String &terrainMaterial,
                                       const String &decalTextureName, const Vector2F &size )
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );

            Ogre::SceneManager *smgr = nullptr;
            sceneMgr->_getObject( (void **)&smgr );

            m_sceneMgr = sceneMgr.get();

            Ogre::MaterialManager *materialMgr = Ogre::MaterialManager::getSingletonPtr();
            Ogre::MaterialPtr terrainMat = materialMgr->getByName( terrainMaterial.c_str() );

            auto s = Ogre::Vector2( size.X(), size.Y() );
            m_decalCursor = new DecalCursor( smgr, terrainMat, s, decalTextureName );
        }

        bool CDecalCursor::isVisible() const
        {
            return m_isVisible;
        }

        void CDecalCursor::setVisible( bool visible )
        {
            m_isVisible = visible;
        }

        Vector3F CDecalCursor::getPosition() const
        {
            return m_position;
        }

        void CDecalCursor::setPosition( const Vector3F &position )
        {
            auto currentTaskId = Thread::getCurrentTask();

            if( currentTaskId != Thread::Task::Render )
            {
                SmartPtr<StateMessageVector3> message( new StateMessageVector3( position ) );
                m_stateObject->addMessage( Thread::Task::Render, message );
            }
            else
            {
                m_decalCursor->setPosition( Ogre::Vector3( position.X(), position.Y(), position.Z() ) );
            }

            if( m_node )
            {
                m_node->setPosition( position );
            }
        }

        Vector2F CDecalCursor::getSize() const
        {
            return m_size;
        }

        void CDecalCursor::setSize( const Vector2F &size )
        {
            m_size = size;
        }

        void CDecalCursor::addDebugEntity( const String &entityName, const Vector3F &scale )
        {
            m_entity = m_sceneMgr->addMesh( entityName );
            m_node = m_sceneMgr->getRootSceneNode()->addChildSceneNode();
            m_node->attachObject( m_entity );
            m_node->setScale( scale );
            // m_node->getStateObject()->add();
        }

        void CDecalCursor::removeDebugEntity()
        {
        }

        SmartPtr<IStateContext> &CDecalCursor::getStateObject()
        {
            return m_stateObject;
        }

        const SmartPtr<IStateContext> &CDecalCursor::getStateObject() const
        {
            return m_stateObject;
        }

        void CDecalCursor::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        String CDecalCursor::getTextureName() const
        {
            return m_textureName;
        }

        void CDecalCursor::setTextureName( const String &val )
        {
            m_textureName = val;
        }

        void CDecalCursor::CDecalCursorStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            // if(message->isExactly(StateMessageVector3::TYPE_INFO))
            //{
            //	SmartPtr<StateMessageVector3> positionMessage = message;

            //	Vector3F position = positionMessage->getValue();
            //	m_owner->setPosition(position);
            //}
        }

        void CDecalCursor::CDecalCursorStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
        }

        void CDecalCursor::CDecalCursorStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        CDecalCursor::CDecalCursorStateListener::~CDecalCursorStateListener()
        {
        }

        CDecalCursor::CDecalCursorStateListener::CDecalCursorStateListener( CDecalCursor *owner ) :
            m_owner( owner )
        {
        }
    }  // end namespace render
}  // end namespace fb
