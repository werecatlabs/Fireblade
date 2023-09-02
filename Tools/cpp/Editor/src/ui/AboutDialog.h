#ifndef AboutDialog_h__
#define AboutDialog_h__

#include <GameEditorPrerequisites.h>
#include <ui/EditorWindow.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    namespace editor
    {

        /** The editor's about dialog. */
        class AboutDialog : public EditorWindow
        {
        public:
            AboutDialog() = default;
            ~AboutDialog() override = default;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

        protected:
            SmartPtr<ui::IUIAbout> m_aboutWindow;
        };
    }  // namespace editor
}  // namespace fb

#endif  // AboutDialog_h__
