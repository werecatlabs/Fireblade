#include <GameEditorPCH.h>
#include "EditorCursor.h"

namespace fb
{
    namespace editor
    {

        EditorCursor::EditorCursor()
        {
        }

        EditorCursor::~EditorCursor()
        {
        }

        void EditorCursor::update()
        {
        }

        void EditorCursor::setState( u32 state )
        {
            m_state = state;
        }

        u32 EditorCursor::getState() const
        {
            return m_state;
        }

        void EditorCursor::setSize( f32 size )
        {
            m_size = size;
        }

    }  // namespace editor
}  // namespace fb
