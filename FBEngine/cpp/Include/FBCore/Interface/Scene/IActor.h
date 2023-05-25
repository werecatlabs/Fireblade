#ifndef IActor_h__
#define IActor_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Math/AABB3.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Interface/Scene/IComponent.h>

namespace fb
{
    namespace scene
    {

        /**
         * Interface for an actor class. Actors are entities in a scene that can be manipulated and moved.
         * @author	Zane Desir
         * @version 1.0
         */
        class IActor : public IResource
        {
        public:
            /**
             * @brief The states of an actor
             */
            enum class State
            {
                None,     ///< The actor is being created
                Create,     ///< The actor is being created
                Destroyed,  ///< The actor has been destroyed
                Edit,       ///< The actor is being edited
                Play,       ///< The actor is being played

                Count  ///< The number of possible states
            };

            /**
             * @brief The game states of an actor
             */
            enum class GameState
            {
                None,     ///< The actor is being created
                Create,     ///< The actor is being created
                Edit,       ///< The actor is being edited
                Play,       ///< The actor is being played
                Destroyed,  ///< The actor has been destroyed
                Reset,      ///< The actor is being reset

                Count  ///< The number of possible game states
            };

            static const u32 ActorFlagReserved;  ///< Reserved actor flag
            static const u32 ActorFlagStatic;    ///< Actor is static
            static const u32 ActorFlagVisible;   ///< Actor is visible
            static const u32 ActorFlagEnabled;   ///< Actor is enabled
            static const u32 ActorFlagMine;      ///< Actor belongs to the client in network game
            static const u32
                ActorFlagPerpetual;             ///< Actor should be destroyed when a new scene is loaded
            static const u32 ActorFlagDirty;    ///< Actor is dirty
            static const u32 ActorFlagAwake;    ///< Actor is awake
            static const u32 ActorFlagStarted;  ///< Actor is started
            static const u32 ActorFlagDummy;    ///< Actor is a dummy
            static const u32 ActorFlagInScene;  ///< Actor is in the scene
            static const u32 ActorFlagEnabledInScene;  ///< Actor is enabled in the scene
            static const u32 ActorFlagIsEditor;  ///< Actor is visible in the editor

            /**
             * The destructor.
             */
            ~IActor() override = default;

            /**
             * Update the actor when it's marked as dirty.
             * @param flags A bitmask of flags indicating which aspects of the actor have changed.
             * @param oldFlags A bitmask of flags indicating the previous state of the actor.
             */
            virtual void updateDirty( u32 flags, u32 oldFlags ) = 0;

            /**
             * Gets the local transform of the actor.
             * @return A Transform3 object representing the local transform of the actor.
             */
            virtual Transform3<real_Num> getLocalTransform() const = 0;

            /**
             * Gets the world transform of the actor.
             * @return A Transform3 object representing the world transform of the actor.
             */
            virtual Transform3<real_Num> getWorldTransform() const = 0;

            /**
             * Gets the local position of the actor.
             * @return A Vector3 object representing the local position of the actor.
             */
            virtual Vector3<real_Num> getLocalPosition() const = 0;

            /**
             * Sets the local position of the actor.
             * @param localPosition The new local position of the actor.
             */
            virtual void setLocalPosition( const Vector3<real_Num> &localPosition ) = 0;

            /**
             * Gets the local scale of the actor.
             * @return A Vector3 object representing the local scale of the actor.
             */
            virtual Vector3<real_Num> getLocalScale() const = 0;

            /**
             * Sets the local scale of the actor.
             * @param localScale The new local scale of the actor.
             */
            virtual void setLocalScale( const Vector3<real_Num> &localScale ) = 0;

            /**
             * Gets the local orientation of the actor.
             * @return A Quaternion object representing the local orientation of the actor.
             */
            virtual Quaternion<real_Num> getLocalOrientation() const = 0;

            /**
             * Sets the local orientation of the actor.
             * @param localOrientation The new local orientation of the actor.
             */
            virtual void setLocalOrientation( const Quaternion<real_Num> &localOrientation ) = 0;

            /**
             * Gets the world position of the actor.
             * @return A Vector3 object representing the world position of the actor.
             */
            virtual Vector3<real_Num> getPosition() const = 0;

            /**
             * Make the actor look at a world position.
             * @param position The world position to look at.
             */
            virtual void lookAt( const Vector3<real_Num> &position ) = 0;

            /**
             * Make the actor look at a world position.
             * @param position The world position to look at.
             * @param yawAxis The yaw axis of the actor.
             */
            virtual void lookAt( const Vector3<real_Num> &position,
                                 const Vector3<real_Num> &yawAxis ) = 0;

            /**
             * Set the world position of the actor.
             * @param position The world position to set.
             */
            virtual void setPosition( const Vector3<real_Num> &position ) = 0;

            /**
             * Get the world scale of the actor.
             * @return The world scale of the actor.
             */
            virtual Vector3<real_Num> getScale() const = 0;

            /**
             * Set the world scale of the actor.
             * @param scale The world scale to set.
             */
            virtual void setScale( const Vector3<real_Num> &scale ) = 0;

            /**
             * Get the world orientation of the actor.
             * @return The world orientation of the actor.
             */
            virtual Quaternion<real_Num> getOrientation() const = 0;

            /**
             * Set the world orientation of the actor.
             * @param orientation The world orientation to set.
             */
            virtual void setOrientation( const Quaternion<real_Num> &orientation ) = 0;

            /**
             * Called when the actor is awakened.
             */
            virtual void awake() = 0;

            /**
             * Called when the actor is played.
             */
            virtual void play() = 0;

            /**
             * Called when the actor is edited.
             */
            virtual void edit() = 0;

            /**
             * Called when the level is loaded.
             * @param scene The loaded scene.
             */
            virtual void levelWasLoaded( SmartPtr<IScene> scene ) = 0;

            /**
             * Called when the actor is reset.
             */
            virtual void reset() = 0;

            /**
             * Called when the hierarchy changes.
             */
            virtual void hierarchyChanged() = 0;

            /**
             * Called when a child is added to the actor.
             * @param child The added child actor.
             */
            virtual void childAdded( SmartPtr<IActor> child ) = 0;

            /**
             * Called when a child is removed from the actor.
             * @param child The removed child actor.
             */
            virtual void childRemoved( SmartPtr<IActor> child ) = 0;

            /**
             * Called when a child is added to the actor's hierarchy.
             * @param child The added child actor.
             */
            virtual void childAddedInHierarchy( SmartPtr<IActor> child ) = 0;

            /**
             * Called when a child is removed from the actor's hierarchy.
             * @param child The removed child actor.
             */
            virtual void childRemovedInHierarchy( SmartPtr<IActor> child ) = 0;

            /**
             * Get the name of the actor.
             * @return The name of the actor.
             */
            virtual String getName() const = 0;

            /**
             * Set the name of the actor.
             * @param name The name to set.
             */
            virtual void setName( const String &name ) = 0;

            /** Returns a boolean to indicate if this actor should be destroyed when a new scene is loaded.
            @returns A boolean value. The value will be true if the actor is going to be destroyed. A false
            value to opposite.
            */
            virtual bool getPerpetual() const = 0;

            /** Sets a boolean to indicate if this actor should be destroyed when a new scene is loaded.
            @param perpetual A boolean value to indicate if this actor should be destroyed.
            */
            virtual void setPerpetual( bool perpetual ) = 0;

            /** Adds a component instance. */
            virtual void addComponentInstance( SmartPtr<IComponent> component ) = 0;

            /** Removes a component from the actor. */
            virtual void removeComponentInstance( SmartPtr<IComponent> component ) = 0;

            /**
             * Adds a component.
             *
             * @tparam T The type of component to add.
             * @return A SmartPtr to the new component.
             */
            template <class T>
            SmartPtr<T> addComponent()
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto component = factoryManager->make_ptr<T>();
                addComponentInstance( component );
                component->load( nullptr );
                return component;
            }

            /** Gets a component by the template type and the id.
            @param id The id of the component.
            */
            template <class T>
            SmartPtr<T> getComponentById( s32 id ) const
            {
                auto components = getComponents();
                for( auto &component : components )
                {
                    if( component )
                    {
                        auto derivedComponent = fb::dynamic_pointer_cast<T>( component );
                        if( derivedComponent )
                        {
                            auto handle = derivedComponent->getHandle();
                            FB_ASSERT( handle );

                            if( handle->getId() == id )
                            {
                                return derivedComponent;
                            }
                        }
                    }
                }

                return nullptr;
            }

            /** Gets a component by the template type. */
            template <class T>
            SmartPtr<T> getComponent() const
            {
                auto components = getComponents();
                for( auto &component : components )
                {
                    if( component )
                    {
                        auto derivedComponent = fb::dynamic_pointer_cast<T>( component );
                        if( derivedComponent )
                        {
                            return derivedComponent;
                        }
                    }
                }

                return nullptr;
            }

            /** Gets a component in the actor and it's children by the template type. */
            template <class T>
            SmartPtr<T> getComponentInChildren() const
            {
                if( auto p = getChildrenPtr() )
                {
                    auto &children = *p;
                    for( auto &child : children )
                    {
                        if( auto component = child->getComponent<T>() )
                        {
                            return component;
                        }
                    }
                }

                return nullptr;
            }

            /** Gets a component in the actor and it's children by the template type. */
            template <class T>
            SmartPtr<T> getComponentAndInChildren() const
            {
                auto components = getComponents();
                for( auto &component : components )
                {
                    if( component )
                    {
                        if( auto derivedComponent = fb::dynamic_pointer_cast<T>( component ) )
                        {
                            return derivedComponent;
                        }
                    }
                }

                if( auto p = getChildrenPtr() )
                {
                    auto &children = *p;
                    for( auto &child : children )
                    {
                        if( auto component = child->getComponentAndInChildren<T>() )
                        {
                            return component;
                        }
                    }
                }

                return nullptr;
            }

            /** Gets components in the actor and it's children by the template type. */
            template <class T>
            Array<SmartPtr<T>> getComponentsInChildren() const
            {
                Array<SmartPtr<T>> componentsInChildren;
                componentsInChildren.reserve( 32 );

                if( auto p = getChildrenPtr() )
                {
                    auto &children = *p;
                    for( auto child : children )
                    {
                        if( child )
                        {
                            auto components = child->getComponentsByType<T>();
                            if( !components.empty() )
                            {
                                componentsInChildren.insert( componentsInChildren.end(),
                                                             components.begin(), components.end() );
                            }

                            auto childComponents = child->getComponentsInChildren<T>();
                            if( !childComponents.empty() )
                            {
                                componentsInChildren.insert( componentsInChildren.end(),
                                                             childComponents.begin(),
                                                             childComponents.end() );
                            }
                        }
                    }
                }

                return componentsInChildren;
            }

            /** Gets components in the actor and it's children by the template type. */
            template <class T>
            Array<SmartPtr<T>> getAllComponentsInChildren() const
            {
                Array<SmartPtr<T>> componentsInChildren;
                componentsInChildren.reserve( 32 );

                auto children = getAllChildren();
                for( auto &child : children )
                {
                    auto childComponents = child->getComponentsByType<T>();
                    if( !childComponents.empty() )
                    {
                        componentsInChildren.insert( componentsInChildren.end(), childComponents.begin(),
                                                     childComponents.end() );
                    }
                }

                return componentsInChildren;
            }

            /** Gets components in the actor and it's children by the template type. */
            template <class T>
            Array<SmartPtr<T>> getComponentsAndInChildren() const
            {
                const auto components = getComponents();

                Array<SmartPtr<T>> componentsInChildren;
                componentsInChildren.reserve( components.size() );

                for( auto &component : components )
                {
                    if( component )
                    {
                        if( auto derivedComponent = fb::dynamic_pointer_cast<T>( component ) )
                        {
                            componentsInChildren.push_back( derivedComponent );
                        }
                    }
                }

                if( auto p = getChildrenPtr() )
                {
                    auto &children = *p;
                    for( auto &child : children )
                    {
                        auto childComponents = child->getComponentsInChildren<T>();
                        if( !childComponents.empty() )
                        {
                            componentsInChildren.insert( componentsInChildren.end(),
                                                         childComponents.begin(),
                                                         childComponents.end() );
                        }
                    }
                }

                return componentsInChildren;
            }

            /** Gets components in the actor and it's children by the template type. */
            template <class T>
            Array<SmartPtr<T>> getAllComponentsAndInChildren() const
            {
                const auto components = getComponents();

                Array<SmartPtr<T>> componentsInChildren;
                componentsInChildren.reserve( components.size() );

                for( auto &component : components )
                {
                    if( component )
                    {
                        if( auto derivedComponent = fb::dynamic_pointer_cast<T>( component ) )
                        {
                            componentsInChildren.push_back( derivedComponent );
                        }
                    }
                }

                auto children = getAllChildren();
                for( auto &child : children )
                {
                    auto childComponents = child->getComponents();
                    if( !childComponents.empty() )
                    {
                        componentsInChildren.insert( componentsInChildren.end(), childComponents.begin(),
                                                     childComponents.end() );
                    }
                }

                return componentsInChildren;
            }

            /** Gets components in the actor by the template type. */
            template <class T>
            Array<SmartPtr<T>> getComponentsByType() const
            {
                const auto components = getComponents();

                Array<SmartPtr<T>> componentsByType;
                componentsByType.reserve( components.size() );

                for( auto &component : components )
                {
                    if( component )
                    {
                        if( auto derivedComponent = fb::dynamic_pointer_cast<T>( component ) )
                        {
                            componentsByType.push_back( derivedComponent );
                        }
                    }
                }

                return componentsByType;
            }

            /** Gets all components in the actor. */
            virtual Array<SmartPtr<scene::IComponent>> getComponents() const = 0;

            /** Gets all components in the actor. */
            virtual SharedPtr<ConcurrentArray<SmartPtr<IComponent>>> getComponentsPtr() const = 0;

            /** Used to know if the actor has a component. */
            template <class T>
            bool hasComponent() const
            {
                const auto components = getComponents();
                for( auto &component : components )
                {
                    if( component )
                    {
                        if( auto derivedComponent = fb::dynamic_pointer_cast<T>( component ) )
                        {
                            return true;
                        }
                    }
                }

                return false;
            }

            template <typename T>
            SmartPtr<T> getComponentInParent()
            {
                auto currentActor = getSharedFromThis<IActor>();

                while( currentActor != nullptr )
                {
                    auto componentInParent = currentActor->getComponent<T>();
                    if( componentInParent != nullptr )
                    {
                        return componentInParent;
                    }

                    currentActor = currentActor->getParent();
                }

                return nullptr;
            }

            /** Gets a child by index. */
            virtual SmartPtr<IActor> getChildByIndex(u32 index) const = 0;

            /** Gets the number of children. */
            virtual u32 getNumChildren() const = 0;

            /** Gets the sibling index. */
            virtual s32 getSiblingIndex() const = 0;

            /** Marks a component as dirty so it can be updated. */
            virtual void addDirtyComponent( Thread::Task taskId, Thread::UpdateState updateType,
                                            SmartPtr<IComponent> component ) = 0;

            /** To know if component is dirty. */
            virtual bool isComponentDirty( Thread::Task taskId, Thread::UpdateState updateType,
                                           SmartPtr<IComponent> component ) = 0;

            /** Register the component for updates. */
            virtual void registerComponentUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                                  SmartPtr<scene::IComponent> object ) = 0;

            /** Unregister the component for updates. */
            virtual void unregisterComponentUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                                    SmartPtr<scene::IComponent> object ) = 0;

            /** Unregister the component for updates. */
            virtual void unregisterAllComponent( SmartPtr<scene::IComponent> object ) = 0;

            /** Unregister the component for updates. */
            virtual void triggerEnter( SmartPtr<scene::IComponent> collision ) = 0;

            /** Unregister the component for updates. */
            virtual void triggerLeave( SmartPtr<scene::IComponent> collision ) = 0;

            /** Called when a component is loaded. */
            virtual void componentLoaded( SmartPtr<IComponent> component ) = 0;

            /** Gets the parent. */
            virtual SmartPtr<IActor> getParent() const = 0;

            /** Set the parent. Used internally */
            virtual void setParent( SmartPtr<IActor> parent ) = 0;

            /** Adds a child to this actor. */
            virtual void addChild( SmartPtr<IActor> child ) = 0;

            /** Removes a child of this actor. */
            virtual void removeChild( SmartPtr<IActor> child ) = 0;

            /** Removes all children. */
            virtual void removeChildren() = 0;

            /** Return the child with the name provided. Return null if a child is not found. */
            virtual SmartPtr<IActor> findChild( const String &name ) = 0;

            /** Gets children. */
            virtual SharedPtr<ConcurrentArray<SmartPtr<IActor>>> getChildrenPtr() const = 0;

            /** Gets children. */
            virtual Array<SmartPtr<IActor>> getAllChildren() const = 0;

            /** Used to know if the actor belongs to the client in network game. */
            virtual bool isMine() const = 0;

            /** Used to know if the actor belongs to the client in network game. */
            virtual void setMine( bool mine ) = 0;

            /** Marks the actor as static. Useful for optimizations, collision and lighting. */
            virtual bool isStatic() const = 0;

            /** Marks the actor as static. Useful for optimizations, collision and lighting. */
            virtual void setStatic( bool isstatic ) = 0;

            /** Used to know if the actor is enabled. */
            virtual bool isEnabledInScene() const = 0;

            /** Used to know if the actor is enabled. */
            virtual bool isEnabled() const = 0;

            /** Used to know if the actor is enabled. */
            virtual void setEnabled( bool enabled ) = 0;

            /** Used to know if the actor is visible in the editor. */
            virtual bool isVisible() const = 0;

            /** Used to know if the actor is visible in the editor. */
            virtual void setVisible( bool visible, bool cacade = true ) = 0;

            /** Used to know if the actor is dirty. */
            virtual bool isDirty() const = 0;

            /** Used to know if the actor is dirty. */
            virtual void setDirty( bool dirty ) = 0;

            /** Updates the actor and component transformations. */
            virtual void updateTransform() = 0;

            /** Gets the root of actor. The is the actor attached to the scene. */
            virtual SmartPtr<IActor> getSceneRoot() const = 0;

            /** Gets an integer value indicating the scene depth in the actors heir achy.
            @return An integer value with the scene depth of this actor.
            */
            virtual u32 getSceneLevel() const = 0;

            /** The actor transform. */
            virtual SmartPtr<ITransform> getTransform() const = 0;

            /** The actor transform. */
            virtual void setTransform( SmartPtr<ITransform> transform ) = 0;

            /** Gets the actor's state object.
            @return The state object. Can be null.
            */
            SmartPtr<IStateContext> getStateObject() const override = 0;

            /** Sets the actor's state object.
            @param stateObject The state object. Can be null.
            */
            void setStateObject( SmartPtr<IStateContext> stateObject ) override = 0;

            /** Sets the state of the actor.
            @param state The new state of the actor.
            */
            virtual void setState( State state ) = 0;

            /** Gets the state of the actor.
            @return The current state of the actor.
            */
            virtual State getState() const = 0;

            /** Sets the game state of the actor.
            @param state The new game state of the actor.
            */
            virtual void setGameState( GameState state ) = 0;

            /** Gets the game state of the actor.
            @return The current game state of the actor.
            */
            virtual GameState getGameState() const = 0;

            /** Gets the current flags of the actor.
            @return The current flags of the actor.
            */
            virtual u32 getFlags() const = 0;

            /** Sets the flags of the actor.
            @param flags The new flags of the actor.
            */
            virtual void setFlags( u32 flags ) = 0;

            /** Gets a specific flag of the actor.
            @param flag The flag to get.
            @return The value of the specified flag.
            */
            virtual bool getFlag( u32 flag ) const = 0;

            /** Sets a specific flag of the actor.
            @param flag The flag to set.
            @param value The new value of the flag.
            */
            virtual void setFlag( u32 flag, bool value ) = 0;

            /** Gets the new flags of the actor.
            @return The new flags of the actor.
            */
            virtual u32 getNewFlags() const = 0;

            /** Sets the new flags of the actor.
            @param flags The new flags of the actor.
            */
            virtual void setNewFlags( u32 flags ) = 0;

            /** Gets a specific new flag of the actor.
            @param flag The new flag to get.
            @return The value of the specified new flag.
            */
            virtual bool getNewFlag( u32 flag ) const = 0;

            /** Sets a specific new flag of the actor.
            @param flag The new flag to set.
            @param value The new value of the new flag.
            */
            virtual void setNewFlag( u32 flag, bool value ) = 0;

            /** Gets the scene this actor belongs to.
            @return A smart pointer to the scene this actor belongs to.
            */
            virtual SmartPtr<IScene> getScene() const = 0;

            /** Sets the scene this actor belongs to.
            @param scene A smart pointer to the scene this actor belongs to.
            */
            virtual void setScene( SmartPtr<IScene> scene ) = 0;

            /** Updates the necessary flags. */
            virtual void updateVisibility() = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // namespace scene
}  // namespace fb

#endif  // IActor_h__
