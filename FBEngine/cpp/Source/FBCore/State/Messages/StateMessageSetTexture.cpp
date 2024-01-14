#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageSetTexture.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageSetTexture, StateMessage );

    StateMessageSetTexture::StateMessageSetTexture() = default;

    StateMessageSetTexture::~StateMessageSetTexture() = default;

    auto StateMessageSetTexture::getTexture() const -> SmartPtr<render::ITexture>
    {
        return m_texture;
    }

    void StateMessageSetTexture::setTexture( SmartPtr<render::ITexture> texture )
    {
        m_texture = texture;
    }

    auto StateMessageSetTexture::getTextureName() const -> String
    {
        return m_textureName;
    }

    void StateMessageSetTexture::setTextureName( const String &textureName )
    {
        m_textureName = textureName;
    }

    auto StateMessageSetTexture::getTextureIndex() const -> u32
    {
        return m_textureIndex;
    }

    void StateMessageSetTexture::setTextureIndex( u32 textureIndex )
    {
        m_textureIndex = textureIndex;
    }
}  // end namespace fb
