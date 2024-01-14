#ifndef TableLayout_h__
#define TableLayout_h__

#include <FBCore/Scene/Components/UI/LayoutContainer.h>

namespace fb
{
    namespace scene
    {
        class TableLayout : public LayoutContainer
        {
        public:
            TableLayout();
            ~TableLayout() override;

            void updateTransform() override;

            /** @copydoc LayoutContainer::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc LayoutContainer::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            SmartPtr<IActor> m_tableParent;

            s32 numRows = 3;
            s32 numColumns = 3;
            f32 cellWidth = 100.0f;
            f32 cellHeight = 50.0f;
        };
    }  // namespace scene
}  // namespace fb

#endif  // TableLayout_h__
