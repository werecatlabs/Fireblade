#ifndef __FB_CActor_h__
#define __FB_CActor_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Resource/Resource.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/ConcurrentArray.h>
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

                void unload( SmartPtr<ISharedObject> data ) override;

                IFSM::ReturnType handleEvent( u32 state, IFSM::Event eventType ) override;

                SmartPtr<Actor> getOwner() const;
                void setOwner( SmartPtr<Actor> owner );

            private:
                AtomicSmartPtr<Actor> m_owner;
            };

            class ActorGameFsmListener : public FSMListener
            {
            public:
                ActorGameFsmListener() = default;
                ~ActorGameFsmListener() override = default;

                void unload( SmartPtr<ISharedObject> data ) override;

                IFSM::ReturnType handleEvent( u32 state, IFSM::Event eventType ) override;

                SmartPtr<Actor> getOwner() const;
                void setOwner( SmartPtr<Actor> owner );

            private:
                AtomicSmartPtr<Actor> m_owner;
            };

            /** @brief Constructor. */
            Actor();

            /** @brief Destructor. */
            ~Actor() override;

            /** @copydoc IActor::getName */
            String getName() const override;

            /** @copydoc IActor::setName */
            void setName( const String &name ) override;

            /** @copydoc IActor::getLocalTransform */
            Transform3<real_Num> getLocalTransform() const override;

            /** @copydoc IActor::getWorldTransform */
            Transform3<real_Num> getWorldTransform() const override;

            /** @copydoc IActor::getLocalTransform */
            Transform3<real_Num> getLocalTransform( time_interval t ) const override;

            /** @copydoc IActor::getWorldTransform */
            Transform3<real_Num> getWorldTransform( time_interval t ) const override;

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

            /** @copydoc IActor::levelWasLoaded */
            void levelWasLoaded( SmartPtr<IScene> scene ) override;

            /** @copydoc IActor::hierarchyChanged */
            void hierarchyChanged() override;

            /** @copydoc IActor::childAdded */
            void childAdded( SmartPtr<IActor> child ) override;

            /** @copydoc IActor::childRemoved */
            void childRemoved( SmartPtr<IActor> child ) override;

            /** @copydoc IActor::updateDirty */
            void updateDirty( u32 flags, u32 oldFlags ) override;

            /** @copydoc IActor::preUpdate */
            void preUpdate() override;

            /** @copydoc IActor::update */
            void update() override;

            /** @copydoc IActor::postUpdate */
            void postUpdate() override;

            /** @copydoc Resource<IActor>::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Resource<IActor>::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Resource<IActor>::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Resource<IActor>::addComponentInstance */
            void addComponentInstance( SmartPtr<IComponent> component ) override;

            /** @copydoc Resource<IActor>::removeComponent */
            void removeComponentInstance( SmartPtr<IComponent> component ) override;

            /** @copydoc IActor::hasComponent */
            bool hasComponent( int handle );

            /** @copydoc IActor::getComponent */
            SmartPtr<IComponent> getComponent( int handle ) const;

            /** @copydoc IActor::getComponents */
            Array<SmartPtr<IComponent>> getComponents() const override;

            /** @copydoc IActor::getComponentsPtr */
            SharedPtr<ConcurrentArray<SmartPtr<IComponent>>> getComponentsPtr() const override;

            /** @copydoc IActor::getPerpetual */
            bool getPerpetual() const override;

            /** @copydoc IActor::setPerpetual */
            void setPerpetual( bool perpetual, bool cascade = false ) override;

            /** @copydoc IActor::getScene */
            SmartPtr<IScene> getScene() const override;

            /** @copydoc IActor::setScene */
            void setScene( SmartPtr<IScene> scene ) override;

            /** @copydoc IActor::triggerEnter */
            void triggerEnter( SmartPtr<IComponent> collision ) override;

            /** @copydoc IActor::triggerLeave */
            void triggerLeave( SmartPtr<IComponent> collision ) override;

            /** @copydoc IActor::componentLoaded */
            void componentLoaded( SmartPtr<IComponent> loadedComponent ) override;

            /** @copydoc IActor::getChildByIndex */
            SmartPtr<IActor> getChildByIndex( u32 index ) const override;

            /** @copydoc IActor::getNumChildren */
            u32 getNumChildren() const override;

            /** @copydoc IActor::getSiblingIndex */
            s32 getSiblingIndex() const override;

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
            Array<SmartPtr<IActor>> getChildren() const override;

            /** @copydoc IActor::getChildrenPtr */
            SharedPtr<ConcurrentArray<SmartPtr<IActor>>> getChildrenPtr() const override;

            /** @copydoc IActor::getAllChildren */
            Array<SmartPtr<IActor>> getAllChildren() const override;

            /** @copydoc IActor::getAllChildren */
            Array<SmartPtr<IActor>> getAllChildren( SmartPtr<IActor> parent ) const;

            /** @copydoc IActor::setSiblingIndex */
            void setSiblingIndex( s32 index ) override;

            /** @copydoc IActor::setChildSiblingIndex */
            void setChildSiblingIndex( SmartPtr<IActor> child, s32 index ) override;

            /** @copydoc IActor::toData */
            SmartPtr<ISharedObject> toData() const override;

            /** @copydoc IActor::fromData */
            void fromData( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IActor::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IActor::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IActor::isMine */
            bool isMine() const override;

            /** @copydoc IActor::setMine */
            void setMine( bool mine ) override;

            /** @copydoc IActor::isStatic */
            bool isStatic() const override;

            /** @copydoc IActor::setStatic */
            void setStatic( bool isstatic ) override;

            /** @copydoc IActor::isEnabledInScene */
            bool isEnabledInScene() const override;

            /** Used to know if the actor is enabled. */
            bool isEnabled() const override;

            /** Used to know if the actor is enabled. */
            void setEnabled( bool enabled ) override;

            /** @copydoc IActor::isVisible */
            bool isVisible() const override;

            /** @copydoc IActor::setVisible */
            void setVisible( bool visible, bool cacade = true ) override;

            /** @copydoc IActor::isDirty */
            bool isDirty() const override;

            /** @copydoc IActor::setDirty */
            void setDirty( bool dirty ) override;

            /** @copydoc ISharedObject::isValid */
            bool isValid() const override;

            /** @copydoc IActor::updateTransform */
            void updateTransform() override;

            /** @copydoc IActor::getParent */
            SmartPtr<IActor> getParent() const override;

            /** @copydoc IActor::setParent */
            void setParent( SmartPtr<IActor> parent ) override;

            /** @copydoc IActor::compareTag */
            bool compareTag( const String &tag ) const override;

            /** @copydoc IActor::getSceneRoot */
            SmartPtr<IActor> getSceneRoot() const override;

            /** @copydoc IActor::getSceneLevel */
            u32 getSceneLevel() const override;

            /** @copydoc IActor::getTransform */
            SmartPtr<ITransform> &getTransform() override;

            /** @copydoc IActor::getTransform */
            const SmartPtr<ITransform> &getTransform() const override;

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

            /** @copydoc IActor::getFlags */
            u32 getFlags() const override;

            /** @copydoc IActor::setFlags */
            void setFlags( u32 flags ) override;

            /** @copydoc IActor::getFlag */
            bool getFlag( u32 flag ) const override;

            /** @copydoc IActor::setFlag */
            void setFlag( u32 flag, bool value ) override;

            /** @copydoc IActor::updateVisibility */
            void updateVisibility() override;

            /** @copydoc IActor::handleEvent */
            Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                   const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                   SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

            /** @copydoc IActor::sendEvent */
            Parameter sendEvent( IEvent::Type eventType, hash_type eventValue,
                                 const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                 SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

            static void registerClass();

            FB_CLASS_REGISTER_DECL;

        protected:
            /** Handle an actor event. */
            IFSM::ReturnType handleActorEvent( u32 state, IFSM::Event eventType );

            /** Handle an actor game event. */
            IFSM::ReturnType handleActorGameEvent( u32 state, IFSM::Event eventType );

            // The scene this actor belongs to.
            SmartPtr<IScene> m_scene;

            // The parent of this actor.
            SmartPtr<IActor> m_parent;

            // The transform of this actor.
            SmartPtr<ITransform> m_transform;

            // The components.
            AtomicSharedPtr<ConcurrentArray<SmartPtr<IComponent>>> m_components;

            // The actor children.
            AtomicSharedPtr<ConcurrentArray<SmartPtr<IActor>>> m_children;

            // The actor factory manager.
            AtomicWeakPtr<IFactoryManager> m_factoryManager;

            // The actor flags.
            u32 *m_flags = nullptr;

            // The actor flags.
            Array<String> m_tags;
            
            // Used to generate unique ids.
            static u32 m_idExt;
        };
    }  // namespace scene
}  // namespace fb

#endif  // CActor_h__
