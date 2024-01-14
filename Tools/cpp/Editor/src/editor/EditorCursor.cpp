#include <GameEditorPCH.h>
#include "EditorCursor.h"

namespace fb::editor
{

    EditorCursor::EditorCursor() = default;

    EditorCursor::~EditorCursor() = default;

    void EditorCursor::update()
    {
    }

    void EditorCursor::setState( u32 state )
    {
        m_state = state;
    }

    auto EditorCursor::getState() const -> u32
    {
        return m_state;
    }

    void EditorCursor::setSize( f32 size )
    {
        m_size = size;
    }

}  // namespace fb::editor
