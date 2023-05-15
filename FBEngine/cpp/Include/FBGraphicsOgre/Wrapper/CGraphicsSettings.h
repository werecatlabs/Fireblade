#ifndef CGraphicsSettings_h__
#define CGraphicsSettings_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IGraphicsSettings.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    namespace render
    {
        class CGraphicsSettings : public CSharedObject<IGraphicsSettings>
        {
        public:
            CGraphicsSettings() = default;
            ~CGraphicsSettings() override = default;

            /** @copydoc IGraphicsSettings::getCreateWindow */
            bool getCreateWindow() const override;

            /** @copydoc IGraphicsSettings::setCreateWindow */
            void setCreateWindow( bool createWindow ) override;

            /** @copydoc IGraphicsSettings::getShowDialog */
            bool getShowDialog() const override;

            /** @copydoc IGraphicsSettings::setShowDialog */
            void setShowDialog( bool showDialog ) override;

        protected:
            bool m_showDialog = false;
            bool m_createWindow = true;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // CGraphicsSystemConfiguration_h__
