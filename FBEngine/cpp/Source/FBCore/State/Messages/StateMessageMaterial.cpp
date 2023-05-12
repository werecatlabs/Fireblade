#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageMaterial.h>
#include <FBCore/Interface/Graphics/IMaterial.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageMaterial, StateMessage );

    SmartPtr<render::IMaterial> StateMessageMaterial::getMaterial() const
    {
        return m_material;
    }

    void StateMessageMaterial::setMaterial( SmartPtr<render::IMaterial> material )
    {
        m_material = material;
    }

    s32 StateMessageMaterial::getIndex() const
    {
        return m_index;
    }

    void StateMessageMaterial::setIndex( s32 index )
    {
        m_index = index;
    }

}  // end namespace fb
