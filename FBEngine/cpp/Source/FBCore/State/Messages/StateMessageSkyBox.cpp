#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageSkyBox.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageSkyBox, StateMessage );

    auto StateMessageSkyBox::getMaterial() const -> SmartPtr<render::IMaterial>
    {
        return m_material;
    }

    void StateMessageSkyBox::setMaterial( SmartPtr<render::IMaterial> material )
    {
        m_material = material;
    }

    auto StateMessageSkyBox::getMaterialName() const -> String
    {
        return m_materialName;
    }

    void StateMessageSkyBox::setMaterialName( const String &materialName )
    {
        m_materialName = materialName;
    }

    auto StateMessageSkyBox::getDistance() const -> f32
    {
        return m_distance;
    }

    void StateMessageSkyBox::setDistance( f32 val )
    {
        m_distance = val;
    }

    auto StateMessageSkyBox::getEnable() const -> bool
    {
        return m_enable;
    }

    void StateMessageSkyBox::setEnable( bool val )
    {
        m_enable = val;
    }

    auto StateMessageSkyBox::getDrawFirst() const -> bool
    {
        return m_drawFirst;
    }

    void StateMessageSkyBox::setDrawFirst( bool val )
    {
        m_drawFirst = val;
    }
}  // end namespace fb
