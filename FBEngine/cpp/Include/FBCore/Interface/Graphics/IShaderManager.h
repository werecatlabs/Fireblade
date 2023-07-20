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
        };

    }  // end namespace render
}  // end namespace fb

#endif  // IShaderManager_h__
