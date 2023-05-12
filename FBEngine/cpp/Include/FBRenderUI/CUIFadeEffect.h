#ifndef _CGUIFadeEffect_H
#define _CGUIFadeEffect_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {
        
        class CUIFadeEffect : public CUIElement<IUIElement>
        {
        public:
            enum FadeState
            {
                FS_IDLE,
                FS_FADEIN,
                FS_FADEOUT,

                FS_COUNT
            };

            CUIFadeEffect();
            ~CUIFadeEffect() override;

            void initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode );

            void update() override;

            void onToggleVisibility() override;

            void setMaterialName( const String &materialName );

            void setPosition( const Vector2F &position ) override;
            void setSize( const Vector2F &size ) override;

            void OnEnterState( u8 state );
            void OnUpdateState( u8 state );
            void OnLeaveState( u8 state );

            u8 GetStateFromName( const String &stateName ) const;
            String GetStateNameFromId( u8 stateId ) const;

            SmartPtr<IFSM> &getFSM();

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

            void handleEvent( const String &eventType );

            SmartPtr<IFSM> m_fsm;
            SmartPtr<render::IOverlayElementContainer> m_container;
            f32 m_alpha;
            u32 m_displayHitUntilTime;
        };

        using CGUIFadeEffectPtr = SmartPtr<CUIFadeEffect>;
    } // end namespace ui
}     // end namespace fb

#endif
