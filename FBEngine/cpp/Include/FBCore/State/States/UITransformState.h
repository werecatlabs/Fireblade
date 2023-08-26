#ifndef UITransformState_h__
#define UITransformState_h__

#include <FBCore/State/States/BaseState.h>
#include "FBCore/Math/Vector2.h"

namespace fb
{

    class UITransformState : public BaseState
    {
    public:
        UITransformState();

        ~UITransformState();

        ISharedObject *getUIComponent() const;

        void setUIComponent( ISharedObject *uiComponent );

        ISharedObject *getOwner() const;

        void setOwner( ISharedObject *owner );

        Vector2F getPosition() const;

        void setPosition( const Vector2F &position );

        Vector2F getSize() const;

        void setSize( const Vector2F &size );

        Vector2F getAbsolutePosition() const;

        void setAbsolutePosition( const Vector2F &absolutePosition );

        Vector2F getAbsoluteSize() const;

        void setAbsoluteSize( const Vector2F &absoluteSize );

        Vector2F getAnchor() const;

        void setAnchor( const Vector2F &anchor );

        Vector2F getAnchorMin() const;

        void setAnchorMin( const Vector2F &anchorMin );

        Vector2F getAnchorMax() const;

        void setAnchorMax( const Vector2F &anchorMax );

        u8 getVerticalAlignment() const;
        void setVerticalAlignment( u8 verticalAlignment );

        u8 getHorizontalAlignment() const;
        void setHorizontalAlignment( u8 horizontalAlignment );

        FB_CLASS_REGISTER_DECL;

    protected:
        ISharedObject *m_uiComponent = nullptr;
        ISharedObject *m_owner = nullptr;

        Vector2F m_position = Vector2F::zero();
        Vector2F m_size = Vector2F( 300.0f, 100.0f );

        Vector2F m_absolutePosition = Vector2F::zero();
        Vector2F m_absoluteSize = Vector2F::zero();

        Vector2F m_anchor = Vector2F( 0.5f, 0.5f );

        Vector2F m_anchorMin = Vector2F( 0.5f, 0.5f );
        Vector2F m_anchorMax = Vector2F( 0.5f, 0.5f );

        u8 m_verticalAlignment = 0;
        u8 m_horizontalAlignment = 0;
    };

}  // namespace fb

#endif  // UITransformState_h__
