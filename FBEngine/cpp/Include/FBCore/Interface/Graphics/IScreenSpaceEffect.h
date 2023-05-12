#ifndef IScreenSpaceEffect_h__
#define IScreenSpaceEffect_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {

        /**
         * Interface for a screen-space effect.
         */
        class IScreenSpaceEffect : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~IScreenSpaceEffect() override = default;

            /**
             * Gets the renderer for the screen-space effect.
             * @return A smart pointer to the screen-space effect renderer.
             */
            virtual SmartPtr<IScreenSpaceEffectRenderer> getRenderer() const = 0;

            /**
             * Sets the renderer for the screen-space effect.
             * @param renderer A smart pointer to the screen-space effect renderer to set.
             */
            virtual void setRenderer( SmartPtr<IScreenSpaceEffectRenderer> renderer ) = 0;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // IScreenSpaceEffect_h__
