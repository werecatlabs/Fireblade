#ifndef _ISound2D_H
#define _ISound2D_H

#include <FBCore/Interface/Sound/ISound.h>

namespace fb
{
    /**
     * @brief An interface for a 2D sound.
     */
    class ISound2 : public ISound
    {
    public:
        /**
         * @brief Destructor for ISound2.
         */
        ~ISound2() override = default;

        /**
         * @brief Sets the pan of the sound.
         * @param pan The new pan of the sound.
         */
        virtual void setPan( f32 pan ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif
