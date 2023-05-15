#ifndef ImGuiVector3_h__
#define ImGuiVector3_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIVector3.h>

namespace fb
{
    namespace ui
    {
        //---------------------------------------------
        class ImGuiVector3 : public CImGuiElement<IUIVector3>
        {
        public:
            ImGuiVector3();
            ~ImGuiVector3() override;

            /** @copydoc IUIVector3::getValue */
            Vector3<real_Num> getValue() const override;

            /** @copydoc IUIVector3::setValue */
            void setValue( const Vector3<real_Num> &value ) override;

            /** @copydoc IUIVector3::getLabel */
            String getLabel() const override;

            /** @copydoc IUIVector3::setLabel */
            void setLabel( const String &label ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            Vector3<real_Num> m_value;
            String m_label;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // ImGuiVector3_h__
