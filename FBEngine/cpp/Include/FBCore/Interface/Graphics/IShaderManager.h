#ifndef IShaderManager_h__
#define IShaderManager_h__

#include <FBCore/Interface/Resource/IResourceManager.h>

namespace fb
{
    namespace render
    {

        class IShaderManager : public IResourceManager
        {
        public:
            /** Virtual destructor. */
            ~IShaderManager() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // IShaderManager_h__
