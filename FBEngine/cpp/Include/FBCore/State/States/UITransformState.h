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

        Vector2<real_Num> getPosition() const;

        void setPosition( const Vector2<real_Num> &position );

        Vector2<real_Num> getSize() const;

        void setSize( const Vector2<real_Num> &size );

        Vector2<real_Num> getAbsolutePosition() const;

        void setAbsolutePosition( const Vector2<real_Num> &absolutePosition );

        Vector2<real_Num> getAbsoluteSize() const;

        void setAbsoluteSize( const Vector2<real_Num> &absoluteSize );

        Vector2<real_Num> getAnchor() const;

        void setAnchor( const Vector2<real_Num> &anchor );

        Vector2<real_Num> getAnchorMin() const;

        void setAnchorMin( const Vector2<real_Num> &anchorMin );

        Vector2<real_Num> getAnchorMax() const;

        void setAnchorMax( const Vector2<real_Num> &anchorMax );

        u8 getVerticalAlignment() const;
        void setVerticalAlignment( u8 verticalAlignment );

        u8 getHorizontalAlignment() const;
        void setHorizontalAlignment( u8 horizontalAlignment );

        FB_CLASS_REGISTER_DECL;

    protected:
        ISharedObject *m_uiComponent = nullptr;
        ISharedObject *m_owner = nullptr;

        Vector2<real_Num> m_position = Vector2<real_Num>::zero();
        Vector2<real_Num> m_size = Vector2<real_Num>( 300.0f, 100.0f );

        Vector2<real_Num> m_absolutePosition = Vector2<real_Num>::zero();
        Vector2<real_Num> m_absoluteSize = Vector2<real_Num>::zero();

        Vector2<real_Num> m_anchor = Vector2<real_Num>( 0.5f, 0.5f );

        Vector2<real_Num> m_anchorMin = Vector2<real_Num>( 0.5f, 0.5f );
        Vector2<real_Num> m_anchorMax = Vector2<real_Num>( 0.5f, 0.5f );

        u8 m_verticalAlignment = 0;
        u8 m_horizontalAlignment = 0;
    };

}  // namespace fb

#endif  // UITransformState_h__
