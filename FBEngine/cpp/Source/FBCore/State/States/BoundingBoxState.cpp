#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/BoundingBoxState.h>

namespace fb
{

    BoundingBoxState::BoundingBoxState() = default;
    BoundingBoxState::~BoundingBoxState() = default;

    auto BoundingBoxState::getLocalAABB() const -> AABB3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        return m_localAABB;
    }

    void BoundingBoxState::setLocalAABB( const AABB3<real_Num> &localAABB )
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        m_localAABB = localAABB;
    }

    auto BoundingBoxState::getWorldAABB() const -> AABB3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        return m_worldAABB;
    }

    void BoundingBoxState::setWorldAABB( const AABB3<real_Num> &worldAABB )
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        m_worldAABB = worldAABB;
    }

}  // namespace fb
