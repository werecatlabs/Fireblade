#include <FBCore/FBCorePCH.h>
#include <FBCore/System/FactoryManager.h>
#include <FBCore/FBCore.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, FactoryManager, SharedObject<IFactoryManager> );

    FactoryManager::FactoryManager() : SharedObject<IFactoryManager>()
    {
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

    s32 FactoryManager::getFactoryUnloadPriority( SmartPtr<IFactory> factory )
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
                auto factories = getFactoriesArray();
                factories->push_back( factory );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    bool FactoryManager::removeFactory( SmartPtr<IFactory> factory )
    {
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
        m_factories = nullptr;
    }

    const SmartPtr<IFactory> FactoryManager::getFactory( hash64 id ) const
    {
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

    SmartPtr<IFactory> FactoryManager::getFactoryByName( const String &name ) const
    {
        auto pFactories = getFactoriesArray();
        auto &factories = *pFactories;

        for( auto &factory : factories )
        {
            auto objectType = factory->getObjectType();
            if( objectType == name )
            {
                return factory;
            }
        }

        auto factoryName = StringUtil::replaceAll( name, "class ", "" );
        for( auto &factory : factories )
        {
            auto objectType = factory->getObjectType();
            if( objectType == factoryName )
            {
                return factory;
            }
        }

        for( auto &factory : factories )
        {
            auto factoryObjectType = factory->getObjectType();
            auto factoryObjectTypeCleaned = StringUtil::replaceAll( factoryObjectType, "class ", "" );
            if( factoryObjectTypeCleaned == factoryName )
            {
                return factory;
            }
        }

        return nullptr;
    }

    SmartPtr<IFactory> FactoryManager::getFactoryById( hash64 id ) const
    {
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

    bool FactoryManager::hasFactory( const String &typeName ) const
    {
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

    bool FactoryManager::hasFactoryByName( const String &name ) const
    {
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

    bool FactoryManager::hasFactoryById( hash64 typeId ) const
    {
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

    Array<SmartPtr<IFactory>> FactoryManager::getFactories() const
    {
        auto pFactories = getFactoriesArray();
        auto &factories = *pFactories;
        return Array<SmartPtr<IFactory>>( factories.begin(), factories.end() );
    }

    SmartPtr<ISharedObject> FactoryManager::createById( hash64 typeId ) const
    {
        auto typeManager = TypeManager::instance();
        FB_ASSERT( typeManager );

        auto typeHash = typeManager->getHash( (u32)typeId );
        FB_ASSERT( typeHash != 0 );

        auto factory = getFactoryById( typeHash );
        if( !factory )
        {
            auto typeName = typeManager->getName( (u32)typeId );
            FB_ASSERT( !typeName.empty() );

            factory = getFactoryByName( typeName );
        }

        if( !factory )
        {
            auto derivedTypes = typeManager->getDerivedTypes( (u32)typeId );

            for( auto derivedType : derivedTypes )
            {
                auto factoryHash = typeManager->getHash( derivedType );
                FB_ASSERT( factoryHash != 0 );

                factory = getFactoryById( factoryHash );
                if( factory )
                {
                    break;
                }
            }
        }

        if( factory )
        {
            auto ptr = (ISharedObject *)factory->createObject();
            auto p = SmartPtr<ISharedObject>( ptr );
            ptr->removeReference();
            return p;
        }

        return nullptr;
    }

    void *FactoryManager::_create( const String &type )
    {
        auto hash = StringUtil::getHash( type );
        return _createById( hash );
    }

    void *FactoryManager::_createById( hash64 typeId )
    {
        if( auto objectFactory = getFactory( typeId ) )
        {
            return objectFactory->createObject();
        }

        return nullptr;
    }

    void *FactoryManager::_createFromPool( hash64 typeId )
    {
        if( auto objectFactory = getFactory( typeId ) )
        {
            return objectFactory->createObjectFromPool();
        }

        return nullptr;
    }

    void *FactoryManager::_createArrayById( hash64 typeId, u32 numElements, u32 &objectSize )
    {
        if( auto objectFactory = getFactory( typeId ) )
        {
            objectSize = objectFactory->getObjectSize();
            return objectFactory->createArray( numElements );
        }

        return nullptr;
    }

    SharedPtr<ConcurrentArray<SmartPtr<IFactory>>> FactoryManager::getFactoriesArray() const
    {
        return m_factories;
    }

    void FactoryManager::setFactoriesArray( SharedPtr<ConcurrentArray<SmartPtr<IFactory>>> ptr )
    {
        m_factories = ptr;
    }

    void FactoryManager::setPoolSize( hash64 typeId, size_t size )
    {
        if( auto factory = getFactory( typeId ) )
        {
            factory->setDataSize( (u32)size );
        }
    }
}  // end namespace fb
