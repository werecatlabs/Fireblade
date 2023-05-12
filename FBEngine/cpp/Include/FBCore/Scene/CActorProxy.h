#ifndef CActorProxy_h__
#define CActorProxy_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/IActorProxy.h>
#include <FBCore/Resource/CResource.h>

namespace fb
{
    namespace scene
    {
        class CActorProxy : public CResource<IActorProxy>
        {
        public:
            CActorProxy();
            ~CActorProxy() override;

            SmartPtr<IDirector> getDirector() const override;

            void setDirector( SmartPtr<IDirector> director ) override;

            SmartPtr<IActor> getActorImpl() const override;

            void setActorImpl( SmartPtr<IActor> actorImpl ) override;

            void updateDirty( u32 flags, u32 oldFlags ) override;

            Transform3<real_Num> getLocalTransform() const override;

            Transform3<real_Num> getWorldTransform() const override;

            Vector3<real_Num> getLocalPosition() const override;

            void setLocalPosition( const Vector3<real_Num> &localPosition ) override;

            Vector3<real_Num> getLocalScale() const override;

            void setLocalScale( const Vector3<real_Num> &localScale ) override;

            Quaternion<real_Num> getLocalOrientation() const override;

            void setLocalOrientation( const Quaternion<real_Num> &localOrientation ) override;

            Vector3<real_Num> getPosition() const override;

            void lookAt( const Vector3<real_Num> &position ) override;

            void lookAt( const Vector3<real_Num> &position, const Vector3<real_Num> &yawAxis ) override;

            void setPosition( const Vector3<real_Num> &position ) override;

            Vector3<real_Num> getScale() const override;

            void setScale( const Vector3<real_Num> &scale ) override;

            Quaternion<real_Num> getOrientation() const override;

            void setOrientation( const Quaternion<real_Num> &orientation ) override;

            void awake() override;

            void play() override;

            void edit() override;

            void levelWasLoaded( SmartPtr<IScene> scene ) override;

            void reset() override;

            void hierarchyChanged() override;

            void childAdded( SmartPtr<IActor> child ) override;

            void childRemoved( SmartPtr<IActor> child ) override;

            void childAddedInHierarchy( SmartPtr<IActor> child ) override;

            void childRemovedInHierarchy( SmartPtr<IActor> child ) override;

            String getName() const override;

            void setName( const String &name ) override;

            bool getPerpetual() const override;

            void setPerpetual( bool perpetual ) override;

            void addComponentInstance( SmartPtr<IComponent> component ) override;

            void removeComponentInstance( SmartPtr<IComponent> component ) override;

            u32 getEntity() const override;

            void setEntity( u32 entity ) override;

            Array<SmartPtr<scene::IComponent>> getComponents() const override;

            SharedPtr<ConcurrentArray<SmartPtr<IComponent>>> getComponentsPtr() const override;

            SmartPtr<IActor> getChildByIndex( u32 index ) const override;

            u32 getNumChildren() const override;

            s32 getSiblingIndex() const override;

            void addDirtyComponent( Thread::Task taskId, Thread::UpdateState updateType,
                                    SmartPtr<IComponent> component ) override;

            bool isComponentDirty( Thread::Task taskId, Thread::UpdateState updateType,
                                   SmartPtr<IComponent> component ) override;

            void registerComponentUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                          SmartPtr<scene::IComponent> object ) override;

            void unregisterComponentUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                            SmartPtr<scene::IComponent> object ) override;

            void unregisterAllComponent( SmartPtr<scene::IComponent> object ) override;

            void triggerEnter( SmartPtr<scene::IComponent> collision ) override;

            void triggerLeave( SmartPtr<scene::IComponent> collision ) override;

            void componentLoaded( SmartPtr<IComponent> component ) override;

            SmartPtr<IActor> getParent() const override;

            void setParent( SmartPtr<IActor> parent ) override;

            void addChild( SmartPtr<IActor> child ) override;

            void removeChild( SmartPtr<IActor> child ) override;

            void removeChildren() override;

            SmartPtr<IActor> findChild( const String &name ) override;

            SharedPtr<ConcurrentArray<SmartPtr<IActor>>> getChildrenPtr() const override;

            Array<SmartPtr<IActor>> getAllChildren() const override;

            bool isMine() const override;

            void setMine( bool mine ) override;

            bool isStatic() const override;

            void setStatic( bool isstatic ) override;

            bool isEnabledInScene() const override;

            bool isEnabled() const override;

            void setEnabled( bool enabled ) override;

            bool isVisible() const override;

            void setVisible( bool visible, bool cacade = true ) override;

            bool isDirty() const override;

            void setDirty( bool dirty ) override;

            void updateTransform() override;

            SmartPtr<IActor> getSceneRoot() const override;

            u32 getSceneLevel() const override;

            SmartPtr<ITransform> getTransform() const override;

            void setTransform( SmartPtr<ITransform> transform ) override;

            void setState( State state ) override;

            State getState() const override;

            void setGameState( GameState state ) override;

            GameState getGameState() const override;

            u32 getFlags() const override;

            void setFlags( u32 flags ) override;

            bool getFlag( u32 flag ) const override;

            void setFlag( u32 flag, bool value ) override;

            u32 getNewFlags() const override;

            void setNewFlags( u32 flags ) override;

            bool getNewFlag( u32 flag ) const override;

            void setNewFlag( u32 flag, bool value ) override;

            SmartPtr<IScene> getScene() const override;

            void setScene( SmartPtr<IScene> scene ) override;

            void updateVisibility() override;

        protected:
            AtomicSmartPtr<IDirector> m_director;
            AtomicSmartPtr<IActor> m_actorImpl;
            AtomicWeakPtr<Transform> m_transform;
            AtomicWeakPtr<IActor> m_parent;
            SharedPtr<ConcurrentArray<SmartPtr<IActor>>> m_children;
        };
    }  // namespace scene
}  // namespace fb

#endif  // CActorProxy_h__
