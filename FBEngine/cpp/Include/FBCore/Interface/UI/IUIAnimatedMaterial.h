#ifndef IAnimatedMaterial_h__
#define IAnimatedMaterial_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        class IUIAnimatedMaterial : public IUIElement
        {
        public:
            ~IUIAnimatedMaterial() override = default;

            /** Sets the material used. */
            virtual void setMaterialName( const String &materialName ) = 0;

            /** Gets the name of the material used. */
            virtual String getMaterialName() const = 0;

            /** Plays the animation. */
            virtual void play() = 0;

            /** Pauses the animation. */
            virtual void pause() = 0;

            /** Stops the animation. */
            virtual void stop() = 0;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // IAnimatedMaterial_h__
