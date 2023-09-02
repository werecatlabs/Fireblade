#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiApplication.h>
#include <FBImGui/ImGuiMenuItem.h>
#include <FBImGui/ImGuiMenuBar.h>
#include <FBImGui/ImGuiMenu.h>
#include <FBCore/FBCore.h>

#if 0
#    include "imgui.h"
#    include "imgui_impl_sdl.h"
#    include "imgui_impl_opengl3.h"
#    include <stdio.h>
#    include <SDL.h>
#    include <SDL_image.h>

#    if defined( IMGUI_IMPL_OPENGL_ES2 )
#        include <SDL_opengles2.h>
#    else
#        include <SDL_opengl.h>
#    endif
#else

#    include <imgui.h>
#    include <stdio.h>

#    if FB_BUILD_SDL2
#        include <SDL.h>
#        include <SDL_image.h>
#        include <SDL_syswm.h>

#        include "imgui_impl_sdl.h"
#        include "imgui_impl_sdlrenderer.h"
#    endif
#endif

#if FB_GRAPHICS_SYSTEM_OGRENEXT

#    include <FBImGui/ImguiOgre/ImguiManagerOgre.h>
#    include <FBImGui/ImguiOgre/CompositorPassImGuiProvider.h>

#    include <FBGraphicsOgreNext/Wrapper/CTextureOgreNext.h>

#    include <OgreRoot.h>
#    include <OgreTextureBox.h>
#    include <Compositor/OgreCompositorManager2.h>

#    if defined FB_PLATFORM_WIN32
#        if FB_BUILD_RENDERER_DX11
#            include <imgui_impl_win32.h>
#            include <imgui_impl_dx11.h>
#            include <d3d11.h>

#            include <FBGraphicsOgreNext/Ogre/RenderSystems/Direct3D11/include/OgreD3D11TextureGpu.h>
#        elif FB_BUILD_RENDERER_GL3PLUS
#            include <imgui_impl_win32.h>
#            include "imgui_impl_opengl3.h"
#            include <FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/OgreGL3PlusPrerequisites.h>
#            include <FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/OgreGL3PlusTextureGpu.h>
#        elif FB_BUILD_RENDERER_OPENGL
#            include <imgui_impl_win32.h>
#            include "imgui_impl_opengl3.h"
#            include <FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/OgreGL3PlusPrerequisites.h>
#            include <FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/OgreGL3PlusTextureGpu.h>
#        endif
#    elif defined FB_PLATFORM_APPLE
#        if FB_BUILD_RENDERER_OPENGL
#            include <FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/OgreGL3PlusPrerequisites.h>
#            include <FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/OgreGL3PlusTextureGpu.h>
#        elif FB_BUILD_RENDERER_METAL

//#            include <FBGraphicsOgreNext/Ogre/RenderSystems/Metal/include/OgreMetalPrerequisites.h>
//#            include <FBGraphicsOgreNext/Ogre/RenderSystems/Metal/include/OgreMetalTextureGpu.h>

//#            include <imgui_impl_metal.h>
//#            include <imgui_impl_osx.h>
#        endif
#    elif defined FB_PLATFORM_LINUX
#        include <FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/OgreGL3PlusPrerequisites.h>
#        include <FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/OgreGL3PlusTextureGpu.h>
#    endif
#elif FB_GRAPHICS_SYSTEM_OGRE
#    include <Ogre.h>
#    include <FBImGui/ImguiOgre/ImGuiOverlayOgre.h>
#    include <FBImGui/ImguiOgre/ImGuiRenderTargetListener.h>
#    include <OgreRenderTargetListener.h>
#    include <OgreRenderWindow.h>

#    if defined FB_PLATFORM_WIN32
#        if FB_BUILD_RENDERER_DX11
#            include <imgui_impl_win32.h>
#            include <imgui_impl_dx11.h>
#            include <d3d11.h>
#        elif FB_BUILD_RENDERER_DX9
#            include "imgui_impl_dx9.h"
#            include "imgui_impl_win32.h"
#            include <d3d9.h>
#        endif
#    endif
#else
#endif

#include "ImGuizmo.h"
#include "ImSequencer.h"
//#include "ImZoomSlider.h"
#include "ImCurveEdit.h"
#include "GraphEditor.h"
#include <math.h>
#include <vector>
#include <algorithm>
#include <imgui_impl_win32.h>

#include "ImGuiManager.h"
#include "ImGuiPropertyGrid.h"
#include "ImGuiTreeCtrl.h"
#include "ImGuiTreeNode.h"
#include "ImGuiUtil.h"

#if defined FB_PLATFORM_APPLE
#    include <FBImGui/Apple/ImGuiApplicationOSX.h>
#endif

extern void ShowExampleAppDockSpace( bool *p_open );

#if defined FB_PLATFORM_WIN32
// Copy this line into your .cpp file to forward declare the function.
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam,
                                                              LPARAM lParam );
#endif

// extern void ImGui_ImplDX11_SetWindowSize(ImGuiViewport* viewport, ImVec2 size);

#ifdef FB_PLATFORM_WIN32
#    include "minwindef.h"
extern ImGuiKey ImGui_ImplWin32_VirtualKeyToImGuiKey( WPARAM wParam );
extern void ImGui_ImplWin32_AddKeyEvent( ImGuiKey key, bool down, int native_keycode,
                                         int native_scancode = -1 );
#endif

namespace fb
{
    namespace ui
    {
        class Test
        {
        public:
            enum class eImGuiKey
            {
                // Keyboard
                ImGuiKey_None = 0,
                ImGuiKey_Tab = 512,
                // == ImGuiKey_NamedKey_BEGIN
                ImGuiKey_LeftArrow,
                ImGuiKey_RightArrow,
                ImGuiKey_UpArrow,
                ImGuiKey_DownArrow,
                ImGuiKey_PageUp,
                ImGuiKey_PageDown,
                ImGuiKey_Home,
                ImGuiKey_End,
                ImGuiKey_Insert,
                ImGuiKey_Delete,
                ImGuiKey_Backspace,
                ImGuiKey_Space,
                ImGuiKey_Enter,
                ImGuiKey_Escape,
                ImGuiKey_LeftCtrl,
                ImGuiKey_LeftShift,
                ImGuiKey_LeftAlt,
                ImGuiKey_LeftSuper,
                ImGuiKey_RightCtrl,
                ImGuiKey_RightShift,
                ImGuiKey_RightAlt,
                ImGuiKey_RightSuper,
                ImGuiKey_Menu,
                ImGuiKey_0,
                ImGuiKey_1,
                ImGuiKey_2,
                ImGuiKey_3,
                ImGuiKey_4,
                ImGuiKey_5,
                ImGuiKey_6,
                ImGuiKey_7,
                ImGuiKey_8,
                ImGuiKey_9,
                ImGuiKey_A,
                ImGuiKey_B,
                ImGuiKey_C,
                ImGuiKey_D,
                ImGuiKey_E,
                ImGuiKey_F,
                ImGuiKey_G,
                ImGuiKey_H,
                ImGuiKey_I,
                ImGuiKey_J,
                ImGuiKey_K,
                ImGuiKey_L,
                ImGuiKey_M,
                ImGuiKey_N,
                ImGuiKey_O,
                ImGuiKey_P,
                ImGuiKey_Q,
                ImGuiKey_R,
                ImGuiKey_S,
                ImGuiKey_T,
                ImGuiKey_U,
                ImGuiKey_V,
                ImGuiKey_W,
                ImGuiKey_X,
                ImGuiKey_Y,
                ImGuiKey_Z,
                ImGuiKey_F1,
                ImGuiKey_F2,
                ImGuiKey_F3,
                ImGuiKey_F4,
                ImGuiKey_F5,
                ImGuiKey_F6,
                ImGuiKey_F7,
                ImGuiKey_F8,
                ImGuiKey_F9,
                ImGuiKey_F10,
                ImGuiKey_F11,
                ImGuiKey_F12,
                ImGuiKey_Apostrophe,
                // '
                ImGuiKey_Comma,
                // ,
                ImGuiKey_Minus,
                // -
                ImGuiKey_Period,
                // .
                ImGuiKey_Slash,
                // /
                ImGuiKey_Semicolon,
                // ;
                ImGuiKey_Equal,
                // =
                ImGuiKey_LeftBracket,
                // [
                ImGuiKey_Backslash,
                // \ (this text inhibit multiline comment caused by backslash)
                ImGuiKey_RightBracket,
                // ]
                ImGuiKey_GraveAccent,
                // `
                ImGuiKey_CapsLock,
                ImGuiKey_ScrollLock,
                ImGuiKey_NumLock,
                ImGuiKey_PrintScreen,
                ImGuiKey_Pause,
                ImGuiKey_Keypad0,
                ImGuiKey_Keypad1,
                ImGuiKey_Keypad2,
                ImGuiKey_Keypad3,
                ImGuiKey_Keypad4,
                ImGuiKey_Keypad5,
                ImGuiKey_Keypad6,
                ImGuiKey_Keypad7,
                ImGuiKey_Keypad8,
                ImGuiKey_Keypad9,
                ImGuiKey_KeypadDecimal,
                ImGuiKey_KeypadDivide,
                ImGuiKey_KeypadMultiply,
                ImGuiKey_KeypadSubtract,
                ImGuiKey_KeypadAdd,
                ImGuiKey_KeypadEnter,
                ImGuiKey_KeypadEqual,

                // Gamepad (some of those are analog values, 0.0f to 1.0f) // NAVIGATION action
                ImGuiKey_GamepadStart,
                // Menu (Xbox)          + (Switch)   Start/Options (PS) // --
                ImGuiKey_GamepadBack,
                // View (Xbox)          - (Switch)   Share (PS)         // --
                ImGuiKey_GamepadFaceUp,
                // Y (Xbox)             X (Switch)   Triangle (PS)      // ->
                // ImGuiNavInput_Input
                ImGuiKey_GamepadFaceDown,
                // A (Xbox)             B (Switch)   Cross (PS)         // ->
                // ImGuiNavInput_Activate
                ImGuiKey_GamepadFaceLeft,
                // X (Xbox)             Y (Switch)   Square (PS)        // ->
                // ImGuiNavInput_Menu
                ImGuiKey_GamepadFaceRight,
                // B (Xbox)             A (Switch)   Circle (PS)        // ->
                // ImGuiNavInput_Cancel
                ImGuiKey_GamepadDpadUp,
                // D-pad Up                                             // ->
                // ImGuiNavInput_DpadUp
                ImGuiKey_GamepadDpadDown,
                // D-pad Down                                           // ->
                // ImGuiNavInput_DpadDown
                ImGuiKey_GamepadDpadLeft,
                // D-pad Left                                           // ->
                // ImGuiNavInput_DpadLeft
                ImGuiKey_GamepadDpadRight,
                // D-pad Right                                          // ->
                // ImGuiNavInput_DpadRight
                ImGuiKey_GamepadL1,
                // L Bumper (Xbox)      L (Switch)   L1 (PS)            // ->
                // ImGuiNavInput_FocusPrev + ImGuiNavInput_TweakSlow
                ImGuiKey_GamepadR1,
                // R Bumper (Xbox)      R (Switch)   R1 (PS)            // ->
                // ImGuiNavInput_FocusNext + ImGuiNavInput_TweakFast
                ImGuiKey_GamepadL2,
                // L Trigger (Xbox)     ZL (Switch)  L2 (PS) [Analog]
                ImGuiKey_GamepadR2,
                // R Trigger (Xbox)     ZR (Switch)  R2 (PS) [Analog]
                ImGuiKey_GamepadL3,
                // L Thumbstick (Xbox)  L3 (Switch)  L3 (PS)
                ImGuiKey_GamepadR3,
                // R Thumbstick (Xbox)  R3 (Switch)  R3 (PS)
                ImGuiKey_GamepadLStickUp,
                // [Analog]                                             // ->
                // ImGuiNavInput_LStickUp
                ImGuiKey_GamepadLStickDown,
                // [Analog]                                             //
                // -> ImGuiNavInput_LStickDown
                ImGuiKey_GamepadLStickLeft,
                // [Analog]                                             //
                // -> ImGuiNavInput_LStickLeft
                ImGuiKey_GamepadLStickRight,
                // [Analog]                                             //
                // -> ImGuiNavInput_LStickRight
                ImGuiKey_GamepadRStickUp,
                // [Analog]
                ImGuiKey_GamepadRStickDown,
                // [Analog]
                ImGuiKey_GamepadRStickLeft,
                // [Analog]
                ImGuiKey_GamepadRStickRight,
                // [Analog]

                // Keyboard Modifiers (explicitly submitted by backend via AddKeyEvent() calls)
                // - This is mirroring the data also written to io.KeyCtrl, io.KeyShift, io.KeyAlt,
                // io.KeySuper, in a format allowing
                //   them to be accessed via standard key API, allowing calls such as IsKeyPressed(),
                //   IsKeyReleased(), querying duration etc.
                // - Code polling every keys (e.g. an interface to detect a key press for input mapping)
                // might want to ignore those
                //   and prefer using the real keys (e.g. ImGuiKey_LeftCtrl, ImGuiKey_RightCtrl instead
                //   of ImGuiKey_ModCtrl).
                // - In theory the value of keyboard modifiers should be roughly equivalent to a logical
                // or of the equivalent left/right keys.
                //   In practice: it's complicated; mods are often provided from different sources.
                //   Keyboard layout, IME, sticky keys and backends tend to interfere and break that
                //   equivalence. The safer decision is to relay that ambiguity down to the end-user...
                ImGuiKey_ModCtrl,
                ImGuiKey_ModShift,
                ImGuiKey_ModAlt,
                ImGuiKey_ModSuper,

                // End of list
                ImGuiKey_COUNT,
                // No valid ImGuiKey is ever greater than this value

                // [Internal] Prior to 1.87 we required user to fill io.KeysDown[512] using their own
                // native index + a io.KeyMap[] array. We are ditching this method but keeping a legacy
                // path for user code doing e.g. IsKeyPressed(MY_NATIVE_KEY_CODE)
                ImGuiKey_NamedKey_BEGIN = 512,
                ImGuiKey_NamedKey_END = ImGuiKey_COUNT,
                ImGuiKey_NamedKey_COUNT = ImGuiKey_NamedKey_END - ImGuiKey_NamedKey_BEGIN,
#ifdef IMGUI_DISABLE_OBSOLETE_KEYIO
                ImGuiKey_KeysData_SIZE =
                    ImGuiKey_NamedKey_COUNT,  // Size of KeysData[]: only hold named keys
                ImGuiKey_KeysData_OFFSET = ImGuiKey_NamedKey_BEGIN  // First key stored in KeysData[0]
#else
                ImGuiKey_KeysData_SIZE = ImGuiKey_COUNT,
                // Size of KeysData[]: hold legacy 0..512 keycodes + named keys
                ImGuiKey_KeysData_OFFSET = 0  // First key stored in KeysData[0]
#endif

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
                ,
                ImGuiKey_KeyPadEnter = ImGuiKey_KeypadEnter  // Renamed in 1.87
#endif
            };
        };

        float cameraView[16] = { 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
                                 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f };

        float cameraProjection[16];

        float objectMatrix[4][16] = {
            { 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f },

            { 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 2.f, 0.f, 0.f, 1.f },

            { 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 2.f, 0.f, 2.f, 1.f },

            { 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 2.f, 1.f }
        };

        static const float identityMatrix[16] = { 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
                                                  0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f };

        void Frustum( float left, float right, float bottom, float top, float znear, float zfar,
                      float *m16 )
        {
            float temp, temp2, temp3, temp4;
            temp = 2.0f * znear;
            temp2 = right - left;
            temp3 = top - bottom;
            temp4 = zfar - znear;
            m16[0] = temp / temp2;
            m16[1] = 0.0;
            m16[2] = 0.0;
            m16[3] = 0.0;
            m16[4] = 0.0;
            m16[5] = temp / temp3;
            m16[6] = 0.0;
            m16[7] = 0.0;
            m16[8] = ( right + left ) / temp2;
            m16[9] = ( top + bottom ) / temp3;
            m16[10] = ( -zfar - znear ) / temp4;
            m16[11] = -1.0f;
            m16[12] = 0.0;
            m16[13] = 0.0;
            m16[14] = ( -temp * zfar ) / temp4;
            m16[15] = 0.0;
        }

        void Perspective( float fovyInDegrees, float aspectRatio, float znear, float zfar, float *m16 )
        {
            float ymax, xmax;
            ymax = znear * tanf( fovyInDegrees * 3.141592f / 180.0f );
            xmax = ymax * aspectRatio;
            Frustum( -xmax, xmax, -ymax, ymax, znear, zfar, m16 );
        }

        void Cross( const float *a, const float *b, float *r )
        {
            r[0] = a[1] * b[2] - a[2] * b[1];
            r[1] = a[2] * b[0] - a[0] * b[2];
            r[2] = a[0] * b[1] - a[1] * b[0];
        }

        float Dot( const float *a, const float *b )
        {
            return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
        }

        void Normalize( const float *a, float *r )
        {
            float il = 1.f / ( sqrtf( Dot( a, a ) ) + FLT_EPSILON );
            r[0] = a[0] * il;
            r[1] = a[1] * il;
            r[2] = a[2] * il;
        }

        void LookAt( const float *eye, const float *at, const float *up, float *m16 )
        {
            float X[3], Y[3], Z[3], tmp[3];

            tmp[0] = eye[0] - at[0];
            tmp[1] = eye[1] - at[1];
            tmp[2] = eye[2] - at[2];
            Normalize( tmp, Z );
            Normalize( up, Y );

            Cross( Y, Z, tmp );
            Normalize( tmp, X );

            Cross( Z, X, tmp );
            Normalize( tmp, Y );

            m16[0] = X[0];
            m16[1] = Y[0];
            m16[2] = Z[0];
            m16[3] = 0.0f;
            m16[4] = X[1];
            m16[5] = Y[1];
            m16[6] = Z[1];
            m16[7] = 0.0f;
            m16[8] = X[2];
            m16[9] = Y[2];
            m16[10] = Z[2];
            m16[11] = 0.0f;
            m16[12] = -Dot( X, eye );
            m16[13] = -Dot( Y, eye );
            m16[14] = -Dot( Z, eye );
            m16[15] = 1.0f;
        }

        void OrthoGraphic( const float l, float r, float b, const float t, float zn, const float zf,
                           float *m16 )
        {
            m16[0] = 2 / ( r - l );
            m16[1] = 0.0f;
            m16[2] = 0.0f;
            m16[3] = 0.0f;
            m16[4] = 0.0f;
            m16[5] = 2 / ( t - b );
            m16[6] = 0.0f;
            m16[7] = 0.0f;
            m16[8] = 0.0f;
            m16[9] = 0.0f;
            m16[10] = 1.0f / ( zf - zn );
            m16[11] = 0.0f;
            m16[12] = ( l + r ) / ( l - r );
            m16[13] = ( t + b ) / ( b - t );
            m16[14] = zn / ( zn - zf );
            m16[15] = 1.0f;
        }

        inline void rotationY( const float angle, float *m16 )
        {
            float c = cosf( angle );
            float s = sinf( angle );

            m16[0] = c;
            m16[1] = 0.0f;
            m16[2] = -s;
            m16[3] = 0.0f;
            m16[4] = 0.0f;
            m16[5] = 1.f;
            m16[6] = 0.0f;
            m16[7] = 0.0f;
            m16[8] = s;
            m16[9] = 0.0f;
            m16[10] = c;
            m16[11] = 0.0f;
            m16[12] = 0.f;
            m16[13] = 0.f;
            m16[14] = 0.f;
            m16[15] = 1.0f;
        }

        ImGuiOverlayOgre *ImGuiApplication::m_overlay = nullptr;

#include "imgui_internal.h"

        void ItemRowsBackground( float lineHeight = -1.0f,
                                 const ImColor &color = ImColor( 20, 20, 20, 64 ) )
        {
            auto *drawList = ImGui::GetWindowDrawList();
            const auto &style = ImGui::GetStyle();

            if( lineHeight < 0 )
            {
                lineHeight = ImGui::GetTextLineHeight();
            }
            lineHeight += style.ItemSpacing.y;

            float scrollOffsetH = ImGui::GetScrollX();
            float scrollOffsetV = ImGui::GetScrollY();
            float scrolledOutLines = floorf( scrollOffsetV / lineHeight );
            scrollOffsetV -= lineHeight * scrolledOutLines;

            ImVec2 clipRectMin( ImGui::GetWindowPos().x, ImGui::GetWindowPos().y );
            ImVec2 clipRectMax( clipRectMin.x + ImGui::GetWindowWidth(),
                                clipRectMin.y + ImGui::GetWindowHeight() );

            if( ImGui::GetScrollMaxX() > 0 )
            {
                clipRectMax.y -= style.ScrollbarSize;
            }

            drawList->PushClipRect( clipRectMin, clipRectMax );

            bool isOdd = ( static_cast<int>( scrolledOutLines ) % 2 ) == 0;

            float yMin = clipRectMin.y - scrollOffsetV + ImGui::GetCursorPosY();
            float yMax = clipRectMax.y - scrollOffsetV + lineHeight;
            float xMin = clipRectMin.x + scrollOffsetH + ImGui::GetWindowContentRegionMin().x;
            float xMax = clipRectMin.x + scrollOffsetH + ImGui::GetWindowContentRegionMax().x;

            for( float y = yMin; y < yMax; y += lineHeight, isOdd = !isOdd )
            {
                if( isOdd )
                {
                    drawList->AddRectFilled( { xMin, y - style.ItemSpacing.y }, { xMax, y + lineHeight },
                                             color );
                }
            }

            drawList->PopClipRect();
        }

        bool ShowVector3( const char *label, Vector3F &vec )
        {
#if 1
            float label_width =
                ImGui::GetWindowWidth() *
                0.4f;  //ImGui::CalcTextSize( label ).x + ImGui::GetStyle().FramePadding.x * 2;

            bool ret = false;
            ImGui::Text( "%s:", label );
            ImGui::SameLine();
            ImGui::PushID( label );
            ImGui::SetCursorPosX( label_width );
            ImGui::PushItemWidth( ImGui::GetWindowWidth() * 0.6f );
            ret = ImGui::DragFloat3( "", &vec[0], 0.1f );
            ImGui::PopItemWidth();
            ImGui::PopID();

            return ret;
#else
            bool ret = false;

            //float label_width = ImGui::CalcTextSize( label ).x + ImGui::GetStyle().FramePadding.x * 2;
            float label_width =
                ImGui::GetWindowWidth() *
                0.4f;  //ImGui::CalcTextSize( label ).x + ImGui::GetStyle().FramePadding.x * 2;

            ImGui::Text( "%s:", label );
            ImGui::SameLine();
            ImGui::PushID( label );
            ImGui::SetCursorPosX( label_width );
            ImGui::PushItemWidth( ImGui::GetWindowWidth() * 0.15f );

            //ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 1.0f, 0.0f, 0.0f, 1.0f ) );
            //ImGui::Text( "X:", label );
            //ImGui::PopStyleColor();

            //ImGui::PushStyleColor( ImGuiCol_TextSelectedBg, ImVec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
            ImGui::SameLine();
            if( ImGui::InputFloat( "x", &vec.x, 0.1f, 1.0f, "%.2f" ) )
            {
                ret = true;
            }

            //ImGui::PopStyleColor();

            //ImGui::SameLine();
            //ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 0.0f, 1.0f, 0.0f, 1.0f ) );
            //ImGui::Text( "Y:", label );
            //ImGui::PopStyleColor();

            //ImGui::PushStyleColor( ImGuiCol_TextSelectedBg, ImVec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
            ImGui::SameLine();
            if( ImGui::InputFloat( "y", &vec.y, 0.1f, 1.0f, "%.2f" ) )
            {
                ret = true;
            }

            //ImGui::PopStyleColor();

            //ImGui::SameLine();
            //ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 0.0f, 0.0f, 1.0f, 1.0f ) );
            //ImGui::Text( "Z:", label );
            //ImGui::PopStyleColor();

            //ImGui::PushStyleColor( ImGuiCol_TextSelectedBg, ImVec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
            ImGui::SameLine();
            if( ImGui::InputFloat( "z", &vec.z, 0.1f, 1.0f, "%.2f" ) )
            {
                ret = true;
            }

            //ImGui::PopStyleColor();

            ImGui::PopItemWidth();
            ImGui::PopID();

            return ret;
#endif
        }

        ImGuiApplication::ImGuiApplication()
        {
        }

        ImGuiApplication::~ImGuiApplication()
        {
            unload( nullptr );
        }

        void ImGuiApplication::load( SmartPtr<ISharedObject> data )
        {
            FB_ASSERT( isLoaded() == false );

#if FB_GRAPHICS_SYSTEM_OGRENEXT
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto sceneManager = graphicsSystem->getGraphicsScene();
            FB_ASSERT( sceneManager );

            auto window = graphicsSystem->getDefaultWindow();
            FB_ASSERT( window );

            auto listener = fb::make_ptr<WindowListener>();
            listener->setOwner( this );
            window->addListener( listener );
            m_windowListener = listener;

            size_t windowHandle = 0;
            window->getCustomAttribute( "WINDOW", &windowHandle );

#    if defined FB_PLATFORM_WIN32
#        if FB_BUILD_RENDERER_DX11
            ID3D11Device *device = nullptr;
            window->getCustomAttribute( "RENDERDOC_DEVICE", &device );
#        elif FB_BUILD_RENDERER_GL3PLUS
            void *device;
            window->getCustomAttribute( "RENDERDOC_DEVICE", (void *)&device );
#        elif FB_BUILD_RENDERER_OPENGL
            void *device;
            window->getCustomAttribute( "RENDERDOC_DEVICE", (void *)&device );
#        elif FB_BUILD_RENDERER_METAL
#        endif
#    endif

            m_hwnd = (void *)windowHandle;

            auto root = Ogre::Root::getSingletonPtr();
            auto compoProvider = OGRE_NEW Ogre::CompositorPassImGuiProvider();
            auto compositorManager = root->getCompositorManager2();
            compositorManager->setCompositorPassProvider( compoProvider );

            //m_imguiManagerOgre = new ImguiManagerOgre;

            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            (void)io;
            // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
            // io.ConfigFlags |= ImGuiDockNodeFlags_PassthruCentralNode;

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            // ImGui::StyleColorsClassic();

#    if defined FB_PLATFORM_WIN32
#        if FB_BUILD_RENDERER_DX11
            // Setup Platform/Renderer backends
            if( windowHandle != 0 )
            {
                ImGui_ImplWin32_Init( (HWND)windowHandle );

                if( device )
                {
                    ID3D11DeviceContext *device_context = nullptr;
                    ( device )->GetImmediateContext( &device_context );

                    if( device_context )
                    {
                        ImGui_ImplDX11_Init( device, device_context );
                    }
                }
            }
#        elif FB_BUILD_RENDERER_GL3PLUS
            // Setup Platform/Renderer backends
            if( windowHandle != 0 )
            {
                ImGui_ImplWin32_Init( (HWND)windowHandle );

                ImGui_ImplOpenGL3_Init();
            }
#        elif FB_BUILD_RENDERER_OPENGL
            // Setup Platform/Renderer backends
            if( windowHandle != 0 )
            {
                ImGui_ImplWin32_Init( (HWND)windowHandle );

                ImGui_ImplOpenGL3_Init();
            }
#        elif FB_BUILD_RENDERER_METAL
#        endif
#    elif defined FB_PLATFORM_APPLE
#        if FB_BUILD_RENDERER_OPENGL
#        elif FB_BUILD_RENDERER_METAL

            app = new ImGuiApplicationOSX;
            app->load();
#        endif
#    endif

            // Ogre::SceneManager* smgr = 0;
            // sceneManager->_getObject((void**)&smgr);
            // m_imguiManagerOgre->init(smgr)
#elif FB_GRAPHICS_SYSTEM_OGRE

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto sceneManager = graphicsSystem->getGraphicsScene();
            FB_ASSERT( sceneManager );

            auto window = graphicsSystem->getDefaultWindow();
            FB_ASSERT( window );

            auto listener = fb::make_ptr<WindowListener>();
            listener->setOwner( this );
            window->addListener( listener );
            m_windowListener = listener;

#    if defined FB_PLATFORM_WIN32
            HWND windowHandle = nullptr;
            window->getCustomAttribute( "WINDOW", &windowHandle );

            m_hwnd = windowHandle;

#        if FB_BUILD_RENDERER_DX11
            ID3D11Device *device = nullptr;
            window->getCustomAttribute( "D3DDEVICE", &device );
#        elif FB_BUILD_RENDERER_DX9
            IDirect3DDevice9 *device = nullptr;
            window->getCustomAttribute( "D3DDEVICE", &device );
#        elif FB_BUILD_RENDERER_OPENGL
            void *device;
            window->getCustomAttribute( "RENDERDOC_DEVICE", &device );
#        elif FB_BUILD_RENDERER_METAL
#        endif
#    elif defined FB_PLATFORM_APPLE

            app = new ImGuiApplicationOSX;
            app->load();

#    elif defined FB_PLATFORM_LINUX
#    endif

            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            (void)io;
            // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
            // io.ConfigFlags |= ImGuiDockNodeFlags_PassthruCentralNode;

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            // ImGui::StyleColorsClassic();

            // handle DPI scaling
#    if defined FB_PLATFORM_WIN32
            auto vpScale = 1.0f;
#    else
            auto vpScale = 2.0f;  //Ogre::OverlayManager::getSingleton().getPixelRatio();
#    endif
            //Ogre::OverlayManager::getSingleton().setPixelRatio(vpScale);
            ImGui::GetIO().FontGlobalScale =
                std::round( vpScale );  // default font does not work with fractional scaling
            ImGui::GetStyle().ScaleAllSizes( vpScale );

            auto imguiOverlay = new ImGuiOverlayOgre();
            imguiOverlay->setZOrder( 300 );
            //imguiOverlay->show();
            //Ogre::OverlayManager::getSingleton().addOverlay( imguiOverlay );
            m_overlay = imguiOverlay;

            m_RenderTargetListener = new ImGuiRenderTargetListener;

            Ogre::RenderWindow *renderWindow = nullptr;
            window->_getObject( (void **)&renderWindow );

            renderWindow->addListener( m_RenderTargetListener );

#    if FB_GRAPHICS_SYSTEM_OGRE
#        if defined FB_PLATFORM_WIN32
            ImGui_ImplWin32_Init( windowHandle );
#        endif
#    else
#        if defined FB_PLATFORM_WIN32
#            if FB_BUILD_RENDERER_DX9
            // Setup Platform/Renderer backends
            if( windowHandle != 0 )
            {
                ImGui_ImplWin32_Init( (HWND)windowHandle );

                if( device )
                {
                    ImGui_ImplDX9_Init( device );
                }
            }
#            endif
#        endif
#    endif
#elif 0

            // Setup SDL
            // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a
            // minority of Windows systems, depending on whether SDL_INIT_GAMECONTROLLER is enabled or
            // disabled.. updating to latest version of SDL is recommended!)
            if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) != 0 )
            {
                printf( "Error: %s\n", SDL_GetError() );
                return;
            }

            // Decide GL+GLSL versions
#    if defined( IMGUI_IMPL_OPENGL_ES2 )
            // GL ES 2.0 + GLSL 100
            const char *glsl_version = "#version 100";
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, 0 );
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
#    elif defined( __APPLE__ )
            // GL 3.2 Core + GLSL 150
            const char *glsl_version = "#version 150";
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS,
                                 SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG );  // Always required on Mac
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
#    else
            // GL 3.0 + GLSL 130
            const char *glsl_version = "#version 130";
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, 0 );
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
#    endif

            // Create window with graphics context
            SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
            SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
            SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
            SDL_WindowFlags window_flags =
                (SDL_WindowFlags)( SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI );
            SDL_Window *window =
                SDL_CreateWindow( "Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags );
            SDL_GLContext gl_context = SDL_GL_CreateContext( window );
            SDL_GL_MakeCurrent( window, gl_context );
            SDL_GL_SetSwapInterval( 1 );  // Enable vsync

            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            (void)io;
            // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            // ImGui::StyleColorsClassic();

            // Setup Platform/Renderer backends
            ImGui_ImplSDL2_InitForOpenGL( window, gl_context );
            ImGui_ImplOpenGL3_Init( glsl_version );

            // Load Fonts
            // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple
            // fonts and use ImGui::PushFont()/PopFont() to select them.
            // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select
            // the font among multiple.
            // - If the file cannot be loaded, the function will return NULL. Please handle those errors
            // in your application (e.g. use an assertion, or display an error and quit).
            // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture
            // when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below
            // will call.
            // - Read 'docs/FONTS.md' for more instructions and details.
            // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need
            // to write a double backslash \\ !
            // io.Fonts->AddFontDefault();
            // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
            // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
            // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
            // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
            // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
            // NULL, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);

            // Our state
            bool show_demo_window = true;
            bool show_another_window = false;
            ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );

#elif FB_BUILD_SDL2
            // Setup SDL
            // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a
            // minority of Windows systems, depending on whether SDL_INIT_GAMECONTROLLER is enabled or
            // disabled.. updating to latest version of SDL is recommended!)
            // if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
            if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) != 0 )
            {
                printf( "Error: %s\n", SDL_GetError() );
                return;
            }

            // Setup window
            SDL_WindowFlags window_flags =
                (SDL_WindowFlags)( SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI );
            window = SDL_CreateWindow( "Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags );

#    ifdef FB_PLATFORM_WIN32
            SDL_SysWMinfo wmInfo;
            SDL_VERSION( &wmInfo.version );
            SDL_GetWindowWMInfo( window, &wmInfo );
            HWND hwnd = wmInfo.info.win.window;
            m_hwnd = hwnd;
#    endif

            // Setup SDL_Renderer instance
            renderer =
                SDL_CreateRenderer( window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED );
            if( renderer == NULL )
            {
                SDL_Log( "Error creating SDL_Renderer!" );
                return;
            }

            // SDL_RendererInfo info;
            // SDL_GetRendererInfo(renderer, &info);
            // SDL_Log("Current SDL_Renderer: %s", info.name);

            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            (void)io;
            // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            // ImGui::StyleColorsClassic();

            // Setup Platform/Renderer backends
            ImGui_ImplSDL2_InitForSDLRenderer( window );
            ImGui_ImplSDLRenderer_Init( renderer );

            // Load Fonts
            // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple
            // fonts and use ImGui::PushFont()/PopFont() to select them.
            // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select
            // the font among multiple.
            // - If the file cannot be loaded, the function will return NULL. Please handle those errors
            // in your application (e.g. use an assertion, or display an error and quit).
            // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture
            // when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below
            // will call.
            // - Read 'docs/FONTS.md' for more instructions and details.
            // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need
            // to write a double backslash \\ !
            // io.Fonts->AddFontDefault();
            // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
            // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
            // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
            // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
            // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
            // NULL, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);

            // texture = IMG_LoadTexture(renderer, "checker.png");

            texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_INDEX8, SDL_TEXTUREACCESS_STREAMING,
                                         512, 512 );
#endif

            // create an empty texture with a white pixel
            //static unsigned whitePixel[4] = { 255, 255, 255, 255 };
            //auto emptyTextureId = io.Fonts->TexID;
            //ImGui::GetIO().Fonts->TexPixelsRGBA32 = (unsigned *)&whitePixel[0];
            //ImGui::GetIO().Fonts->TexWidth = 1;
            //ImGui::GetIO().Fonts->TexHeight = 1;
            //ImGui::GetIO().Fonts->TexID = emptyTextureId;
            //setEmptyTexture( emptyTextureId );

            setLoadingState( LoadingState::Loaded );
        }

        void ImGuiApplication::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    if( m_windowListener )
                    {
                        m_windowListener->unload( nullptr );
                        m_windowListener = nullptr;
                    }

                    m_toolbar = nullptr;
                    m_menuBar = nullptr;

                    // Cleanup
                    // ImGui_ImplSDLRenderer_Shutdown();
                    // ImGui_ImplSDL2_Shutdown();
                    // ImGui::DestroyContext();

#if defined FB_PLATFORM_WIN32
                    ImGui_ImplWin32_Shutdown();
#endif

#if FB_GRAPHICS_SYSTEM_OGRENEXT

#    if defined FB_PLATFORM_WIN32
#        if FB_BUILD_RENDERER_DX11
                    ImGui_ImplDX11_Shutdown();
#        endif
#    elif defined FB_PLATFORM_APPLE
#        if FB_BUILD_RENDERER_METAL
                    //ImGui_ImplMetal_Shutdown();
#        endif
#    endif
#elif FB_BUILD_SDL2
                    SDL_DestroyRenderer( renderer );
                    SDL_DestroyWindow( window );
                    SDL_Quit();
#endif

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        size_t ImGuiApplication::messagePump( SmartPtr<ISharedObject> data )
        {
#if defined FB_PLATFORM_WIN32
            if( data )
            {
                if( m_hwnd )
                {
                    auto windowMessageData = fb::static_pointer_cast<WindowMessageData>( data );

                    auto handle = windowMessageData->getWindowHandle();
                    auto message = windowMessageData->getMessage();
                    auto wParam = windowMessageData->getWParam();
                    auto lParam = windowMessageData->getLParam();

                    return ImGui_ImplWin32_WndProcHandler( static_cast<HWND>( handle ), message, wParam,
                                                           lParam );
                }
            }
#elif defined( FB_PLATFORM_APPLE )
#endif

            return 0;
        }

        void ImGuiApplication::handleWindowEvent( SmartPtr<render::IWindowEvent> event )
        {
            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
#if defined FB_PLATFORM_WIN32
                if( m_hwnd )
                {
                    auto windowMessageData = fb::static_pointer_cast<WindowMessageData>( event );

                    auto handle = windowMessageData->getWindowHandle();
                    auto message = windowMessageData->getMessage();
                    auto wParam = windowMessageData->getWParam();
                    auto lParam = windowMessageData->getLParam();

                    ImGui_ImplWin32_WndProcHandler( static_cast<HWND>( handle ), message, wParam,
                                                    lParam );
                }
#elif defined FB_PLATFORM_APPLE
                if( app )
                {
                    app->handleWindowEvent( event );
                }
#endif
            }
        }

        bool ImGuiApplication::handleInputEvent( SmartPtr<IInputEvent> event )
        {
            /*
            ImGuiIO &io = ImGui::GetIO();

            auto eventType = event->getEventType();
            if( eventType == IInputEvent::EventType::Mouse )
            {
                auto mouseState = event->getMouseState();
                auto mouseEventType = mouseState->getEventType();
                if( mouseEventType == IMouseState::Event::LeftPressed )
                {
                    auto position = mouseState->getAbsolutePosition();
                    io.AddMousePosEvent( position.X(), position.Y() );

                    int button = static_cast<int>( mouseEventType );
                    if( button >= 0 && button < ImGuiMouseButton_COUNT )
                    {
                        io.AddMouseButtonEvent( ImGuiMouseButton_Left, true );
                    }

                    // FB_LOG("Test mouse pressed");
                }
                else if( mouseEventType == IMouseState::Event::LeftReleased )
                {
                    int button = static_cast<int>( mouseEventType );
                    if( button >= 0 && button < ImGuiMouseButton_COUNT )
                    {
                        io.AddMouseButtonEvent( ImGuiMouseButton_Left, false );
                    }
                }

                if( mouseEventType == IMouseState::Event::RightPressed )
                {
                    auto position = mouseState->getAbsolutePosition();
                    io.AddMousePosEvent( position.X(), position.Y() );

                    int button = static_cast<int>( mouseEventType );
                    if( button >= 0 && button < ImGuiMouseButton_COUNT )
                    {
                        io.AddMouseButtonEvent( ImGuiMouseButton_Right, true );
                    }

                    // FB_LOG("Test mouse pressed");
                }
                else if( mouseEventType == IMouseState::Event::RightReleased )
                {
                    int button = static_cast<int>( mouseEventType );
                    if( button >= 0 && button < ImGuiMouseButton_COUNT )
                    {
                        io.AddMouseButtonEvent( ImGuiMouseButton_Right, false );
                    }
                }

                if( mouseEventType == IMouseState::Event::Moved )
                {
                    auto p = mouseState->getAbsolutePosition();
                    auto position = Vector2I( p.X(), p.Y() );
                    if( !MathUtil<s32>::equals( position, m_currentMousePosition ) )
                    {
                        io.AddMousePosEvent( static_cast<float>( position.X() ),
                                             static_cast<float>( position.Y() ) );
                        m_currentMousePosition = position;

                        // FB_LOG("Test mouse move");
                    }
                }

                return io.WantCaptureMouse;
            }

            if( eventType == IInputEvent::EventType::Key )
            {
                Map<KeyCodes, ImGuiKey_> keyboardMap;

                for( size_t i = 0; i < int( KeyCodes::KEY_COUNT ); ++i )
                {
                    auto keyCode = (KeyCodes)i;
                    auto imGuiKey = (ImGuiKey_)( (size_t)ImGuiKey_NamedKey_BEGIN + i );

                    switch( keyCode )
                    {
                    case KeyCodes::KEY_KEY_0:
                    {
                        imGuiKey = ImGuiKey_::ImGuiKey_0;
                    }
                    break;
                    case KeyCodes::KEY_KEY_1:
                    {
                        imGuiKey = ImGuiKey_::ImGuiKey_1;
                    }
                    break;
                    case KeyCodes::KEY_KEY_2:
                    {
                        imGuiKey = ImGuiKey_::ImGuiKey_2;
                    }
                    break;
                    case KeyCodes::KEY_KEY_3:
                    {
                        imGuiKey = ImGuiKey_::ImGuiKey_3;
                    }
                    break;
                    case KeyCodes::KEY_KEY_4:
                    {
                        imGuiKey = ImGuiKey_::ImGuiKey_4;
                    }
                    break;
                    case KeyCodes::KEY_KEY_5:
                    {
                        imGuiKey = ImGuiKey_::ImGuiKey_5;
                    }
                    break;
                    case KeyCodes::KEY_KEY_6:
                    {
                        imGuiKey = ImGuiKey_::ImGuiKey_6;
                    }
                    break;
                    case KeyCodes::KEY_KEY_7:
                    {
                        imGuiKey = ImGuiKey_::ImGuiKey_7;
                    }
                    break;
                    case KeyCodes::KEY_KEY_8:
                    {
                        imGuiKey = ImGuiKey_::ImGuiKey_8;
                    }
                    break;
                    case KeyCodes::KEY_KEY_9:
                    {
                        imGuiKey = ImGuiKey_::ImGuiKey_9;
                    }
                    break;
                    case KeyCodes::KEY_KEY_A:
                    case KeyCodes::KEY_KEY_B:
                    case KeyCodes::KEY_KEY_C:
                    case KeyCodes::KEY_KEY_D:
                    case KeyCodes::KEY_KEY_E:
                    case KeyCodes::KEY_KEY_F:
                    {
                        imGuiKey = ImGuiKey_::ImGuiKey_F;
                    }
                    break;
                    case KeyCodes::KEY_KEY_G:
                    case KeyCodes::KEY_KEY_H:
                    case KeyCodes::KEY_KEY_I:
                    case KeyCodes::KEY_KEY_J:
                    case KeyCodes::KEY_KEY_K:
                    case KeyCodes::KEY_KEY_L:
                    case KeyCodes::KEY_KEY_M:
                    case KeyCodes::KEY_KEY_N:
                    case KeyCodes::KEY_KEY_O:
                    case KeyCodes::KEY_KEY_P:
                    case KeyCodes::KEY_KEY_Q:
                    case KeyCodes::KEY_KEY_R:
                    case KeyCodes::KEY_KEY_S:
                    case KeyCodes::KEY_KEY_T:
                    case KeyCodes::KEY_KEY_U:
                    case KeyCodes::KEY_KEY_V:
                    case KeyCodes::KEY_KEY_W:
                    case KeyCodes::KEY_KEY_X:
                    case KeyCodes::KEY_KEY_Y:
                    case KeyCodes::KEY_KEY_Z:
                    {
                    }
                    break;
                    default:
                    {
                    }
                    };

                    keyboardMap[keyCode] = imGuiKey;
                }

                auto keyboardState = event->getKeyboardState();
                auto keycode = keyboardState->getKeyCode();
                auto rawKeyCode = keyboardState->getRawKeyCode();
                auto keyChar = keyboardState->getChar();
                bool is_key_down = keyboardState->isPressedDown();

                auto key = keyboardMap[(KeyCodes)keycode];

                if( (ImGuiKey)key != ImGuiKey_None )
                {
                    io.AddKeyEvent( (ImGuiKey)key, is_key_down );
                }
            }
            */

            return false;
        }

        void ImGuiApplication::run()
        {
            while( !done )
            {
                update();
            }
        }

        void ImGuiApplication::createSubMenus( SmartPtr<IUIMenu> menu )
        {
            auto subMenuElements = menu->getMenuItems();
            for( auto subMenuElement : subMenuElements )
            {
                if( subMenuElement->isDerived<IUIMenu>() )
                {
                    auto subMenu = fb::static_pointer_cast<IUIMenu>( subMenuElement );

                    auto menuBar = getMenubar();

                    auto label = subMenu->getLabel();
                    if( ImGui::BeginMenu( label.c_str() ) )
                    {
                        auto menuItemElements = subMenu->getMenuItems();
                        for( auto menuItemElement : menuItemElements )
                        {
                            if( menuItemElement->isDerived<IUIMenuItem>() )
                            {
                                auto menuItem = fb::static_pointer_cast<IUIMenuItem>( menuItemElement );

                                auto text = menuItem->getText();

                                bool selected = false;
                                ImGui::MenuItem( text.c_str(), nullptr, &selected );

                                if( selected )
                                {
                                    auto menuItemId = menuItem->getElementId();

                                    auto listeners = menuBar->getObjectListeners();
                                    for( auto listener : listeners )
                                    {
                                        listener->handleEvent( IEvent::Type::UI, menuItemId,
                                                               Array<Parameter>(), menuItemElement, this,
                                                               nullptr );
                                    }
                                }
                            }
                        }

                        ImGui::EndMenu();
                    }

                    createSubMenus( subMenu );
                }
            }
        }

        void ImGuiApplication::ShowPlaceholderObject( const char *prefix, int uid )
        {
            // Use object uid as identifier. Most commonly you could also use the object pointer as a
            // base ID.
            ImGui::PushID( uid );

            // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we
            // add vertical spacing to make the tree lines equal high.
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex( 0 );
            ImGui::AlignTextToFramePadding();
            bool node_open = ImGui::TreeNode( "Object", "%s_%u", prefix, uid );
            ImGui::TableSetColumnIndex( 1 );
            ImGui::Text( "my sailor is rich" );

            if( node_open )
            {
                static float placeholder_members[8] = { 0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f };
                for( int i = 0; i < 8; i++ )
                {
                    ImGui::PushID( i );  // Use field index as identifier.
                    if( i < 2 )
                    {
                        ShowPlaceholderObject( "Child", 424242 );
                    }
                    else
                    {
                        // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as
                        // well)
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex( 0 );
                        ImGui::AlignTextToFramePadding();
                        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf |
                                                   ImGuiTreeNodeFlags_NoTreePushOnOpen |
                                                   ImGuiTreeNodeFlags_Bullet;
                        ImGui::TreeNodeEx( "Field", flags, "Field_%d", i );

                        ImGui::TableSetColumnIndex( 1 );
                        ImGui::SetNextItemWidth( -FLT_MIN );
                        if( i >= 5 )
                            ImGui::InputFloat( "##value", &placeholder_members[i], 1.0f );
                        else
                            ImGui::DragFloat( "##value", &placeholder_members[i], 0.01f );
                        ImGui::NextColumn();
                    }
                    ImGui::PopID();
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }

        // int inputTextCallback(ImGuiInputTextCallbackData* data)
        //{

        //}

        void ImGuiApplication::createElement( SmartPtr<IUIElement> element )
        {
            if( element )
            {
                if( element->getSameLine() )
                {
                    ImGui::SameLine( 0, 5 );
                }

                //if( element->isDerived<IUIEventWindow>() )
                //{
                //    element->update();
                //}

                if( element->isDerived<IUIWindow>() )
                {
                    auto window = fb::static_pointer_cast<IUIWindow>( element );
                    if( window )
                    {
                        if( window->isVisible() )
                        {
                            auto label = window->getLabel();
                            if( StringUtil::isNullOrEmpty( label ) )
                            {
                                if( auto parent = window->getParent() )
                                {
                                    auto parentWindow = fb::static_pointer_cast<IUIWindow>( parent );
                                    label = parentWindow->getLabel() + String( "_Child_" ) +
                                            StringUtil::toString( m_childWindowCount++ );
                                }
                            }

                            auto border = window->hasBorder();

                            auto windowFlags = static_cast<s32>( ImGuiWindowFlags_None );
                            windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;

                            auto size = window->getSize();
                            auto size_arg = ImVec2( size.X(), size.Y() );
                            if( ImGui::BeginChild( label.c_str(), size_arg, border, windowFlags ) )
                            {
                                element->update();

                                if( auto p = element->getChildren() )
                                {
                                    auto &children = *p;
                                    for( auto child : children )
                                    {
                                        createElement( child );
                                    }

                                    const bool is_hovered = ImGui::IsItemHovered();  // Hovered
                                    const bool is_active = ImGui::IsItemActive();    // Held

                                    if( is_hovered && ImGui::IsMouseClicked( 0 ) )
                                    {
                                        auto listeners = window->getObjectListeners();
                                        for( auto listener : listeners )
                                        {
                                            auto args = Array<Parameter>();
                                            args.resize( 1 );

                                            args[0].object = window;

                                            listener->handleEvent( IEvent::Type::UI,
                                                                   IEvent::handleMouseClicked, args,
                                                                   window, this, nullptr );
                                        }
                                    }

                                    if( auto contextMenu = window->getContextMenu() )
                                    {
                                        if( ImGui::BeginPopupContextWindow(
                                                label.c_str(), ImGuiPopupFlags_MouseButtonRight ) )
                                        {
                                            auto label = contextMenu->getLabel();
                                            if( StringUtil::isNullOrEmpty( label ) )
                                            {
                                                label = "Untitled";
                                            }

                                            //if( ImGui::BeginMenu( label.c_str() ) )
                                            {
                                                auto menuItems = contextMenu->getMenuItems();
                                                for( auto menuItemElement : menuItems )
                                                {
                                                    createMenuItem( contextMenu, menuItemElement );
                                                }

                                                //ImGui::EndMenu();
                                            }

                                            ImGui::EndPopup();
                                        }
                                    }
                                }

                                // auto pos = ImVec2( 0, 0 );
                                // ImGui::SetNextWindowPos( pos );
                            }

                            ImGui::EndChild();
                        }
                    }
                }
                else
                {
                    //if( element->isDerived<IUIButton>() )
                    //{
                    //    auto button = fb::static_pointer_cast<IUIButton>( element );

                    //    auto label = button->getLabel();
                    //    if( StringUtil::isNullOrEmpty( label ) )
                    //    {
                    //        label = "Untitled";
                    //    }

                    //    if( ImGui::Button( label.c_str() ) )
                    //    {
                    //        if( auto parent = button->getParent() )
                    //        {
                    //            if( parent->isDerived<IUIToolbar>() )
                    //            {
                    //                auto toolbar = fb::static_pointer_cast<IUIToolbar>( parent );
                    //                FB_ASSERT( toolbar );

                    //                auto listeners = toolbar->getObjectListeners();
                    //                for( auto listener : listeners )
                    //                {
                    //                    auto args = Array<Parameter>();
                    //                    listener->handleEvent( IEvent::Type::UI, IEvent::handleSelection,
                    //                                           args, toolbar, button, nullptr );
                    //                }
                    //            }
                    //            else
                    //            {
                    //                auto listeners = button->getObjectListeners();
                    //                for( auto listener : listeners )
                    //                {
                    //                    auto args = Array<Parameter>();
                    //                    listener->handleEvent( IEvent::Type::UI, IEvent::handleSelection,
                    //                                           args, button, button, nullptr );
                    //                }
                    //            }
                    //        }
                    //    }
                    //}
                    //else if( element->isDerived<IUIText>() )
                    //{
                    //    auto text = fb::static_pointer_cast<IUIText>( element );
                    //    auto str = text->getText();
                    //    if( StringUtil::isNullOrEmpty( str ) )
                    //    {
                    //        str = "";
                    //    }

                    //    ImGui::Text( str.c_str() );
                    //}
                    if( element->isDerived<IUITreeCtrl>() )
                    {
                        ImGuiTreeCtrl::createElement( element );
                    }
                    else if( element->isDerived<IUILabelCheckboxPair>() )
                    {
                        auto labelCheckboxPair =
                            fb::static_pointer_cast<IUILabelCheckboxPair>( element );
                        auto label = labelCheckboxPair->getLabel();
                        auto value = labelCheckboxPair->getValue();

                        if( StringUtil::isNullOrEmpty( label ) )
                        {
                            label = "Untitled";
                        }

                        ImGui::Text( label.c_str() );

                        ImGui::SameLine( 0, 5 );

                        if( ImGuiUtil::ToggleButton( label.c_str(), &value ) )
                        //if( ImGui::Checkbox( label.c_str(), &value ) )
                        {
                            labelCheckboxPair->setValue( value );

                            auto listeners = element->getObjectListeners();
                            for( auto listener : listeners )
                            {
                                auto args = Array<Parameter>();
                                args.resize( 1 );

                                args[0].object = element;

                                listener->handleEvent( IEvent::Type::UI, IEvent::handleValueChanged,
                                                       args, element, this, nullptr );
                            }
                        }
                    }
                    else if( element->isDerived<IUILabelTextInputPair>() )
                    {
                        auto labelTextInputPair =
                            fb::static_pointer_cast<IUILabelTextInputPair>( element );
                        auto label = labelTextInputPair->getLabel();
                        auto value = labelTextInputPair->getValue();

                        if( StringUtil::isNullOrEmpty( label ) )
                        {
                            label = "Untitled";
                        }

                        constexpr s32 strSize = 256;
                        FixedString<strSize> str;
                        str = value;

                        if( ImGui::InputText( label.c_str(), (char *)str.c_str(), strSize ) )
                        {
                            labelTextInputPair->setValue( String( str.c_str() ) );

                            auto listeners = element->getObjectListeners();
                            for( auto listener : listeners )
                            {
                                auto args = Array<Parameter>();
                                args.resize( 1 );

                                args[0].object = element;

                                listener->handleEvent( IEvent::Type::UI, IEvent::handleValueChanged,
                                                       args, element, this, nullptr );
                            }
                        }
                    }
                    else if( element->isDerived<IUIDropdown>() )
                    {
                        auto dropdown = fb::static_pointer_cast<IUIDropdown>( element );
                        auto options = dropdown->getOptions();

                        Array<char *> optionsStr;
                        optionsStr.resize( options.size() );

                        for( size_t i = 0; i < options.size(); ++i )
                        {
                            optionsStr[i] = (char *)options[i].c_str();
                        }

                        auto item_current = static_cast<s32>( dropdown->getSelectedOption() );
                        auto strSize = (int)optionsStr.size();

                        if( ImGui::Combo( "combo", &item_current, &optionsStr[0], strSize ) )
                        {
                            dropdown->setSelectedOption( static_cast<u32>( item_current ) );

                            auto listeners = dropdown->getObjectListeners();
                            for( auto listener : listeners )
                            {
                                auto args = Array<Parameter>();
                                args.resize( 1 );

                                args[0] = Parameter( item_current );

                                listener->handleEvent( IEvent::Type::UI, IEvent::handleSelection, args,
                                                       element, this, nullptr );
                            }
                        }
                    }
                    else if( element->isDerived<IUIPropertyGrid>() )
                    {
                        ImGuiPropertyGrid::createElement( element );
                    }
                    else if( element->isDerived<IUIVector2>() )
                    {
                        auto vectorUI = fb::static_pointer_cast<IUIVector2>( element );

                        auto name = vectorUI->getLabel();
                        auto vector = vectorUI->getValue();

                        f32 value[2] = { 0 };

                        value[0] = vector.X();
                        value[1] = vector.Y();

                        if( ImGui::InputFloat2( name.c_str(), value ) )
                        {
                            vectorUI->setValue( Vector2( value[0], value[1] ) );

                            auto listeners = vectorUI->getObjectListeners();
                            for( auto listener : listeners )
                            {
                                auto args = Array<Parameter>();
                                args.resize( 1 );

                                listener->handleEvent( IEvent::Type::UI, IEvent::handleValueChanged,
                                                       args, element, this, nullptr );
                            }
                        }
                    }
                    else if( element->isDerived<IUIVector3>() )
                    {
                        auto vectorUI = fb::static_pointer_cast<IUIVector3>( element );

                        auto name = vectorUI->getLabel();
                        auto vector = vectorUI->getValue();

                        //if( ImGui::InputFloat3( name.c_str(), value ) )
                        if( ShowVector3( name.c_str(), vector ) )
                        {
                            vectorUI->setValue( vector );

                            auto listeners = vectorUI->getObjectListeners();
                            for( auto listener : listeners )
                            {
                                auto args = Array<Parameter>();
                                args.resize( 1 );

                                listener->handleEvent( IEvent::Type::UI, IEvent::handleValueChanged,
                                                       args, element, this, nullptr );
                            }
                        }
                    }
                    else if( element->isDerived<IUIVector4>() )
                    {
                        auto vectorUI = fb::static_pointer_cast<IUIVector4>( element );

                        auto name = vectorUI->getLabel();
                        auto vector = vectorUI->getValue();

                        f32 value[4] = { 0 };

                        value[0] = vector.X();
                        value[1] = vector.Y();
                        value[2] = vector.Z();
                        value[3] = vector.W();

                        if( ImGui::InputFloat4( name.c_str(), value ) )
                        {
                            vectorUI->setValue( Vector4( value[0], value[1], value[2], value[3] ) );

                            auto listeners = vectorUI->getObjectListeners();
                            for( auto listener : listeners )
                            {
                                auto args = Array<Parameter>();
                                args.resize( 1 );

                                listener->handleEvent( IEvent::Type::UI, IEvent::handleValueChanged,
                                                       args, element, this, nullptr );
                            }
                        }
                    }
                    else if( element->isDerived<IUIToolbar>() )
                    {
                        auto toolbar = fb::static_pointer_cast<IUIToolbar>( element );

                        static ImGuiAxis toolbar1_axis = ImGuiAxis_X;
                        DockingToolbar( "Toolbar1", &toolbar1_axis, toolbar );
                        return;
                    }
                    else if( element->isDerived<IUITerrainEditor>() )
                    {
                        element->update();
                    }
                    else if( element->isDerived<IUIImage>() )
                    {
                        element->update();
                    }
                    else
                    {
                        element->update();
                    }

                    if( auto p = element->getChildren() )
                    {
                        auto &children = *p;
                        for( auto child : children )
                        {
                            createElement( child );
                        }
                    }
                }
            }
        }

        void ImGuiApplication::TestDoc()
        {
            const bool enableDocking = ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable;
            if( enableDocking )
            {
                ImGuiViewport *viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos( viewport->WorkPos );
                ImGui::SetNextWindowSize( viewport->WorkSize );
                ImGui::SetNextWindowViewport( viewport->ID );

                ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
                ImGuiWindowFlags host_window_flags = 0;
                host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoDocking;
                host_window_flags |=
                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
                // if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                //	host_window_flags |= ImGuiWindowFlags_NoBackground;

                ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
                ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
                ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
                ImGui::Begin( "DockSpace Window", nullptr, host_window_flags );
                ImGui::PopStyleVar( 3 );

                ImGuiID dockspace_id = ImGui::GetID( "DockSpace" );
                ImGui::DockSpace( dockspace_id, ImVec2( 0.0f, 0.0f ), dockspace_flags, nullptr );
                ImGui::End();
            }
        }

        void ImGuiApplication::ShowApp( bool *p_open )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto selectionManager = applicationManager->getSelectionManager();
            // FB_ASSERT( selectionManager );

            auto ui = fb::static_pointer_cast<ImGuiManager>( applicationManager->getUI() );
            FB_ASSERT( ui );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto sceneManager = graphicsSystem->getGraphicsScene();
            FB_ASSERT( sceneManager );

            auto window = graphicsSystem->getDefaultWindow();
            FB_ASSERT( window );

            m_childWindowCount = 0;

            // auto camera = sceneManager->getCamera( "CameraComponent0" );
            // auto viewMatrix = camera->getViewMatrix().transpose();
            // auto projectionMatrix = camera->getProjectionMatrix().transpose();

            // auto viewMatrix = camera->getViewMatrix();
            // auto projectionMatrix = camera->getProjectionMatrix();

            // If you strip some features of, this demo is pretty much equivalent to calling
            // DockSpaceOverViewport()! In most cases you should be able to just call
            // DockSpaceOverViewport() and ignore all the code below! In this specific demo, we are not
            // using DockSpaceOverViewport() because:
            // - we allow the host window to be floating/moveable instead of filling the viewport (when
            // opt_fullscreen == false)
            // - we allow the host window to have padding (when opt_padding == true)
            // - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() +
            // DockSpaceOverViewport() in your code!) TL;DR; this demo is more complicated than what you
            // would normally use. If we removed all the options we are showcasing, this demo would
            // become:
            //     void ShowExampleAppDockSpace()
            //     {
            //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
            //     }

            static bool opt_fullscreen = true;
            static bool opt_padding = false;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable
            // into, because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if( opt_fullscreen )
            {
                const ImGuiViewport *viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos( viewport->WorkPos );
                ImGui::SetNextWindowSize( viewport->WorkSize );
                ImGui::SetNextWindowViewport( viewport->ID );
                ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
                ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }
            else
            {
                // dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
            // and handle the pass-thru hole, so we ask Begin() to not render a background.
            // if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            //{
            //	window_flags |= ImGuiWindowFlags_NoBackground;
            //}

            // window_flags |= ImGuiWindowFlags_NoBackground;

            if( dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode )
            {
                window_flags |= ImGuiWindowFlags_NoBackground;
            }

            dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

            ImGuiWindowFlags host_window_flags = 0;
            host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoDocking;
            host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            if( dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode )
            {
                host_window_flags |= ImGuiWindowFlags_NoBackground;
            }

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive
            // docking, otherwise any change of dockspace/settings would lead to windows being stuck in
            // limbo and never being visible.
            if( !opt_padding )
            {
                ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
            }

            ImGui::Begin( "Fireblade", p_open, window_flags );
            ImGuizmo::BeginFrame();

            try
            {
                if( !opt_padding )
                {
                    ImGui::PopStyleVar();
                }

                if( opt_fullscreen )
                {
                    ImGui::PopStyleVar( 2 );
                }

                auto dockspace_id = ImGui::GetID( "Fireblade" );
                auto dockspace_node_id = ImGui::GetID( "docktoolbar" );

                //ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

                // Submit the DockSpace
                ImGuiIO &io = ImGui::GetIO();
                if( io.ConfigFlags & ImGuiConfigFlags_DockingEnable )
                {
                    ImGui::DockSpace( dockspace_id, ImVec2( 0.0f, 0.0f ), dockspace_flags );
                    auto node = ImGui::DockBuilderGetNode( dockspace_id );
                }
                else
                {
                    //ShowDockingDisabledMessage();
                }

                auto dockLeft_id = ImGui::GetID( "docktoolbar" );
                auto node = ImGui::DockBuilderGetNode( dockspace_id );
                if( !node->IsSplitNode() )
                {
                    auto dockUp = ImGui::DockBuilderSplitNode( dockspace_id, ImGuiDir_Down, 0.80f,
                                                               &dockLeft_id_up, &dockLeft_id_down );

                    auto dockDown = ImGui::DockBuilderSplitNode( dockUp, ImGuiDir_Left, 0.60f,
                                                                 &dockLeft_id_left, &dockLeft_id_right );
                }

                auto menuBar = getMenubar();
                if( menuBar )
                {
                    if( ImGui::BeginMenuBar() )
                    {
                        try
                        {
                            auto menus = menuBar->getMenus();
                            for( auto menu : menus )
                            {
                                auto label = menu->getLabel();

                                if( ImGui::BeginMenu( label.c_str() ) )
                                {
                                    try
                                    {
                                        auto menuItemElements = menu->getMenuItems();
                                        for( auto menuItemElement : menuItemElements )
                                        {
                                            createMenuItem( menu, menuItemElement );
                                        }
                                    }
                                    catch( std::exception &e )
                                    {
                                        FB_LOG_EXCEPTION( e );
                                    }

                                    ImGui::EndMenu();
                                }
                            }
                        }
                        catch( std::exception &e )
                        {
                            FB_LOG_EXCEPTION( e );
                        }
                    }

                    ImGui::EndMenuBar();
                }

                try
                {
                    auto toolbar = getToolbar();
                    if( toolbar )
                    {
                        ImGui::SetNextWindowDockID( dockLeft_id_up, ImGuiCond_FirstUseEver );
                        createElement( toolbar );
                    }
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }

                // DockSpaceUI();
                // ToolbarUI();

                // static ImGuiAxis toolbar1_axis = ImGuiAxis_X;
                // DockingToolbar("Toolbar1", &toolbar1_axis);

                auto matId = 0;
                // EditTransform(viewMatrix.ptr(), projectionMatrix.ptr(), objectMatrix[matId], false);

                // auto matrix = (float*) & objectMatrix[matId];
                // ImGuizmo::Manipulate(viewMatrix.ptr(), projectionMatrix.ptr(), ImGuizmo::TRANSLATE,
                //	ImGuizmo::LOCAL, matrix, NULL);

                auto renderWindows = ui->getRenderWindows();
                for( auto renderWindow : renderWindows )
                {
                    auto visible = renderWindow->isVisible();
                    if( visible )
                    {
                        if( !renderWindow->isDocked() )
                        {
                            ImGui::SetNextWindowDockID( dockLeft_id_left, ImGuiCond_FirstUseEver );
                            renderWindow->setDocked( true );
                        }

                        auto label = renderWindow->getLabel();
                        if( StringUtil::isNullOrEmpty( label ) )
                        {
                            label = "Untitled";
                        }

#if FB_GRAPHICS_SYSTEM_OGRENEXT

                        if( ImGui::Begin( label.c_str(), &visible, ImGuiNextWindowDataFlags_HasSize ) )
                        {
                            renderWindow->setVisible( visible, false );

                            draw( renderWindow );

                            // ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Appearing);
                            // ImGui::SetNextWindowPos(ImVec2(400, 20), ImGuiCond_Appearing);
                            // ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));
                            ImGuizmo::SetDrawlist();
                            float windowWidth = ImGui::GetWindowWidth();
                            float windowHeight = ImGui::GetWindowHeight();
                            ImGuizmo::SetRect( ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
                                               windowWidth, windowHeight );
                            // viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
                            // viewManipulateTop = ImGui::GetWindowPos().y;
                            auto window = ImGui::GetCurrentWindow();

                            // ImGui::GetWindowDrawList()->AddImage(
                            //	(void*)texture, ImVec2(0,0),
                            //	ImVec2(1,1), ImVec2(0, 1), ImVec2(1, 0));

                            //                        auto renderTexture = renderWindow->getRenderTexture();
                            //                        if( renderTexture )
                            //                        {
                            //                            auto pRenderTexture =
                            //                                fb::static_pointer_cast<render::CTextureOgre>(
                            //                                renderTexture );
                            //                            //// todo hack
                            //                            auto ogreTexture = pRenderTexture->getTexture();
                            //
                            //#    if defined FB_PLATFORM_WIN32
                            //#        if FB_BUILD_RENDERER_DX11
                            //                            auto glTexture = static_cast<Ogre::D3D11TextureGpu
                            //                            *>( ogreTexture ); auto tex =
                            //                            glTexture->getDefaultDisplaySrv();
                            //#        elif FB_BUILD_RENDERER_OPENGL
                            //                            auto glTexture = (Ogre::GL3PlusTextureGpu
                            //                            *)ogreTexture; auto tex =
                            //                            glTexture->getFinalTextureName();
                            //#        endif
                            //
                            //#    elif defined FB_PLATFORM_APPLE
                            //#        if FB_BUILD_RENDERER_METAL
                            //                            // auto glTexture =
                            //                            static_cast<Ogre::MetalTextureGpu *>( ogreTexture
                            //                            );
                            //                            // auto tex = glTexture->getFinalTextureName();
                            //#        elif FB_BUILD_RENDERER_OPENGL
                            //                            auto glTexture = (Ogre::GL3PlusTextureGpu
                            //                            *)ogreTexture; auto tex =
                            //                            glTexture->getFinalTextureName();
                            //#        endif
                            //#    endif
                            //
                            //                            // if (!ogreTexture->isDataReady())
                            //                            //{
                            //                            //	ogreTexture->_syncGpuResidentToSystemRam();
                            //                            // }
                            //
                            //                            ////Ogre::TextureBox srcBox(512, 512, 32, 1, 4,
                            //                            128, 4096);
                            //                            // Ogre::TextureBox dstBox();
                            //                            ////Ogre::PixelFormatGpu dstFormat =
                            //                            Ogre::PixelFormatGpu::PFG_A8P8;
                            //                            ////ogreTexture->copyContentsToMemory(srcBox,
                            //                            dstBox);
                            //
                            //                            // auto textureBox =
                            //                            ogreTexture->_getSysRamCopyAsBox(0);
                            //
                            //                            // int pitch = 0;
                            //                            // u8* pixels = NULL;
                            //                            ////SDL_LockTexture(texture, NULL, (void**)&pixels,
                            //                            &pitch);
                            //
                            //                            // auto src = (u8*)textureBox.data;
                            //
                            //                            // SDL_UnlockTexture(texture);
                            //
                            //#    if defined FB_PLATFORM_WIN32
                            //                            ImVec2 size = ImGui::GetWindowSize();
                            //                            ImVec2 avail_size = ImGui::GetContentRegionAvail();
                            //
                            //                            ImGui::Image( tex, avail_size );
                            //
                            //                            renderTexture->setSize( Vector2I( avail_size.x,
                            //                            avail_size.y ) );
                            //
                            //                            // ImGui::GetWindowDrawList()->AddImage((void*)tex,
                            //                            ImVec2(0, 0),ImVec2(1, 1));
                            //#    endif
                            //                        }

                            // ImGui::Image((void*)texture, ImVec2(128, 128));

                            auto matrix = (float *)&objectMatrix[matId];

                            // ImGuizmo::DrawGrid(viewMatrix.ptr(), projectionMatrix.ptr(), identityMatrix,
                            // 100.f); ImGuizmo::DrawCubes(viewMatrix.ptr(), projectionMatrix.ptr(),
                            // &objectMatrix[0][0], gizmoCount);

                            // if (ImGuizmo::Manipulate(viewMatrix.ptr(), projectionMatrix.ptr(),
                            // ImGuizmo::TRANSLATE, ImGuizmo::WORLD, matrix, NULL))
                            //{
                            //	auto position = Vector3F::zero();
                            //	auto scale = Vector3F::zero();
                            //	auto rotation = QuaternionF::identity();

                            //	auto transform = Matrix4F(matrix);
                            //	transform = transform.transpose();
                            //	transform.decomposition(position, scale, rotation);

                            //	auto selection = selectionManager->getSelection();
                            //	for (auto selected : selection)
                            //	{
                            //		if (selected->isDerived<IActor>())
                            //		{
                            //			auto actor = fb::static_pointer_cast<IActor>(selected);

                            //			actor->setPosition(position);
                            //			actor->setScale(scale);
                            //			actor->setOrientation(rotation);
                            //		}
                            //	}
                            //}

                            // if (m_showEditor)
                            //{
                            //	ShowEditor(&m_showEditor);
                            // }

                            // ImGui::PopStyleColor(1);
                        }

                        ImGui::End();
#elif FB_GRAPHICS_SYSTEM_OGRE
                        if( ImGui::Begin( label.c_str(), &visible, ImGuiNextWindowDataFlags_HasSize ) )
                        {
                            try
                            {
                                renderWindow->setVisible( visible, false );

                                draw( renderWindow );
                            }
                            catch( std::exception &e )
                            {
                                FB_LOG_EXCEPTION( e );
                            }
                        }

                        ImGui::End();
#endif
                    }
                }

                auto windows = ui->getWindows();
                for( auto window : windows )
                {
                    if( window->isVisible() )
                    {
                        if( !window->isDocked() )
                        {
                            ImGui::SetNextWindowDockID( dockLeft_id_right, ImGuiCond_FirstUseEver );
                            window->setDocked( true );
                        }

                        auto windowParent = window->getParent();
                        if( windowParent == nullptr )
                        {
                            auto label = window->getLabel();
                            if( StringUtil::isNullOrEmpty( label ) )
                            {
                                label = "Untitled";
                            }

                            auto windowFlags = static_cast<s32>( ImGuiWindowFlags_None );
                            windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;

                            auto visible = window->isVisible();
                            if( ImGui::Begin( label.c_str(), &visible, windowFlags ) )
                            {
                                try
                                {
                                    if( auto p = window->getChildren() )
                                    {
                                        auto &children = *p;
                                        for( auto child : children )
                                        {
                                            createElement( child );
                                        }
                                    }

                                    auto dropTarget = window->getDropTarget();
                                    if( dropTarget )
                                    {
                                        if( ImGui::BeginDragDropTarget() )
                                        {
                                            auto payload = ImGui::AcceptDragDropPayload( "_TREENODE" );
                                            if( payload )
                                            {
                                                auto pData = static_cast<const char *>( payload->Data );
                                                auto dataSize = payload->DataSize;
                                                auto data = String( pData, dataSize );

                                                auto args = Array<Parameter>();
                                                args.reserve( 1 );

                                                args.push_back( Parameter( data ) );

                                                dropTarget->handleEvent( IEvent::Type::UI,
                                                                         IEvent::handleDrop, args,
                                                                         window, window, nullptr );
                                            }

                                            ImGui::EndDragDropTarget();
                                        }
                                    }

                                    if( auto contextMenu = window->getContextMenu() )
                                    {
                                        if( ImGui::BeginPopupContextWindow(
                                                label.c_str(), ImGuiPopupFlags_MouseButtonRight ) )
                                        {
                                            try
                                            {
                                                auto label = contextMenu->getLabel();
                                                if( StringUtil::isNullOrEmpty( label ) )
                                                {
                                                    label = "Untitled";
                                                }

                                                //if( ImGui::BeginMenu( label.c_str() ) )
                                                {
                                                    try
                                                    {
                                                        auto menuItems = contextMenu->getMenuItems();
                                                        for( auto menuItemElement : menuItems )
                                                        {
                                                            createMenuItem( contextMenu,
                                                                            menuItemElement );
                                                        }
                                                    }
                                                    catch( std::exception &e )
                                                    {
                                                        FB_LOG_EXCEPTION( e );
                                                    }

                                                    //ImGui::EndMenu();
                                                }
                                            }
                                            catch( std::exception &e )
                                            {
                                                FB_LOG_EXCEPTION( e );
                                            }

                                            ImGui::EndPopup();
                                        }
                                    }

                                    const bool is_hovered = ImGui::IsItemHovered();  // Hovered
                                    const bool is_active = ImGui::IsItemActive();    // Held

                                    if( is_hovered && ImGui::IsMouseClicked( 0 ) )
                                    {
                                        auto listeners = window->getObjectListeners();
                                        for( auto listener : listeners )
                                        {
                                            auto args = Array<Parameter>();
                                            args.resize( 1 );

                                            args[0].object = window;

                                            listener->handleEvent( IEvent::Type::UI,
                                                                   IEvent::handleMouseClicked, args,
                                                                   window, this, nullptr );
                                        }
                                    }

                                    window->setVisible( visible, false );
                                }
                                catch( std::exception &e )
                                {
                                    FB_LOG_EXCEPTION( e );
                                }
                            }

                            ImGui::End();
                        }
                    }
                }

                try
                {
                    auto fileDialogs = ui->getFileBrowsers();
                    for( auto fileDialog : fileDialogs )
                    {
                        fileDialog->update();
                    }
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }

                // if (m_showEditor)
                //{
                //	ShowEditor(&m_showEditor);
                // }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            ImGui::End();
        }

        void ImGuiApplication::draw( SmartPtr<IUIRenderWindow> renderWindow )
        {
            try
            {
                if( auto renderTexture = renderWindow->getRenderTexture() )
                {
#if FB_GRAPHICS_SYSTEM_OGRENEXT
                    //static size_t pTexture = 0;
                    void *pTexture = nullptr;
                    // pTexture = renderTexture->getTextureHandle();
                    renderTexture->getTextureFinal( &pTexture );

                    if( pTexture )
                    {
                        auto pos = ImGui::GetWindowPos();
                        auto size = ImGui::GetWindowSize();
                        auto avail_size = ImGui::GetContentRegionAvail();

                        ImGui::Image( pTexture, avail_size );

                        auto textureSize = Vector2I( static_cast<s32>( avail_size.x ),
                                                     static_cast<s32>( avail_size.y ) );
                        renderTexture->setSize( textureSize );

                        renderWindow->setPosition( Vector2F( pos.x, pos.y ) );
                        renderWindow->setSize( Vector2F( textureSize.x, textureSize.y ) );
                    }
#elif FB_GRAPHICS_SYSTEM_OGRE
                    static size_t iTexture = 0;
                    //pTexture = renderTexture->getTextureHandle();

                    iTexture = renderTexture->getTextureHandle();
                    auto pTexture = reinterpret_cast<ImTextureID>( &iTexture );

                    if( pTexture )
                    {
                        auto pos = ImGui::GetWindowPos();
                        auto size = ImGui::GetWindowSize();
                        auto avail_size = ImGui::GetContentRegionAvail();

                        ImGui::Image( pTexture, avail_size );

                        auto textureSize = Vector2I( static_cast<s32>( avail_size.x ),
                                                     static_cast<s32>( avail_size.y ) );
                        renderTexture->setSize( textureSize );

                        renderWindow->setPosition( Vector2F( pos.x, pos.y ) );
                        renderWindow->setSize( Vector2F( (f32)textureSize.x, (f32)textureSize.y ) );
                    }
#endif
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void *ImGuiApplication::getEmptyTexture() const
        {
            return m_emptyTexture;
        }

        void ImGuiApplication::setEmptyTexture( void *emptyTexture )
        {
            m_emptyTexture = emptyTexture;
        }

        ImGuiOverlayOgre *ImGuiApplication::getOverlay()
        {
            return m_overlay;
        }

        void ImGuiApplication::createMenuItem( SmartPtr<IUIMenu> rootMenu,
                                               SmartPtr<IUIElement> menuItemElement )
        {
            if( menuItemElement->isDerived<IUIMenu>() )
            {
                auto menu = fb::static_pointer_cast<IUIMenu>( menuItemElement );
                auto menuLabel = menu->getLabel();

                if( ImGui::BeginMenu( menuLabel.c_str() ) )
                {
                    auto subMenuElements = menu->getMenuItems();

                    for( auto subMenuElement : subMenuElements )
                    {
                        if( subMenuElement->isDerived<IUIMenu>() )
                        {
                            auto subMenu = fb::static_pointer_cast<IUIMenu>( subMenuElement );

                            auto label = subMenu->getLabel();
                            if( StringUtil::isNullOrEmpty( label ) )
                            {
                                label = "Untitled";
                            }

                            if( ImGui::BeginMenu( label.c_str() ) )
                            {
                                auto menuItemElements = subMenu->getMenuItems();
                                for( auto menuItemElement : menuItemElements )
                                {
                                    createMenuItem( rootMenu, menuItemElement );
                                }

                                ImGui::EndMenu();
                            }

                            createMenuItem( rootMenu, subMenu );
                        }
                        else if( subMenuElement->isDerived<IUIMenuItem>() )
                        {
                            createMenuItem( rootMenu, subMenuElement );
                        }
                    }

                    ImGui::EndMenu();
                }
            }
            else if( menuItemElement->isDerived<IUIMenuItem>() )
            {
                auto menuItem = fb::static_pointer_cast<IUIMenuItem>( menuItemElement );
                auto menuItemType = menuItem->getMenuItemType();
                switch( menuItemType )
                {
                case IUIMenuItem::Type::Normal:
                {
                    auto text = menuItem->getText();

                    auto selected = false;
                    ImGui::MenuItem( text.c_str(), nullptr, &selected );

                    if( selected )
                    {
                        auto parent = rootMenu->getParent();
                        if( parent && parent->isDerived<IUIMenubar>() )
                        {
                            auto menuBar = fb::static_pointer_cast<IUIMenubar>( parent );
                            auto listeners = menuBar->getObjectListeners();
                            for( auto &menuListener : listeners )
                            {
                                menuListener->handleEvent( IEvent::Type::UI, IEvent::handleSelection,
                                                           Array<Parameter>(), rootMenu, menuItem,
                                                           nullptr );
                            }
                        }
                        else
                        {
                            auto listeners = rootMenu->getObjectListeners();
                            for( auto &menuListener : listeners )
                            {
                                menuListener->handleEvent( IEvent::Type::UI, IEvent::handleSelection,
                                                           Array<Parameter>(), rootMenu, menuItem,
                                                           nullptr );
                            }
                        }
                    }
                }
                break;
                case IUIMenuItem::Type::Separator:
                {
                    ImGui::Separator();
                }
                break;
                default:
                {
                }
                }
            }
        }

        Vector2I ImGuiApplication::getWindowSize() const
        {
            return Vector2I::zero();
        }

        void ImGuiApplication::setWindowSize( const Vector2I &size )
        {
            if( m_size != size )
            {
                m_size = size;

                auto viewport = ImGui::GetMainViewport();
                // ImGui_ImplDX11_SetWindowSize(viewport, ImVec2(size.X(), size.Y()));

                // ImGui_ImplDX11_InvalidateDeviceObjects();
                // ImGui_ImplDX11_CreateDeviceObjects();
            }
        }

#if FB_BUILD_SDL2
        SDL_Texture *ImGuiApplication::IMG_LoadTexture( SDL_Renderer *renderer, const char *file )
        {
            SDL_Texture *texture = NULL;
            SDL_Surface *surface = IMG_Load( file );
            if( surface )
            {
                texture = SDL_CreateTextureFromSurface( renderer, surface );
                SDL_FreeSurface( surface );
            }
            return texture;
        }
#endif

        static ImGuizmo::OPERATION mCurrentGizmoOperation( ImGuizmo::TRANSLATE );

        // Camera projection
        bool isPerspective = true;
        float fov = 27.f;
        float viewWidth = 10.f;  // for orthographic
        float camYAngle = 165.f / 180.f * 3.14159f;
        float camXAngle = 32.f / 180.f * 3.14159f;

        bool firstFrame = true;

        void ImGuiApplication::EditTransform( float *cameraView, float *cameraProjection, float *matrix,
                                              bool editTransformDecomposition )
        {
            static ImGuizmo::MODE mCurrentGizmoMode( ImGuizmo::LOCAL );
            static bool useSnap = false;
            static float snap[3] = { 1.f, 1.f, 1.f };
            static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
            static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
            static bool boundSizing = false;
            static bool boundSizingSnap = false;

            if( editTransformDecomposition )
            {
                if( ImGui::IsKeyPressed( 90 ) )
                    mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
                if( ImGui::IsKeyPressed( 69 ) )
                    mCurrentGizmoOperation = ImGuizmo::ROTATE;
                if( ImGui::IsKeyPressed( 82 ) )  // r Key
                    mCurrentGizmoOperation = ImGuizmo::SCALE;
                if( ImGui::RadioButton( "Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE ) )
                    mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
                ImGui::SameLine();
                if( ImGui::RadioButton( "Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE ) )
                    mCurrentGizmoOperation = ImGuizmo::ROTATE;
                ImGui::SameLine();
                if( ImGui::RadioButton( "Scale", mCurrentGizmoOperation == ImGuizmo::SCALE ) )
                    mCurrentGizmoOperation = ImGuizmo::SCALE;
                if( ImGui::RadioButton( "Universal", mCurrentGizmoOperation == ImGuizmo::UNIVERSAL ) )
                    mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;
                float matrixTranslation[3], matrixRotation[3], matrixScale[3];
                ImGuizmo::DecomposeMatrixToComponents( matrix, matrixTranslation, matrixRotation,
                                                       matrixScale );
                ImGui::InputFloat3( "Tr", matrixTranslation );
                ImGui::InputFloat3( "Rt", matrixRotation );
                ImGui::InputFloat3( "Sc", matrixScale );
                ImGuizmo::RecomposeMatrixFromComponents( matrixTranslation, matrixRotation, matrixScale,
                                                         matrix );

                if( mCurrentGizmoOperation != ImGuizmo::SCALE )
                {
                    if( ImGui::RadioButton( "Local", mCurrentGizmoMode == ImGuizmo::LOCAL ) )
                        mCurrentGizmoMode = ImGuizmo::LOCAL;
                    ImGui::SameLine();
                    if( ImGui::RadioButton( "World", mCurrentGizmoMode == ImGuizmo::WORLD ) )
                        mCurrentGizmoMode = ImGuizmo::WORLD;
                }
                if( ImGui::IsKeyPressed( 83 ) )
                    useSnap = !useSnap;

                // return;
                // ImGui::Checkbox("", &useSnap);
                ImGui::SameLine();

                switch( mCurrentGizmoOperation )
                {
                case ImGuizmo::TRANSLATE:
                    ImGui::InputFloat3( "Snap", &snap[0] );
                    break;
                case ImGuizmo::ROTATE:
                    ImGui::InputFloat( "Angle Snap", &snap[0] );
                    break;
                case ImGuizmo::SCALE:
                    ImGui::InputFloat( "Scale Snap", &snap[0] );
                    break;
                }
                ImGui::Checkbox( "Bound Sizing", &boundSizing );
                if( boundSizing )
                {
                    ImGui::PushID( 3 );
                    ImGui::Checkbox( "", &boundSizingSnap );
                    ImGui::SameLine();
                    ImGui::InputFloat3( "Snap", boundsSnap );
                    ImGui::PopID();
                }
            }

            ImGuiIO &io = ImGui::GetIO();
            float viewManipulateRight = io.DisplaySize.x;
            float viewManipulateTop = 0;
            static ImGuiWindowFlags gizmoWindowFlags = 0;
            if( useWindow )
            {
                ImGui::SetNextWindowSize( ImVec2( 800, 400 ), ImGuiCond_Appearing );
                ImGui::SetNextWindowPos( ImVec2( 400, 20 ), ImGuiCond_Appearing );
                ImGui::PushStyleColor( ImGuiCol_WindowBg,
                                       static_cast<ImVec4>( ImColor( 0.35f, 0.3f, 0.3f ) ) );
                ImGui::Begin( "Gizmo", nullptr, gizmoWindowFlags );
                ImGuizmo::SetDrawlist();
                float windowWidth = ImGui::GetWindowWidth();
                float windowHeight = ImGui::GetWindowHeight();
                ImGuizmo::SetRect( ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth,
                                   windowHeight );
                viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
                viewManipulateTop = ImGui::GetWindowPos().y;
                auto window = ImGui::GetCurrentWindow();
                gizmoWindowFlags =
                    ImGui::IsWindowHovered() &&
                            ImGui::IsMouseHoveringRect( window->InnerRect.Min, window->InnerRect.Max )
                        ? ImGuiWindowFlags_NoMove
                        : 0;
            }
            else
            {
                ImGuizmo::SetRect( 0, 0, io.DisplaySize.x, io.DisplaySize.y );
            }

            ImGuizmo::DrawGrid( cameraView, cameraProjection, identityMatrix, 100.f );
            ImGuizmo::DrawCubes( cameraView, cameraProjection, &objectMatrix[0][0], gizmoCount );
            Manipulate( cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix,
                        nullptr, useSnap ? &snap[0] : nullptr, boundSizing ? bounds : nullptr,
                        boundSizingSnap ? boundsSnap : nullptr );

            ImGuizmo::ViewManipulate( cameraView, camDistance,
                                      ImVec2( viewManipulateRight - 128, viewManipulateTop ),
                                      ImVec2( 128, 128 ), 0x10101010 );

            if( useWindow )
            {
                ImGui::End();
                ImGui::PopStyleColor( 1 );
            }
        }

        void ImGuiApplication::ShowEditor( bool *p_open )
        {
            // create a window and insert the inspector
            ImGui::SetNextWindowPos( ImVec2( 10, 10 ), ImGuiCond_Appearing );
            ImGui::SetNextWindowSize( ImVec2( 320, 340 ), ImGuiCond_Appearing );
            ImGui::Begin( "Editor" );

            if( ImGui::RadioButton( "Full view", !useWindow ) )
            {
                useWindow = false;
            }

            ImGui::SameLine();

            if( ImGui::RadioButton( "Window", useWindow ) )
            {
                useWindow = true;
            }

            ImGui::Text( "Camera" );
            bool viewDirty = false;

            if( ImGui::RadioButton( "Perspective", isPerspective ) )
                isPerspective = true;

            ImGui::SameLine();

            if( ImGui::RadioButton( "Orthographic", !isPerspective ) )
                isPerspective = false;

            if( isPerspective )
            {
                ImGui::SliderFloat( "Fov", &fov, 20.f, 110.f );
            }
            else
            {
                ImGui::SliderFloat( "Ortho width", &viewWidth, 1, 20 );
            }

            viewDirty |= ImGui::SliderFloat( "Distance", &camDistance, 1.f, 10.f );
            ImGui::SliderInt( "Gizmo count", &gizmoCount, 1, 4 );

            if( viewDirty || firstFrame )
            {
                float eye[] = { cosf( camYAngle ) * cosf( camXAngle ) * camDistance,
                                sinf( camXAngle ) * camDistance,
                                sinf( camYAngle ) * cosf( camXAngle ) * camDistance };
                float at[] = { 0.f, 0.f, 0.f };
                float up[] = { 0.f, 1.f, 0.f };
                LookAt( eye, at, up, cameraView );
                firstFrame = false;
            }

            // ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);

            if( ImGuizmo::IsUsing() )
            {
                ImGui::Text( "Using gizmo" );
            }
            else
            {
                ImGui::Text( ImGuizmo::IsOver() ? "Over gizmo" : "" );
                ImGui::SameLine();
                ImGui::Text( IsOver( ImGuizmo::TRANSLATE ) ? "Over translate gizmo" : "" );
                ImGui::SameLine();
                ImGui::Text( IsOver( ImGuizmo::ROTATE ) ? "Over rotate gizmo" : "" );
                ImGui::SameLine();
                ImGui::Text( IsOver( ImGuizmo::SCALE ) ? "Over scale gizmo" : "" );
            }

            ImGui::Separator();

            for( int matId = 0; matId < gizmoCount; matId++ )
            {
                ImGuizmo::SetID( matId );

                EditTransform( cameraView, cameraProjection, objectMatrix[matId], lastUsing == matId );
                if( ImGuizmo::IsUsing() )
                {
                    lastUsing = matId;
                }
            }

            ImGui::End();
        }

        void ImGuiApplication::ShowGuizmo()
        {
            ImGuiIO &io = ImGui::GetIO();
            if( isPerspective )
            {
                Perspective( fov, io.DisplaySize.x / io.DisplaySize.y, 0.1f, 100.f, cameraProjection );
            }
            else
            {
                float viewHeight = viewWidth * io.DisplaySize.y / io.DisplaySize.x;
                OrthoGraphic( -viewWidth, viewWidth, -viewHeight, viewHeight, 1000.f, -1000.f,
                              cameraProjection );
            }

            ImGuizmo::SetOrthographic( !isPerspective );
            ImGuizmo::BeginFrame();

            ImGui::SetNextWindowPos( ImVec2( 1024, 100 ), ImGuiCond_Appearing );
            ImGui::SetNextWindowSize( ImVec2( 256, 256 ), ImGuiCond_Appearing );

            // create a window and insert the inspector
            ImGui::SetNextWindowPos( ImVec2( 10, 10 ), ImGuiCond_Appearing );
            ImGui::SetNextWindowSize( ImVec2( 320, 340 ), ImGuiCond_Appearing );
            ImGui::Begin( "Editor" );

            if( ImGui::RadioButton( "Full view", !useWindow ) )
                useWindow = false;

            ImGui::SameLine();

            if( ImGui::RadioButton( "Window", useWindow ) )
                useWindow = true;

            ImGui::Text( "Camera" );
            bool viewDirty = false;
            if( ImGui::RadioButton( "Perspective", isPerspective ) )
                isPerspective = true;
            ImGui::SameLine();
            if( ImGui::RadioButton( "Orthographic", !isPerspective ) )
                isPerspective = false;
            if( isPerspective )
            {
                ImGui::SliderFloat( "Fov", &fov, 20.f, 110.f );
            }
            else
            {
                ImGui::SliderFloat( "Ortho width", &viewWidth, 1, 20 );
            }
            viewDirty |= ImGui::SliderFloat( "Distance", &camDistance, 1.f, 10.f );
            ImGui::SliderInt( "Gizmo count", &gizmoCount, 1, 4 );

            if( viewDirty || firstFrame )
            {
                float eye[] = { cosf( camYAngle ) * cosf( camXAngle ) * camDistance,
                                sinf( camXAngle ) * camDistance,
                                sinf( camYAngle ) * cosf( camXAngle ) * camDistance };
                float at[] = { 0.f, 0.f, 0.f };
                float up[] = { 0.f, 1.f, 0.f };
                LookAt( eye, at, up, cameraView );
                firstFrame = false;
            }

            ImGui::Text( "X: %f Y: %f", io.MousePos.x, io.MousePos.y );
            if( ImGuizmo::IsUsing() )
            {
                ImGui::Text( "Using gizmo" );
            }
            else
            {
                ImGui::Text( ImGuizmo::IsOver() ? "Over gizmo" : "" );
                ImGui::SameLine();
                ImGui::Text( IsOver( ImGuizmo::TRANSLATE ) ? "Over translate gizmo" : "" );
                ImGui::SameLine();
                ImGui::Text( IsOver( ImGuizmo::ROTATE ) ? "Over rotate gizmo" : "" );
                ImGui::SameLine();
                ImGui::Text( IsOver( ImGuizmo::SCALE ) ? "Over scale gizmo" : "" );
            }

            ImGui::Separator();

            for( int matId = 0; matId < gizmoCount; matId++ )
            {
                ImGuizmo::SetID( matId );

                EditTransform( cameraView, cameraProjection, objectMatrix[matId], lastUsing == matId );
                if( ImGuizmo::IsUsing() )
                {
                    lastUsing = matId;
                }
            }

            ImGui::End();

            ImGui::SetNextWindowPos( ImVec2( 10, 350 ), ImGuiCond_Appearing );

            ImGui::SetNextWindowSize( ImVec2( 940, 480 ), ImGuiCond_Appearing );
            // ImGui::Begin("Other controls");
            // if (ImGui::CollapsingHeader("Zoom Slider"))
            //{
            //	static float uMin = 0.4f, uMax = 0.6f;
            //	static float vMin = 0.4f, vMax = 0.6f;
            //	ImGui::Image((ImTextureID)(uint64_t)procTexture, ImVec2(900, 300), ImVec2(uMin, vMin),
            // ImVec2(uMax, vMax));
            //	{
            //		ImGui::SameLine();
            //		ImGui::PushID(18);
            //		ImZoomSlider::ImZoomSlider(0.f, 1.f, vMin, vMax, 0.01f,
            // ImZoomSlider::ImGuiZoomSliderFlags_Vertical); 		ImGui::PopID();
            //	}

            //	{
            //		ImGui::PushID(19);
            //		ImZoomSlider::ImZoomSlider(0.f, 1.f, uMin, uMax);
            //		ImGui::PopID();
            //	}
            //}
            // if (ImGui::CollapsingHeader("Sequencer"))
            //{
            //	// let's create the sequencer
            //	static int selectedEntry = -1;
            //	static int firstFrame = 0;
            //	static bool expanded = true;
            //	static int currentFrame = 100;

            //	ImGui::PushItemWidth(130);
            //	ImGui::InputInt("Frame Min", &mySequence.mFrameMin);
            //	ImGui::SameLine();
            //	ImGui::InputInt("Frame ", &currentFrame);
            //	ImGui::SameLine();
            //	ImGui::InputInt("Frame Max", &mySequence.mFrameMax);
            //	ImGui::PopItemWidth();
            //	Sequencer(&mySequence, &currentFrame, &expanded, &selectedEntry, &firstFrame,
            // ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD |
            // ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE |
            // ImSequencer::SEQUENCER_CHANGE_FRAME);
            //	// add a UI to edit that particular item
            //	if (selectedEntry != -1)
            //	{
            //		const MySequence::MySequenceItem& item = mySequence.myItems[selectedEntry];
            //		ImGui::Text("I am a %s, please edit me", SequencerItemTypeNames[item.mType]);
            //		// switch (type) ....
            //	}
            //}

            //// Graph Editor
            // static GraphEditor::Options options;
            // static GraphEditorDelegate delegate;
            // static GraphEditor::ViewState viewState;
            // static GraphEditor::FitOnScreen fit = GraphEditor::Fit_None;
            // static bool showGraphEditor = true;

            // if (ImGui::CollapsingHeader("Graph Editor"))
            //{
            //	ImGui::Checkbox("Show GraphEditor", &showGraphEditor);
            //	GraphEditor::EditOptions(options);
            // }

            // ImGui::End();

            // if (showGraphEditor)
            //{
            //	ImGui::Begin("Graph Editor", NULL, 0);
            //	if (ImGui::Button("Fit all nodes"))
            //	{
            //		fit = GraphEditor::Fit_AllNodes;
            //	}
            //	ImGui::SameLine();
            //	if (ImGui::Button("Fit selected nodes"))
            //	{
            //		fit = GraphEditor::Fit_SelectedNodes;
            //	}
            //	GraphEditor::Show(delegate, options, viewState, true, &fit);

            //	ImGui::End();
            //}
        }

        const float toolbarSize = 50;
        float menuBarHeight = 60.0f;

        void ImGuiApplication::DockSpaceUI()
        {
            ImGuiViewport *viewport = ImGui::GetMainViewport();

            auto toolbarSizeVec = ImVec2( 0, toolbarSize );
            ImGui::SetNextWindowPos(
                ImVec2( viewport->Pos.x + toolbarSizeVec.x, viewport->Pos.y + toolbarSizeVec.y ) );
            ImGui::SetNextWindowSize(
                ImVec2( viewport->Size.x - toolbarSizeVec.x, viewport->Size.y - toolbarSizeVec.y ) );
            ImGui::SetNextWindowViewport( viewport->ID );
            ImGuiWindowFlags window_flags =
                0 | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
            ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
            ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
            ImGui::Begin( "Master DockSpace", nullptr, window_flags );
            ImGuiID dockMain = ImGui::GetID( "MyDockspace" );

            // Save off menu bar height for later.
            menuBarHeight = ImGui::GetCurrentWindow()->MenuBarHeight();

            ImGui::DockSpace( dockMain );
            ImGui::End();
            ImGui::PopStyleVar( 3 );
        }

        void ImGuiApplication::ToolbarUI()
        {
            ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos( ImVec2( viewport->Pos.x, viewport->Pos.y + menuBarHeight ) );
            ImGui::SetNextWindowSize( ImVec2( viewport->Size.x, toolbarSize ) );
            ImGui::SetNextWindowViewport( viewport->ID );

            ImGuiWindowFlags window_flags = 0 | ImGuiWindowFlags_NoDocking |
                                            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                            ImGuiWindowFlags_NoSavedSettings;
            ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0 );
            ImGui::Begin( "TOOLBAR", nullptr, window_flags );
            ImGui::PopStyleVar();

            ImGui::Button( "Toolbar goes here", ImVec2( 0, 37 ) );
            ImGui::Button( "Toolbar goes here 2", ImVec2( 100, 37 ) );

            ImGui::End();
        }

        // Toolbar test [Experimental]
        // Usage:
        // {
        //   static ImGuiAxis toolbar1_axis = ImGuiAxis_X; // Your storage for the current direction.
        //   DockingToolbar("Toolbar1", &toolbar1_axis);
        // }
        void ImGuiApplication::DockingToolbar( const char *name, ImGuiAxis *p_toolbar_axis,
                                               SmartPtr<IUIElement> toolbarElement )
        {
            // [Option] Automatically update axis based on parent split (inside of doing it via
            // right-click on the toolbar) Pros:
            // - Less user intervention.
            // - Avoid for need for saving the toolbar direction, since it's automatic.
            // Cons:
            // - This is currently leading to some glitches.
            // - Some docking setup won't return the axis the user would expect.
            const bool TOOLBAR_AUTO_DIRECTION_WHEN_DOCKED = true;

            // ImGuiAxis_X = horizontal toolbar
            // ImGuiAxis_Y = vertical toolbar
            ImGuiAxis toolbar_axis = *p_toolbar_axis;

            // 1. We request auto-sizing on one axis
            // Note however this will only affect the toolbar when NOT docked.
            ImVec2 requested_size =
                ( toolbar_axis == ImGuiAxis_X ) ? ImVec2( -1.0f, 0.0f ) : ImVec2( 0.0f, -1.0f );
            ImGui::SetNextWindowSize( requested_size );

            // 2. Specific docking options for toolbars.
            // Currently they add some constraint we ideally wouldn't want, but this is simplifying our
            // first implementation
            ImGuiWindowClass window_class;
            window_class.DockingAllowUnclassed = true;
            window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoCloseButton;
            window_class.DockNodeFlagsOverrideSet |=
                ImGuiDockNodeFlags_HiddenTabBar;  // ImGuiDockNodeFlags_NoTabBar // FIXME: Will need a
            // working Undock widget for _NoTabBar to work
            window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingSplitMe;
            window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverMe;
            window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverOther;
            if( toolbar_axis == ImGuiAxis_X )
                window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeY;
            else
                window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeX;
            ImGui::SetNextWindowClass( &window_class );

            // 3. Begin into the window
            const float font_size = ImGui::GetFontSize();
            const ImVec2 icon_size( ImFloor( font_size * 1.7f ), ImFloor( font_size * 1.7f ) );
            ImGui::Begin( name, nullptr,
                          ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                              ImGuiWindowFlags_NoScrollbar );

            // 4. Overwrite node size
            ImGuiDockNode *node = ImGui::GetWindowDockNode();
            if( node != nullptr )
            {
                // Overwrite size of the node
                ImGuiStyle &style = ImGui::GetStyle();
                const auto toolbar_axis_perp = static_cast<ImGuiAxis>( toolbar_axis ^ 1 );
                const float TOOLBAR_SIZE_WHEN_DOCKED =
                    style.WindowPadding[toolbar_axis_perp] * 2.0f + icon_size[toolbar_axis_perp];
                node->WantLockSizeOnce = true;
                node->Size[toolbar_axis_perp] = node->SizeRef[toolbar_axis_perp] =
                    TOOLBAR_SIZE_WHEN_DOCKED;

                if( TOOLBAR_AUTO_DIRECTION_WHEN_DOCKED )
                    if( node->ParentNode && node->ParentNode->SplitAxis != ImGuiAxis_None )
                        toolbar_axis = static_cast<ImGuiAxis>( node->ParentNode->SplitAxis ^ 1 );
            }

            // 5. Dummy populate tab bar
            ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 3.0f );
            ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 5.0f, 5.0f ) );

            // UndockWidget(icon_size, toolbar_axis);
            // for (int icon_n = 0; icon_n < 10; icon_n++)
            //{
            //	char label[32];
            //	ImFormatString(label, IM_ARRAYSIZE(label), "%02d", icon_n);
            //	if (icon_n > 0 && toolbar_axis == ImGuiAxis_X)
            //		ImGui::SameLine();

            //	ImGui::Button(label, icon_size);
            //}

            // ImGui::ImageButton(my_tex_id, size);

            auto childCount = 0;

            if( auto p = toolbarElement->getChildren() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    if( childCount > 0 && toolbar_axis == ImGuiAxis_X )
                    {
                        ImGui::SameLine();
                    }

                    createElement( child );

                    childCount++;
                }
            }

            ImGui::PopStyleVar( 2 );

            // 6. Context-menu to change axis
            if( node == nullptr || !TOOLBAR_AUTO_DIRECTION_WHEN_DOCKED )
            {
                if( ImGui::BeginPopupContextWindow() )
                {
                    ImGui::TextUnformatted( name );
                    ImGui::Separator();

                    if( ImGui::MenuItem( "Horizontal", "", ( toolbar_axis == ImGuiAxis_X ) ) )
                        toolbar_axis = ImGuiAxis_X;

                    if( ImGui::MenuItem( "Vertical", "", ( toolbar_axis == ImGuiAxis_Y ) ) )
                        toolbar_axis = ImGuiAxis_Y;

                    ImGui::EndPopup();
                }
            }

            ImGui::End();

            // Output user stored data
            *p_toolbar_axis = toolbar_axis;
        }

        void ImGuiApplication::update()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            FB_ASSERT( ui );

            auto application = ui->getApplication();
            FB_ASSERT( application );

            auto graphicsSystem = applicationManager->getGraphicsSystem();

            ISharedObject::ScopedLock lock( graphicsSystem );

#if FB_GRAPHICS_SYSTEM_OGRENEXT
            // Start the Dear ImGui frame
#    if defined FB_PLATFORM_WIN32
#        if FB_BUILD_RENDERER_DX11
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
#        elif FB_BUILD_RENDERER_GL3PLUS
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplWin32_NewFrame();
#        elif FB_BUILD_RENDERER_OPENGL
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplWin32_NewFrame();
#        endif
#    elif defined FB_PLATFORM_APPLE
#        if FB_BUILD_RENDERER_OPENGL
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplWin32_NewFrame();
#        elif FB_BUILD_RENDERER_METAL
            app->update();
#        endif
#    endif

            ImGui::NewFrame();

            // auto show_demo_window = true;
            // if (show_demo_window)
            //{
            //	ImGui::ShowDemoWindow(&show_demo_window);
            // }

            // if (showDockSpace)
            //{
            //	ShowApp(&showDockSpace);
            // }

            ImGuiIO &io = ImGui::GetIO();
            if( isPerspective )
            {
                Perspective( fov, io.DisplaySize.x / io.DisplaySize.y, 0.1f, 100.f, cameraProjection );
            }
            else
            {
                float viewHeight = viewWidth * io.DisplaySize.y / io.DisplaySize.x;
                OrthoGraphic( -viewWidth, viewWidth, -viewHeight, viewHeight, 1000.f, -1000.f,
                              cameraProjection );
            }

            ImGuizmo::SetOrthographic( !isPerspective );
            ImGuizmo::BeginFrame();

            if( showDockSpace )
            {
                ShowApp( &showDockSpace );
            }

            // if (m_showEditor)
            //{
            //	ShowEditor(&m_showEditor);
            // }

            // TestDoc();

            // ShowGuizmo();

            // Rendering
            ImGui::Render();

#    if defined FB_PLATFORM_WIN32
#        if FB_BUILD_RENDERER_DX11
            auto drawData = ImGui::GetDrawData();
            ImGui_ImplDX11_RenderDrawData( drawData );
#        elif FB_BUILD_RENDERER_GL3PLUS
            auto drawData = ImGui::GetDrawData();
            ImGui_ImplOpenGL3_RenderDrawData( drawData );
#        elif FB_BUILD_RENDERER_OPENGL
            auto drawData = ImGui::GetDrawData();
            ImGui_ImplOpenGL3_RenderDrawData( drawData );
#        endif
#    elif defined FB_PLATFORM_APPLE
            app->postUpdate();
#    endif
#elif FB_GRAPHICS_SYSTEM_OGRE
            //#    if defined FB_PLATFORM_WIN32
            //#        if FB_BUILD_RENDERER_DX11
            //            ImGui_ImplDX11_NewFrame();
            //            ImGui_ImplWin32_NewFrame();
            //#        elif FB_BUILD_RENDERER_DX9
            //            ImGui_ImplDX9_NewFrame();
            //            ImGui_ImplWin32_NewFrame();
            //#        elif FB_BUILD_RENDERER_OPENGL
            //            ImGui_ImplOpenGL3_NewFrame();
            //            ImGui_ImplWin32_NewFrame();
            //#        endif
            //#    elif defined FB_PLATFORM_APPLE
            //#        if FB_BUILD_RENDERER_OPENGL
            //            ImGui_ImplOpenGL3_NewFrame();
            //            ImGui_ImplWin32_NewFrame();
            //#        elif FB_BUILD_RENDERER_METAL
            //            app->update();
            //#        endif
            //#        endif

            // ImGui::NewFrame();

            if( m_overlay )
            {
                if( m_overlay->getLoadingState() != LoadingState::Loaded )
                {
                    m_overlay->load();
                    m_overlay->show();
                }
            }

            /*
            ImGuiIO &io = ImGui::GetIO();
            if( isPerspective )
            {
                Perspective( fov, io.DisplaySize.x / io.DisplaySize.y, 0.1f, 100.f, cameraProjection );
            }
            else
            {
                float viewHeight = viewWidth * io.DisplaySize.y / io.DisplaySize.x;
                OrthoGraphic( -viewWidth, viewWidth, -viewHeight, viewHeight, 1000.f, -1000.f,
                              cameraProjection );
            }

            ImGuizmo::SetOrthographic( !isPerspective );
            ImGuizmo::BeginFrame();
             */

            if( showDockSpace )
            {
                ShowApp( &showDockSpace );
                //ShowExampleAppDockSpace(&showDockSpace);
            }

            // Rendering
            // ImGui::Render();

            /*
#        if defined FB_PLATFORM_WIN32
#            if FB_BUILD_RENDERER_DX11
            auto drawData = ImGui::GetDrawData();
            ImGui_ImplDX11_RenderDrawData( drawData );
#            elif FB_BUILD_RENDERER_DX9
            auto drawData = ImGui::GetDrawData();
            ImGui_ImplDX9_RenderDrawData( drawData );
#            elif FB_BUILD_RENDERER_OPENGL
            auto drawData = ImGui::GetDrawData();
            ImGui_ImplOpenGL3_RenderDrawData( drawData );
#            endif
#        elif defined FB_PLATFORM_APPLE
            app->postUpdate();
#        endif
            */
#elif 0
            // Poll and handle events (inputs, window resize, etc.)
            // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui
            // wants to use your inputs.
            // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main
            // application.
            // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main
            // application. Generally you may always pass all inputs to dear imgui, and hide them from
            // your application based on those two flags.
            SDL_Event event;
            while( SDL_PollEvent( &event ) )
            {
                ImGui_ImplSDL2_ProcessEvent( &event );
                if( event.type == SDL_QUIT )
                    done = true;
                if( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                    event.window.windowID == SDL_GetWindowID( window ) )
                    done = true;
            }

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            if( showDockSpace )
            {
                ShowApp( &showDockSpace );
            }

            // Rendering
            ImGui::Render();
            glViewport( 0, 0, (int)1280, (int)720 );
            glClearColor( clear_color.r * clear_color.a, clear_color.b * clear_color.a,
                          clear_color.b * clear_color.a, clear_color.a );
            glClear( GL_COLOR_BUFFER_BIT );
            ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

            // Update and Render additional Platform Windows
            // (Platform functions may change the current OpenGL context, so we save/restore it to make
            // it easier to paste this code elsewhere.
            //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context)
            //  directly)
            // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            //{
            //	SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            //	SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            //	ImGui::UpdatePlatformWindows();
            //	ImGui::RenderPlatformWindowsDefault();
            //	SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
            //}

            SDL_GL_SwapWindow( window );
#elif FB_BUILD_SDL2

            // Poll and handle events (inputs, window resize, etc.)
            // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui
            // wants to use your inputs.
            // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main
            // application.
            // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main
            // application. Generally you may always pass all inputs to dear imgui, and hide them from
            // your application based on those two flags.
            SDL_Event event;
            while( SDL_PollEvent( &event ) )
            {
                ImGui_ImplSDL2_ProcessEvent( &event );

                if( event.type == SDL_QUIT )
                {
                    done = true;
                }

                if( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                    event.window.windowID == SDL_GetWindowID( window ) )
                {
                    done = true;
                }
            }

            // Start the Dear ImGui frame
            ImGui_ImplSDLRenderer_NewFrame();
            ImGui_ImplSDL2_NewFrame( window );
            ImGui::NewFrame();

            if( showDockSpace )
            {
                ShowApp( &showDockSpace );
            }

            // Rendering
            ImGui::Render();
            SDL_SetRenderDrawColor( renderer, (Uint8)( clear_color.r * 255 ),
                                    (Uint8)( clear_color.g * 255 ), (Uint8)( clear_color.b * 255 ),
                                    (Uint8)( clear_color.a * 255 ) );
            SDL_RenderClear( renderer );
            ImGui_ImplSDLRenderer_RenderDrawData( ImGui::GetDrawData() );
            SDL_RenderPresent( renderer );
#endif
        }

        void *ImGuiApplication::getHWND() const
        {
            return m_hwnd;
        }

        SmartPtr<IUIMenubar> ImGuiApplication::getMenubar() const
        {
            return m_menuBar;
        }

        void ImGuiApplication::setMenubar( SmartPtr<IUIMenubar> menubar )
        {
            m_menuBar = menubar;
        }

        SmartPtr<IUIToolbar> ImGuiApplication::getToolbar() const
        {
            return m_toolbar;
        }

        void ImGuiApplication::setToolbar( SmartPtr<IUIToolbar> toolbar )
        {
            m_toolbar = toolbar;
        }

        void ImGuiApplication::WindowListener::unload( SmartPtr<ISharedObject> data )
        {
            m_owner = nullptr;
        }

        void ImGuiApplication::WindowListener::handleEvent( SmartPtr<render::IWindowEvent> event )
        {
            if( auto owner = getOwner() )
            {
                owner->handleWindowEvent( event );
            }
        }

        Parameter ImGuiApplication::WindowListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            if( eventValue == windowClosingHash )
            {
                return Parameter( true );
            }

            return Parameter();
        }

        void ImGuiApplication::WindowListener::setOwner( SmartPtr<ImGuiApplication> owner )
        {
            m_owner = owner;
        }

        SmartPtr<ImGuiApplication> ImGuiApplication::WindowListener::getOwner() const
        {
            auto p = m_owner.load();
            return p.lock();
        }
    }  // end namespace ui
}  // end namespace fb
