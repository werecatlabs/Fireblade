#ifndef MeshShapeState_h__
#define MeshShapeState_h__

#include <FBCore/State/States/ShapeState.h>

namespace fb
{
    class MeshShapeState : public ShapeState
    {
    public:
        MeshShapeState() = default;
        ~MeshShapeState() override = default;

        SmartPtr<IMeshResource> getMeshResource() const;

        void setMeshResource( SmartPtr<IMeshResource> meshResource );

        FB_CLASS_REGISTER_DECL;

    private:
        SmartPtr<IMeshResource> m_meshResource;
    };
}  // namespace fb

#endif  // MeshShapeState_h__
