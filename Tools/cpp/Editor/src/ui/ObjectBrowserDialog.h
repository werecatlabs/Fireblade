#ifndef ObjectBrowserDialog_h__
#define ObjectBrowserDialog_h__

#include "ui/EditorWindow.h"
#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    namespace editor
    {

        class ObjectBrowserDialog : public EditorWindow
        {
        public:
            enum WidgetId
            {
                Tree,
                AddComponent,

                Count
            };

            ObjectBrowserDialog();
            ~ObjectBrowserDialog() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void populate();

            String getSelectedObject() const;
            void setSelectedObject( const String &val );

            SmartPtr<ui::IUITreeCtrl> getTree() const;
            void setTree( SmartPtr<ui::IUITreeCtrl> tree );

            void setWindowVisible( bool visible );

        protected:
            class UIElementListener : public IEventListener
            {
            public:
                UIElementListener() = default;
                ~UIElementListener() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

                ObjectBrowserDialog *getOwner() const;

                void setOwner( ObjectBrowserDialog *owner );

            private:
                ObjectBrowserDialog *m_owner = nullptr;
            };

            SmartPtr<ui::IUITreeCtrl> m_tree;
            SmartPtr<IEventListener> m_uiListener;

            SmartPtr<ui::IUIButton> m_addComponentButton;

            String m_selectedObject;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // AddBodyDialog_h__
