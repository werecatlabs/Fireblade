#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIDial.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {

        CUIDial::CUIDial() :
            m_fStartAngle( 0.f ),
            m_fEndAngle( 0.f ),
            m_fNeedleAngle( 0.f ),
            m_bIsVisible( true )
        {
        }

        CUIDial::~CUIDial()
        {
            removeAllChildren();
        }

        void CUIDial::initialise()
        {
        }

        void CUIDial::setNeedlePosition( f32 position )
        {
            m_fNeedleAngle = m_fStartAngle + ( ( m_fEndAngle - m_fStartAngle ) * position );
        }
    }  // namespace ui
}  // end namespace fb
