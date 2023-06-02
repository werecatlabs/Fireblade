#ifndef FactoryUtil_h__
#define FactoryUtil_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Memory/TypeManager.h>
#include <FBCore/Memory/FactoryTemplate.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{

    class FactoryUtil
    {
    public:
        template <class T>
        static void addFactory()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto typeManager = TypeManager::instance();
            FB_ASSERT( typeManager );

            auto typeInfo = T::typeInfo();
            auto name = typeManager->getName( typeInfo );
            auto hash = typeManager->getHash( typeInfo );

            auto pFactory = fb::make_ptr<FactoryTemplate<T>>();
            pFactory->setObjectType( name );
            pFactory->setObjectTypeId( hash );
            pFactory->load( nullptr );

            factoryManager->addFactory( pFactory );
        }

        template <class T>
        static void addFactoryByName( const String &name )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto hash = StringUtil::getHash64( name );

            auto pFactory = fb::make_ptr<FactoryTemplate<T>>();
            pFactory->setObjectType( name );
            pFactory->setObjectTypeId( hash );
            pFactory->load( nullptr );

            factoryManager->addFactory( pFactory );
        }

        template <class T>
        static void addFactoryByName( const String &name, u32 poolSize )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto hash = StringUtil::getHash64( name );

            auto pFactory = fb::make_ptr<FactoryTemplate<T>>();
            pFactory->setObjectType( name );
            pFactory->setObjectTypeId( hash );
            pFactory->load( nullptr );

            factoryManager->addFactory( pFactory );
            factoryManager->setPoolSize( hash, poolSize );
        }

        static void createFactories();
    };

}  // namespace fb

#endif  // FactoryUtil_h__
