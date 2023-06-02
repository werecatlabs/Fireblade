#include <FBCore/FBCorePCH.h>
#include <FBCore/Memory/GarbageCollector.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/Memory/ISharedObjectListener.h>
#include <FBCore/Memory/TypeManager.h>
#include <FBCore/Memory/Memory.h>
#include <FBCore/Core/Util.h>

#if _DEBUG
#    include <FBCore/Core/LogManager.h>
#endif

namespace fb
{
    GarbageCollector GarbageCollector::m_sGarbageCollector;

    GarbageCollector::GarbageCollector()
    {
        auto count = 0;
        for( auto &objectData : m_objectData )
        {
            objectData.setTypeGroup( static_cast<TypeGroups>( count ) );
            count++;
        }

        load();
    }

    GarbageCollector::~GarbageCollector()
    {
        unload();
    }

    void GarbageCollector::load()
    {
    }

    void GarbageCollector::unload()
    {
        for( auto &objectData : m_objectData )
        {
            objectData.unload();
        }
    }

    void GarbageCollector::update()
    {
        switch( auto mode = getMode() )
        {
        case GarbageCollectorMode::Deferred:
        {
            Array<Array<u32>> references;
            references.resize( static_cast<u32>( TypeGroups::Count ) );

            for( size_t i = 0; i < references.size(); ++i )
            {
                references[i].reserve( 32 );
            }

            auto typeManager = TypeManager::instance();

            while( !m_addReference.empty() )
            {
                Pair<u32, u32> p;
                if( m_addReference.try_pop( p ) )
                {
                    //auto &objectData = getObjectData( p.first );
                    //objectData.addReference( p.second );
                    const auto group = typeManager->getTypeGroup( p.first );
                    references[group].push_back( p.second );
                }
            }

            auto group = 0;
            for( auto &r : references )
            {
                std::sort( r.begin(), r.end() );
                auto &objectData = m_objectData[group];
                for( auto &ref : r )
                {
                    objectData.addReference( ref );
                }

                ++group;
            }

            if( !m_removeReference.empty() )
            {
                Pair<u32, u32> p;
                if( m_removeReference.try_pop( p ) )
                {
                    auto &objectData = getObjectData( p.first );
                    objectData.removeReference( p.second );
                }
            }
        }
        break;
        default:
        {
        }
        }
    }

    void GarbageCollector::updateReferences()
    {
    }

    s32 GarbageCollector::addReference( u32 typeInfo, u32 id )
    {
        switch( auto mode = getMode() )
        {
        case GarbageCollectorMode::LowConsumption:
        case GarbageCollectorMode::Balanced:
        {
            auto &objectData = getObjectData( typeInfo );
            return objectData.addReference( id );
        }
        break;
        case GarbageCollectorMode::Deferred:
        {
            m_addReference.push( std::make_pair( typeInfo, id ) );
        }
        break;
        default:
        {
        }
        }

        return 0;
    }

    s32 GarbageCollector::removeReference( u32 typeInfo, u32 id )
    {
        switch( auto mode = getMode() )
        {
        case GarbageCollectorMode::LowConsumption:
        case GarbageCollectorMode::Balanced:
        {
            auto &objectData = getObjectData( typeInfo );
            return objectData.removeReference( id );
        }
        break;
        case GarbageCollectorMode::Deferred:
        {
            m_removeReference.push( std::make_pair( typeInfo, id ) );
        }
        break;
        default:
        {
        }
        }

        return 0;
    }

    void GarbageCollector::destroyObject( u32 typeInfo, u32 id )
    {
        auto &objectData = getObjectData( typeInfo );
        objectData.destroyObject( id );
    }

    GarbageCollectorData &GarbageCollector::getObjectData( u32 typeInfo )
    {
        auto typeManager = TypeManager::instance();
        const auto index = typeManager->getTypeGroup( typeInfo );
        return m_objectData[index];
    }

    const GarbageCollectorData &GarbageCollector::getObjectData( u32 typeInfo ) const
    {
        auto typeManager = TypeManager::instance();
        const auto index = typeManager->getTypeGroup( typeInfo );
        return m_objectData[index];
    }

    u32 GarbageCollector::addObject( ISharedObject *object )
    {
        auto typeInfo = object->getTypeInfo();

        auto &objectData = getObjectData( typeInfo );
        objectData.load();

        return objectData.addObject( object );
    }

    void GarbageCollector::removeObject( u32 typeInfo, u32 id )
    {
        auto &objectData = getObjectData( typeInfo );
        //objectData.load();

        return objectData.removeObject( id );
    }

    const Atomic<LoadingState> &GarbageCollector::getLoadingState( u32 typeInfo, u32 id ) const
    {
        auto &objectData = getObjectData( typeInfo );
        return objectData.getLoadingState( id );
    }

    Atomic<LoadingState> *GarbageCollector::getLoadingStatePtr( u32 typeInfo, u32 id ) const
    {
        auto &objectData = getObjectData( typeInfo );
        return objectData.getLoadingStatePtr( id );
    }

    void GarbageCollector::setLoadingState( u32 typeInfo, u32 id, const Atomic<LoadingState> &state )
    {
        auto &objectData = getObjectData( typeInfo );
        objectData.setLoadingState( id, state );
    }

    ISharedObjectListener *GarbageCollector::getObjectListener( u32 typeInfo, u32 id ) const
    {
        auto &objectData = getObjectData( typeInfo );
        return objectData.getObjectListener( id );
    }

    void GarbageCollector::setObjectListener( u32 typeInfo, u32 id, ISharedObjectListener *listener )
    {
        auto &objectData = getObjectData( typeInfo );
        objectData.setObjectListener( id, listener );
    }

    Handle *GarbageCollector::getHandle( u32 typeInfo, u32 id ) const
    {
        auto &objectData = getObjectData( typeInfo );
        return objectData.getHandle( id );
    }

    void *GarbageCollector::getUserData( u32 typeInfo, u32 id ) const
    {
        auto &objectData = getObjectData( typeInfo );
        return objectData.getUserData( id );
    }

    void GarbageCollector::setCreatorData( u32 typeInfo, u32 id, void *creatorData )
    {
        auto &objectData = getObjectData( typeInfo );
        objectData.setCreatorData( id, creatorData );
    }

    void *GarbageCollector::getCreatorData( u32 typeInfo, u32 id ) const
    {
        auto &objectData = getObjectData( typeInfo );
        return objectData.getCreatorData( id );
    }

    void GarbageCollector::setUserData( u32 typeInfo, u32 id, void *userData )
    {
        auto &objectData = getObjectData( typeInfo );
        objectData.setUserData( id, userData );
    }

    hash32 GarbageCollector::getFactoryData( u32 typeInfo, u32 id ) const
    {
        const auto &objectData = getObjectData( typeInfo );
        return objectData.getFactoryData( id );
    }

    void GarbageCollector::setFactoryData( u32 typeInfo, u32 id, hash32 factoryData )
    {
        auto &objectData = getObjectData( typeInfo );
        objectData.setFactoryData( id, factoryData );
    }

    void GarbageCollector::setReferences( u32 typeInfo, u32 id, s32 references )
    {
        auto &objectData = getObjectData( typeInfo );
        objectData.setReferences( id, references );
    }

    ISharedObject **GarbageCollector::getObjectPtr( u32 typeInfo, u32 id ) const
    {
        const auto &objectData = getObjectData( typeInfo );
        return objectData.getObjectPtr( id );
    }

    ISharedObject *GarbageCollector::getObject( u32 typeInfo, u32 id ) const
    {
        const auto &objectData = getObjectData( typeInfo );
        return objectData.getObject( id );
    }

    void GarbageCollector::setObject( u32 typeInfo, u32 id, ISharedObject *object )
    {
        auto &objectData = getObjectData( typeInfo );
        objectData.setObject( id, object );
    }

    atomic_s32 *GarbageCollector::getReferencesPtr( u32 typeInfo, u32 id )
    {
        auto &objectData = getObjectData( typeInfo );
        return objectData.getReferencesPtr( id );
    }

    GarbageCollectorMode GarbageCollector::getMode() const
    {
        return m_mode;
    }

    void GarbageCollector::setMode( GarbageCollectorMode mode )
    {
        m_mode = mode;
    }

    bool GarbageCollector::isValid() const
    {
        for( auto &objectData : m_objectData )
        {
            if( !objectData.isValid() )
            {
                return false;
            }
        }

        return true;
    }

    void GarbageCollector::queueFreedObject( u32 typeInfo, u32 id )
    {
        auto &objectData = getObjectData( typeInfo );
        objectData.queueFreedObject( id );
    }

    s32 GarbageCollector::getReferences( u32 typeInfo, u32 id ) const
    {
        const auto &objectData = getObjectData( typeInfo );
        return objectData.getReferences( id );
    }

    void GarbageCollector::setFlag( u32 typeInfo, u32 id, u8 flag, bool value )
    {
        auto &objectData = getObjectData( typeInfo );
        objectData.setFlag( id, flag, value );
    }

    bool GarbageCollector::getFlag( u32 typeInfo, u32 id, u8 flag ) const
    {
        const auto &objectData = getObjectData( typeInfo );
        return objectData.getFlag( id, flag );
    }

    atomic_u8 *GarbageCollector::getFlagPtr( u32 typeInfo, u32 id ) const
    {
        const auto &objectData = getObjectData( typeInfo );
        return objectData.getFlagPtr( id );
    }

    Array<ISharedObject *> GarbageCollector::getObjects() const
    {
        Array<ISharedObject *> objects;
        for( auto &objectData : m_objectData )
        {
            auto objs = objectData.getObjects();
            objects.insert( objects.begin(), objs.begin(), objs.end() );
        }

        return objects;
    }

}  // end namespace fb
