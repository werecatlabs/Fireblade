#ifndef _CTextureOgreApple_H
#define _CTextureOgreApple_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBGraphicsOgreNext/Wrapper/CTextureOgreNext.h>

namespace fb
{
    namespace render
    {

        class CTextureOgreApple : public CTextureOgreNext
        {
        public:
            void getTextureFinal(void **ppTexture) const;
        };

    } // end namespace render
}     // end namespace fb

#endif
