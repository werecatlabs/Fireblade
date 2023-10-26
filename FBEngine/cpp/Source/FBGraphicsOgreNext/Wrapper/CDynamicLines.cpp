#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CDynamicLines.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h>
#include <FBGraphicsOgreNext/DynamicLines.h>
#include <Ogre.h>
#include <Math/Array/OgreObjectMemoryManager.h>

namespace fb
{
    namespace render
    {

        CDynamicLines::CDynamicLines( SmartPtr<render::IGraphicsScene> creator ) : m_creator( creator )
        {
        }

        CDynamicLines::~CDynamicLines()
        {
            if( m_dynamicLines )
            {
                delete m_dynamicLines;
                m_dynamicLines = nullptr;
            }
        }

        void CDynamicLines::initialise()
        {
            Ogre::SceneManager *smgr = nullptr;
            m_creator->_getObject( (void **)&smgr );

            auto id = Ogre::Id::generateNewId<DynamicLines>();
            m_dynamicLines = new DynamicLines( id, new Ogre::ObjectMemoryManager(), smgr );
        }

        void CDynamicLines::update()
        {
            if( m_dynamicLines )
            {
                m_dynamicLines->update();
            }
        }

        void CDynamicLines::setName( const String &name )
        {
            m_name = name;
        }

        String CDynamicLines::getName() const
        {
            return m_name;
        }

        u32 CDynamicLines::getId() const
        {
            return 0;
        }

        void CDynamicLines::setRenderQueueGroup( u8 renderQueue )
        {
            // m_dynamicLines->setRenderQueueGroup(renderQueue);
        }

        SmartPtr<IGraphicsObject> CDynamicLines::clone( const String &name ) const
        {
            return nullptr;
        }

        void CDynamicLines::_getObject( void **ppObject ) const
        {
            *ppObject = m_dynamicLines;
        }

        void CDynamicLines::addPoint( const Vector3F &point )
        {
            m_dynamicLines->addPoint( point.X(), point.Y(), point.Z() );
        }

        void CDynamicLines::setPoint( u32 index, const Vector3F &point )
        {
            Ogre::Vector3 ogrePoint;  // (point);
            m_dynamicLines->setPoint( index, ogrePoint );
        }

        Vector3F CDynamicLines::getPoint( u32 index ) const
        {
            Ogre::Vector3 point = m_dynamicLines->getPoint( index );
            return Vector3F( point.ptr() );
        }

        u32 CDynamicLines::getNumPoints() const
        {
            return m_dynamicLines->getNumPoints();
        }

        void CDynamicLines::clear()
        {
            m_dynamicLines->clear();
        }

        void CDynamicLines::setDirty()
        {
            m_dynamicLines->update();
        }

        void CDynamicLines::setOperationType( u32 opType )
        {
            // m_dynamicLines->setOperationType((Ogre::RenderOperation::OperationType)opType);
        }

        u32 CDynamicLines::getOperationType() const
        {
            return 0;  //(u32)m_dynamicLines->getOperationType();
        }

        Ogre::ObjectMemoryManager *CDynamicLines::getMemoryManager() const
        {
            return m_memoryManager;
        }

        void CDynamicLines::setMemoryManager( Ogre::ObjectMemoryManager *val )
        {
            m_memoryManager = val;
        }

    }  // end namespace render
}  // end namespace fb
