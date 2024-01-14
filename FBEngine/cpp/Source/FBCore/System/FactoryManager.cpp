#include <FBCore/FBCorePCH.h>
#include <FBCore/System/FactoryManager.h>
#include <FBCore/System/Factory.h>
#include <FBCore/Core/Exception.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/Interface/System/IState.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, FactoryManager, IFactoryManager );

    FactoryManager::FactoryManager()
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "FactoryManager" );
        }

        auto factories = fb::make_shared<ConcurrentArray<SmartPtr<IFactory>>>();
        factories->reserve( 1024 );
        setFactoriesArray( factories );
    }

    FactoryManager::~FactoryManager()
    {
        unload( nullptr );
    }

    void FactoryManager::load( SmartPtr<ISharedObject> data )
    {
    }

    void FactoryManager::unload( SmartPtr<ISharedObject> data )
    {
        if( auto pFactories = getFactoriesArray() )
        {
            auto &factories = *pFactories;

            for( auto &factory : factories )
            {
                auto objects = factory->getInstanceObjects();
                for( auto object : objects )
                {
                    if( object )
                    {
                        object->unload( nullptr );
                    }
                }
            }
        }

        if( auto pFactories = getFactoriesArray() )
        {
            auto &factories = *pFactories;

            for( auto &factory : factories )
            {
                factory->unload( nullptr );
            }
        }

        removeAllFactories();
        setFactoriesArray( nullptr );
    }

    auto FactoryManager::getFactoryUnloadPriority( SmartPtr<IFactory> factory ) -> s32
    {
        if( factory->isObjectDerivedFrom<IStateContext>() )
        {
            return 2000;
        }
        if( factory->isObjectDerivedFrom<IState>() )
        {
            return 1000;
        }

        return 0;
    }

    void FactoryManager::addFactory( SmartPtr<IFactory> factory )
    {
        try
        {
            auto objectType = factory->getObjectType();

            if( !hasFactory( objectType ) )
            {
                RecursiveMutex::ScopedLock lock( m_mutex );

                if( auto p = getFactoriesArray() )
                {
                    auto &factories = *p;
                    factories.push_back( factory );
                }
                else
                {
                    FB_LOG_ERROR( "FactoryManager::addFactory: p is nullptr" );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto FactoryManager::removeFactory( SmartPtr<IFactory> factory ) -> bool
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto pFactories = getFactoriesArray();
        auto &factories = *pFactories;

        auto factoriesArray = Array<SmartPtr<IFactory>>( factories.begin(), factories.end() );

        auto it = std::find( factoriesArray.begin(), factoriesArray.end(), factory );
        if( it != factoriesArray.end() )
        {
            factoriesArray.erase( it );

            auto pNewFactories = fb::make_shared<ConcurrentArray<SmartPtr<IFactory>>>();

            *pNewFactories =
                ConcurrentArray<SmartPtr<IFactory>>( factoriesArray.begin(), factoriesArray.end() );
            setFactoriesArray( pNewFactories );
            return true;
        }

        return false;
    }

    void FactoryManager::removeAllFactories()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_factories = nullptr;
    }

    auto FactoryManager::getFactory( hash64 id ) const -> const SmartPtr<IFactory>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto pFactories = getFactoriesArray();
        auto &factories = *pFactories;

        for( auto &factory : factories )
        {
            if( factory->getObjectTypeId() == id )
            {
                return factory;
            }
        }

        return nullptr;
    }

    auto FactoryManager::getFactoryByName( const String &name ) const -> SmartPtr<IFactory>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto pFactories = getFactoriesArray();
        auto &factories = *pFactories;

        auto splitNames = StringUtil::split( name, "::" );
        std::reverse( splitNames.begin(), splitNames.end() );

        for( auto &factory : factories )
        {
            auto objectType = factory->getObjectType();
            if( objectType == name )
            {
                return factory;
            }

            auto splitFactroyNames = StringUtil::split( objectType, "::" );
            std::reverse( splitFactroyNames.begin(), splitFactroyNames.end() );

            if( splitFactroyNames.front() == splitNames.front() )
            {
                return factory;
            }
        }

        return nullptr;
    }

    auto FactoryManager::getFactoryById( hash64 id ) const -> SmartPtr<IFactory>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto pFactories = getFactoriesArray();
        auto &factories = *pFactories;

        for( auto &factory : factories )
        {
            FB_ASSERT( factory );

            auto factoryObjectType = factory->getObjectTypeId();
            if( factoryObjectType == id )
            {
                return factory;
            }
        }

        return nullptr;
    }

    auto FactoryManager::hasFactory( const String &typeName ) const -> bool
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto pFactories = getFactoriesArray();
        auto &factories = *pFactories;

        for( auto &factory : factories )
        {
            auto objectType = factory->getObjectType();
            if( objectType == typeName )
            {
                return true;
            }
        }

        return false;
    }

    auto FactoryManager::hasFactoryByName( const String &name ) const -> bool
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto pFactories = getFactoriesArray();
        auto &factories = *pFactories;

        for( auto &factory : factories )
        {
            auto factoryName = factory->getObjectType();
            if( factoryName == name )
            {
                return true;
            }
        }

        return false;
    }

    auto FactoryManager::hasFactoryById( hash64 typeId ) const -> bool
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto pFactories = getFactoriesArray();
        auto &factories = *pFactories;

        for( auto &factory : factories )
        {
            auto objectTypeId = factory->getObjectTypeId();
            if( objectTypeId == typeId )
            {
                return true;
            }
        }

        return false;
    }

    auto FactoryManager::getFactories() const -> Array<SmartPtr<IFactory>>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto pFactories = getFactoriesArray();
        auto &factories = *pFactories;
        return Array<SmartPtr<IFactory>>( factories.begin(), factories.end() );
    }

    auto FactoryManager::createById( hash64 typeId ) const -> SmartPtr<ISharedObject>
    {
        auto typeManager = TypeManager::instance();
        FB_ASSERT( typeManager );

        auto typeHash = typeManager->getHash( static_cast<u32>( typeId ) );
        FB_ASSERT( typeHash != 0 );

        auto factory = getFactoryById( typeHash );
        if( !factory )
        {
            auto typeName = typeManager->getName( static_cast<u32>( typeId ) );
            factory = getFactoryByName( typeName );
        }

        if( !factory )
        {
            auto derivedTypes = typeManager->getDerivedTypes( static_cast<u32>( typeId ) );

            for( auto derivedType : derivedTypes )
            {
                auto factoryHash = typeManager->getHash( derivedType );
                FB_ASSERT( factoryHash != 0 );

                factory = getFactoryById( factoryHash );
                if( factory )
                {
                    break;
                }

                auto factoryName = typeManager->getName( derivedType );

                factory = getFactoryByName( factoryName );
                if( factory )
                {
                    break;
                }
            }
        }

        if( factory )
        {
            auto ptr = static_cast<ISharedObject *>( factory->createObject() );
            auto p = SmartPtr<ISharedObject>( ptr );
            ptr->removeReference();
            return p;
        }

        return nullptr;
    }

    SmartPtr<ISharedObject> FactoryManager::createById( hash64 typeId, const String &hint ) const
    {
        auto typeManager = TypeManager::instance();
        FB_ASSERT( typeManager );

        auto typeHash = typeManager->getHash( static_cast<u32>( typeId ) );
        FB_ASSERT( typeHash != 0 );

        auto factory = getFactoryById( typeHash );
        if( !factory )
        {
            auto typeName = typeManager->getName( static_cast<u32>( typeId ) );
            factory = getFactoryByName( typeName );
        }

        if( !factory )
        {
            auto derivedTypes = typeManager->getDerivedTypes( static_cast<u32>( typeId ) );

            for( auto derivedType : derivedTypes )
            {
                auto factoryHash = typeManager->getHash( derivedType );
                FB_ASSERT( factoryHash != 0 );

                factory = getFactoryById( factoryHash );
                if( factory )
                {
                    break;
                }

                auto factoryName = typeManager->getName( derivedType );

                factory = getFactoryByName( factoryName );
                if( factory )
                {
                    break;
                }
            }
        }

        if( factory )
        {
            auto ptr = static_cast<ISharedObject *>( factory->createObject() );
            auto p = SmartPtr<ISharedObject>( ptr );
            ptr->removeReference();
            return p;
        }

        return nullptr;
    }

    auto FactoryManager::getFactoriesArray() const -> SharedPtr<ConcurrentArray<SmartPtr<IFactory>>>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_factories;
    }

    void FactoryManager::setFactoriesArray( SharedPtr<ConcurrentArray<SmartPtr<IFactory>>> ptr )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_factories = ptr;
    }

    void FactoryManager::setPoolSize( hash64 typeId, size_t size )
    {
        if( auto factory = getFactory( typeId ) )
        {
            factory->setGrowSize( static_cast<u32>( size ) );
            factory->allocatePoolData();
        }
    }
}  // end namespace fb
