#ifndef TransformState_h__
#define TransformState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Atomics/Atomics.h>

namespace fb
{
    class TransformState : public BaseState
    {
    public:
        TransformState() = default;
        ~TransformState() override = default;

        SmartPtr<IState> clone();

        FB_CLASS_REGISTER_DECL;

        SmartPtr<scene::IActor> parent;
        SmartPtr<TransformState> parentTransformState;

        SmartPtr<scene::IActor> m_actor;

        Transform3<real_Num> m_parentTransform;
        Transform3<real_Num> m_localTransform;
        Transform3<real_Num> m_worldTransform;

        /// To know if the transform is dirty.
        atomic_bool m_isLocalDirty = false;

        /// To know if the transform is dirty.
        atomic_bool m_isDirty = false;

        /// To know if the component is enabled.
        atomic_bool m_enabled = true;
    };
}  // end namespace fb

#endif  // TransformState_h__
