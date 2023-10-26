#ifndef UIManager_h__
#define UIManager_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/UI/IUIManager.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Core/Map.h>
#include <FBCore/Core/Parameter.h>

namespace fb
{
    namespace ui
    {
        /** Implementation of a ui system the uses the render system. */
        class UIManager : public ui::IUIManager
        {
        public:
            UIManager();
            ~UIManager() override;

            void load( SmartPtr<ISharedObject> data ) override;

            void reload( SmartPtr<ISharedObject> data ) override;

            void unload( SmartPtr<ISharedObject> data ) override;

            bool handleEvent( const SmartPtr<IInputEvent> &event );

            void _getObject( void **ppObject );
            void update();

            size_t messagePump( SmartPtr<ISharedObject> data ) override;

            SmartPtr<ui::IUIApplication> addApplication() override;

            void removeApplication( SmartPtr<ui::IUIApplication> application ) override;

            SmartPtr<ui::IUIApplication> getApplication() const override;

            void setApplication( SmartPtr<ui::IUIApplication> application ) override;

            SmartPtr<ui::IUIElement> addElement( hash64 type ) override;

            void removeElement( SmartPtr<ui::IUIElement> element ) override;

            void clear() override;

            SmartPtr<ui::IUICursor> getCursor() const override;

            SmartPtr<ui::IUIElement> findElement( const String &id ) const override;

            bool isDragging() const override;

            void setDragging( bool dragging ) override;

            SmartPtr<ui::IUIWindow> getMainWindow() const override;

            void setMainWindow( SmartPtr<ui::IUIWindow> uiWindow ) override;

            void createScene02();
            void createScene01();
            void destroyScene();

            Colibri::ColibriManager *getColibriManager() const;

            void setColibriManager( Colibri::ColibriManager *colibriManager );

            Colibri::Window *getLayoutWindow() const;

            void setLayoutWindow( Colibri::Window *layoutWindow );

        protected:
            class InputListener : public IEventListener
            {
            public:
                InputListener() = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

                fb::SmartPtr<fb::ui::UIManager> getOwner() const;

                void setOwner( fb::SmartPtr<fb::ui::UIManager> owner );

            private:
                AtomicSmartPtr<UIManager> m_owner;
            };

            SmartPtr<IEventListener> m_inputListener;

            Array<SmartPtr<ui::IUIElement>> m_elements;
            Colibri::Window *m_layoutWindow = nullptr;
            fb::SmartPtr<fb::ui::IUIWindow> m_mainWindow;
            bool m_dragging = false;
            Colibri::ColibriManager *m_colibriManager = 0;
            mutable RecursiveMutex m_mutex;
        };
    }  // namespace ui
}  // namespace fb

#endif  // UIManager_h__
