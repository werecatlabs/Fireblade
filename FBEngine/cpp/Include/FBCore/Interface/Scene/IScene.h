#ifndef IScene_h__
#define IScene_h__

#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Thread/ThreadTypes.h>
#include <FBCore/Interface/Scene/IActor.h>

namespace fb
{
    namespace scene
    {
        /**
         * @file IScene.h
         * @brief Interface for a scene class.
         * @author Zane Desir
         * @version 1.0
         */
        class IScene : public IResource
        {
        public:
            /**
             * @enum State
             * @brief An enumeration representing the state of the scene.
             */
            enum class State
            {
                ///< The scene is in an undefined state.
                None,

                ///< The scene is in edit mode.
                Edit,

                ///< The scene is in play mode.
                Play,

                ///< The scene is resetting to its initial state.
                Reset,

                ///< The number of states in the enumeration.
                Count
            };

            /**
             * @enum SceneLoadingState
             * @brief An enumeration representing the loading state of the scene.
             */
            enum class SceneLoadingState
            {
                ///< None state.
                None,

                ///< The scene is loaded.
                Loaded,

                ///< The scene is unloaded.
                Unloaded,

                ///< The number of states in the enumeration.
                Count
            };

            /**
             * @brief Virtual destructor.
             */
            ~IScene() override = default;

            /**
             * @brief Loads a scene from the specified file path.
             * @param path The path to the file containing the scene data.
             */
            virtual void loadScene( const String &path ) = 0;

            /**
             * @brief Saves the scene to the specified file path.
             * @param path The path to save the scene data to.
             */
            virtual void saveScene( const String &path ) = 0;

            /**
             * @brief Saves the scene to its current file path.
             */
            virtual void saveScene() = 0;

            /**
             * @brief Clears the scene, removing all actors.
             */
            virtual void clear() = 0;

            /**
             * @brief Returns the name of the scene.
             * @return The name of the scene.
             */
            virtual String getName() const = 0;

            /**
             * @brief Sets the name of the scene.
             * @param name The new name for the scene.
             */
            virtual void setName( const String &name ) = 0;

            /**
             * @brief Adds an actor to the scene.
             * @param actor A smart pointer to the actor to add.
             */
            virtual void addActor( SmartPtr<IActor> actor ) = 0;

            /**
             * @brief Removes an actor from the scene.
             * @param actor A smart pointer to the actor to remove.
             */
            virtual void removeActor( SmartPtr<IActor> actor ) = 0;

            /**
             * @brief Removes all actors from the scene.
             */
            virtual void removeAllActors() = 0;

            /**
             * @brief Finds an actor in the scene by ID.
             * @param id The ID of the actor to find.
             * @return A smart pointer to the found actor, or null if no actor was found.
             */
            virtual SmartPtr<IActor> findActorById( s32 id ) const = 0;

            /**
             * @brief Returns an array of smart pointers to all actors in the scene.
             * @return An array of smart pointers to all actors in the scene.
             */
            virtual Array<SmartPtr<IActor>> getActors() const = 0;

            /** Register's the actor on a specific task.
            @param taskId The id of the task the actor is registered for updates.
            @param actor The actor object.
            */
            virtual void registerUpdates( Thread::Task taskId, SmartPtr<IActor> actor ) = 0;

            /** Register's the actor on a specific task.
            @param taskId The id of the task the actor is registered for updates.
            @param updateType The update type used to update the actor object.
            @param actor The actor object.
            */
            virtual void registerUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                         SmartPtr<IActor> actor ) = 0;

            /** Register the actor for updates on tasks.
            @param actor The actor object.
            */
            virtual void registerAllUpdates( SmartPtr<IActor> actor ) = 0;

            /** Unregister the actor on a specific task.
            @param taskId The id of the task the actor is registered for updates.
            @param updateType The update type used to update the actor object.
            @param actor The actor object.
            */
            virtual void unregisterUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                           SmartPtr<IActor> object ) = 0;

            /** Unregister the actor from all tasks.
            @param actor The actor object.
            */
            virtual void unregisterAll( SmartPtr<IActor> actor ) = 0;

            /** Gets the file path. */
            virtual String getFilePath() const = 0;

            /** Sets the file path. */
            virtual void setFilePath( const String &val ) = 0;

            /** Sets the component state. */
            virtual void setState( State state ) = 0;

            /** Gets the component state. */
            virtual State getState() const = 0;

            /** Sets the component state. */
            virtual void setSceneLoadingState( SceneLoadingState state ) = 0;

            /** Gets the component state. */
            virtual SceneLoadingState getSceneLoadingState() const = 0;

            /**
             * @brief Enters play mode for the scene.
             */
            virtual void play() = 0;

            /**
             * @brief Enters edit mode for the scene.
             */
            virtual void edit() = 0;

            /**
             * @brief Stops the scene.
             */
            virtual void stop() = 0;

            /**
             * @brief Gets a smart pointer to the first component of the specified type attached to any actor in the scene.
             * @tparam T The type of component to get.
             * @return A smart pointer to the first component of the specified type attached to any actor in the scene, or null if no such component was found.
             */
            template <class T>
            SmartPtr<T> getComponent() const;

            /**
             * @brief Gets an array of smart pointers to all components of the specified type attached to any actor in the scene.
             * @tparam T The type of component to get.
             * @return An array of smart pointers to all components of the specified type attached to any actor in the scene.
             */
            template <class T>
            Array<SmartPtr<T>> getComponents() const;

            FB_CLASS_REGISTER_DECL;
        };

        template <class T>
        SmartPtr<T> IScene::getComponent() const
        {
            FB_ASSERT( isValid() );

            const auto actors = getActors();
            for( auto actor : actors )
            {
                if( actor )
                {
                    if( auto actorComponent = actor->getComponent<T>() )
                    {
                        return actorComponent;
                    }
                }
            }

            return nullptr;
        }

        template <class T>
        Array<SmartPtr<T>> IScene::getComponents() const
        {
            FB_ASSERT( isValid() );

            Array<SmartPtr<T>> components;
            components.reserve( 10 );

            const auto actors = getActors();
            for( auto actor : actors )
            {
                if( actor )
                {
                    auto actorComponents = actor->getComponentsByType<T>();
                    if( !actorComponents.empty() )
                    {
                        components.insert( components.end(), actorComponents.begin(),
                                           actorComponents.end() );
                    }
                }
            }

            return components;
        }
    }  // namespace scene
}  // namespace fb

#endif  // IScene_h__
