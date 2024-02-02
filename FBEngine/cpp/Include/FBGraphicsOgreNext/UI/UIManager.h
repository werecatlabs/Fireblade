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
        class UIManager : public IUIManager
        {
        public:
            /** Constructor. */
            UIManager();

            /** Destructor. */
            ~UIManager() override;

            void load( SmartPtr<ISharedObject> data ) override;

            void reload( SmartPtr<ISharedObject> data ) override;

            void unload( SmartPtr<ISharedObject> data ) override;

            bool handleEvent( const SmartPtr<IInputEvent> &event );

            void _getObject( void **ppObject ) override;
            void update() override;

            size_t messagePump( SmartPtr<ISharedObject> data ) override;

            SmartPtr<IUIApplication> addApplication() override;

            void removeApplication( SmartPtr<IUIApplication> application ) override;

            SmartPtr<IUIApplication> getApplication() const override;

            void setApplication( SmartPtr<IUIApplication> application ) override;

            SmartPtr<IUIElement> addElement( hash64 type ) override;

            void removeElement( SmartPtr<IUIElement> element ) override;

            void clear() override;

            SmartPtr<IUICursor> getCursor() const override;

            SmartPtr<IUIElement> findElement( const String &id ) const override;

            bool isDragging() const override;

            void setDragging( bool dragging ) override;

            SmartPtr<IUIWindow> getMainWindow() const override;

            void setMainWindow( SmartPtr<IUIWindow> uiWindow ) override;

            Colibri::ColibriManager *getColibriManager() const;

            void setColibriManager( Colibri::ColibriManager *colibriManager );

            Colibri::Window *getLayoutWindow() const;

            void setLayoutWindow( Colibri::Window *layoutWindow );

            SmartPtr<IFactoryManager> getFactoryManager() const;

            void setFactoryManager( SmartPtr<IFactoryManager> factoryManager );

            Array<SmartPtr<IUIElement>> getElements() const;

            void setElements( Array<SmartPtr<IUIElement>> elements );

            SmartPtr<render::IGraphicsScene> getGraphicsScene() const;

            void setGraphicsScene( SmartPtr<render::IGraphicsScene> graphicsScene );

            FB_CLASS_REGISTER_DECL;

        protected:
            class InputListener : public IEventListener
            {
            public:
                InputListener() = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                SmartPtr<UIManager> getOwner() const;

                void setOwner( SmartPtr<UIManager> owner );

            private:
                AtomicSmartPtr<UIManager> m_owner;
            };

            void setupSceneManager( SmartPtr<render::IGraphicsScene> sceneManager );

            void createLayoutWindow();

            SmartPtr<render::IGraphicsScene> m_graphicsScene;

            SmartPtr<IFactoryManager> m_factoryManager;
            SmartPtr<IEventListener> m_inputListener;

            Array<SmartPtr<IUIElement>> m_elements;
            Colibri::Window *m_layoutWindow = nullptr;
            SmartPtr<IUIWindow> m_mainWindow;
            bool m_dragging = false;
            Colibri::ColibriManager *m_colibriManager = nullptr;
            mutable RecursiveMutex m_mutex;
        };
    }  // namespace ui
}  // namespace fb

#endif  // UIManager_h__
