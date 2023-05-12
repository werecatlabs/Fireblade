#ifndef IComponentScript_h__
#define IComponentScript_h__

#include <FBCore/Interface/Resource/IResource.h>

namespace fb
{
    namespace scene
    {

        /** Interface for a component script class. */
        class IComponentScript : public IResource
        {
        public:
            /** Virtual destructor. */
            ~IComponentScript() override = default;

             FB_CLASS_REGISTER_DECL;
        };

    }  // namespace scene
}  // namespace fb

#endif  // IComponentScript_h__
