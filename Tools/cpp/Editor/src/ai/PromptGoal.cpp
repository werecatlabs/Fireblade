#include <GameEditorPCH.h>
#include <ai/PromptGoal.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        PromptGoal::PromptGoal( const String &label, const Array<String> &tags ) :
            m_label( label ),
            m_tags( tags )
        {
        }

        PromptGoal::PromptGoal()
        {
        }

        void PromptGoal::start()
        {
        }

        void PromptGoal::finish()
        {
        }

        void PromptGoal::setState( u32 state )
        {
        }

        u32 PromptGoal::getState() const
        {
            return 0;
        }

        u32 PromptGoal::getType() const
        {
            return 0;
        }

        void PromptGoal::setType( u32 type )
        {
        }

    }  // namespace editor
}  // namespace fb
