#ifndef ___ISceneManager_h__
#define ___ISceneManager_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/Scene/IScene.h>

namespace fb
{
    namespace scene
    {

        /**
         * Interface for a scene manager object.
         * @author	Zane Desir
         * @version 1.0
         */
        class ISceneManager : public ISharedObject
        {
        public:
            static const hash_type sceneLoadedHash;

            /** Virtual destructor. */
            ~ISceneManager() override = default;

            /**
             * Loads a scene from a file.
             * @param filePath The file path to the scene.
             */
            virtual void loadScene( const String &filePath ) = 0;

            /**
             * Gets the FSM manager.
             * @return The FSM manager.
             */
            virtual SmartPtr<IFSMManager> getFsmManager() const = 0;

            /**
             * Sets the FSM manager.
             * @param fsmManager The FSM manager.
             */
            virtual void setFsmManager( SmartPtr<IFSMManager> fsmManager ) = 0;

            /**
             * Gets the current scene.
             * @return The current scene object. Can be null.
             */
            virtual SmartPtr<IScene> getCurrentScene() const = 0;

            /**
             * Sets the current scene.
             * @param scene The current scene object.
             */
            virtual void setCurrentScene( SmartPtr<IScene> scene ) = 0;

            /**
             * Gets the actors in the scene.
             * @return The actors in the scene.
             */
            virtual Array<SmartPtr<IActor>> getActors() const = 0;

            /**
             * Creates a new actor.
             * @return The newly created actor.
             */
            virtual SmartPtr<IActor> createActor() = 0;

            /**
             * Destroys an actor.
             * @param actor The actor to destroy.
             */
            virtual void destroyActor( SmartPtr<IActor> actor ) = 0;

            /**
             * Destroys all actors in the scene.
             */
            virtual void destroyActors() = 0;

            /**
             * Triggers the play event.
             */
            virtual void play() = 0;

            /**
             * Triggers the edit event.
             */
            virtual void edit() = 0;

            /**
             * Triggers the stop event.
             */
            virtual void stop() = 0;

            /**
             * Adds a component to the scene.
             * @param component The component to add.
             * @return The unique identifier for the component.
             */
            virtual u32 addComponent( SmartPtr<IComponent> component ) = 0;

            /**
             * Removes a component from the scene.
             * @param component The component to remove.
             * @return The unique identifier for the component.
             */
            virtual u32 removeComponent( SmartPtr<IComponent> component ) = 0;

            /**
             * Gets all components in the scene.
             * @return The components in the scene.
             */
            virtual Array<SmartPtr<IComponent>> getComponents() const = 0;

            virtual void addSystem( u32 id, SmartPtr<ISystem> system ) = 0;

            virtual void removeSystem( u32 id ) = 0;

            /**
             * Gets the actor with the specified ID.
             * @param id The ID of the actor to retrieve.
             * @return The actor with the specified ID. Can be null.
             */
            virtual SmartPtr<IActor> getActor( u32 id ) const = 0;

            /**
             * Gets the actor with the specified name.
             * @param name The name of the actor to retrieve.
             * @return The actor with the specified name. Can be null.
             */
            virtual SmartPtr<IActor> getActorByName( const String &name ) const = 0;

            virtual SmartPtr<IActor> getActorByFileId( const String &id ) const = 0;

            /**
             * Gets the component with the specified ID.
             * @param id The ID of the component to retrieve.
             * @return The component with the specified ID. Can be null.
             */
            virtual SmartPtr<IComponent> getComponent( u32 id ) const = 0;

            /**
             * Queues properties to be set on an object.
             * @param object The object to set the properties on.
             * @param properties The properties object.
             */
            virtual void queueProperties( SmartPtr<ISharedObject> object,
                                          SmartPtr<Properties> properties ) = 0;

            /**
             * Returns the task used to update the scene.
             * @return A Thread::Task object representing the task used to update the scene.
             */
            virtual Thread::Task getStateTask() const = 0;

            /**
             * Returns the current task used for the scene.
             * @return A Thread::Task object representing the current task used for the scene.
             */
            virtual Thread::Task getSceneTask() const = 0;

            /**
             * Returns an array of all components of the specified type attached to the actors in the current scene.
             * @param type The type of component to search for.
             * @return An array of SmartPtrs to components of the specified type.
             */
            virtual Array<SmartPtr<IComponent>> getComponents( u32 type ) const = 0;
                        
            /** Gets the list of types to ignore. */
            virtual Array<String> getComponentFactoryIgnoreList() const = 0;

            /** Sets the list of types to ignore. */
            virtual void setComponentFactoryIgnoreList( const Array<String> &ignoreList ) = 0;

            /** Gets the component factory to map components to a factory. */
            virtual Map<String, String> getComponentFactoryMap() const = 0;

            /** Sets the component factory to map components to a factory. */
            virtual void setComponentFactoryMap( const Map<String, String> &map ) = 0;

            /** Gets a factory to build a component. */
            virtual String getComponentFactoryType( const String &type ) const = 0;

            /**
             * Returns an array of all components of the specified type attached to the actors in the current scene.
             * @tparam T The type of component to search for.
             * @param type The type of component to search for.
             * @return An array of SmartPtrs to components of the specified type.
             */
            template <class T>
            Array<SmartPtr<T>> getComponentsByType( u32 type ) const;

            /**
             * Returns the first object of the specified type found in the current scene.
             * @tparam T The type of object to search for.
             * @return A SmartPtr to the first object of the specified type found in the current scene, or a null pointer if none found.
             */
            template <class T>
            SmartPtr<T> getObjectByType() const;

            FB_CLASS_REGISTER_DECL;
        };

        template <class T>
        Array<SmartPtr<T>> ISceneManager::getComponentsByType( u32 type ) const
        {
            auto typeInfo = T::typeInfo();
            auto components = getComponents( typeInfo );
            return Array<SmartPtr<T>>( components.begin(), components.end() );
        }

        template <class T>
        SmartPtr<T> ISceneManager::getObjectByType() const
        {
            auto scene = getCurrentScene();
            auto actors = scene->getActors();
            for( auto actor : actors )
            {
                auto component = actor->getComponent<T>();
                if( component )
                {
                    return component;
                }

                auto children = actor->getAllChildren();
                for( auto child : children )
                {
                    component = child->getComponent<T>();
                    if( component )
                    {
                        return component;
                    }
                }
            }

            return nullptr;
        }

    }  // namespace scene
}  // namespace fb

#endif  // ISceneManager_h__
