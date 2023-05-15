#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CDynamicLines.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsSceneOgre.h>
#include <FBGraphicsOgre/DynamicLines.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CDynamicLines, CGraphicsObjectOgre<IDynamicLines> );

        CDynamicLines::CDynamicLines( SmartPtr<IGraphicsScene> creator ) : m_creator( creator )
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
            m_dynamicLines = new DynamicLines;
        }

        void CDynamicLines::update()
        {
            if( m_dynamicLines )
            {
                m_dynamicLines->update();
            }
        }

        void CDynamicLines::setMaterialName( const String &materialName, s32 index )
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            // m_dynamicLines->setMaterial(materialName.c_str());
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
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            m_dynamicLines->addPoint( point.X(), point.Y(), point.Z() );
        }

        void CDynamicLines::setPoint( u32 index, const Vector3F &point )
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            Ogre::Vector3 ogrePoint;  // (point);
            m_dynamicLines->setPoint( index, ogrePoint );
        }

        Vector3F CDynamicLines::getPoint( u32 index ) const
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            Ogre::Vector3 point = m_dynamicLines->getPoint( index );
            return Vector3F( point.ptr() );
        }

        u32 CDynamicLines::getNumPoints() const
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_dynamicLines->getNumPoints();
        }

        void CDynamicLines::clear()
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            m_dynamicLines->clear();
        }

        void CDynamicLines::setDirty()
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            m_dynamicLines->update();
        }

        void CDynamicLines::setOperationType( u32 opType )
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            // m_dynamicLines->setOperationType((Ogre::RenderOperation::OperationType)opType);
        }

        u32 CDynamicLines::getOperationType() const
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            return 0;  //(u32)m_dynamicLines->getOperationType();
        }
    }  // end namespace render
}  // end namespace fb
