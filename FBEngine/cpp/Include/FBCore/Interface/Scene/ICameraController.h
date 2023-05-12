#ifndef ICameraController_h__
#define ICameraController_h__

#include <FBCore/Interface/Scene/IComponent.h>

namespace fb
{
    namespace scene
    {

        class ICameraController : public IComponent
        {
        public:
            /** Virtual destructor. */
            ~ICameraController() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace scene
}  // namespace fb

#endif  // ICameraController_h__
