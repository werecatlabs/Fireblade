#ifndef ImGuiVector2_h__
#define ImGuiVector2_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIVector2.h>

namespace fb
{
    namespace ui
    {
        //---------------------------------------------
        class ImGuiVector2 : public CImGuiElement<IUIVector2>
        {
        public:
            ImGuiVector2();
            ~ImGuiVector2() override;

            Vector2<real_Num> getValue() const override;
            void setValue( const Vector2<real_Num> &value ) override;

            /** @copydoc IUIVector3::getLabel */
            String getLabel() const override;

            /** @copydoc IUIVector3::setLabel */
            void setLabel( const String &label ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            Vector2<real_Num> m_value;
            String m_label;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // ImGuiVector2_h__
