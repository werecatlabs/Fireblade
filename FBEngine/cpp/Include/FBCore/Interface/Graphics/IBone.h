#ifndef IBone_h__
#define IBone_h__

#include <FBCore/Interface/Graphics/INode.h>

namespace fb
{
    namespace render
    {

        class IBone : public INode
        {
        public:
            /** Virtual destructor. */
            ~IBone() override = default;

            /** Creates a new Bone as a child of this bone. */
            virtual SmartPtr<IBone> createChild(
                u16 handle, const Vector3<real_Num> &translate = Vector3<real_Num>::zero(),
                const Quaternion<real_Num> &rotate = Quaternion<real_Num>::identity() ) = 0;

            /** Sets the current position / orientation to be the 'binding pose' ie the layout in which
                bones were originally bound to a mesh.
            */
            virtual void setBindingPose() = 0;

            /** Resets the position and orientation of this Bone to the original binding position. */
            virtual void reset() = 0;

            /** Sets whether or not this bone is manually controlled. */
            virtual void setManuallyControlled( bool manuallyControlled ) = 0;

            /** Getter for mManuallyControlled Flag */
            virtual bool isManuallyControlled() const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace render
}  // namespace fb

#endif  // IBone_h__
