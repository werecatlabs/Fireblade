#ifndef BaseWindow_h__
#define BaseWindow_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/Scene/IEditor.h>
#include <FBCore/Interface/UI/IUIDragSource.h>
#include <FBCore/Interface/UI/IUIDropTarget.h>

namespace fb
{
    namespace editor
    {
        /**
        @brief Base class for a window object in the user interface.
        */
        class EditorWindow : public scene::IEditor
        {
        public:
            /*
             * @brief Constructor.
             */
            EditorWindow();

            /**
             * @brief Destructor.
             */
            ~EditorWindow() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void reload( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            /**
             * @brief Gets the parent of the window.
             *
             * @return SmartPtr<ui::IUIWindow> The parent of the window.
             */
            SmartPtr<ui::IUIWindow> getParent() const override;

            /**
             * @brief Sets the parent of the window.
             *
             * @param parent The parent of the window.
             */
            void setParent( SmartPtr<ui::IUIWindow> parent ) override;

            /**
             * @brief Gets the parent window of the window.
             *
             * @return SmartPtr<ui::IUIWindow> The parent window of the window.
             */
            SmartPtr<ui::IUIWindow> getParentWindow() const override;

            /**
             * @brief Sets the parent window of the window.
             *
             * @param parentWindow The parent window of the window.
             */
            void setParentWindow( SmartPtr<ui::IUIWindow> parentWindow ) override;

            SmartPtr<ui::IUIWindow> getDebugWindow() const;

            void setDebugWindow( SmartPtr<ui::IUIWindow> debugWindow );

            /**
             * @brief Checks if the window is visible.
             *
             * @return bool True if the window is visible, false otherwise.
             */
            bool isWindowVisible() const override;

            /**
             * @brief Sets the window visible or hidden.
             *
             * @param visible True to make the window visible, false to hide it.
             */
            void setWindowVisible( bool visible ) override;

            /**
             * @brief Updates the selection of the window.
             *
             */
            void updateSelection() override;

            SmartPtr<IEventListener> getEventListener() const override;

            void setEventListener( SmartPtr<IEventListener> eventListener ) override;

            SmartPtr<ui::IUIDragSource> getWindowDragSource() const override;

            void setWindowDragSource( SmartPtr<ui::IUIDragSource> windowDragSource ) override;

            SmartPtr<ui::IUIDropTarget> getWindowDropTarget() const override;

            void setWindowDropTarget( SmartPtr<ui::IUIDropTarget> windowDropTarget ) override;

            void setDraggable( SmartPtr<ui::IUIElement> element, bool draggable ) override;

            bool isDraggable( SmartPtr<ui::IUIElement> element ) const override;

            void setDroppable( SmartPtr<ui::IUIElement> element, bool droppable ) override;

            bool isDroppable( SmartPtr<ui::IUIElement> element ) const override;

            void setHandleEvents( SmartPtr<ui::IUIElement> element, bool handleEvents ) override;

            bool getHandleEvents( SmartPtr<ui::IUIElement> element ) const override;

            Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                   const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                   SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

            String handleDrag( const Vector2I &position, SmartPtr<ui::IUIElement> element );

            void handleDrop( const Vector2I &position, SmartPtr<ui::IUIElement> element,
                             const String &data );

            String getClassName() const;

            void setClassName( const String &className );

            SmartPtr<IScriptInvoker> getInvoker() const;

            void setInvoker( SmartPtr<IScriptInvoker> invoker );

            SmartPtr<IScriptReceiver> getReceiver() const;

            void setReceiver( SmartPtr<IScriptReceiver> receiver );

            /* @copydoc IEditor::getScriptClass */
            SmartPtr<IScriptClass> getScriptClass() const;

            /* @copydoc IEditor::setScriptClass */
            void setScriptClass( SmartPtr<IScriptClass> scriptClass );

            FB_CLASS_REGISTER_DECL;

        protected:
            class UIListener : public IEventListener
            {
            public:
                UIListener() = default;
                ~UIListener() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                EditorWindow *getOwner() const;

                void setOwner( EditorWindow *owner );

            private:
                EditorWindow *m_owner = nullptr;
            };

            class UIDragSource : public ui::IUIDragSource
            {
            public:
                UIDragSource() = default;
                ~UIDragSource() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

                String handleDrag( const Vector2I &position, SmartPtr<ui::IUIElement> element );

                EditorWindow *getOwner() const;

                void setOwner( EditorWindow *owner );

            private:
                EditorWindow *m_owner = nullptr;
            };

            class UIDropTarget : public ui::IUIDropTarget
            {
            public:
                UIDropTarget() = default;
                ~UIDropTarget() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

                bool handleDrop( const Vector2I &position, SmartPtr<ui::IUIElement> src,
                                 SmartPtr<ui::IUIElement> dst, const String &data );

                EditorWindow *getOwner() const;

                void setOwner( EditorWindow *owner );

            private:
                EditorWindow *m_owner = nullptr;
            };

            void addData( SmartPtr<ISharedObject> data );

            void removeData( SmartPtr<ISharedObject> data );

            void clearData();

            Array<SmartPtr<ISharedObject>> getData() const;

            mutable RecursiveMutex m_mutex;

            SmartPtr<ui::IUIDragSource> m_windowDragSource;
            SmartPtr<ui::IUIDropTarget> m_windowDropTarget;

            SmartPtr<IEventListener> m_eventListener;

            /**  The parent of the window. */
            SmartPtr<ui::IUIWindow> m_parent;

            /** The parent window of the window. */
            SmartPtr<ui::IUIWindow> m_parentWindow;

            /** The parent window of the window. */
            SmartPtr<ui::IUIWindow> m_debugWindow;

            /** True if the window is visible, false otherwise. */
            bool m_windowVisible = true;

            /** The array of shared objects. */
            Array<SmartPtr<ISharedObject>> m_dataArray;

            bool m_updateInEditMode = false;

            String m_className;

            SmartPtr<IScriptClass> m_scriptClass;

            /// Used to call script functions.
            SmartPtr<IScriptInvoker> m_invoker;

            /// Used to receive script calls.
            SmartPtr<IScriptReceiver> m_receiver;

            /// The data used by the script system.
            SmartPtr<IScriptData> m_scriptData;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // BaseWindow_h__
