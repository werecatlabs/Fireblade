#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/GraphicsObjectState.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, GraphicsObjectState, BaseState );

    GraphicsObjectState::GraphicsObjectState()
    {
    }

    GraphicsObjectState::~GraphicsObjectState()
    {
    }

    u32 GraphicsObjectState::getVisibilityMask() const
    {
        return m_visibilityMask;
    }

    void GraphicsObjectState::setVisibilityMask( u32 visibilityMask )
    {
        //if( m_visibilityMask != visibilityMask )
        {
            m_visibilityMask = visibilityMask;
            setDirty( true );
        }
    }

    bool GraphicsObjectState::isVisible() const
    {
        return m_visible;
    }

    void GraphicsObjectState::setVisible( bool visible )
    {
        if( m_visible != visible )
        {
            m_visible = visible;
            setDirty( true );
        }
    }

    bool GraphicsObjectState::getReceiveShadows() const
    {
        return m_receiveShadows;
    }

    void GraphicsObjectState::setReceiveShadows( bool receiveShadows )
    {
        m_receiveShadows = receiveShadows;
        setDirty( true );
    }

    bool GraphicsObjectState::getCastShadows() const
    {
        return m_castShadows;
    }

    void GraphicsObjectState::setCastShadows( bool castShadows )
    {
        m_castShadows = castShadows;
        setDirty( true );
    }

    u32 GraphicsObjectState::getRenderQueueGroup() const
    {
        return m_renderQueueGroup;
    }

    void GraphicsObjectState::setRenderQueueGroup( u32 renderQueueGroup )
    {
        m_renderQueueGroup = renderQueueGroup;
        setDirty( true );
    }

    AABB3F GraphicsObjectState::getLocalAABB() const
    {
        return m_localAABB;
    }

    void GraphicsObjectState::setLocalAABB( const AABB3F &localAABB )
    {
        m_localAABB = localAABB;
        setDirty( true );
    }

    u32 GraphicsObjectState::getFlags() const
    {
        return m_flags;
    }

    void GraphicsObjectState::setFlags( u32 flags )
    {
        m_flags = flags;
        setDirty( true );
    }

    bool GraphicsObjectState::isAttached() const
    {
        return m_isAttached;
    }

    void GraphicsObjectState::setAttached( bool attached )
    {
        m_isAttached = attached;
        setDirty( true );
    }

    u32 GraphicsObjectState::getRenderTechnique() const
    {
        return m_renderTechnique;
    }

    void GraphicsObjectState::setRenderTechnique( u32 val )
    {
        m_renderTechnique = val;
        setDirty( true );
    }

    u32 GraphicsObjectState::getZorder() const
    {
        return m_zorder;
    }

    void GraphicsObjectState::setZorder( u32 zorder )
    {
        m_zorder = zorder;
        setDirty( true );
    }

}  // end namespace fb
