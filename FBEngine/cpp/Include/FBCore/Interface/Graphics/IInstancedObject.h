#ifndef IInstancedObject_h__
#define IInstancedObject_h__

#include <FBCore/Interface/Graphics/IGraphicsObject.h>

namespace fb
{
    namespace render
    {

        /**
         * Interface for an instanced object.
         */
        class IInstancedObject : public IGraphicsObject
        {
        public:
            static const hash_type RENDER_QUEUE_HASH;
            static const hash_type VISIBILITY_FLAGS_HASH;
            static const hash_type CUSTOM_PARAMETER_HASH;
            static const u32 MAX_CUSTOM_PARAMS = 8;

            /** Virtual destructor. */
            ~IInstancedObject() override = default;

            /** Sets the position.
             * @param position The position to set.
             */
            virtual void setPosition( const Vector3F &position ) = 0;

            /** Gets the position.
             * @return The position of the instance.
             */
            virtual Vector3F getPosition() const = 0;

            /** Sets the orientation of this node via Quaternion parameters.
             * @param orientation The orientation to set.
             */
            virtual void setOrientation( const QuaternionF &orientation ) = 0;

            /** Returns a Quaternion representing the nodes orientation.
             * @return The orientation of the instance.
             */
            virtual QuaternionF getOrientation() const = 0;

            /** Sets the scaling factor applied to this node.
             * @param scale The scaling factor to set.
             */
            virtual void setScale( const Vector3F &scale ) = 0;

            /** Gets the scaling factor of this node.
             * @return The scaling factor of the instance.
             */
            virtual Vector3F getScale() const = 0;

            /** Sets the custom parameter for this instance.
             * @param idx The index of the parameter to set.
             * @param newParam The value of the parameter to set.
             */
            virtual void setCustomParam( u8 idx, const Vector4F &newParam ) = 0;

            /** Gets the custom parameter for this instance.
             * @param idx The index of the parameter to get.
             * @return The value of the parameter.
             */
            virtual Vector4F getCustomParam( u8 idx ) = 0;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // IInstancedObject_h__
