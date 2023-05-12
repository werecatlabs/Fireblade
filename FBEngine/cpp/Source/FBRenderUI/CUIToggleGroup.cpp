#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIToggleGroup.h>
#include <FBRenderUI/CUIToggleButton.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {

        CUIToggleGroup::CUIToggleGroup( const String &id ) : m_curToggledBtn( nullptr )
        {
            setName( id );
        }

        CUIToggleGroup::~CUIToggleGroup()
        {
        }

        void CUIToggleGroup::addToggleButton( CUIToggleButton *button )
        {
            m_toggleButtons.push_back( button );
        }

        bool CUIToggleGroup::removeToggleButton( CUIToggleButton *button )
        {
            return false;  // m_toggleButtons.erase_element(button);
        }

        void CUIToggleGroup::OnSetButtonToggled( CUIToggleButton *button )
        {
            if( m_curToggledBtn != button )
            {
                for( u32 i = 0; i < m_toggleButtons.size(); ++i )
                {
                    m_toggleButtons[i]->setToggled( false );
                }

                m_curToggledBtn = button;
                m_curToggledBtn->setToggled( true );
            }
        }

        CUIToggleButton *CUIToggleGroup::getCurToggledButton() const
        {
            return m_curToggledBtn;
        }
    }  // end namespace ui
}  // end namespace fb
