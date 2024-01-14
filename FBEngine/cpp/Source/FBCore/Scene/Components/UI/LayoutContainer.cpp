#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/LayoutContainer.h>

namespace fb::scene
{

    FB_CLASS_REGISTER_DERIVED( fb::scene, LayoutContainer, Component );

    void LayoutContainer::updateFlags( u32 flags, u32 oldFlags )
    {
        updateTransform();
    }

    void LayoutContainer::updateTransform()
    {
        /* do nothing */
    }

    auto LayoutContainer::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = Component::getProperties();
        properties->setPropertyAsButton( "UpdateTransform", "Update Transform" );
        return properties;
    }

    void LayoutContainer::setProperties( SmartPtr<Properties> properties )
    {
        if( properties->isButtonPressed( "UpdateTransform" ) )
        {
            updateTransform();
        }
    }

}  // namespace fb::scene
