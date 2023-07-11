#ifndef EditorCursor_H
#define EditorCursor_H

#include <GameEditorPrerequisites.h>
#include <FBCore/Memory/SharedObject.h>

#include "FBCore/Math/Triangle3.h"

namespace fb
{
    namespace editor
    {
                
        class EditorCursor : public SharedObject<ISharedObject>
        {
        public:
            enum EditorCursorState
            {
                ECS_EDIT_NORMAL,
                ECS_EDIT_TERRAIN,
                ECS_EDIT_FOLIAGE,
                ECS_EDIT_CITY,

                ECS_COUNT
            };

            EditorCursor();
            ~EditorCursor();

            void update();

            void setState( u32 state );
            u32 getState() const;

            void setSize( f32 size );

        protected:
            Triangle3F m_curTriangle;

            DecalCursor *m_decalCursor = nullptr;

            f32 m_size = 0.0f;
            u32 m_state = 0;

            bool m_updateCursorPos = false;
        };

    }  // end namespace editor
}  // end namespace fb

#endif
