#ifndef IChordDetector_h__
#define IChordDetector_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>

namespace fb
{

    /**
     * Interface for a chord detector, which tracks a set of input actions and detects when they occur simultaneously.
     */
    class IChordDetector : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IChordDetector() override = default;

        /**
         * Adds an action to the set of input actions to track.
         *
         * @param action The action to add.
         */
        virtual void addAction( u32 action ) = 0;

        /**
         * Removes an action from the set of input actions being tracked.
         *
         * @param action The action to remove.
         */
        virtual void removeAction( u32 action ) = 0;

        /**
         * Clears an action from the set of input actions being tracked.
         *
         * @param action The action to clear.
         */
        virtual void clearAction( u32 action ) = 0;

        /**
         * Gets the current set of input actions being tracked by the chord detector.
         *
         * @return The set of input actions being tracked.
         */
        virtual Array<u32> getActions() const = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IChordDetector_h__
