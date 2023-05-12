#ifndef IAiGoal_h__
#define IAiGoal_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * @brief Interface for an AI goal. Inherits from ISharedObject.
     */
    class IAiGoal : public ISharedObject
    {
    public:
        /**
         * @brief Enum representing the possible states of an AI goal.
         */
        enum class AiGoalState
        {
            AGS_READY,
            AGS_EXECUTING,
            AGS_FINISHED,
            AGS_FAILED
        };

        /**
         * @brief Destroy the IAiGoal object.
         */
        ~IAiGoal() override = default;

        /**
         * @brief Start the AI goal. Called when the AI begins using this goal.
         */
        virtual void start() = 0;

        /**
         * @brief Finish the AI goal. Called when the goal has been completed or terminated.
         */
        virtual void finish() = 0;

        /**
         * @brief Set the state of the AI goal.
         *
         * @param state The desired state for the AI goal.
         */
        virtual void setState( u32 state ) = 0;

        /**
         * @brief Get the current state of the AI goal.
         *
         * @return u32 The current state of the AI goal.
         */
        virtual u32 getState() const = 0;

        /**
         * @brief Get the type of the AI goal.
         *
         * @return u32 An integer representing the goal type.
         */
        virtual u32 getType() const = 0;

        /**
         * @brief Set the type of the AI goal.
         *
         * @param type An integer representing the goal type.
         */
        virtual void setType( u32 type ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IAiGoal_h__
