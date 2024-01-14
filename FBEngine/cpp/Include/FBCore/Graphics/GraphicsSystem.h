#ifndef CGraphicsSystem_h__
#define CGraphicsSystem_h__

#include <FBGraphics/FBGraphicsPrerequisites.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Graphics/SharedGraphicsObject.h>

namespace fb
{
    namespace render
    {
        /** @brief Graphics system implementation */
        class GraphicsSystem : public SharedGraphicsObject<IGraphicsSystem>
        {
        public:
            /** Constructor */
            GraphicsSystem();

            /** Destructor */
            ~GraphicsSystem() override;

            /** @copydoc IGraphicsSystem::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IGraphicsSystem::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IGraphicsSystem::lock */
            void lock() override;

            /** @copydoc IGraphicsSystem::unlock */
            void unlock() override;

            /** @copydoc IGraphicsSystem::getWindows */
            Array<SmartPtr<IWindow>> getWindows() const override;

            Array<SmartPtr<IGraphicsScene>> getSceneManagers() const override;

            SharedPtr<ConcurrentArray<SmartPtr<IGraphicsScene>>> getScenesPtr() const;

            void setScenesPtr( SharedPtr<ConcurrentArray<SmartPtr<IGraphicsScene>>> scenes );

            void addScenePtr( SmartPtr<IGraphicsScene> sceneManager );

            void removeScenePtr( SmartPtr<IGraphicsScene> sceneManager );

            SmartPtr<IGraphicsScene> getGraphicsScene( const String &name ) const override;

            SmartPtr<IGraphicsScene> getGraphicsSceneById( hash32 id ) const override;

            SmartPtr<IGraphicsScene> getGraphicsScene() const override;

            void setGraphicsScene( SmartPtr<IGraphicsScene> smgr );

            FB_CLASS_REGISTER_DECL;

        protected:
            mutable RecursiveMutex m_mutex;

            Array<SmartPtr<IWindow>> m_windows;

            AtomicSmartPtr<IGraphicsScene> m_defaultSceneManager;

            AtomicSharedPtr<ConcurrentArray<SmartPtr<IGraphicsScene>>> m_scenes;
        };
    }  // namespace render
}  // namespace fb

#endif  // CGraphicsSystem_h__
