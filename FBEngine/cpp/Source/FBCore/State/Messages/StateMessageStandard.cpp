#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageStandard.h>

namespace fb
{
    //---------------------------------------------
    StateMessageStandard::StateMessageStandard()
    {
    }

    //---------------------------------------------
    StateMessageStandard::StateMessageStandard( u32 subjectId ) : m_subjectId( subjectId )
    {
    }

    //---------------------------------------------
    StateMessageStandard::~StateMessageStandard() = default;

    //---------------------------------------------
    void StateMessageStandard::setSubjectId( u32 id )
    {
        m_subjectId = id;
    }

    //---------------------------------------------
    auto StateMessageStandard::getSubjectId() const -> u32
    {
        return m_subjectId;
    }
}  // end namespace fb
