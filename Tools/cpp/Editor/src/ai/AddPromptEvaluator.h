#ifndef AddPromptEvaluator_h__
#define AddPromptEvaluator_h__

#include "ai/PromptEvaluator.h"

namespace fb
{
    namespace editor
    {

        class AddPromptEvaluator : public PromptEvaluator
        {
        public:
            AddPromptEvaluator();

            AddPromptEvaluator( AddActorCmd::ActorType actorType, const String &label,
                             const Array<String> &tags );

            ~AddPromptEvaluator();

            void activateGoal() override;

            f32 getRating() override;

            SmartPtr<ISharedObject> getOwner() const override;

            void setOwner( SmartPtr<ISharedObject> owner ) override;

            f32 getBias() const override;

            void setBias( f32 bias ) override;

            AddActorCmd::ActorType m_actorType = AddActorCmd::ActorType::Actor;

        protected:
            String m_className;
            SmartPtr<scene::IActor> m_owner;
            f32 m_bias;
        };

    }  // namespace editor
}  // namespace fb

#endif  // AddPromptEvaluator_h__
