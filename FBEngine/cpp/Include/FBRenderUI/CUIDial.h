#ifndef _CGUIDIAL_H
#define _CGUIDIAL_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Interface/UI/IUIDial.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {
        
        //! interface for dial class used to make speedometers etc
        class CUIDial : public CUIElement<IUIDial>
        {
        public:
            CUIDial();
            ~CUIDial() override;

            virtual void initialise();

            void setNeedlePosition( f32 position ) override;

        protected:
            f32 m_fStartAngle;  // the minimum angle of the dial needle
            f32 m_fEndAngle;    // the maximum angle the dial need
            f32 m_fNeedleAngle; // the angle of the dial needle

            bool m_bIsVisible; // value to know if the dial is visible or not
        };
    } // end namespace ui
}     // end namespace fb

#endif
