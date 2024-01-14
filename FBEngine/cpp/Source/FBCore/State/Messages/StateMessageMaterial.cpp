#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageMaterial.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageMaterial, StateMessage );

    auto StateMessageMaterial::getMaterial() const -> SmartPtr<render::IMaterial>
    {
        return m_material;
    }

    void StateMessageMaterial::setMaterial( SmartPtr<render::IMaterial> material )
    {
        m_material = material;
    }

    auto StateMessageMaterial::getIndex() const -> s32
    {
        return m_index;
    }

    void StateMessageMaterial::setIndex( s32 index )
    {
        m_index = index;
    }

}  // end namespace fb
