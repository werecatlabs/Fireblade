#ifndef __ImGuiManager_h__
#define __ImGuiManager_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBCore/Core/ConcurrentHashMap.h>
#include <FBCore/Interface/UI/IUIManager.h>
#include <FBCore/Interface/Graphics/IWindowListener.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <unordered_map>

namespace fb
{
    namespace ui
    {
        enum class UITypes
        {
            None,
            Application,
            Button,
            Dropdown,
            Filebrowser,
            Labelcheckbox,
            Labeltextinput,
            Menu,
            Menubar,
            Menuitem,
            Propertygrid,
            Renderwindow,
            Text,
            TextEntry,
            ToggleButton,
            ToggleGroup,
            Toolbar,
            Treectrl,
            Treenode,
            Vector2,
            Vector3,
            Vector4,
            Window,

            Count
        };

        class ImGuiManager : public IUIManager
        {
        public:
            ImGuiManager();
            ~ImGuiManager() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            size_t messagePump( SmartPtr<ISharedObject> data ) override;

            void handleWindowEvent( SmartPtr<render::IWindowEvent> event );

            SmartPtr<IUIApplication> addApplication();

            void removeApplication( SmartPtr<IUIApplication> application );

            SmartPtr<IUIElement> addElement( hash64 type ) override;

            SmartPtr<IUIElement> addElement( SmartPtr<IUIElement> parent, u8 type );

            SmartPtr<IUIElement> addElement( SmartPtr<IUIElement> parent, u8 type,
                                             Properties &properties );

            void removeElement( SmartPtr<IUIElement> element ) override;

            void clear() override;

            void reloadCurrentLayout();

            SmartPtr<IUICursor> getCursor() const override;

            SmartPtr<IUIElement> findElement( const String &id ) const override;

            SmartPtr<IUIApplication> getApplication() const override;
            void setApplication( SmartPtr<IUIApplication> application ) override;

            Array<SmartPtr<IUIWindow>> getWindows() const;
            void setWindows( Array<SmartPtr<IUIWindow>> val );

            Array<SmartPtr<IUIFileBrowser>> getFileBrowsers() const;
            void setFileBrowsers( Array<SmartPtr<IUIFileBrowser>> val );

            Array<SmartPtr<IUIRenderWindow>> getRenderWindows() const;
            void setRenderWindows( Array<SmartPtr<IUIRenderWindow>> val );

            bool isDragging() const override;

            void setDragging( bool dragging ) override;

            SmartPtr<IUIWindow> getMainWindow() const;

            void setMainWindow( SmartPtr<IUIWindow> uiWindow );

        protected:
            class InputListener : public IEventListener
            {
            public:
                /** Triggered when an input event has occurred. */
                bool inputEvent( SmartPtr<IInputEvent> event );

                /** Triggered when the input devices have been updated. */
                bool updateEvent( const SmartPtr<IInputEvent> &event );

                /** Sets the listener priority. */
                void setPriority( s32 priority );

                /** Gets the listener priority. */
                s32 getPriority() const;

                void setOwner( ImGuiManager *owner );
                ImGuiManager *getOwner() const;

            protected:
                ImGuiManager *m_owner = nullptr;
            };

            class WindowListener : public render::IWindowListener
            {
            public:
                WindowListener() = default;
                ~WindowListener() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                void setOwner( ImGuiManager *owner );
                ImGuiManager *getOwner() const;

            protected:
                ImGuiManager *m_owner = nullptr;
            };

            void addElement( u32 type, SmartPtr<IUIElement> node );
            void removeElement( u32 type, SmartPtr<IUIElement> node );

            SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> getElementsPtr( u32 type ) const;
            void setElementsPtr( u32 type, SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> p );

            Array<SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>>> getElementsByType() const;

            SmartPtr<IUIWindow> m_uiWindow;

            SmartPtr<IUIApplication> m_application;

            mutable RecursiveMutex m_elementsMutex;
            std::unordered_map<u32, AtomicSharedPtr<ConcurrentArray<SmartPtr<IUIElement>>>> m_elements;

            bool m_dragging = false;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // CEGUIManager_h__
