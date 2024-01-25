#ifndef __FB_Scene_h__
#define __FB_Scene_h__

#include <FBCore/Interface/Scene/IScene.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Core/ConcurrentQueue.h>
#include <FBCore/Core/ConcurrentHashMap.h>
#include <FBCore/Core/FixedArray.h>
#include <FBCore/Resource/Resource.h>

namespace fb
{
    namespace scene
    {

        /** Scene implementation. */
        class Scene : public Resource<IScene>
        {
        public:
            /** Constructor. */
            Scene();

            /** Destructor. */
            ~Scene() override;

            /** @copydoc IScene::loadScene */
            void loadScene( const String &path ) override;

            /** @copydoc IScene::saveScene */
            void saveScene( const String &path ) override;

            /** @copydoc IScene::saveScene */
            void saveScene() override;

            /** @copydoc IScene::getLabel */
            String getLabel() const override;

            /** @copydoc IScene::setLabel */
            void setLabel( const String &label ) override;

            /** @copydoc IScene::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IScene::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IScene::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IScene::preUpdate */
            void preUpdate() override;

            /** @copydoc IScene::update */
            void update() override;

            /** @copydoc IScene::postUpdate */
            void postUpdate() override;

            /** @copydoc IScene::addActor */
            void addActor( SmartPtr<IActor> actor ) override;

            /** @copydoc IScene::removeActor */
            void removeActor( SmartPtr<IActor> actor ) override;

            /** @copydoc IScene::removeAllActors */
            void removeAllActors() override;

            /** @copydoc IScene::findActorById */
            SmartPtr<IActor> findActorById( int id ) const override;

            /** @copydoc IScene::getActors */
            Array<SmartPtr<IActor>> getActors() const override;

            /** @copydoc IScene::clear */
            void clear() override;

            /** @copydoc Scene::registerAllUpdates */
            void registerAllUpdates( SmartPtr<IActor> actor ) override;

            /** @copydoc Scene::registerUpdates */
            void registerUpdates( Thread::Task taskId, SmartPtr<IActor> actor ) override;

            /** @copydoc Scene::registerUpdate */
            void registerUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                 SmartPtr<IActor> object ) override;

            /** @copydoc Scene::unregisterUpdate */
            void unregisterUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                   SmartPtr<IActor> object ) override;

            /** @copydoc Scene::unregisterAll */
            void unregisterAll( SmartPtr<IActor> object ) override;

            /** @copydoc Scene::getRegisteredObjects */
            SharedPtr<ConcurrentArray<SmartPtr<IActor>>> getRegisteredObjects(
                Thread::UpdateState updateState, Thread::Task task ) const override;

            /** @copydoc Scene::setRegisteredObjects */
            void setRegisteredObjects( Thread::UpdateState updateState, Thread::Task task,
                                       SharedPtr<ConcurrentArray<SmartPtr<IActor>>> objects ) override;

            /** @copydoc Scene::isValid */
            bool isValid() const override;

            /** @copydoc Scene::toData */
            SmartPtr<ISharedObject> toData() const override;

            /** @copydoc Scene::fromData */
            void fromData( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Scene::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc Scene::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** Sets the component state. */
            void setState( State state ) override;

            /** Gets the component state. */
            State getState() const override;

            /** Sets the component state. */
            void setSceneLoadingState( SceneLoadingState state ) override;

            /** Gets the component state. */
            SceneLoadingState getSceneLoadingState() const override;

            /** Sorts the objects in memory. */
            void sortObjects();

            FB_CLASS_REGISTER_DECL;

        protected:
            /** Destroys the necessary objects when a scene is loaded. */
            void destroyOnLoad();

            /** Clears any cache data. */
            void clearCache();

            /** Sets up the cache data. */
            void setupCache();

            /** Gets the actor array pointer. */
            SharedPtr<Array<SmartPtr<IActor>>> getActorsPtr() const;

            /** Sets the actor array pointer. */
            void setActorsPtr( SharedPtr<Array<SmartPtr<IActor>>> ptr );

            // The scene mutex.
            mutable RecursiveMutex m_mutex;

            // The lighting settings.
            SmartPtr<SceneLightingDirector> m_sceneLightingDirector;

            // The scene state.
            State m_state = State::None;

            // The scene loading state.
            SceneLoadingState m_sceneLoadingState = SceneLoadingState::None;

            // The scene label.
            String m_label;

            // The queue to set actors to play mode.
            ConcurrentQueue<SmartPtr<IActor>> m_playQueue;

            // The queue to set actors to edit mode.
            ConcurrentQueue<SmartPtr<IActor>> m_editQueue;

            // An array of actors.
            AtomicSharedPtr<Array<SmartPtr<IActor>>> m_actors;

            // The scene update objects.
            Array<Array<SharedPtr<ConcurrentArray<SmartPtr<IActor>>>>> m_updateObjects;
        };
    }  // namespace scene
}  // namespace fb

#endif  // Scene_h__
