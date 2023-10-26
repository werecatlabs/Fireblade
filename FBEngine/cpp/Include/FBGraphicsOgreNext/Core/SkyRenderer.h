#ifndef SkyRenderer_h__
#define SkyRenderer_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>

namespace fb
{
    namespace render
    {
        //--------------------------------------------
        class SkyRenderer : public ISharedObject
        {
        public:
            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;
        };

    }  // namespace render
}  // namespace fb

#endif  // SkyRenderer_h__
