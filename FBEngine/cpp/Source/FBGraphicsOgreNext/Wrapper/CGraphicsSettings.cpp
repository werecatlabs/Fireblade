#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CGraphicsSettings.h>

namespace fb
{
    namespace render
    {

        CGraphicsSettings::CGraphicsSettings()
        {
        }

        CGraphicsSettings::~CGraphicsSettings()
        {
        }

        bool CGraphicsSettings::getCreateWindow() const
        {
            return m_createWindow;
        }

        void CGraphicsSettings::setCreateWindow( bool createWindow )
        {
            m_createWindow = createWindow;
        }

        bool CGraphicsSettings::getShowDialog() const
        {
            return m_showDialog;
        }

        void CGraphicsSettings::setShowDialog( bool showDialog )
        {
            m_showDialog = showDialog;
        }

    }  // end namespace render
}  // end namespace fb
