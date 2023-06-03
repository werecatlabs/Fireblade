#ifndef __FB_CActor_h__
#define __FB_CActor_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Resource/Resource.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>
#include <FBCore/Core/FixedArray.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Core/FSMListener.h>

namespace fb
{
    namespace scene
    {
        /** Concrete actor implementation.
        @author	Zane Desir
        @version 1.0
        */
        class Actor : public Resource<IActor>
        {
        public:
            class ActorFsmListener : public FSMListener
            {
            public:
                ActorFsmListener() = default;
                ~ActorFsmListener() override = default;

                IFSM::ReturnType handleEvent( u32 state, IFSM::Event eventType ) override;

                Actor *getOwner() const;
                void setOwner( Actor *owner );

            private:
                Actor *m_owner = nullptr;
            };

            class ActorGameFsmListener : public FSMListener
            {
            public:
                ActorGameFsmListener() = default;
                ~ActorGameFsmListener() override = default;

                IFSM::ReturnType handleEvent( u32 state, IFSM::Event eventType ) override;

                Actor *getOwner() const;
                void setOwner( Actor *owner );

            private:
                Actor *m_owner = nullptr;
            };

            Actor();
            ~Actor() override;

            /** @copydoc IActor::getName */
            String getName() const override;

            /** @copydoc IActor::setName */
            void setName( const String &name ) override;

            Transform3<real_Num> getLocalTransform() const override;

            Transform3<real_Num> getWorldTransform() const override;

            virtual Transform3<real_Num> getLocalTransform( time_interval t ) const;

            virtual Transform3<real_Num> getWorldTransform( time_interval t ) const;

            /** @copydoc IActor::getLocalPosition */
            Vector3<real_Num> getLocalPosition() const override;

            /** @copydoc IActor::setLocalPosition */
            void setLocalPosition( const Vector3<real_Num> &localPosition ) override;

            /** @copydoc IActor::getLocalScale */
            Vector3<real_Num> getLocalScale() const override;

            /** @copydoc IActor::setLocalScale */
            void setLocalScale( const Vector3<real_Num> &localScale ) override;

            /** @copydoc IActor::getLocalOrientation */
            Quaternion<real_Num> getLocalOrientation() const override;

            /** @copydoc IActor::setLocalOrientation */
            void setLocalOrientation( const Quaternion<real_Num> &localOrientation ) override;

            /** @copydoc IActor::getPosition */
            Vector3<real_Num> getPosition() const override;

            /** @copydoc IActor::lookAt */
            void lookAt( const Vector3<real_Num> &position ) override;

            /** @copydoc IActor::lookAt */
            void lookAt( const Vector3<real_Num> &position, const Vector3<real_Num> &yawAxis ) override;

            /** @copydoc IActor::setPosition */
            void setPosition( const Vector3<real_Num> &position ) override;

            /** @copydoc IActor::getScale */
            Vector3<real_Num> getScale() const override;

            /** @copydoc IActor::setScale */
            void setScale( const Vector3<real_Num> &scale ) override;

            /** @copydoc IActor::getOrientation */
            Quaternion<real_Num> getOrientation() const override;

            /** @copydoc IActor::setOrientation */
            void setOrientation( const Quaternion<real_Num> &orientation ) override;

            /** @copydoc IActor::awake */
            void awake() override;

            /** @copydoc IActor::start */
            void play() override;

            /** @copydoc IActor::edit */
            void edit() override;

            /** @copydoc IActor::levelWasLoaded */
            void levelWasLoaded( SmartPtr<IScene> scene ) override;

            /** @copydoc IActor::reset */
            void reset() override;

            /** @copydoc IActor::hierarchyChanged */
            void hierarchyChanged() override;

            /** @copydoc IActor::childAdded */
            void childAdded( SmartPtr<IActor> child ) override;

            /** @copydoc IActor::childRemoved */
            void childRemoved( SmartPtr<IActor> child ) override;

            template <class T>
            SmartPtr<T> removeComponent();

            template <class T>
            SmartPtr<T> removeComponents();

            /** @copydoc IActor::updateDirty */
            void updateDirty( u32 flags, u32 oldFlags ) override;

            /** @copydoc IActor::preUpdate */
            void preUpdate() override;

            /** @copydoc IActor::update */
            void update() override;

            /** @copydoc IActor::postUpdate */
            void postUpdate() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IActor::addComponentInstance */
            void addComponentInstance( SmartPtr<IComponent> component ) override;

            /** @copydoc IActor::removeComponent */
            void removeComponentInstance( SmartPtr<IComponent> component ) override;

            /** @copydoc IActor::hasComponent */
            bool hasComponent( int handle );

            /** @copydoc IActor::getComponent */
            SmartPtr<IComponent> getComponent( int handle ) const;

            /** @copydoc IActor::getComponents */
            Array<SmartPtr<IComponent>> getComponents() const override;

            SharedPtr<ConcurrentArray<SmartPtr<IComponent>>> getComponentsPtr() const override;

            /** @copydoc IActor::getPerpetual */
            bool getPerpetual() const override;

            /** @copydoc IActor::setPerpetual */
            void setPerpetual( bool destroyOnLoad ) override;

            /** @copydoc IActor::getAutoUpdateComponents */
            bool getAutoUpdateComponents() const;

            /** @copydoc IActor::setAutoUpdateComponents */
            void setAutoUpdateComponents( bool autoUpdateComponents );

            /** @copydoc IActor::getScene */
            SmartPtr<IScene> getScene() const override;

            /** @copydoc IActor::setScene */
            void setScene( SmartPtr<IScene> scene ) override;

            /** @copydoc IActor::isRegistered */
            bool isRegistered( Thread::Task taskId, Thread::UpdateState updateType ) const;

            /** @copydoc IActor::registerUpdate */
            void registerUpdate( Thread::Task taskId, Thread::UpdateState updateType );

            /** @copydoc IActor::unregisterUpdate */
            void unregisterUpdate( Thread::Task taskId, Thread::UpdateState updateType );

            /** @copydoc IActor::unregisterAll */
            void unregisterAll();

            /** @copydoc IActor::registerComponentUpdate */
            void registerComponentUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                          SmartPtr<IComponent> object ) override;

            /** @copydoc IActor::unregisterComponentUpdate */
            void unregisterComponentUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                            SmartPtr<IComponent> object ) override;

            /** @copydoc IActor::unregisterAllComponent */
            void unregisterAllComponent( SmartPtr<IComponent> object ) override;

            /** @copydoc IActor::handleEvent */
            void handleEvent( const String &data );

            /** @copydoc IActor::getGraphicsDataAsJSON */
            String getGraphicsDataAsJSON() const;

            /** @copydoc IActor::triggerEnter */
            void triggerEnter( SmartPtr<IComponent> collision ) override;

            /** @copydoc IActor::triggerLeave */
            void triggerLeave( SmartPtr<IComponent> collision ) override;

            /** @copydoc IActor::componentLoaded */
            void componentLoaded( SmartPtr<IComponent> component ) override;

            SmartPtr<IActor> getChildByIndex( u32 index ) const;

            u32 getNumChildren() const;

            s32 getSiblingIndex() const;

            /** @copydoc IActor::addDirtyComponent */
            void addDirtyComponent( Thread::Task taskId, Thread::UpdateState updateType,
                                    SmartPtr<IComponent> component ) override;

            /** @copydoc IActor::isComponentSiblingDirty */
            bool isComponentSiblingDirty( SmartPtr<IComponent> sibling, Thread::Task taskId,
                                          Thread::UpdateState updateType,
                                          SmartPtr<IComponent> component );

            /** @copydoc IActor::isComponentDirty */
            bool isComponentDirty( Thread::Task taskId, Thread::UpdateState updateType,
                                   SmartPtr<IComponent> component ) override;

            /** @copydoc IActor::getRegisteredComponents */
            SharedPtr<ConcurrentArray<SmartPtr<IComponent>>> getRegisteredComponents(
                Thread::UpdateState updateState, Thread::Task task ) const;

            /** @copydoc IActor::setRegisteredComponents */
            void setRegisteredComponents( Thread::UpdateState updateState, Thread::Task task,
                                          boost::shared_ptr<ConcurrentArray<SmartPtr<IComponent>>> p );

            /** Adds a child to this actor. */
            void addChild( SmartPtr<IActor> child ) override;

            /** Removes a child of this actor. */
            void removeChild( SmartPtr<IActor> child ) override;

            /** Triggered when a child is added. */
            void childAddedInHierarchy( SmartPtr<IActor> child ) override;

            /** Triggered when a child is remove. */
            void childRemovedInHierarchy( SmartPtr<IActor> child ) override;

            /** Removes all children. */
            void removeChildren() override;

            /** Return the child with the name provided. Return null if a child is not found. */
            SmartPtr<IActor> findChild( const String &name ) override;

            /** @copydoc IActor::getChildren */
            SharedPtr<ConcurrentArray<SmartPtr<IActor>>> getChildrenPtr() const override;

            /** @copydoc IActor::getAllChildren */
            Array<SmartPtr<IActor>> getAllChildren() const override;

            /** @copydoc IActor::getAllChildren */
            Array<SmartPtr<IActor>> getAllChildren( SmartPtr<IActor> parent ) const;

            /** @copydoc IActor::toData */
            SmartPtr<ISharedObject> toData() const;

            /** @copydoc IActor::fromData */
            void fromData( SmartPtr<ISharedObject> data );

            /** @copydoc IActor::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IActor::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IActor::isMine */
            bool isMine() const override;

            /** @copydoc IActor::setMine */
            void setMine( bool val ) override;

            /** @copydoc IActor::isStatic */
            bool isStatic() const override;

            /** @copydoc IActor::setStatic */
            void setStatic( bool isstatic ) override;

            /** @copydoc IActor::isEnabledInScene */
            bool isEnabledInScene() const override;

            /** Used to know if the actor is enabled. */
            bool isEnabled() const override;

            /** Used to know if the actor is enabled. */
            void setEnabled( bool val ) override;

            /** @copydoc IActor::isVisible */
            bool isVisible() const override;

            /** @copydoc IActor::setVisible */
            void setVisible( bool visible, bool cacade = true ) override;

            /** @copydoc IActor::isDirty */
            bool isDirty() const override;

            /** @copydoc IActor::setDirty */
            void setDirty( bool val ) override;

            /** @copydoc ISharedObject::isValid */
            bool isValid() const override;

            /** @copydoc IActor::updateTransform */
            void updateTransform() override;

            /** @copydoc IActor::getParent */
            SmartPtr<IActor> getParent() const override;

            /** @copydoc IActor::setParent */
            void setParent( SmartPtr<IActor> parent ) override;

            /** @copydoc IActor::preUpdateDirtyComponents */
            void preUpdateDirtyComponents();

            /** @copydoc IActor::updateDirtyComponents */
            void updateDirtyComponents();

            /** @copydoc IActor::postUpdateDirtyComponents */
            void postUpdateDirtyComponents();

            /** @copydoc IActor::compareTag */
            bool compareTag( const String &tag ) const;

            /** @copydoc IActor::getSceneRoot */
            SmartPtr<IActor> getSceneRoot() const override;

            /** @copydoc IActor::getSceneLevel */
            u32 getSceneLevel() const override;

            /** @copydoc IActor::getTransform */
            SmartPtr<ITransform> getTransform() const override;

            /** @copydoc IActor::setTransform */
            void setTransform( SmartPtr<ITransform> transform ) override;

            /** @copydoc IActor::setState */
            void setState( State state ) override;

            /** @copydoc IActor::setState */
            State getState() const override;

            /** @copydoc IActor::setGameState */
            void setGameState( GameState state ) override;

            /** @copydoc IActor::getGameState */
            GameState getGameState() const override;

            u32 getFlags() const override;

            void setFlags( u32 flags ) override;

            bool getFlag( u32 flag ) const override;

            void setFlag( u32 flag, bool value ) override;

            u32 getNewFlags() const override;

            void setNewFlags( u32 flags ) override;

            bool getNewFlag( u32 flag ) const override;

            void setNewFlag( u32 flag, bool value ) override;

            void updateVisibility() override;

            static void registerClass();

            FB_CLASS_REGISTER_DECL;

        protected:
            void updateComponents();
            void rebuildComponentCache();

            void preUpdateDirtyComponent( SmartPtr<IComponent> &component );
            void updateDirtyComponent( SmartPtr<IComponent> &component );
            void postUpdateDirtyComponent( SmartPtr<IComponent> &component );

            IFSM::ReturnType handleActorEvent( u32 state, IFSM::Event eventType );

            IFSM::ReturnType handleActorGameEvent( u32 state, IFSM::Event eventType );

            AtomicWeakPtr<IScene> m_scene;

            AtomicWeakPtr<IActor> m_parent;

            AtomicWeakPtr<Transform> m_transform;

            SharedPtr<ConcurrentArray<SmartPtr<IComponent>>> m_components;

            SharedPtr<ConcurrentArray<SmartPtr<IActor>>> m_children;

            AtomicWeakPtr<IFactoryManager> m_factoryManager;

            atomic_bool m_autoUpdateComponents = true;
            atomic_bool m_destroyOnLoad = false;

            static u32 m_idExt;
        };
    }  // namespace scene
}  // namespace fb

#endif  // CActor_h__
