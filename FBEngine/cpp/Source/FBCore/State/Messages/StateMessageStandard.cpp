#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageStandard.h>

namespace fb
{
    //---------------------------------------------
    StateMessageStandard::StateMessageStandard() : m_subjectId( 0 )
    {
    }

    //---------------------------------------------
    StateMessageStandard::StateMessageStandard( u32 subjectId ) : m_subjectId( subjectId )
    {
    }

    //---------------------------------------------
    StateMessageStandard::~StateMessageStandard()
    {
    }

    //---------------------------------------------
    void StateMessageStandard::setSubjectId( u32 id )
    {
        m_subjectId = id;
    }

    //---------------------------------------------
    u32 StateMessageStandard::getSubjectId() const
    {
        return m_subjectId;
    }
}  // end namespace fb
