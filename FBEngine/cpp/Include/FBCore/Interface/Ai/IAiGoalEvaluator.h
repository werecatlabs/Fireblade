#ifndef IAiGoalEvaluator_h__
#define IAiGoalEvaluator_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * @brief Interface for an AI goal evaluator. Inherits from ISharedObject.
     */
    class IAiGoalEvaluator : public ISharedObject
    {
    public:
        /**
         * @brief Destroy the IAiGoalEvaluator object.
         */
        ~IAiGoalEvaluator() override = default;

        /**
         * @brief Activate the AI goal associated with this evaluator.
         */
        virtual void activateGoal() = 0;

        /**
         * @brief Get the rating of the AI goal.
         *
         * @return f32 The rating of the AI goal.
         */
        virtual f32 getRating() = 0;

        /**
         * @brief Get the owner of the AI goal evaluator.
         *
         * @return SmartPtr<scene::IActor> A smart pointer to the owner actor.
         */
        virtual SmartPtr<scene::IActor> getOwner() const = 0;

        /**
         * @brief Set the owner of the AI goal evaluator.
         *
         * @param owner A smart pointer to the owner actor.
         */
        virtual void setOwner( SmartPtr<scene::IActor> owner ) = 0;

        /**
         * @brief Get the bias value of the AI goal evaluator.
         *
         * @return f32 The bias value.
         */
        virtual f32 getBias() const = 0;

        /**
         * @brief Set the bias value of the AI goal evaluator.
         *
         * @param bias The bias value to set.
         */
        virtual void setBias( f32 bias ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IAiGoalEvaluator_h__
