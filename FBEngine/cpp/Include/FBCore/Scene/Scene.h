#ifndef __FB_Scene_h__
#define __FB_Scene_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/IScene.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Base/LogManager.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Base/ConcurrentArray.h>
#include <FBCore/Base/ConcurrentQueue.h>
#include <FBCore/Base/ConcurrentHashMap.h>
#include <FBCore/Base/FixedArray.h>

namespace fb
{
    namespace scene
    {

        /** Scene implementation. */
        class Scene : public CSharedObject<IScene>
        {
        public:
            Scene();
            ~Scene() override;

            /** @copydoc IScene::loadScene */
            void loadScene( const String &path ) override;

            /** @copydoc IScene::saveScene */
            void saveScene( const String &path ) override;

            /** @copydoc IScene::saveScene */
            void saveScene() override;

            /** The name of the scene. */
            String getName() const override;

            /** The name of the scene. */
            void setName( const String &name ) override;

            /** @copydoc IScene::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IScene::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IScene::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void clearCache();

            /** @copydoc IScene::preUpdate */
            void preUpdate() override;

            /** @copydoc IScene::update */
            void update() override;

            /** @copydoc IScene::postUpdate */
            void postUpdate() override;

            ConcurrentArray<SmartPtr<scene::IActor>> getUpdateArray( Thread::UpdateState updateState,
                                                                     int task );

            void addActor( SmartPtr<scene::IActor> actor ) override;
            void removeActor( SmartPtr<scene::IActor> actor ) override;
            void removeAllActors() override;

            SmartPtr<scene::IActor> findActorById( int id ) const override;
            Array<SmartPtr<scene::IActor>> getActors() const override;

            void clear() override;

            void destroyOnLoad();

            void registerAllUpdates( SmartPtr<scene::IActor> actor ) override;

            void registerUpdates( Thread::Task taskId, SmartPtr<scene::IActor> actor ) override;
            void registerUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                 SmartPtr<scene::IActor> object ) override;

            void sortObjects();

            void unregisterUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                   SmartPtr<scene::IActor> object ) override;
            void unregisterAll( SmartPtr<scene::IActor> object ) override;
            void refreshRegistration( SmartPtr<scene::IActor> object );

            boost::shared_ptr<ConcurrentArray<SmartPtr<scene::IActor>>> getRegisteredObjects(
                Thread::UpdateState updateState, Thread::Task task ) const;
            void setRegisteredObjects(
                Thread::UpdateState updateState, Thread::Task task,
                boost::shared_ptr<ConcurrentArray<SmartPtr<scene::IActor>>> objects );

            bool isValid() const override;

            String getFilePath() const override;
            void setFilePath( const String &val ) override;

            void play();
            void edit();
            void stop();

            /** Sets the component state. */
            void setState( State state ) override;

            /** Gets the component state. */
            State getState() const override;

            /** Sets the component state. */
            void setSceneLoadingState( SceneLoadingState state ) override;

            /** Gets the component state. */
            SceneLoadingState getSceneLoadingState() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            void setupCache();

            SharedPtr<Array<SmartPtr<scene::IActor>>> getActorsPtr() const;

            void setActorsPtr( SharedPtr<Array<SmartPtr<scene::IActor>>> ptr );

            State m_state;
            SceneLoadingState m_sceneLoadingState;

            Array<u32> m_actorFlags;

            String m_filePath;
            String m_name;

            ConcurrentQueue<SmartPtr<scene::IActor>> m_playQueue;
            ConcurrentQueue<SmartPtr<scene::IActor>> m_editQueue;

            AtomicSharedPtr<Array<SmartPtr<scene::IActor>>> m_actors;
            FixedArray<FixedArray<SharedPtr<ConcurrentArray<SmartPtr<scene::IActor>>>,
                                  static_cast<int>( Thread::Task::Count )>,
                       static_cast<int>( Thread::UpdateState::Count )>
                m_updateObjects;
        };

    }  // namespace scene
}  // namespace fb

#endif  // Scene_h__
