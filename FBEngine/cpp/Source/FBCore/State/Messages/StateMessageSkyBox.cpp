#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageSkyBox.h>
#include <FBCore/Interface/Graphics/IMaterial.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageSkyBox, StateMessage );

    SmartPtr<render::IMaterial> StateMessageSkyBox::getMaterial() const
    {
        return m_material;
    }

    void StateMessageSkyBox::setMaterial( SmartPtr<render::IMaterial> material )
    {
        m_material = material;
    }

    String StateMessageSkyBox::getMaterialName() const
    {
        return m_materialName;
    }

    void StateMessageSkyBox::setMaterialName( const String &materialName )
    {
        m_materialName = materialName;
    }

    f32 StateMessageSkyBox::getDistance() const
    {
        return m_distance;
    }

    void StateMessageSkyBox::setDistance( f32 val )
    {
        m_distance = val;
    }

    bool StateMessageSkyBox::getEnable() const
    {
        return m_enable;
    }

    void StateMessageSkyBox::setEnable( bool val )
    {
        m_enable = val;
    }

    bool StateMessageSkyBox::getDrawFirst() const
    {
        return m_drawFirst;
    }

    void StateMessageSkyBox::setDrawFirst( bool val )
    {
        m_drawFirst = val;
    }
}  // end namespace fb
