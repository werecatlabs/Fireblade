#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/MaterialState.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, MaterialState, BaseState );

    MaterialState::MaterialState()
    {
    }

    MaterialState::~MaterialState()
    {
    }

    render::IMaterial::MaterialType MaterialState::getMaterialType() const
    {
        return m_materialType;
    }

    void MaterialState::setMaterialType( render::IMaterial::MaterialType materialType )
    {
        if(m_materialType != materialType)
        {
            m_materialType = materialType;
            setDirty( true );
        }
    }
} // namespace fb
