#ifndef IShader_h__
#define IShader_h__

#include <FBCore/Interface/Resource/IResource.h>

namespace fb
{
    namespace render
    {
        
        class IMaterialShader : public IResource
        {
        public:
            /** Virtual destructor. */
            ~IMaterialShader() override = default;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // IShader_h__
