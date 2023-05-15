#ifndef ImGuiPropertyGrid_h__
#define ImGuiPropertyGrid_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIPropertyGrid.h>

namespace fb
{
    namespace ui
    {
        
        class ImGuiPropertyGrid : public CImGuiElement<IUIPropertyGrid>
        {
        public:
            ImGuiPropertyGrid();
            ~ImGuiPropertyGrid() override;

            /** @copydoc IUIPropertyGrid::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IUIPropertyGrid::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            static void createElement( SmartPtr<IUIElement> element );

            static void handlePropertyButtonClicked( SmartPtr<IUIPropertyGrid> propertyGrid, const String& name,
                                                     const String& value );

            static void handlePropertyChanged( SmartPtr<IUIPropertyGrid> propertyGrid, const String& name, const String& str );

            FB_CLASS_REGISTER_DECL;

        protected:
            AtomicSmartPtr<Properties> m_properties;
            String m_name;
            static u32 m_childWindowCount;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // ImGuiPropertyGrid_h__
