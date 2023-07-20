#ifndef IVideoTexture_h__
#define IVideoTexture_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{

    class IVideoTexture : public render::ITexture
    {
    public:
        ~IVideoTexture() override = default;

        virtual void initialise( const String &name, const Vector2I &size ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IVideoTexture_h__
