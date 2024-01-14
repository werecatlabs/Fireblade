#ifndef SceneDropJob_h__
#define SceneDropJob_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {
        class SceneDropJob : public Job
        {
        public:
            SceneDropJob();
            ~SceneDropJob() override;

            void execute() override;

            String getData() const;

            void setData( const String &data );

            String getFilePath() const;

            void setFilePath( const String &filePath );

            SmartPtr<ui::IUIElement> getSender() const;

            void setSender( SmartPtr<ui::IUIElement> sender );

            SmartPtr<ICommand> getDragDropActorCmd() const;

            void setDragDropActorCmd( SmartPtr<ICommand> dragDropActorCmd );

            SmartPtr<ui::IUITreeCtrl> getTree() const;

            void setTree( SmartPtr<ui::IUITreeCtrl> tree );

            fb::SmartPtr<fb::editor::SceneWindow> getOwner() const;

            void setOwner( fb::SmartPtr<fb::editor::SceneWindow> owner );

            s32 getSiblingIndex() const;
            void setSiblingIndex( s32 siblingIndex );

        protected:
            SmartPtr<SceneWindow> m_owner;
            SmartPtr<ui::IUITreeCtrl> m_tree;
            SmartPtr<ui::IUIElement> m_sender;
            SmartPtr<ICommand> m_dragDropActorCmd;
            String m_data;
            String m_filePath;
            s32 m_siblingIndex = -1;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // SceneDropJob_h__
