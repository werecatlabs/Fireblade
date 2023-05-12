#include <FBCore/FBCorePCH.h>
#include <FBCore/Memory/CObjectYield.h>

namespace fb
{

    CObjectYield::CObjectYield( RawPtr<IObject> pObject ) : m_object( pObject )
    {
        m_lineNumber = 0;
    }

    CObjectYield::CObjectYield()
    {
        m_lineNumber = 0;
    }

    CObjectYield::~CObjectYield()
    {
    }

    void CObjectYield::operator()()
    {
        FB_ASSERT( false );  // please use macro yield
    }

    s32 CObjectYield::getLineNumber() const
    {
        return m_lineNumber;
    }

    void CObjectYield::setLineNumber( s32 lineNumber )
    {
        FB_ASSERT( lineNumber > m_lineNumber );
        m_lineNumber = lineNumber;
    }

    void CObjectYield::stop()
    {
    }

    void CObjectYield::yield()
    {
    }
}  // end namespace fb
