#ifndef RemovePromptEvaluator_h__
#define RemovePromptEvaluator_h__

#include "ai/PromptEvaluator.h"

namespace fb
{
    namespace editor
    {

        class RemovePromptEvaluator : public PromptEvaluator
        {
        public:
            RemovePromptEvaluator( const String &label, const Array<String> &tags );

            void activateGoal() override;

            f32 getRating() override;

            SmartPtr<ISharedObject> getOwner() const override;

            void setOwner( SmartPtr<ISharedObject> owner ) override;

            f32 getBias() const override;

            void setBias( f32 bias ) override;

        protected:
            String m_className;
            SmartPtr<ISharedObject> m_owner;
            f32 m_bias;
        };

    }  // namespace editor
}  // namespace fb

#endif  // RemovePromptEvaluator_h__
