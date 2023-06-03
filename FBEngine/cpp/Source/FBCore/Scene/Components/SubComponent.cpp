#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/SubComponent.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb, SubComponent, Resource<ISubComponent> );

        u32 SubComponent::m_idExt = 0;

        SubComponent::SubComponent()
        {
        }

        SubComponent::~SubComponent()
        {
        }

        SmartPtr<IComponent> SubComponent::getParent() const
        {
            return m_parent;
        }

        void SubComponent::setParent( SmartPtr<IComponent> parent )
        {
            m_parent = parent;
        }

        SmartPtr<Properties> SubComponent::getProperties() const
        {
            auto properties = fb::make_ptr<Properties>();
            return properties;
        }

        void SubComponent::setProperties( SmartPtr<Properties> properties )
        {
        }
    }  // namespace scene
}  // namespace fb
