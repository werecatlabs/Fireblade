#include <FBCore/FBCorePCH.h>
#include <FBCore/Memory/ObjectYield.h>

namespace fb
{

    ObjectYield::ObjectYield( RawPtr<IObject> pObject ) : m_object( pObject )
    {
        m_lineNumber = 0;
    }

    ObjectYield::ObjectYield()
    {
        m_lineNumber = 0;
    }

    ObjectYield::~ObjectYield()
    {
    }

    void ObjectYield::operator()()
    {
        FB_ASSERT( false );  // please use macro yield
    }

    s32 ObjectYield::getLineNumber() const
    {
        return m_lineNumber;
    }

    void ObjectYield::setLineNumber( s32 lineNumber )
    {
        FB_ASSERT( lineNumber > m_lineNumber );
        m_lineNumber = lineNumber;
    }

    void ObjectYield::stop()
    {
    }

    void ObjectYield::yield()
    {
    }
}  // end namespace fb
