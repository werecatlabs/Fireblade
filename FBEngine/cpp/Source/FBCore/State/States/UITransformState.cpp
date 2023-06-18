#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/UITransformState.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, UITransformState, BaseState );

    UITransformState::UITransformState()
    {
    }

    UITransformState::~UITransformState()
    {
    }

    ISharedObject *UITransformState::getUIComponent() const
    {
        return m_uiComponent;
    }

    void UITransformState::setUIComponent( ISharedObject *uiComponent )
    {
        m_uiComponent = uiComponent;
        setDirty( true );
    }

    ISharedObject *UITransformState::getOwner() const
    {
        return m_owner;
    }

    void UITransformState::setOwner( ISharedObject *owner )
    {
        m_owner = owner;
        setDirty( true );
    }

    Vector2F UITransformState::getPosition() const
    {
        return m_position;
    }

    void UITransformState::setPosition( const Vector2F &position )
    {
        m_position = position;
        setDirty( true );
    }

    Vector2F UITransformState::getSize() const
    {
        return m_size;
    }

    void UITransformState::setSize( const Vector2F &size )
    {
        m_size = size;
        setDirty( true );
    }

    Vector2F UITransformState::getAbsolutePosition() const
    {
        return m_absolutePosition;
    }

    void UITransformState::setAbsolutePosition( const Vector2F &absolutePosition )
    {
        m_absolutePosition = absolutePosition;
        setDirty( true );
    }

    Vector2F UITransformState::getAbsoluteSize() const
    {
        return m_absoluteSize;
    }

    void UITransformState::setAbsoluteSize( const Vector2F &absoluteSize )
    {
        m_absoluteSize = absoluteSize;
        setDirty( true );
    }

    Vector2F UITransformState::getAnchor() const
    {
        return m_anchor;
    }

    void UITransformState::setAnchor( const Vector2F &anchor )
    {
        m_anchor = anchor;
        setDirty( true );
    }

    Vector2F UITransformState::getAnchorMin() const
    {
        return m_anchorMin;
    }

    void UITransformState::setAnchorMin( const Vector2F &anchorMin )
    {
        m_anchorMin = anchorMin;
        setDirty( true );
    }

    Vector2F UITransformState::getAnchorMax() const
    {
        return m_anchorMax;
    }

    void UITransformState::setAnchorMax( const Vector2F &anchorMax )
    {
        m_anchorMax = anchorMax;
        setDirty( true );
    }

    u8 UITransformState::getVerticalAlignment() const
    {
        return m_verticalAlignment;
    }

    void UITransformState::setVerticalAlignment( u8 verticalAlignment )
    {
        m_verticalAlignment = verticalAlignment;
        setDirty( true );
    }

    u8 UITransformState::getHorizontalAlignment() const
    {
        return m_horizontalAlignment;
    }

    void UITransformState::setHorizontalAlignment( u8 horizontalAlignment )
    {
        m_horizontalAlignment = horizontalAlignment;
        setDirty( true );
    }
} // namespace fb
