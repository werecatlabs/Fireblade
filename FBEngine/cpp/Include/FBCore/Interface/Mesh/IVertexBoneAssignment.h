#ifndef IVertexBoneAssignment_h__
#define IVertexBoneAssignment_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class IVertexBoneAssignment : public ISharedObject
    {
    public:
        ~IVertexBoneAssignment() override;

        virtual u32 getVertexIndex() const = 0;
        virtual void setVertexIndex( u32 vertexIndex ) = 0;

        virtual u16 getBoneIndex() const = 0;
        virtual void setBoneIndex( u16 boneIndex ) = 0;

        virtual f32 getWeight() const = 0;
        virtual void setWeight( f32 weight ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // namespace fb

#endif  // IVertexBoneAssignment_h__
