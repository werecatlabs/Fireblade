#ifndef CGraphicsSystem_h__
#define CGraphicsSystem_h__

#include <FBGraphics/FBGraphicsPrerequisites.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBGraphics/Wrapper/CSharedGraphicsObject.h>

namespace fb
{
    namespace render
    {

        class CGraphicsSystem : public CSharedGraphicsObject<IGraphicsSystem>
        {
        public:
            CGraphicsSystem() = default;
            ~CGraphicsSystem() = default;

            /** @copydoc IGraphicsSystem::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IGraphicsSystem::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IGraphicsSystem::getWindows */
            Array<SmartPtr<IWindow>> getWindows() const override;

        protected:
            SmartPtr<GraphicsSystemState> m_state;
        };

    }  // namespace render
}  // namespace fb

#endif  // CGraphicsSystem_h__
