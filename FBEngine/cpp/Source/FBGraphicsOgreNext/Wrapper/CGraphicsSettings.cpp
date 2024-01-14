#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CGraphicsSettings.h>

namespace fb::render
{

    CGraphicsSettings::CGraphicsSettings() = default;

    CGraphicsSettings::~CGraphicsSettings() = default;

    auto CGraphicsSettings::getCreateWindow() const -> bool
    {
        return m_createWindow;
    }

    void CGraphicsSettings::setCreateWindow( bool createWindow )
    {
        m_createWindow = createWindow;
    }

    auto CGraphicsSettings::getShowDialog() const -> bool
    {
        return m_showDialog;
    }

    void CGraphicsSettings::setShowDialog( bool showDialog )
    {
        m_showDialog = showDialog;
    }

}  // namespace fb::render
