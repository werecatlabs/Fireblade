#ifndef __FB_Scene_h__
#define __FB_Scene_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/IScene.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Interface/Scene/IActor.h>
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

            /** @copydoc IScene::preUpdate */
            void preUpdate() override;

            /** @copydoc IScene::update */
            void update() override;

            /** @copydoc IScene::postUpdate */
            void postUpdate() override;

            SharedPtr<ConcurrentArray<SmartPtr<IActor>>> getUpdateArray( Thread::UpdateState updateState,
                s32 task );

            void addActor( SmartPtr<IActor> actor ) override;
            void removeActor( SmartPtr<IActor> actor ) override;
            void removeAllActors() override;

            SmartPtr<IActor> findActorById( int id ) const override;
            Array<SmartPtr<IActor>> getActors() const override;

            void clear() override;

            void registerAllUpdates( SmartPtr<IActor> actor ) override;

            void registerUpdates( Thread::Task taskId, SmartPtr<IActor> actor ) override;
            void registerUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                 SmartPtr<IActor> object ) override;

            void unregisterUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                   SmartPtr<IActor> object ) override;
            void unregisterAll( SmartPtr<IActor> object ) override;
            void refreshRegistration( SmartPtr<IActor> object );

            SharedPtr<ConcurrentArray<SmartPtr<IActor>>> getRegisteredObjects(
                Thread::UpdateState updateState, Thread::Task task ) const;
            void setRegisteredObjects( Thread::UpdateState updateState, Thread::Task task,
                                       SharedPtr<ConcurrentArray<SmartPtr<IActor>>> objects );

            void sortObjects();

            bool isValid() const override;

            SmartPtr<ISharedObject> toData() const override;

            void fromData( SmartPtr<ISharedObject> data ) override;

            String getFilePath() const override;
            void setFilePath( const String &filePath ) override;

            void play() override;
            void edit() override;
            void stop() override;

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
            void destroyOnLoad();

            void clearCache();
            void setupCache();

            SharedPtr<Array<SmartPtr<IActor>>> getActorsPtr() const;

            void setActorsPtr( SharedPtr<Array<SmartPtr<IActor>>> ptr );

            State m_state;
            SceneLoadingState m_sceneLoadingState;

            Array<u32> m_actorFlags;

            String m_filePath;
            String m_name;

            ConcurrentQueue<SmartPtr<IActor>> m_playQueue;
            ConcurrentQueue<SmartPtr<IActor>> m_editQueue;

            AtomicSharedPtr<Array<SmartPtr<IActor>>> m_actors;
            FixedArray<FixedArray<SharedPtr<ConcurrentArray<SmartPtr<IActor>>>,
                                  static_cast<s32>(Thread::Task::Count)>,
                       static_cast<s32>(Thread::UpdateState::Count)>
            m_updateObjects;
        };
    } // namespace scene
}     // namespace fb

#endif  // Scene_h__
