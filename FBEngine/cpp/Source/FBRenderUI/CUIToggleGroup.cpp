#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIToggleGroup.h>
#include <FBRenderUI/CUIToggleButton.h>
#include <FBCore/FBCore.h>

namespace fb::ui
{

    CUIToggleGroup::CUIToggleGroup( const String &id ) : m_curToggledBtn( nullptr )
    {
        setName( id );
    }

    CUIToggleGroup::~CUIToggleGroup() = default;

    void CUIToggleGroup::addToggleButton( CUIToggleButton *button )
    {
        m_toggleButtons.push_back( button );
    }

    auto CUIToggleGroup::removeToggleButton( CUIToggleButton *button ) -> bool
    {
        return false;  // m_toggleButtons.erase_element(button);
    }

    void CUIToggleGroup::OnSetButtonToggled( CUIToggleButton *button )
    {
        if( m_curToggledBtn != button )
        {
            for( auto &m_toggleButton : m_toggleButtons )
            {
                m_toggleButton->setToggled( false );
            }

            m_curToggledBtn = button;
            m_curToggledBtn->setToggled( true );
        }
    }

    auto CUIToggleGroup::getCurToggledButton() const -> CUIToggleButton *
    {
        return m_curToggledBtn;
    }
}  // namespace fb::ui
