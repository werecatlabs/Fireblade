#ifndef __FB_CSceneManager_h__
#define __FB_CSceneManager_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Atomics/AtomicFloat.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/UnorderedMap.h>
#include <FBCore/Core/UtilityTypes.h>
#include <FBCore/State/States/TransformState.h>
#include "FBCore/Core/ConcurrentQueue.h"

namespace fb
{
    namespace scene
    {

        /** SceneManager implementation.
        @author	Zane Desir
        @version 1.0
        */
        class SceneManager : public ISceneManager
        {
        public:
            SceneManager();
            ~SceneManager() override;

            /** @copydoc ISceneManager::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISceneManager::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISceneManager::preUpdate */
            void preUpdate() override;

            /** @copydoc ISceneManager::update */
            void update() override;

            /** @copydoc ISceneManager::postUpdate */
            void postUpdate() override;

            void loadScene( const String &filePath ) override;

            /** @copydoc ISceneManager::getFsmManager */
            SmartPtr<IFSMManager> getFsmManager() const override;

            /** @copydoc ISceneManager::setFsmManager */
            void setFsmManager( SmartPtr<IFSMManager> fsmManager ) override;

            /** @copydoc ISceneManager::getFsmManager */
            SmartPtr<IFSMManager> getComponentFsmManager(u32 typeId);

            /** @copydoc ISceneManager::setFsmManager */
            void setComponentFsmManager( u32 typeId, SmartPtr<IFSMManager> fsmManager );

            /** @copydoc ISceneManager::getCurrentScene */
            SmartPtr<IScene> getCurrentScene() const override;

            /** @copydoc ISceneManager::setCurrentScene */
            void setCurrentScene( SmartPtr<IScene> scene ) override;

            /** @copydoc ISceneManager::getActors */
            Array<SmartPtr<IActor>> getActors() const override;

            /** @copydoc ISceneManager::createActor */
            SmartPtr<IActor> createActor() override;

            SmartPtr<IActor> createDummyActor();

            /** @copydoc ISceneManager::destroyActor */
            void destroyActor( SmartPtr<IActor> actor ) override;

            void destroyActors() override;

            void play() override;
            void edit() override;
            void stop() override;

            u32 addTransformComponent( SmartPtr<Transform> transformComponent );

            u32 addComponent( SmartPtr<IComponent> component ) override;
            u32 removeComponent( SmartPtr<IComponent> component ) override;

            void addSystem( u32 id, SmartPtr<ISystem> system );

            void removeSystem( u32 id );

            SmartPtr<IActor> getActor( u32 id ) const override;
            SmartPtr<IActor> getActorByName( const String &name ) const override;
            SmartPtr<IActor> getActorByFileId( const String &id ) const override;

            SmartPtr<IComponent> getComponent( u32 id ) const override;
            SmartPtr<IFSM> getFSM( u32 id ) const;

            SmartPtr<ITransform> createTransformComponent();

            void destroyTransformComponent( SmartPtr<ITransform> );

            SmartPtr<Transform> getTransform( u32 id ) const;

            u32 getCurrentFlags( u32 id ) const;
            void setCurrentFlags( u32 id, u32 flags );

            u32 getNewFlags( u32 id ) const;
            void setNewFlags( u32 id, u32 flags );

            u32 getPreviousFlags( u32 id ) const;
            void setPreviousFlags( u32 id, u32 flags );

            u32 getFlags( u32 id ) const;
            void setFlags( u32 id, u32 flags );

            void addDirty( SmartPtr<IActor> actor );
            void removeDirty( SmartPtr<IActor> actor );

            void addDirtyTransform( SmartPtr<IActor> actor );
            void addDirtyTransform( SmartPtr<ITransform> transform );

            void addDirtyComponent( SmartPtr<IComponent> component );
            void addDirtyComponentTransform( SmartPtr<IComponent> component );

            void queueProperties( SmartPtr<ISharedObject> object,
                                  SmartPtr<Properties> properties ) override;

            /** The task used to update. */
            Thread::Task getStateTask() const override;

            /** The current task used for scene. */
            Thread::Task getSceneTask() const override;

            Array<SmartPtr<IComponent>> getComponents() const override;
            Array<SmartPtr<IComponent>> getComponents( u32 type ) const override;

            Array<SmartPtr<ITransform>> &getTransforms();
            const Array<SmartPtr<ITransform>> &getTransforms() const;
            void setTransforms( Array<SmartPtr<ITransform>> transforms );

            Array<String> getComponentFactoryIgnoreList() const;

            void setComponentFactoryIgnoreList( const Array<String> &ignoreList );

            Map<String, String> getComponentFactoryMap() const;

            void setComponentFactoryMap( const Map<String, String> &map );

            String getComponentFactoryType( const String &type ) const;

            void addTransformState( u32 id, time_interval time, const Transform3<real_Num> &transform );

            bool getTransformState( u32 id, time_interval t, Transform3<real_Num> &transform );

            SharedPtr<Array<SmartPtr<IComponent>>> getRegisteredComponents( Thread::UpdateState state,
                                                                            Thread::Task task ) const;

            void registerComponentUpdate( Thread::Task task, Thread::UpdateState state,
                                          SmartPtr<IComponent> component );

            void unregisterComponentUpdate( Thread::Task task, Thread::UpdateState state,
                                            SmartPtr<IComponent> component );

            void unregisterAllComponent( SmartPtr<IComponent> component );

            s32 getNumActors() const;

            FB_CLASS_REGISTER_DECL;

        protected:
            void updateTransformStatesSize();

            s32 getLoadPriority( SmartPtr<ISharedObject> obj );

            mutable SpinRWMutex m_transformMutex;
            atomic_f64 m_nextTransformSizeTime;
            Array<Array<time_interval>> m_transformTimes;
            Array<Array<Transform3<real_Num>>> m_transformStates;

            mutable RecursiveMutex m_updateMutex;
            Array<Array<bool>> m_updateObjects;
            Array<Array<SharedPtr<Array<SmartPtr<IComponent>>>>> m_updateComponents;

            SmartPtr<IFSMManager> m_fsmManager;
            Map<u32, SmartPtr<IFSMManager>> m_componentFsmManagers;

            SmartPtr<IScene> m_scene;

            ConcurrentQueue<Pair<SmartPtr<ISharedObject>, SmartPtr<Properties>>> m_queueProperties;

            ConcurrentQueue<SmartPtr<IActor>> m_dirtyActors;
            ConcurrentQueue<SmartPtr<IComponent>> m_dirtyComponents;
            ConcurrentQueue<SmartPtr<IComponent>> m_dirtyComponentTransforms;
            ConcurrentQueue<SmartPtr<IActor>> m_dirtyTransforms;
            ConcurrentQueue<SmartPtr<ITransform>> m_dirtyActorTransforms;

            UnorderedMap<u32, Array<SmartPtr<IComponent>>> m_components;
            UnorderedMap<u32, SmartPtr<ISystem>> m_systems;

            Array<SmartPtr<ITransform>> m_transforms;

            Array<SmartPtr<IActor>> m_actors;
            Array<LoadingState> m_actorLoadingStates;

            Array<SmartPtr<IFSM>> m_fsms;
            Array<SmartPtr<IFSMListener>> m_fsmListeners;

            Array<SmartPtr<IFSM>> m_gameFSMs;
            Array<SmartPtr<IFSMListener>> m_gameFsmListeners;

            /// The state object.
            Array<IStateContext *> m_stateObjects;

            /// The state listener.
            Array<IStateListener *> m_stateListeners;

            /// The scene this actor belongs to.
            Array<IScene *> m_scenes;

            Array<u32> m_actorPreviousFlags;
            Array<u32> m_actorCurrentFlags;
            Array<u32> m_actorNewFlags;

            atomic_s32 m_numActors = 0;

            Array<String> m_componentFactoryIgnoreList;
            Map<String, String> m_componentFactoryMap;

            mutable RecursiveMutex m_mutex;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // CSceneManager_h__
