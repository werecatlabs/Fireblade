#ifndef PromptCmd_h__
#define PromptCmd_h__

#include <commands/Command.h>

namespace fb
{
    namespace editor
    {

        class PromptCmd : public Command
        {
        public:
            PromptCmd();
            ~PromptCmd() override;

            void undo() override;
            void redo() override;
            void execute() override;

            void processAIPrompt( const String &promptStr );

	        String getPrompt() const;

            void setPrompt( const String& prompt );

        protected:
            String m_prompt;
            Array<SmartPtr<IAiGoalEvaluator>> goalEvaluators;
        };

    }  // namespace editor
}  // namespace fb


#endif // PromptCmd_h__
