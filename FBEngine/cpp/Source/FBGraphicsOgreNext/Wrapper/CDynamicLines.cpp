#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CDynamicLines.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h>
#include <FBGraphicsOgreNext/DynamicLines.h>
#include <Ogre.h>
#include <Math/Array/OgreObjectMemoryManager.h>

#include <utility>

namespace fb::render
{

    CDynamicLines::CDynamicLines( SmartPtr<render::IGraphicsScene> creator ) :
        m_creator( std::move( creator ) )
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
        m_creator->_getObject( reinterpret_cast<void **>( &smgr ) );

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

    auto CDynamicLines::getName() const -> String
    {
        return m_name;
    }

    auto CDynamicLines::getId() const -> u32
    {
        return 0;
    }

    void CDynamicLines::setRenderQueueGroup( u8 renderQueue )
    {
        // m_dynamicLines->setRenderQueueGroup(renderQueue);
    }

    auto CDynamicLines::clone( const String &name ) const -> SmartPtr<IGraphicsObject>
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

    auto CDynamicLines::getPoint( u32 index ) const -> Vector3F
    {
        Ogre::Vector3 point = m_dynamicLines->getPoint( index );
        return Vector3F( point.ptr() );
    }

    auto CDynamicLines::getNumPoints() const -> u32
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

    auto CDynamicLines::getOperationType() const -> u32
    {
        return 0;  //(u32)m_dynamicLines->getOperationType();
    }

    auto CDynamicLines::getMemoryManager() const -> Ogre::ObjectMemoryManager *
    {
        return m_memoryManager;
    }

    void CDynamicLines::setMemoryManager( Ogre::ObjectMemoryManager *memoryManager )
    {
        m_memoryManager = memoryManager;
    }

}  // namespace fb::render
