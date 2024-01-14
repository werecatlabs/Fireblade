#include <GameEditorPCH.h>
#include <ai/PromptGoal.h>
#include <FBCore/FBCore.h>

#include <utility>

namespace fb::editor
{

    PromptGoal::PromptGoal( String label, const Array<String> &tags ) :
        m_label( std::move( label ) ),
        m_tags( tags )
    {
    }

    PromptGoal::PromptGoal() = default;

    void PromptGoal::start()
    {
    }

    void PromptGoal::finish()
    {
    }

    void PromptGoal::setState( u32 state )
    {
    }

    auto PromptGoal::getState() const -> u32
    {
        return 0;
    }

    auto PromptGoal::getType() const -> u32
    {
        return 0;
    }

    void PromptGoal::setType( u32 type )
    {
    }

}  // namespace fb::editor
