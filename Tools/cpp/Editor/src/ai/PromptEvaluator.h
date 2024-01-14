#ifndef PromptEvaluator_h__
#define PromptEvaluator_h__

#include <FBCore/Interface/AI/IAiGoalEvaluator.h>
#include "commands/AddActorCmd.h"

namespace fb
{
    namespace editor
    {

        class PromptEvaluator : public IAiGoalEvaluator
        {
        public:
            PromptEvaluator();

            PromptEvaluator( String label, const Array<String> &tags );

            ~PromptEvaluator();

            void activateGoal() override;

            f32 getRating() override;

            SmartPtr<ISharedObject> getOwner() const override;

            void setOwner( SmartPtr<ISharedObject> owner ) override;

            f32 getBias() const override;

            void setBias( f32 bias ) override;
                        
            std::vector<std::string> namedEntities;
            std::vector<std::string> m_tags;
            std::string m_label;

        protected:
            String m_className;
            SmartPtr<scene::IActor> m_owner;
            f32 m_bias;
        };


    }  // namespace editor
}  // namespace fb

#endif  // PromptEvaluator_h__
