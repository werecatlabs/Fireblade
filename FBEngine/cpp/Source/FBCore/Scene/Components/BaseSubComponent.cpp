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
