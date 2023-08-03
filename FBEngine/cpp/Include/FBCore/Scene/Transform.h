#ifndef __FB_Transformation_h__
#define __FB_Transformation_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Atomics/AtomicFloat.h>

namespace fb
{
    namespace scene
    {

        class Transform : public SharedObject<ITransform>
        {
        public:
            static const hash_type TRANSFORMATION_POSITION_HASH;

            Transform();
            Transform( const Transform &other ) = delete;
            Transform( u32 componentId );
            ~Transform() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void updateTransform() override;

            void parentChanged( SmartPtr<scene::IActor> newParent,
                                SmartPtr<scene::IActor> oldParent ) override;

            SmartPtr<scene::IActor> getActor() const override;

            void setActor( SmartPtr<scene::IActor> actor ) override;

            bool isLocalDirty() const override;
            void setLocalDirty( bool localDirty, bool cascade = true ) override;

            bool isDirty() const override;
            void setDirty( bool dirty, bool cascade = true ) override;

            void setEnabled( bool enabled );
            bool isEnabled() const;

            Vector3<real_Num> getLocalPosition() const override;
            void setLocalPosition( const Vector3<real_Num> &localPosition ) override;

            Vector3<real_Num> getLocalScale() const override;
            void setLocalScale( const Vector3<real_Num> &localScale ) override;

            Quaternion<real_Num> getLocalOrientation() const override;
            void setLocalOrientation( const Quaternion<real_Num> &localOrientation ) override;

            Vector3<real_Num> getLocalRotation() const override;

            void setLocalRotation( const Vector3<real_Num> &localRotation ) override;

            Vector3<real_Num> getPosition() const override;
            void setPosition( const Vector3<real_Num> &position ) override;

            Vector3<real_Num> getScale() const override;
            void setScale( const Vector3<real_Num> &scale ) override;

            Quaternion<real_Num> getOrientation() const override;
            void setOrientation( const Quaternion<real_Num> &orientation ) override;

            Vector3<real_Num> getRotation() const override;

            void setRotation( const Vector3<real_Num> &rotation ) override;

            /** @copydoc ITransform::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc ITransform::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            SmartPtr<IStateContext> getStateObject() const;
            void setStateObject( SmartPtr<IStateContext> stateObject );

            Transform3<real_Num> getLocalTransform() const override;
            void setLocalTransform( Transform3<real_Num> transform ) override;

            Transform3<real_Num> getWorldTransform() const override;
            void setWorldTransform( Transform3<real_Num> transform ) override;

            void updateWorldFromLocal() override;
            void updateLocalFromWorld() override;

            static void registerClass();

            time_interval getFrameTime() const;

            void setFrameTime( time_interval frameTime );

            time_interval getFrameDeltaTime() const;

            void setFrameDeltaTime( time_interval frameDeltaTime );

            FB_CLASS_REGISTER_DECL;

        protected:
            void updateFrameTime();

            mutable SpinRWMutex m_mutex;

            atomic_f64 m_frameTime = 0.0;
            atomic_f64 m_frameDeltaTime = 0.0;

            // SmartPtr<TransformState> m_state;

            SmartPtr<scene::IActor> m_actor;
            SmartPtr<IStateContext> m_stateObject;

            Transform3<real_Num> m_worldTransform;
            Transform3<real_Num> m_localTransform;

            bool m_isLocalDirty = false;
            bool m_isDirty = false;
            bool m_enabled = true;

            /// The id extension.
            static u32 m_idExt;
        };

    }  // namespace scene
}  // end namespace fb

#endif  // Transformation_h__
