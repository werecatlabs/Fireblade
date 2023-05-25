#ifndef CGraphicsSystem_h__
#define CGraphicsSystem_h__

#include <FBGraphics/FBGraphicsPrerequisites.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBGraphics/Wrapper/CSharedGraphicsObject.h>

namespace fb
{
    namespace render
    {

        /** @brief Graphics system implementation */
        class CGraphicsSystem : public CSharedGraphicsObject<IGraphicsSystem>
        {
        public:
            /** Constructor */
            CGraphicsSystem() = default;

            /** Destructor */
            ~CGraphicsSystem() override = default;

            /** @copydoc IGraphicsSystem::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IGraphicsSystem::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IGraphicsSystem::lock */
            void lock();

            /** @copydoc IGraphicsSystem::unlock */
            void unlock();

            /** @copydoc IGraphicsSystem::getWindows */
            Array<SmartPtr<IWindow>> getWindows() const override;

        protected:
            mutable RecursiveMutex m_mutex;

            Array<SmartPtr<IWindow>> m_windows;
        };
    }  // namespace render
}  // namespace fb

#endif  // CGraphicsSystem_h__
