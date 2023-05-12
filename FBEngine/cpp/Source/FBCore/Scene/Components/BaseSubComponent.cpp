#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/BaseSubComponent.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb, BaseSubComponent, CResource<ISubComponent> );

        u32 BaseSubComponent::m_idExt = 0;

        BaseSubComponent::BaseSubComponent()
        {
        }

        BaseSubComponent::~BaseSubComponent()
        {
        }

        SmartPtr<IComponent> BaseSubComponent::getParent() const
        {
            return m_parent;
        }

        void BaseSubComponent::setParent( SmartPtr<IComponent> parent )
        {
            m_parent = parent;
        }

        SmartPtr<IData> BaseSubComponent::toData() const
        {
            auto pComponentData = fb::make_ptr<Data<data::sub_component_data>>();
            auto componentData = pComponentData->getDataAsType<data::sub_component_data>();

            if( auto handle = getHandle() )
            {
                auto uuid = handle->getUUID();
                if( StringUtil::isNullOrEmpty( uuid ) )
                {
                    uuid = StringUtil::getUUID();
                }

                componentData->uuid = uuid;
            }

            auto typeinfo = getTypeInfo();
            FB_ASSERT( typeinfo );

            auto typeManager = TypeManager::instance();
            FB_ASSERT( typeManager );

            auto className = typeManager->getName( typeinfo );
            componentData->componentType = StringUtil::replaceAll( className, "class ", "" );

            if( auto properties = getProperties() )
            {
                auto propertiesData = properties->toData();
                componentData->properties_ = *propertiesData->getDataAsType<data::properties>();
            }

            return pComponentData;
        }

        void BaseSubComponent::fromData( SmartPtr<IData> data )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            auto componentData = data->getDataAsType<data::sub_component_data>();
            auto name = componentData->name;

            if( auto handle = getHandle() )
            {
                auto uuid = componentData->uuid;
                if( StringUtil::isNullOrEmpty( uuid ) )
                {
                    uuid = StringUtil::getUUID();
                }

                handle->setUUID( uuid );
            }

            if( auto pProperties = PropertiesUtil::getProperties( &componentData->properties_ ) )
            {
                //sceneManager->queueProperties( this, pProperties );
                setProperties( pProperties );
            }
        }

        SmartPtr<Properties> BaseSubComponent::getProperties() const
        {
            auto properties = fb::make_ptr<Properties>();
            return properties;
        }

        void BaseSubComponent::setProperties( SmartPtr<Properties> properties )
        {
        }
    }  // namespace scene
}  // namespace fb
