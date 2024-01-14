#ifndef ImGuiApplication_h__
#define ImGuiApplication_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBCore/Interface/UI/IUIApplication.h>
#include <FBCore/Interface/Graphics/IWindowListener.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/ColourF.h>
#include <imgui_internal.h>

namespace fb
{
    namespace ui
    {

        class ImGuiApplication : public IUIApplication
        {
        public:
            ImGuiApplication();
            ~ImGuiApplication() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void setCustomStyle();
            void setDarkGreenStyle();
            void setDarkBlueStyle();

            size_t messagePump( SmartPtr<ISharedObject> data );

            void handleWindowEvent( SmartPtr<render::IWindowEvent> event ) override;

            bool handleInputEvent( SmartPtr<IInputEvent> event ) override;

            void run();

            void createSubMenus( SmartPtr<IUIMenu> menu );

            void createElement( SmartPtr<IUIElement> element );

            void update() override;

            void *getHWND() const;

            SmartPtr<IUIMenubar> getMenubar() const override;
            void setMenubar( SmartPtr<IUIMenubar> menubar ) override;

            SmartPtr<IUIToolbar> getToolbar() const override;
            void setToolbar( SmartPtr<IUIToolbar> toolbar ) override;

            void ShowApp( bool *p_open );

            void createMenuItem( SmartPtr<IUIMenu> rootMenu, SmartPtr<IUIElement> menuItemElement );

            Vector2I getWindowSize() const override;
            void setWindowSize( const Vector2I &size ) override;

            void draw( SmartPtr<IUIRenderWindow> renderWindow );

            bool getUseInputEvents() const;
            void setUseInputEvents( bool useInputEvents );

            void *getEmptyTexture() const;

            void setEmptyTexture( void *emptyTexture );

            static ImGuiOverlayOgre *getOverlay();

            // todo rename and make accessor functions
            ImGuiID dockLeft_id_left;
            ImGuiID dockLeft_id_right;
            ImGuiID dockLeft_id_up;
            ImGuiID dockLeft_id_down;

        protected:
            class WindowListener : public render::IWindowListener
            {
            public:
                WindowListener() = default;
                ~WindowListener() override = default;

                void unload( SmartPtr<ISharedObject> data ) override;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

                void handleEvent( SmartPtr<render::IWindowEvent> event ) override;

                void setOwner( SmartPtr<ImGuiApplication> owner );
                SmartPtr<ImGuiApplication> getOwner() const;

            protected:
                AtomicWeakPtr<ImGuiApplication> m_owner;
            };

#if FB_BUILD_SDL2
            SDL_Texture *IMG_LoadTexture( SDL_Renderer *renderer, const char *file );
#endif

            void EditTransform( float *cameraView, float *cameraProjection, float *matrix,
                                bool editTransformDecomposition );
            void ShowEditor( bool *p_open );

            void ShowGuizmo();
            void DockSpaceUI();
            void ToolbarUI();
            void DockingToolbar( const char *name, ImGuiAxis *p_toolbar_axis,
                                 SmartPtr<IUIElement> toolbarElement );
            void TestDoc();

            void ShowPlaceholderObject( const char *prefix, int uid );

            Vector2I m_size;

            // Our state
            bool show_demo_window = true;
            bool show_another_window = false;
            bool showDockSpace = true;
            bool done = false;

            ColourF clear_color = ColourF( 0.45f, 0.55f, 0.60f, 1.00f );

            bool m_showEditor = true;
            bool useWindow = true;
            int gizmoCount = 1;
            int lastUsing = 0;
            float camDistance = 8.f;

#if FB_BUILD_SDL2
            SDL_Texture *texture = nullptr;
            SDL_Window *window = nullptr;
            SDL_Renderer *renderer = nullptr;
#endif

            Ogre::RenderTargetListener *m_RenderTargetListener = nullptr;

            void *m_hwnd = nullptr;

            SmartPtr<render::IWindowListener> m_windowListener;

            SmartPtr<IUIMenubar> m_menuBar;
            SmartPtr<IUIToolbar> m_toolbar;

            ImguiManagerOgre *m_imguiManagerOgre = nullptr;

            ImGuiApplicationOSX *app = nullptr;

            static ImGuiOverlayOgre *m_overlay;

            u32 m_childWindowCount = 0;

            Vector2I m_currentMousePosition;

            bool m_useInputEvents = false;

            void *m_emptyTexture = nullptr;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // ImGuiApplication_h__
