#ifndef ITransform_h__
#define ITransform_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/IComponent.h>
#include <FBCore/Math/Transform3.h>

namespace fb
{
    namespace scene
    {

        /** Interface for a transform object.
        @author	Zane Desir
        @version 1.0
        */
        class ITransform : public ISharedObject
        {
        public:
            enum class Type
            {
                None,
                LocalPosition,
                LocalRotation,
                LocalScale,
                Position,
                Rotation,
                Scale,

                Count
            };

            /** Virtual destructor. */
            ~ITransform() override = default;

            virtual SmartPtr<IActor> getActor() const = 0;
            virtual void setActor( SmartPtr<IActor> actor ) = 0;

            virtual Vector3<real_Num> getLocalPosition() const = 0;
            virtual void setLocalPosition( const Vector3<real_Num> &localPosition ) = 0;

            virtual Vector3<real_Num> getLocalScale() const = 0;
            virtual void setLocalScale( const Vector3<real_Num> &localScale ) = 0;

            virtual Quaternion<real_Num> getLocalOrientation() const = 0;
            virtual void setLocalOrientation( const Quaternion<real_Num> &localOrientation ) = 0;

            virtual Vector3<real_Num> getLocalRotation() const = 0;
            virtual void setLocalRotation( const Vector3<real_Num> &localRotation ) = 0;

            virtual Vector3<real_Num> getPosition() const = 0;
            virtual void setPosition( const Vector3<real_Num> &position ) = 0;

            virtual Vector3<real_Num> getScale() const = 0;
            virtual void setScale( const Vector3<real_Num> &scale ) = 0;

            virtual Quaternion<real_Num> getOrientation() const = 0;
            virtual void setOrientation( const Quaternion<real_Num> &orientation ) = 0;

            virtual Vector3<real_Num> getRotation() const = 0;
            virtual void setRotation( const Vector3<real_Num> &rotation ) = 0;

            virtual Transform3<real_Num> getLocalTransform() const = 0;
            virtual void setLocalTransform( Transform3<real_Num> localTransform ) = 0;

            virtual Transform3<real_Num> getWorldTransform() const = 0;
            virtual void setWorldTransform( Transform3<real_Num> worldTransform ) = 0;

            virtual void updateTransform() = 0;
            virtual void updateWorldFromLocal() = 0;
            virtual void updateLocalFromWorld() = 0;

            virtual bool isLocalDirty() const = 0;
            virtual void setLocalDirty( bool localDirty, bool cascade = true ) = 0;

            virtual bool isDirty() const = 0;
            virtual void setDirty( bool dirty, bool cascade = true ) = 0;

            virtual void parentChanged( SmartPtr<IActor> newParent, SmartPtr<IActor> oldParent ) = 0;

            /** Gets the data as a properties object.
            @return The data as a properties object.
            */
            virtual SmartPtr<Properties> getProperties() const = 0;

            /** Sets the data as a properties object.
            @param properties The properties object.
            */
            virtual void setProperties( SmartPtr<Properties> properties ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace scene
}  // end namespace fb

#endif  // ITransform_h__
