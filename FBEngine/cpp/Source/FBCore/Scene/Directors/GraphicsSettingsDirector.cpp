#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Directors/GraphicsSettingsDirector.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, GraphicsSettingsDirector, Director );

    GraphicsSettingsDirector::GraphicsSettingsDirector()
    {
    }

    GraphicsSettingsDirector::~GraphicsSettingsDirector()
    {
    }

    fb::SmartPtr<Properties> GraphicsSettingsDirector::getProperties() const
    {
        auto properties = Director::getProperties();
        return properties;
    }

    void GraphicsSettingsDirector::setProperties( SmartPtr<Properties> properties )
    {
    }

}  // namespace fb::scene
