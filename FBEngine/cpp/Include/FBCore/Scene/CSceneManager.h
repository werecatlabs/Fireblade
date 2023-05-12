#ifndef __FB_CSceneManager_h__
#define __FB_CSceneManager_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Base/UnorderedMap.h>
#include <FBCore/Base/UtilityTypes.h>
#include <FBCore/State/States/TransformState.h>

namespace fb
{
    namespace scene
    {

        /** SceneManager implementation.
        @author	Zane Desir
        @version 1.0
        */
        class CSceneManager final : public CSharedObject<ISceneManager>
        {
        public:
            CSceneManager();
            ~CSceneManager() override;

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

            void loadScene( const String &filePath );

            /** @copydoc ISceneManager::getFsmManager */
            SmartPtr<IFSMManager> getFsmManager() const override;

            /** @copydoc ISceneManager::setFsmManager */
            void setFsmManager( SmartPtr<IFSMManager> fsmManager ) override;

            /** @copydoc ISceneManager::getCurrentScene */
            SmartPtr<IScene> getCurrentScene() const override;

            /** @copydoc ISceneManager::setCurrentScene */
            void setCurrentScene( SmartPtr<IScene> scene ) override;

            /** @copydoc ISceneManager::getActors */
            Array<SmartPtr<scene::IActor>> getActors() const;

            /** @copydoc ISceneManager::createActor */
            SmartPtr<scene::IActor> createActor() override;

            SmartPtr<scene::IActor> createDummyActor();

            /** @copydoc ISceneManager::destroyActor */
            void destroyActor( SmartPtr<scene::IActor> actor ) override;

            void destroyActors() override;

            void play() override;
            void edit() override;
            void stop() override;

            u32 addTransformComponent( SmartPtr<Transform> transformComponent );

            u32 addComponent( SmartPtr<IComponent> component ) override;
            u32 removeComponent( SmartPtr<IComponent> component ) override;

            SmartPtr<scene::IActor> getActor( u32 id ) const override;
            SmartPtr<IActor> getActorByName( const String &name ) const;

            SmartPtr<scene::IComponent> getComponent( u32 id ) const override;
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

            void addDirty( SmartPtr<scene::IActor> actor );
            void removeDirty( SmartPtr<scene::IActor> actor );

            void addDirtyTransform( SmartPtr<scene::IActor> actor );
            void addDirtyTransform( SmartPtr<ITransform> transform );

            void addDirtyComponent( SmartPtr<IComponent> component );
            void addDirtyComponentTransform( SmartPtr<IComponent> component );

            void queueProperties( SmartPtr<ISharedObject> object,
                                  SmartPtr<Properties> properties ) override;

            /** The task used to update. */
            Thread::Task getStateTask() const override;

            /** The current task used for scene. */
            Thread::Task getSceneTask() const override;

            Array<SmartPtr<scene::IComponent>> getComponents() const;
            Array<SmartPtr<IComponent>> getComponents( u32 type ) const;

            FB_CLASS_REGISTER_DECL;

        protected:
            s32 getLoadPriority( SmartPtr<ISharedObject> obj );

            SmartPtr<IFSMManager> m_fsmManager;

            SmartPtr<scene::IScene> m_scene;

            ConcurrentQueue<Pair<SmartPtr<ISharedObject>, SmartPtr<Properties>>> m_queueProperties;

            ConcurrentQueue<SmartPtr<scene::IActor>> m_dirtyActors;
            ConcurrentQueue<SmartPtr<scene::IComponent>> m_dirtyComponents;
            ConcurrentQueue<SmartPtr<scene::IComponent>> m_dirtyComponentTransforms;
            ConcurrentQueue<SmartPtr<scene::IActor>> m_dirtyTransforms;
            ConcurrentQueue<SmartPtr<ITransform>> m_dirtyActorTransforms;

            UnorderedMap<u32, Array<SmartPtr<scene::IComponent>>> m_components;

            Array<SmartPtr<Transform>> m_transforms;

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

            atomic_u32 m_numActors = 0;
        };

    }  // namespace scene
}  // end namespace fb

#endif  // CSceneManager_h__
