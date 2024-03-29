#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/UITransformState.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, UITransformState, BaseState );

    UITransformState::UITransformState() = default;

    UITransformState::~UITransformState() = default;

    auto UITransformState::getUIComponent() const -> ISharedObject *
    {
        return m_uiComponent;
    }

    void UITransformState::setUIComponent( ISharedObject *uiComponent )
    {
        m_uiComponent = uiComponent;
        setDirty( true );
    }

    auto UITransformState::getOwner() const -> ISharedObject *
    {
        return m_owner;
    }

    void UITransformState::setOwner( ISharedObject *owner )
    {
        m_owner = owner;
        setDirty( true );
    }

    auto UITransformState::getPosition() const -> Vector2<real_Num>
    {
        return m_position;
    }

    void UITransformState::setPosition( const Vector2<real_Num> &position )
    {
        m_position = position;
        setDirty( true );
    }

    auto UITransformState::getSize() const -> Vector2<real_Num>
    {
        return m_size;
    }

    void UITransformState::setSize( const Vector2<real_Num> &size )
    {
        m_size = size;
        setDirty( true );
    }

    auto UITransformState::getAbsolutePosition() const -> Vector2<real_Num>
    {
        return m_absolutePosition;
    }

    void UITransformState::setAbsolutePosition( const Vector2<real_Num> &absolutePosition )
    {
        m_absolutePosition = absolutePosition;
        setDirty( true );
    }

    auto UITransformState::getAbsoluteSize() const -> Vector2<real_Num>
    {
        return m_absoluteSize;
    }

    void UITransformState::setAbsoluteSize( const Vector2<real_Num> &absoluteSize )
    {
        m_absoluteSize = absoluteSize;
        setDirty( true );
    }

    auto UITransformState::getAnchor() const -> Vector2<real_Num>
    {
        return m_anchor;
    }

    void UITransformState::setAnchor( const Vector2<real_Num> &anchor )
    {
        m_anchor = anchor;
        setDirty( true );
    }

    auto UITransformState::getAnchorMin() const -> Vector2<real_Num>
    {
        return m_anchorMin;
    }

    void UITransformState::setAnchorMin( const Vector2<real_Num> &anchorMin )
    {
        m_anchorMin = anchorMin;
        setDirty( true );
    }

    auto UITransformState::getAnchorMax() const -> Vector2<real_Num>
    {
        return m_anchorMax;
    }

    void UITransformState::setAnchorMax( const Vector2<real_Num> &anchorMax )
    {
        m_anchorMax = anchorMax;
        setDirty( true );
    }

    auto UITransformState::getVerticalAlignment() const -> u8
    {
        return m_verticalAlignment;
    }

    void UITransformState::setVerticalAlignment( u8 verticalAlignment )
    {
        m_verticalAlignment = verticalAlignment;
        setDirty( true );
    }

    auto UITransformState::getHorizontalAlignment() const -> u8
    {
        return m_horizontalAlignment;
    }

    void UITransformState::setHorizontalAlignment( u8 horizontalAlignment )
    {
        m_horizontalAlignment = horizontalAlignment;
        setDirty( true );
    }
}  // namespace fb
