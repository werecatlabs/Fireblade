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

        Vector3<real_Num> getLookAt() const;
        void setLookAt( const Vector3<real_Num> &lookAt );

        AABB3<real_Num> getLocalAABB() const;
        void setLocalAABB( const AABB3<real_Num> &localAABB );

        AABB3<real_Num> getWorldAABB() const;
        void setWorldAABB( const AABB3<real_Num> &worldAABB );

        bool operator==( SceneNodeState *other ) const;

        bool operator!=( SceneNodeState *other ) const;

        /** Gets the data as a properties object.
        @return The data as a properties object.
        */
        SmartPtr<Properties> getProperties() const override;

        /** Sets the data as a properties object.
        @param properties The properties object.
        */
        void setProperties( SmartPtr<Properties> properties ) override;

        SmartPtr<IState> clone() const override;

        FB_CLASS_REGISTER_DECL;

    protected:
        AABB3<real_Num> m_localAABB;
        AABB3<real_Num> m_worldAABB;

        Vector3<real_Num> m_lookAt;

        atomic_u32 m_id;
        atomic_u32 m_flags;
        atomic_s32 m_queueCount;
        atomic_s32 m_tickCount;
    };
}  // end namespace fb

#endif  // SceneNodeState_h__
