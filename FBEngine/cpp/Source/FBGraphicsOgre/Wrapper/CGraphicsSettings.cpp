#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsSettings.h>

namespace fb
{
    namespace render
    {
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
    }  // namespace render
}  // namespace fb
