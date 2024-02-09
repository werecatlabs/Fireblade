#ifndef GraphicsObjectState_h__
#define GraphicsObjectState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Math/AABB3.h>

namespace fb
{
    class GraphicsObjectState : public BaseState
    {
    public:
        static const u8 isAttachedFlag;
        static const u8 receiveShadowsFlag;
        static const u8 visibleFlag;
        static const u8 castShadowsFlag;

        GraphicsObjectState();
        ~GraphicsObjectState() override;

        u32 getVisibilityMask() const;
        void setVisibilityMask( u32 visibilityMask );

        bool isVisible() const;
        void setVisible( bool visible );

        bool getReceiveShadows() const;

        void setReceiveShadows( bool receiveShadows );

        bool getCastShadows() const;
        void setCastShadows( bool castShadows );

        u32 getRenderQueueGroup() const;

        void setRenderQueueGroup( u32 renderQueueGroup );

        AABB3F getLocalAABB() const;

        void setLocalAABB( const AABB3F &localAABB );

        u32 getFlags() const;

        void setFlags( u32 flags );

        bool isAttached() const;
        void setAttached( bool attached );

        hash_type getRenderTechnique() const;
        void setRenderTechnique( hash_type renderTechnique );

        u32 getZorder() const;

        void setZorder( u32 zorder );

        FB_CLASS_REGISTER_DECL;

    protected:
        AABB3F m_localAABB;

        hash_type m_renderTechnique = 0;
        u32 m_flags = 0;

        u32 m_visibiltyFlags = 0;

        u32 m_zorder = 0;

        u32 m_renderQueueGroup = 50;
        u32 m_visibilityMask = std::numeric_limits<u32>::max();

        bool m_isAttached = false;
        bool m_receiveShadows = true;
        bool m_visible = true;
        bool m_castShadows = false;
    };
}  // end namespace fb

#endif  // GraphicsObjectState_h__
