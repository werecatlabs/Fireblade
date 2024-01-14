#ifndef _ADD_ENTITY_BODY_CMD_H
#define _ADD_ENTITY_BODY_CMD_H

#include <GameEditorPrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/System/ICommand.h>
#include <FBCore/Core/Properties.h>
#include <commands/Command.h>

namespace fb
{
    namespace editor
    {

        class DragDropActorCmd : public Command
        {
        public:
            DragDropActorCmd() = default;
            ~DragDropActorCmd() override = default;

            void undo() override;
            void redo() override;
            void execute() override;

            Vector2I getPosition() const;
            void setPosition( const Vector2I &position );

            SmartPtr<ui::IUIElement> getSrc() const;
            void setSrc( SmartPtr<ui::IUIElement> src );

            SmartPtr<ui::IUIElement> getDst() const;
            void setDst( SmartPtr<ui::IUIElement> dst );

            String getData() const;
            void setData( const String &data );

            s32 getSiblingIndex() const;

            void setSiblingIndex( s32 siblingIndex );

        private:
            Vector2I m_position = Vector2I::zero();
            SmartPtr<ui::IUIElement> m_src;
            SmartPtr<ui::IUIElement> m_dst;
            String m_data;
            s32 m_siblingIndex = -1;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // _ADD_ENTITY_BODY_CMD_H
