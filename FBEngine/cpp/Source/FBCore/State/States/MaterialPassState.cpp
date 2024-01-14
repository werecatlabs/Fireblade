#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/MaterialPassState.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, MaterialPassState, BaseState );

    MaterialPassState::MaterialPassState() = default;

    MaterialPassState::~MaterialPassState() = default;

    auto MaterialPassState::clone() const -> SmartPtr<IState>
    {
        auto state = fb::make_ptr<MaterialPassState>();

        state->m_tint = m_tint;
        state->m_ambient = m_ambient;
        state->m_diffuse = m_diffuse;
        state->m_specular = m_specular;
        state->m_emissive = m_emissive;

        state->m_metalness = m_metalness;
        state->m_roughness = m_roughness;

        return state;
    }

    void MaterialPassState::assign( SmartPtr<IState> state )
    {
        auto materialPassState = fb::static_pointer_cast<MaterialPassState>( state );

        m_tint = materialPassState->m_tint;
        m_ambient = materialPassState->m_ambient;
        m_diffuse = materialPassState->m_diffuse;
        m_specular = materialPassState->m_specular;
        m_emissive = materialPassState->m_emissive;

        m_metalness = materialPassState->m_metalness;
        m_roughness = materialPassState->m_roughness;
    }

    auto MaterialPassState::getTint() const -> ColourF
    {
        return m_tint;
    }

    void MaterialPassState::setTint( ColourF tint )
    {
        m_tint = tint;
        setDirty( true );
    }

    auto MaterialPassState::getAmbient() const -> ColourF
    {
        return m_ambient;
    }

    void MaterialPassState::setAmbient( ColourF ambient )
    {
        m_ambient = ambient;
        setDirty( true );
    }

    auto MaterialPassState::getDiffuse() const -> ColourF
    {
        return m_diffuse;
    }

    void MaterialPassState::setDiffuse( ColourF diffuse )
    {
        if( m_diffuse != diffuse )
        {
            m_diffuse = diffuse;
            setDirty( true );
        }
    }

    auto MaterialPassState::getSpecular() const -> ColourF
    {
        return m_specular;
    }

    void MaterialPassState::setSpecular( ColourF specular )
    {
        m_specular = specular;
        setDirty( true );
    }

    auto MaterialPassState::getEmissive() const -> ColourF
    {
        return m_emissive;
    }

    void MaterialPassState::setEmissive( ColourF emissive )
    {
        m_emissive = emissive;
        setDirty( true );
    }

    auto MaterialPassState::getMetalness() const -> f32
    {
        return m_metalness;
    }

    void MaterialPassState::setMetalness( f32 metalness )
    {
        m_metalness = metalness;
        setDirty( true );
    }

    auto MaterialPassState::getRoughness() const -> f32
    {
        return m_roughness;
    }

    void MaterialPassState::setRoughness( f32 roughness )
    {
        m_roughness = roughness;
        setDirty( true );
    }

    auto MaterialPassState::isLightingEnabled() const -> bool
    {
        return m_lightingEnabled;
    }

    void MaterialPassState::setLightingEnabled( bool lightingEnabled )
    {
        m_lightingEnabled = lightingEnabled;
        setDirty( true );
    }

}  // end namespace fb
