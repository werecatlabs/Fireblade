#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/VerticalLayout.h>
#include <FBCore/Scene/Components/UI/LayoutTransform.h>
#include <FBCore/Interface/Scene/IActor.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, VerticalLayout, LayoutContainer );

    VerticalLayout::VerticalLayout() = default;

    VerticalLayout::~VerticalLayout() = default;

    void VerticalLayout::updateTransform()
    {
        auto offset = 0.0f;

        if( auto actor = getActor() )
        {
            auto p = actor->getChildrenPtr();
            auto children = *p;

            for( auto child : children )
            {
                if( child->isEnabled() )
                {
                    auto transform = child->getComponent<LayoutTransform>();

                    // Position the child vertically
                    auto childPosition = transform->getPosition();
                    auto childSize = transform->getSize();

                    childPosition.y = offset;
                    transform->setPosition( childPosition );

                    // Update the offset for the next child
                    offset += childSize.y + m_spacing;
                }
            }
        }
    }

    auto VerticalLayout::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = LayoutContainer::getProperties();
        properties->setProperty( "spacing", m_spacing );
        return properties;
    }

    void VerticalLayout::setProperties( SmartPtr<Properties> properties )
    {
        LayoutContainer::setProperties( properties );

        properties->getPropertyValue( "spacing", m_spacing );

        updateTransform();
    }

}  // namespace fb::scene
