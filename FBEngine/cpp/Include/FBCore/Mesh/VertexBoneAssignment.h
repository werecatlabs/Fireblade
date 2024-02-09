#ifndef __FB_VertexBoneAssignment_H__
#define __FB_VertexBoneAssignment_H__

#include <FBCore/Interface/Mesh/IVertexBoneAssignment.h>

namespace fb
{
    class VertexBoneAssignment : public IVertexBoneAssignment
    {
    public:
        VertexBoneAssignment();
        VertexBoneAssignment( u32 vertexIndex, u16 boneIndex, f32 weight );
        ~VertexBoneAssignment() override;

        u32 getVertexIndex() const override;

        void setVertexIndex( u32 vertexIndex ) override;

        u16 getBoneIndex() const override;

        void setBoneIndex( u16 boneIndex ) override;

        f32 getWeight() const override;

        void setWeight( f32 weight ) override;

        FB_CLASS_REGISTER_DECL;

        u32 m_vertexIndex = 0;
        u16 m_boneIndex = 0;
        f32 m_weight = 0.0f;
    };
} // namespace fb

#endif
