#ifndef __FB_Transformation_h__
#define __FB_Transformation_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Atomics/AtomicFloat.h>

namespace fb
{
    namespace scene
    {
        /** Transformation implementation. */
        class Transform : public ITransform
        {
        public:
            /** The hash type for the position property. */
            static const hash_type TRANSFORMATION_POSITION_HASH;

            /** Constructor. */
            Transform();

            /** Constructor. */
            Transform( const Transform &other ) = delete;

            /** Destructor. */
            ~Transform() override;

            void load( SmartPtr<ISharedObject> data ) override;

            void unload( SmartPtr<ISharedObject> data ) override;

            void updateTransform() override;

            void parentChanged( SmartPtr<IActor> newParent,
                                SmartPtr<IActor> oldParent ) override;

            SmartPtr<IActor> getActor() const override;

            void setActor( SmartPtr<IActor> actor ) override;

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

            SmartPtr<IStateContext> getStateContext() const;
            void setStateContext( SmartPtr<IStateContext> stateContext );

            Transform3<real_Num> getLocalTransform() const override;
            void setLocalTransform( Transform3<real_Num> transform ) override;

            Transform3<real_Num> getWorldTransform() const override;
            void setWorldTransform( Transform3<real_Num> transform ) override;

            void updateWorldFromLocal() override;
            void updateLocalFromWorld() override;

            static void registerClass();

            time_interval getFrameTime() const override;

            void setFrameTime( time_interval frameTime ) override;

            time_interval getFrameDeltaTime() const override;

            void setFrameDeltaTime( time_interval frameDeltaTime ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            void updateFrameTime();

            mutable SpinRWMutex m_mutex;

            atomic_f64 m_frameTime = 0.0;
            atomic_f64 m_frameDeltaTime = 0.0;

            // SmartPtr<TransformState> m_state;

            SmartPtr<IActor> m_actor;
            SmartPtr<IStateContext> m_stateContext;

            Transform3<real_Num> m_worldTransform;
            Transform3<real_Num> m_localTransform;

            bool m_isLocalDirty = false;
            bool m_isDirty = false;
            bool m_enabled = true;

            /// The id extension.
            static u32 m_idExt;
        };
    } // namespace scene
}     // end namespace fb

#endif  // Transformation_h__
