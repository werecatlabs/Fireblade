#ifndef CGraphicsSystemConfiguration_h__
#define CGraphicsSystemConfiguration_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IGraphicsSettings.h>

namespace fb
{
    namespace render
    {
        class CGraphicsSettings : public IGraphicsSettings
        {
        public:
            CGraphicsSettings();
            ~CGraphicsSettings() override;

            bool getCreateWindow() const override;
            void setCreateWindow( bool createWindow ) override;

            bool getShowDialog() const override;
            void setShowDialog( bool showDialog ) override;

        protected:
            bool m_showDialog = false;
            bool m_createWindow = true;
        };
    } // end namespace render
}     // end namespace fb

#endif  // CGraphicsSystemConfiguration_h__
