#ifndef CRenderTexture_h__
#define CRenderTexture_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBGraphicsOgre/Wrapper/CRenderTarget.h>
#include <FBCore/Interface/Graphics/IRenderTexture.h>
#include <sstream>

namespace fb
{
    namespace render
    {

        /** Implements IRenderTexture interface for Ogre. */
        class CRenderTexture : public CRenderTarget<IRenderTexture>
        {
        public:
            /** Constructor. */
            CRenderTexture() = default;

            /** Destructor. */
            ~CRenderTexture() override = default;

            /** @copydoc IRenderTexture::update */
            void update() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;
        };

    }  // namespace render
}  // namespace fb

#endif  // CRenderTexture_h__
