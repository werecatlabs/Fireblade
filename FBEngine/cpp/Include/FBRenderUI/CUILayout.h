#ifndef _CGUILAYOUT_H
#define _CGUILAYOUT_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Interface/UI/IUILayout.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {

        class CUILayout : public CUIElement<IUILayout>
        {
        public:
            enum LayoutStates
            {
                FS_IDLE,
                FS_FADEIN,
                FS_FADEOUT,

                FS_COUNT
            };

            CUILayout();
            ~CUILayout() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            bool handleEvent( const SmartPtr<IInputEvent> &event ) override;

            void update() override;

	        void handleStateChanged( SmartPtr<IState> &state );

            SmartPtr<IFSM> getFSM() override;
            const SmartPtr<IFSM> &getFSM() const;

            void OnEnterState( u8 state );
            void OnUpdateState( u8 state );
            void OnLeaveState( u8 state );

            u8 GetStateFromName( const String &stateName ) const;
            String GetStateNameFromId( u8 stateId ) const;

            /** Adds a child to this ui item. */
            void addChild( SmartPtr<IUIElement> pGUIItem ) override;

            /** Removes a child of the ui item. */
            bool removeChild( SmartPtr<IUIElement> pGUIItem ) override;

            SmartPtr<ui::IUIWindow> getUiWindow() const;

            void setUiWindow( SmartPtr<ui::IUIWindow> uiWindow );

            void updateZOrder();

            FB_CLASS_REGISTER_DECL;

        private:
            void OnEnterIdleState();
            void OnEnterFadeInState();
            void OnEnterFadeOutState();

            void OnUpdateIdleState();
            void OnUpdateFadeInState();
            void OnUpdateFadeOutState();

            void OnLeaveIdleState();
            void OnLeaveFadeInState();
            void OnLeaveFadeOutState();

            //
            // Callbacks
            //
            void OnActivateCallback();
            void OnSelectCallback();
            void OnDeselectCallback();

            SmartPtr<ui::IUIWindow> m_uiWindow;

            SmartPtr<render::IOverlay> m_overlay;

            SmartPtr<IFSM> m_fsm;

            static u32 m_nameExt;
        };
    }  // end namespace ui
}  // end namespace fb

#endif
