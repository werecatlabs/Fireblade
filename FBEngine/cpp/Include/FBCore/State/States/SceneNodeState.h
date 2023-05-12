#ifndef __SceneNodeState_h__
#define __SceneNodeState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Math/Transform3.h>

namespace fb
{
    class SceneNodeState : public BaseState
    {
    public:
        SceneNodeState();
        SceneNodeState( const SceneNodeState &state );

        Vector3<real_Num> getPosition() const;
        void setPosition( const Vector3<real_Num> &position );

        Vector3<real_Num> getScale() const;
        void setScale( const Vector3<real_Num> &scale );

        Quaternion<real_Num> getOrientation() const;
        void setOrientation( const Quaternion<real_Num> &orientation );

        Vector3<real_Num> getLookAt() const;
        void setLookAt( const Vector3<real_Num> &lookAt );

        Vector3<real_Num> getAbsolutePosition() const;
        void setAbsolutePosition( const Vector3<real_Num> &position );

        Vector3<real_Num> getAbsoluteScale() const;
        void setAbsoluteScale( const Vector3<real_Num> &scale );

        Quaternion<real_Num> getAbsoluteOrientation() const;
        void setAbsoluteOrientation( const Quaternion<real_Num> &orientation );

        AABB3<real_Num> getLocalAABB() const;
        void setLocalAABB( const AABB3<real_Num> &localAABB );

        AABB3<real_Num> getWorldAABB() const;
        void setWorldAABB( const AABB3<real_Num> &worldAABB );

        bool operator==( SceneNodeState *other ) const;

        bool operator!=( SceneNodeState *other ) const;

        bool isRegistered() const;
        void setRegistered( bool val );

        /** Gets the data as a properties object.
        @return The data as a properties object.
        */
        SmartPtr<Properties> getProperties() const override;

        /** Sets the data as a properties object.
        @param properties The properties object.
        */
        void setProperties( SmartPtr<Properties> properties ) override;

        SmartPtr<IState> clone() const override;

        bool isVisible() const;

        void setVisible( bool visible );

        FB_CLASS_REGISTER_DECL;

    protected:
        AABB3<real_Num> m_localAABB;
        AABB3<real_Num> m_worldAABB;

        Transform3<real_Num> m_localTransform;

        // derived transforms
        Vector3<real_Num> m_absolutePosition;
        Vector3<real_Num> m_absoluteScale;
        Quaternion<real_Num> m_absoluteOrientation;

        Vector3<real_Num> m_lookAt;

        atomic_bool m_isVisible = true;
        atomic_bool m_isAttached = false;

        atomic_u32 m_id;
        atomic_u32 m_flags;
        atomic_s32 m_queueCount;
        atomic_s32 m_tickCount;
        atomic_bool m_isRegistered;
    };
}  // end namespace fb

#endif  // SceneNodeState_h__
