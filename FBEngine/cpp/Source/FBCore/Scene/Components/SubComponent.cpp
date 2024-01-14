#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/SubComponent.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, SubComponent, Resource<ISubComponent> );

    u32 SubComponent::m_idExt = 0;

    SubComponent::SubComponent() = default;

    SubComponent::~SubComponent() = default;

    auto SubComponent::getParentComponent() const -> SmartPtr<IComponent>
    {
        return m_parentComponent;
    }

    void SubComponent::setParentComponent( SmartPtr<IComponent> parentComponent )
    {
        m_parentComponent = parentComponent;
    }

    auto SubComponent::getParent() const -> SmartPtr<ISubComponent>
    {
        return m_parent;
    }

    auto SubComponent::toData() const -> SmartPtr<ISharedObject>
    {
        auto data = Resource<ISubComponent>::toData();
        return data;
    }

    void SubComponent::fromData( SmartPtr<ISharedObject> data )
    {
        Resource<ISubComponent>::fromData( data );
    }

    auto SubComponent::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = fb::make_ptr<Properties>();
        return properties;
    }

    void SubComponent::setProperties( SmartPtr<Properties> properties )
    {
    }

    void SubComponent::setParent( SmartPtr<ISubComponent> parent )
    {
        m_parent = parent;
    }

    void SubComponent::addChildByType( hash_type componentType )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto factoryManager = applicationManager->getFactoryManager();

        auto factory = factoryManager->getFactory( componentType );
        if( factory )
        {
            auto child = factory->make_object<ISubComponent>();
            addChild( child );
        }
    }

    void SubComponent::addChild( SmartPtr<ISubComponent> child )
    {
        m_children.push_back( child );
    }

    void SubComponent::removeChild( SmartPtr<ISubComponent> child )
    {
        m_children.erase( std::remove( m_children.begin(), m_children.end(), child ), m_children.end() );
    }

    auto SubComponent::getChildren() const -> Array<SmartPtr<ISubComponent>>
    {
        return m_children;
    }
}  // namespace fb::scene
