#include <FBCore/FBCorePCH.h>
#include <FBCore/System/Factory.h>
#include <FBCore/Memory/Memory.h>

namespace fb
{
    Factory::Factory()
    {
        m_listener = new Listener( this );
    }

    Factory::~Factory()
    {
        if( !m_listener )
        {
            delete m_listener;
            m_listener = nullptr;
        }
    }

    void Factory::load( SmartPtr<ISharedObject> data )
    {
    }

    void Factory::unload( SmartPtr<ISharedObject> data )
    {
    }

    auto Factory::getGrowSize() const -> u32
    {
        return m_nextSize;
    }

    void Factory::setGrowSize( u32 size )
    {
        m_nextSize = size;
    }

    void Factory::allocatePoolData()
    {
    }

    auto Factory::allocateMemory() -> void *
    {
        auto *ptr = Memory::ScalableAlignedMalloc( m_objectSize, FB_ALIGNMENT );
        if( !ptr )
        {
            std::terminate();
        }

        return ptr;
    }

    void Factory::freeMemory( void *ptr )
    {
        Memory::ScalableAlignedFree( ptr );
    }

    auto Factory::createObject() -> void *
    {
        auto *ptr = Memory::ScalableAlignedMalloc( m_objectSize, FB_ALIGNMENT );
        if( !ptr )
        {
            std::terminate();
        }

        return ptr;
    }

    void Factory::freeObject( void *object )
    {
        Memory::ScalableAlignedFree( object );
    }

    auto Factory::getObjectSize() const -> u32
    {
        return m_objectSize;
    }

    void Factory::setObjectSize( u32 objectSize )
    {
        m_objectSize = objectSize;
    }

    auto Factory::getListener() const -> ISharedObjectListener *
    {
        return m_listener;
    }

    auto Factory::isObjectDerivedFromByInfo( u32 typeInfo ) const -> bool
    {
        return false;
    }

    auto Factory::getTypeName() const -> const c8 *
    {
        return m_typeName.c_str();
    }

    void Factory::setTypeName( const String &typeName )
    {
        FB_ASSERT( typeName.size() < 128 );
        m_typeName = typeName;
    }

    auto Factory::createArray( u32 numElements ) -> void *
    {
        return nullptr;
    }

    auto Factory::getObjectType() const -> const c8 *
    {
        return m_objectType.c_str();
    }

    void Factory::setObjectType( const String &type )
    {
        FB_ASSERT( type.size() < 128 );
        m_objectType = type;
    }

    auto Factory::getObjectTypeId() const -> hash64
    {
        return m_objectTypeId;
    }

    void Factory::setObjectTypeId( hash64 id )
    {
        m_objectTypeId = id;
    }

    auto Factory::getMemoryUsed() const -> int
    {
        return 0;
    }

    auto Factory::getInstanceObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        return {};
    }

    Factory::Listener::Listener() = default;

    Factory::Listener::Listener( Factory *factory ) : m_factory( factory )
    {
    }

    Factory::Listener::~Listener() = default;

    void Factory::Listener::loadingStateChanged( ISharedObject *sharedObject, LoadingState oldState,
                                                 LoadingState newState )
    {
    }

    auto Factory::Listener::destroy( void *ptr ) -> bool
    {
        if( m_factory )
        {
            m_factory->freeObject( ptr );
            return true;
        }

        return false;
    }

    Factory *Factory::Listener::getFactory() const
    {
        return m_factory;
    }

    void Factory::Listener::setFactory( Factory *factory )
    {
        m_factory = factory;
    }

}  // namespace fb
