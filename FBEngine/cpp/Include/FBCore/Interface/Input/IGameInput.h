#ifndef IGameInput_h__
#define IGameInput_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * @brief Interface for mapping device input to game actions.
     */
    class IGameInput : public ISharedObject
    {
    public:
        /** The constant value for an unassigned game input. */
        static const u32 UNASSIGNED;

        /**
         * @brief Virtual destructor.
         */
        ~IGameInput() override = default;

        /**
         * @brief Sets the name of this game input device.
         *
         * @param id The hash32 id for this game input device.
         */
        virtual void setId( hash32 id ) = 0;

        /**
         * @brief Gets the id of this game input device.
         *
         * @return The hash32 id of this game input device.
         */
        virtual hash32 getId() const = 0;

        /**
         * @brief Checks if this game input device is assigned to a hardware input device.
         *
         * @return A boolean value indicating if this game input device is assigned to a hardware input device.
         */
        virtual bool isAssigned() const = 0;

        /**
         * @brief Gets the game input map.
         *
         * @return The game input map.
         */
        virtual SmartPtr<IGameInputMap> getGameInputMap() const = 0;

        /**
         * @brief Sets the player index this input belongs to.
         *
         * @param playerIndex The index of the player this input belongs to.
         */
        virtual void setPlayerIndex( u32 playerIndex ) = 0;

        /**
         * @brief Gets the player index this input belongs to.
         *
         * @return The index of the player this input belongs to.
         */
        virtual u32 getPlayerIndex() const = 0;

        /**
         * @brief Gets the joystick id.
         *
         * @return The joystick id.
         */
        virtual u32 getJoystickId() const = 0;

        /**
         * @brief Sets the joystick id.
         *
         * @param joystickId The id of the joystick to assign.
         */
        virtual void setJoystickId( u32 joystickId ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IGameInput_h__
