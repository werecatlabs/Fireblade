#ifndef BoundingBoxState_h__
#define BoundingBoxState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Math/AABB3.h>

namespace fb
{

    class BoundingBoxState : public BaseState
    {
    public:
        BoundingBoxState();
        virtual ~BoundingBoxState();

        AABB3<real_Num> getLocalAABB() const;
        void setLocalAABB( const AABB3<real_Num> &localAABB );

        AABB3<real_Num> getWorldAABB() const;
        void setWorldAABB( const AABB3<real_Num> &worldAABB );

    protected:
        AABB3<real_Num> m_localAABB;
        AABB3<real_Num> m_worldAABB;
    };

}  // namespace fb

#endif  // BoundingBoxState_h__
