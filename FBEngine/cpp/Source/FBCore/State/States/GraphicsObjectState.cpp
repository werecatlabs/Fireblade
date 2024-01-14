#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/GraphicsObjectState.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, GraphicsObjectState, BaseState );

    GraphicsObjectState::GraphicsObjectState() = default;

    GraphicsObjectState::~GraphicsObjectState() = default;

    auto GraphicsObjectState::getVisibilityMask() const -> u32
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_visibilityMask;
    }

    void GraphicsObjectState::setVisibilityMask( u32 visibilityMask )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );

        if( m_visibilityMask != visibilityMask )
        {
            m_visibilityMask = visibilityMask;
            setDirty( true );
        }
    }

    auto GraphicsObjectState::isVisible() const -> bool
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_visible;
    }

    void GraphicsObjectState::setVisible( bool visible )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );

        if( m_visible != visible )
        {
            m_visible = visible;
            setDirty( true );
        }
    }

    auto GraphicsObjectState::getReceiveShadows() const -> bool
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_receiveShadows;
    }

    void GraphicsObjectState::setReceiveShadows( bool receiveShadows )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_receiveShadows = receiveShadows;
        setDirty( true );
    }

    auto GraphicsObjectState::getCastShadows() const -> bool
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_castShadows;
    }

    void GraphicsObjectState::setCastShadows( bool castShadows )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_castShadows = castShadows;
        setDirty( true );
    }

    auto GraphicsObjectState::getRenderQueueGroup() const -> u32
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_renderQueueGroup;
    }

    void GraphicsObjectState::setRenderQueueGroup( u32 renderQueueGroup )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_renderQueueGroup = renderQueueGroup;
        setDirty( true );
    }

    auto GraphicsObjectState::getLocalAABB() const -> AABB3F
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_localAABB;
    }

    void GraphicsObjectState::setLocalAABB( const AABB3F &localAABB )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_localAABB = localAABB;
        setDirty( true );
    }

    auto GraphicsObjectState::getFlags() const -> u32
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_flags;
    }

    void GraphicsObjectState::setFlags( u32 flags )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_flags = flags;
        setDirty( true );
    }

    auto GraphicsObjectState::isAttached() const -> bool
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_isAttached;
    }

    void GraphicsObjectState::setAttached( bool attached )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_isAttached = attached;
        setDirty( true );
    }

    auto GraphicsObjectState::getRenderTechnique() const -> u32
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_renderTechnique;
    }

    void GraphicsObjectState::setRenderTechnique( u32 val )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_renderTechnique = val;
        setDirty( true );
    }

    auto GraphicsObjectState::getZorder() const -> u32
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_zorder;
    }

    void GraphicsObjectState::setZorder( u32 zorder )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_zorder = zorder;
        setDirty( true );
    }

}  // end namespace fb
