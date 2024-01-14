#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/TerrainState.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, TerrainState, BaseState );

    TerrainState::TerrainState() = default;

    TerrainState::~TerrainState() = default;

    auto TerrainState::getWorldTransform() const -> Transform3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_worldTransform;
    }

    void TerrainState::setWorldTransform( const Transform3<real_Num> &worldTransform )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_worldTransform = worldTransform;
        setDirty( true );
    }

}  // namespace fb
